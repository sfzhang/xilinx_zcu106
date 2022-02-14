/*  axiirq.c - The simplest kernel module.

* Copyright (C) 2013 - 2016 Xilinx, Inc
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.

*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program. If not, see <http://www.gnu.org/licenses/>.

*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/interrupt.h>

#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>

#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/of_mdio.h>
#include <linux/of_net.h>


/* Standard module information, edit as appropriate */
MODULE_LICENSE("GPL");
MODULE_AUTHOR
    ("RONOVO Surgical");
MODULE_DESCRIPTION
    ("RONOVO Surgical AXI IRQ module");

#define DRIVER_NAME "axiirq"
#define NETLINK_AXI_IRQ 25
#define NETLINK_GROUP 1

#define GEN_CCU_ADDR 1000
#define GEN_IRQ_SW_ADDR 4
#define IRQ_REG_PHY_ADDR 0x43C00000 + (GEN_CCU_ADDR + GEN_IRQ_SW_ADDR) * 4
#define IRQ_CCU_NUM 1
#define GROUP_ADDR_SIZE 1

typedef struct{
    struct tasklet_struct task;
    unsigned long irq_seq;
} irq_tasklet_t;

static void __iomem *g_irq_reg_vir_addr;

struct sock *g_nl_sk = NULL;
static u32 g_seq = 1;

static void sendnlmsg(void *msg, int msg_length)
{
    int len = 0;
    struct sk_buff *skb = NULL;
    struct nlmsghdr *nlh = NULL;
    int send_result = 0;

    if (!msg || !g_nl_sk) {
        return;
    }

    len = NLMSG_SPACE(msg_length);
    pr_info("msg_length[%d], len[%d]", msg_length, len);

    /* Allocate sk_buffer */
    skb = dev_alloc_skb(len);
    if (!skb) {
        pr_err("alloc_skb() failed: len[%d]!", len);
        return;
    }

    /* Netlink control block */
    NETLINK_CB(skb).portid = 0;     /* Send from kernel, set to 0 */
    NETLINK_CB(skb).dst_group = 1;  /* Dest group */

    /* Initialize netlink message header */
    nlh = nlmsg_put(skb, 0, g_seq++, 0, msg_length, 0);
    if (!nlh) {
        kfree_skb(skb);
        pr_err("nlmsg_put() failed: g_seq[%d]", (g_seq - 1));
        return;
    }

    /* Set payload */
    memcpy(nlmsg_data(nlh), msg, msg_length);
    send_result = netlink_broadcast(g_nl_sk, skb, 0, NETLINK_GROUP, GFP_ATOMIC);
    if (0 != send_result) {
        pr_err("netlink_broadcast() failed,  errno is %d", send_result);
    }
}

static void nl_data_ready(struct sk_buff *skb)
{
    // DO nothing here
    pr_info("nl_data_ready() OK!");
#if 0
    struct nlmsghdr *nlh = NULL;
    int  unmask_irq_id;
    int  data_length;
    void __iomem *irq_reg_vir_tar_addr;
    pr_info("nl_data_ready() begin...");
    if (skb) {
        nlh = (struct nlmsghdr *)skb->data;

        data_length = (nlh)->nlmsg_len - NLMSG_HDRLEN;
        if (0 == data_length){
            pr_info("Not payload!");
            return;
        }

        unmask_irq_id =* ((int *)NLMSG_DATA(nlh));
        pr_info("Receive netlink message[%d]", unmask_irq_id);

        if (unmask_irq_id > IRQ_CCU_NUM){
            return;
        }
        irq_reg_vir_tar_addr = g_irq_reg_vir_addr +
                (unmask_irq_id * GROUP_ADDR_SIZE + IRQ_MASK_ADDR) * 4;
        iowrite32(1, irq_reg_vir_tar_addr);

    }

    pr_info("nl_data_ready() OK!");
#endif
}

static void irq_tasklet_handler(unsigned long data)
{
    irq_tasklet_t * irq_tasklet = (irq_tasklet_t *)data;
    u32 seq;
    if (NULL == irq_tasklet){
        pr_err("received irq_tasklet ptr is null!");
    }
    seq = irq_tasklet->irq_seq;
    kfree((void *)data);
    sendnlmsg(&seq, sizeof(u32));
}

