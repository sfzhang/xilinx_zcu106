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
#include <linux/cdev.h>

struct chrdev_t {
    dev_t devid;
    struct cdev cdev;
    struct class *class;
    struct device *device;
    char buffer[64];
} g_chrdev;

static int chrdev_dev_open(struct inode *inode , struct file *filp)
{
    return 0;
}

static int chrdev_dev_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t chrdev_dev_read(struct file *filp, char __user *buffer, size_t count, loff_t *offset)
{
    // offset is ignored

    unsigned long ret = 0;

    if (count > sizeof(g_chrdev.buffer)) {
        return -EINVAL;
    }

    ret = copy_to_user(buffer, g_chrdev.buffer, count);
    if (ret) {
        pr_err("copy_to_user() failed: ret[%lu]", ret);
        return -EFAULT;
    }

    return count;
}

static ssize_t chrdev_dev_write(struct file *filp, const char __user *buffer, size_t count, loff_t *offset)
{
    // offset is ignored

    unsigned long ret = 0;

    if (count > sizeof(g_chrdev.buffer)) {
        return -EINVAL;
    }

    ret = copy_from_user(g_chrdev.buffer, buffer, count);
    if (ret){
        pr_err("copy_from_user() failed: ret[%lu], buffer[0x%p], count[%zu]", ret, buffer, count);
        return -EFAULT;
    }

    return count;
}

static const struct file_operations chrdev_dev_fops = {
    .owner   = THIS_MODULE,
    .open    = chrdev_dev_open,
    .release = chrdev_dev_release,
    .read    = chrdev_dev_read,
    .write   = chrdev_dev_write,
};

int __init chrdev_dev_init(void)
{
    int ret = alloc_chrdev_region(&g_chrdev.devid, 0, 1, "chrdev");
    if (ret) {
        pr_err("alloc_chrdev_region() failed: ret[%d]", ret);
        goto err_alloc_chrdev_region;
    }

    g_chrdev.cdev.owner = THIS_MODULE;
    cdev_init(&g_chrdev.cdev, &chrdev_dev_fops);

    ret = cdev_add(&g_chrdev.cdev, g_chrdev.devid, 1);
    if (ret) {
        pr_err("cdev_add() failed: ret[%d]", ret);
        goto err_cdev_add;
    }

    g_chrdev.class = class_create(THIS_MODULE, "chrdev");
    if (IS_ERR(g_chrdev.class)) {
        ret = PTR_ERR(g_chrdev.class);
        pr_err("class_create() failed: ret[%d]", ret);
        goto err_class_create;
    }

    g_chrdev.device = device_create(g_chrdev.class, NULL, g_chrdev.devid, NULL, "chrdev");
    if (IS_ERR(g_chrdev.device)) {
        ret = PTR_ERR(g_chrdev.device);
        pr_err("device_create() failed: ret[%d]", ret);
        goto err_device_create;
    }

    pr_info("register_chrdev() chrdev module success");
    return 0;

err_device_create:
    class_destroy(g_chrdev.class);

err_class_create:
    cdev_del(&g_chrdev.cdev);

err_cdev_add:
    unregister_chrdev_region(g_chrdev.devid, 1);

err_alloc_chrdev_region:

    return ret;
}

void __exit chrdev_dev_exit(void)
{
    device_destroy(g_chrdev.class, g_chrdev.devid);

    class_destroy(g_chrdev.class);

    cdev_del(&g_chrdev.cdev);

    unregister_chrdev(g_chrdev.devid, "chrdev");

    pr_info("unregister_chrdev() chrdev module!\n");
}

module_init(chrdev_dev_init);
module_exit(chrdev_dev_exit);

MODULE_AUTHOR("RONOVO Surgical");
MODULE_ALIAS("chrdev");
MODULE_DESCRIPTION("RONOVO Surgical chrdev module");
MODULE_LICENSE("GPL");
