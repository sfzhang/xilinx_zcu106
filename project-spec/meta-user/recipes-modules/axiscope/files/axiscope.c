/*  axiscope.c - The simplest kernel module.

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


/* print format */
#define pr_fmt(fmt) KBUILD_MODNAME "|%s|%d > " fmt "\n", __func__, __LINE__

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/uaccess.h>

#define DEVICE_NAME         "axiscope"
#define AXI4_DEV_PHY_ADDR   0x43C00000 //Modify the address to your peripheral
#define CCU_START_ADDR 0
#define CCU_END_ADDR 10000
#define RRCM_RD_BUF_SIZE 4
#define RRCM_WR_BUF_SIZE 4

static void __iomem *g_axi4_dev_addr;

static int axi4_dev_open(struct inode *inode , struct file *filp)
{
    return 0;
}

static int axi4_dev_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t axi4_dev_read(struct file *filp, char __user *buffer, size_t count,
                             loff_t *offset)
{
    unsigned long ret = 0;
    u32 *target_vir_addr = NULL;
    u32 kbuf[RRCM_RD_BUF_SIZE];
    int i = 0;

    if ((*offset < (CCU_START_ADDR * 4)) ||
        (*offset > (CCU_END_ADDR * 4))) {
        pr_err("offset[%lld] out of range[%d] ~ [%d]!", *offset, CCU_START_ADDR * 4, CCU_END_ADDR * 4);
        return -EINVAL;
    }

    if (count > RRCM_RD_BUF_SIZE * 4) {
        pr_err("count[%zu] out of range[%d]", count, RRCM_RD_BUF_SIZE * 4);
        return -EINVAL;
    }

    if (count % 4) {
        pr_err("count[%zu] invalid, should multiples of [4]", count);
        return -EINVAL;
    }

    target_vir_addr = (u32 *)(g_axi4_dev_addr + *offset);
    //memcpy_fromio(kbuf, target_vir_addr, count);

    for (i = 0; i < (count / 4); i++) {
        kbuf[i] = ioread32(target_vir_addr + i);
        pr_debug("ioread32(): addr[%p], offset[%lld], value[%u]",
                 (target_vir_addr + i), (*offset / 4 + i), kbuf[i]);
    }

    ret = copy_to_user(buffer, kbuf, count);
    if (ret) {
        pr_err("copy_to_user() failed: ret[%lu]", ret);
        return -EFAULT;
    }

    *offset += count;
    return count;
}

static ssize_t axi4_dev_write(struct file *filp, const char __user *buffer,
                              size_t count, loff_t *offset)
{
    unsigned long ret = 0;
    u32 *target_vir_addr = NULL;
    u32 kbuf[RRCM_WR_BUF_SIZE];
    int i = 0;

    if ((*offset < (CCU_START_ADDR * 4)) ||
        (*offset > (CCU_END_ADDR * 4))) {
        pr_err("offset[%lld] out of range[%d] ~ [%d]!", *offset, CCU_START_ADDR * 4, CCU_END_ADDR * 4);
        return -EINVAL;
    }

    if (count > RRCM_WR_BUF_SIZE * 4) {
        pr_err("count[%zu] out of range[%d]", count, RRCM_WR_BUF_SIZE * 4);
        return -EINVAL;
    }

    if (count % 4) {
        pr_err("count[%zu] invalid, should multiples of [4]", count);
        return -EINVAL;
    }

    ret = copy_from_user(kbuf, buffer, count);
    if (ret){
        pr_err("copy_from_user() failed: ret[%lu], buffer[0x%p], count[%zu]",
               ret, buffer, count);
        return -EFAULT;
    }

    target_vir_addr = (u32 *)(g_axi4_dev_addr + *offset);
    for (i = 0; i < (count / 4); i++) {
        iowrite32(kbuf[i], target_vir_addr + i);
        pr_debug("iowrite32(): addr[%p], offset[%lld], value[%u]",
                 (target_vir_addr + i), (*offset / 4 + i), kbuf[i]);
    }
    //memcpy_toio(target_vir_addr, kbuf, count);

    *offset += count;
    return count;
}

static const struct file_operations axi4_dev_fops =
{
    .owner   = THIS_MODULE,
    .open    = axi4_dev_open,
    .release = axi4_dev_release,
    .read    = axi4_dev_read,
    .write   = axi4_dev_write,
};

static struct miscdevice axi4_dev_dev =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &axi4_dev_fops,
};

int __init axi4_dev_init(void)
{
    int ret;
    size_t reg_size;
    //pr_info("Compile time[%s %s]", __DATE__, __TIME__);

    reg_size = CCU_END_ADDR - CCU_START_ADDR;
    g_axi4_dev_addr = ioremap(AXI4_DEV_PHY_ADDR,
                              reg_size * 4);
    if (!g_axi4_dev_addr) {
        pr_err("ioremap() failed: phy_addr[0x%x], size[%d]", AXI4_DEV_PHY_ADDR,
               reg_size * 4);
        return -EIO;
    }
    pr_info("ioremap() success: phy_addr[0x%x], size[%d], vir_addr[0x%p]",
            AXI4_DEV_PHY_ADDR, (reg_size * 4),
            g_axi4_dev_addr);

    ret = misc_register(&axi4_dev_dev);
    if (ret) {
        pr_err("misc_register() failed: ret[%d]", ret);
        return ret;
    }

    pr_info("Module init success!");
    return 0; /* Success */
}

void __exit axi4_dev_exit(void)
{
    iounmap(g_axi4_dev_addr);
    misc_deregister(&axi4_dev_dev);

    pr_warn("Module exit");
}

module_init(axi4_dev_init);
module_exit(axi4_dev_exit);

MODULE_AUTHOR("RONOVO Surgical");
MODULE_ALIAS("axi4_dev");
MODULE_DESCRIPTION("RONOVO Surgical axiscope module");
MODULE_LICENSE("GPL");