static irqreturn_t user_interrupt_handle(int irq_ID, void *dev_id)
{
    irq_tasklet_t * irq_tasklet;
    iowrite32(0, g_irq_reg_vir_addr);
    irq_tasklet = (irq_tasklet_t *)kmalloc(sizeof(irq_tasklet_t), GFP_ATOMIC);
    if (NULL == irq_tasklet){
        pr_err("Failed to malloc mem for tasklet!");
        return IRQ_HANDLED;
    }
    irq_tasklet->irq_seq = g_seq;
    tasklet_init (&(irq_tasklet->task), irq_tasklet_handler, (unsigned long) irq_tasklet);
    tasklet_schedule(&(irq_tasklet->task));
    return IRQ_HANDLED;
}

static int axi_irq_of_probe(struct platform_device *ofdev)
{
    struct resource *res = NULL;
    int ret = 0;
    //int irq_no = -1;
    struct netlink_kernel_cfg cfg = {
        .input = nl_data_ready,
    };

    //pr_info("Compile time[%s %s]", __DATE__, __TIME__);
    res = platform_get_resource(ofdev, IORESOURCE_IRQ, 0);
    if (!res) {
        pr_err("platform_get_resource() IRQ resource failed!");
        return -1;
    }

    /* Create netlink */
    g_nl_sk = netlink_kernel_create(&init_net,
                                    NETLINK_AXI_IRQ,
                                    &cfg);
    if (!g_nl_sk) {
        pr_err("netlink_kernel_create() failed!");
        return -1;
    }
    pr_info("netlink_kernel_create() success!");
    /* Get IRQ address */
    g_irq_reg_vir_addr = ioremap(IRQ_REG_PHY_ADDR, IRQ_CCU_NUM * GROUP_ADDR_SIZE * 4);
    if (!g_irq_reg_vir_addr) {
        pr_err("ioremap() failed: phy_addr[0x%x], size[%d]",
               (unsigned int)IRQ_REG_PHY_ADDR, IRQ_CCU_NUM * GROUP_ADDR_SIZE * 4);
        return -1;
    }
    pr_info("ioremap() success: phy_addr[0x%x], vir_addr[0x%x], size[%d]",
            (unsigned int)IRQ_REG_PHY_ADDR, (unsigned int)g_irq_reg_vir_addr, IRQ_CCU_NUM * GROUP_ADDR_SIZE * 4);

    /* Request IRQ */
    //irq_no = res->start;
    ret = request_irq(res->start, user_interrupt_handle, IRQF_SHARED, "axiirq", (void *)res->start);
    if (ret != 0) {
        pr_err("request_irq() failed: irq_no[%llu], ret[%d]", res->start, ret);
        return -1;
    }
    pr_info("Request irq() irq_no[%llu] success!", res->start);

    return 0;
}

static int axi_irq_of_remove(struct platform_device *ofdev)
{
    struct resource *res = NULL;
    //int irq_no = -1;

    /* Close netlink */
    if (g_nl_sk) {
        sock_release(g_nl_sk->sk_socket);
    }

    /* Free IRQ */
    res = platform_get_resource(ofdev, IORESOURCE_IRQ, 0);
    if (!res) {
        pr_err("platform_get_resource() IRQ resource failed!");
        return -1;
    }

    //irq_no = res->start;
    free_irq(res->start, (void *)res->start);

    /* Unmap IO */
    if (g_irq_reg_vir_addr) {
        iounmap(g_irq_reg_vir_addr);
    }

    return 0;
}

static const struct of_device_id axi_irq_of_match[] = {
    { .compatible = "ronovo,axiirq", },
    { /* end of list */ },
};

MODULE_DEVICE_TABLE(of, axi_irq_of_match);

static struct platform_driver axi_irq_of_driver = {
   .probe  = axi_irq_of_probe,
   .remove = axi_irq_of_remove,
   .driver = {
      .name = "axi_irq_driver",
      .owner = THIS_MODULE,
      .of_match_table = axi_irq_of_match,
   },
};

module_platform_driver(axi_irq_of_driver);


