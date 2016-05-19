#include <linux/init.h>		//包含初始化和清楚函数
#include <linux/module.h>	//包含可装载模块需要的大量符号和函数
#include <linux/types.h>	//dev_t结构,MKDEV,MAJOR,MINOR宏

#include <linux/moduleparam.h>//参数
#include <linux/sched.h>	//包含current 定义 
#include <linux/kernel.h>	//printk
#include <linux/delay.h>	//mdelay udelay 
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/mm.h>

#include <linux/fs.h>		//register_chardev_region alloc_chardev_region unregiser_chrdev_region分配设备编号 file_operations 结构,register_chrdev
#include <linux/cdev.h>		//cdev结构

#include <asm/uaccess.h>	//copy_from_user copy_to_user

#include <linux/gpio.h>
#include "../../arch/arm/mach-omap2/mux.h"

#include "tt.h"

#define TT_MAJOR	200

#define GPIO61_LOW	0x6000
#define GPIO61_HIGH	0x6001

#define GPIO43_LOW	0x6002
#define GPIO43_HIGH	0x6003

#define GPIO15_READ	0x6666


static dev_t tt_major = TT_MAJOR;

struct tt_dev
{
	struct cdev cdev;/**/
	unsigned char mem[4];
};

struct tt_dev devt;

MODULE_LICENSE("Dual BSD/GPL");

static void tt_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	unsigned int tmp, i;
	if (arg)
		i = copy_from_user(&tmp,(void *)arg,sizeof(tmp));
	switch(cmd)
	{
		case GPIO61_LOW:
			gpio_set_value (61, 0);
			break;
		case GPIO61_HIGH:
			gpio_set_value (61, 1);
			break;
		case GPIO43_LOW:
			gpio_set_value (43, 0);
			break;
		case GPIO43_HIGH:
			gpio_set_value (43, 1);
			break;
		case GPIO15_READ:
/*			printk ("gpio15= %d\n", gpio_get_value (15));*/
			for (i = 0; i < 192; i++)
			printk ("D%d=%x \n", i, omap_mux_get_gpio(i));
			break;
		default:break;
	}

/*	printk ("gpio61%d gpio43%d\n", 
			gpio_get_value(61), gpio_get_value(43));
			*/
	return ;
}

struct file_operations tt_fops = {
	.owner = THIS_MODULE,
	//	.llseek	=	tt_llseek,
	//	.read	=	tt_read,
	//	.write	=	tt_write,	
	ioctl:tt_ioctl,
	//	.release	=	tt_release,	
};

static void tt_setup_cdev()
{
	int err, devno = MKDEV(tt_major, 0);

	cdev_init(&devt.cdev, &tt_fops);
	devt.cdev.owner = THIS_MODULE;
	devt.cdev.ops = &tt_fops;
	err = cdev_add(&devt.cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding tt", err);
}


static __init int tt_init(void)
{
	int result;

	dev_t devno = MKDEV(tt_major, 0);

	if (tt_major)
		result = register_chrdev_region(devno, 1, "tt");
	else {
		result = alloc_chrdev_region(&devno, 0, 1, "tt");
		tt_major = MAJOR(devno);
	}

	if (result < 0)
		return result;

	tt_setup_cdev();

	return 0;
}

static __exit void tt_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
	cdev_del(&devt.cdev);
	unregister_chrdev_region(MKDEV(tt_major,0), 1);
}

module_init(tt_init);
module_exit(tt_exit);

