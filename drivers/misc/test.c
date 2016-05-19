#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init omap_dmatest_init(void)
{
	int i;
	int data_addr2;


	request_mem_region(0x90000000, 256*1024*1024, "tt_data");
	data_addr2 = (unsigned int )ioremap(0x90000000, 256*1024*1024);

	printk("data2=%x\n", data_addr2);

	return 0;
}

static void __exit tt_exit(void)
{
	return;
}

MODULE_AUTHOR("TT  <TT@163.com>");
MODULE_DESCRIPTION("OMAP tt test module");
MODULE_LICENSE("Dual BSD/GPL");
module_init(tt_init);
module_exit(tt_exit);
