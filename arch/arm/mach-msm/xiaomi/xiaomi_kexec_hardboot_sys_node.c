/* arch/arm/mach-msm/xiaomi/xiaomi_kexec_hardboot_sys_node.c
 *
 * Interface to get kexec hardboot address
 *
 * Copyright (C) 2016 sndnvaps<sndnvaps@gmail.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kobject.h>
#include <linux/err.h>

#ifdef CONFIG_KEXEC_HARDBOOT
 #include <asm/kexec.h>
 #include <asm/memory.h>

#endif

struct kexec_dev{
    struct platform_device *pdev;
    struct kobject *kobj;
};
/*
//use for set the kexec address
static ssize_t kexec_store(struct device *dev,
                    struct device_attribute *attr,
                    const char *buf, size_t count) 
{
    printk("Hardboot Kexec!\n");

    return count;
}
*/

/*
 * 
 * how to use
 * cat /sys/devices/platform/kexec/kexec_state
 * output: 
 *      Hardboot Reseve at 0x88c00000
 */

static ssize_t kexec_state_show(struct device *dev,
                 struct device_attribute *attr,
                 char *buf)
{  
//    printk("cat debug buf\n");
#ifdef CONFIG_KEXEC_HARDBOOT
    return sprintf(buf,"Hardboot Reseve at 0x%lx\n",KEXEC_HB_PAGE_ADDR);
#else
    return 0;
#endif
}

//static DEVICE_ATTR(kexec,0777,kexec_state_show,kexec_store);

static DEVICE_ATTR(kexec_state,0777,kexec_state_show,NULL);
  
static struct kexec_dev *dev = NULL;  
  
static __devinit int kexec_probe(struct platform_device *ppdev){

    int ret = 0;

    dev->kobj = kobject_create_and_add("kexeckobj", NULL);
    if(dev->kobj == NULL){
        ret = -ENOMEM;
        goto kobj_err;
    }
  
    ret = sysfs_create_file(&dev->pdev->dev.kobj,&dev_attr_kexec_state.attr);
    if(ret < 0){
        goto file_err;
    }
    return 0;

file_err:
     kobject_del(dev->kobj);
kobj_err:
    return ret;
}
  
static struct platform_driver att_driver = {
    .probe = kexec_probe,
    .driver = {
        .owner = THIS_MODULE,
        .name = "kexec",
    },
};
  
static int __init kexec_init(void)  
{
    
    int ret = 0;

    dev = kzalloc(sizeof(struct kexec_dev),GFP_KERNEL);
    if(dev == NULL){
        printk("%s get dev memory error\n",__func__);
        return -ENOMEM;
    }
  
    dev->pdev = platform_device_register_simple("kexec", -1, NULL, 0);
    if(IS_ERR(dev->pdev)){
        printk("%s pdev error\n",__func__);
        return -1;
    }
  
    ret = platform_driver_register(&att_driver);
    if(ret < 0){
        printk("%s register driver error\n",__func__);
        return ret;
    }

    return 0;
}

static void __exit kexec_exit(void)
{  
  
}
module_init(kexec_init);
module_exit(kexec_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sndnvaps<sndnvaps@gmail.com>");