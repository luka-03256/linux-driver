#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("Simple LKM for demonstration of parameters use");


/* Kernel Module's Paramerters*/
static unsigned int gpio_nr = 12;
static char *dev_name = "testdevice";

module_param(gpio_nr, uint, S_IRUGO);
module_param(dev_name, charp, S_IRUGO);

MODULE_PARM_DESC(gpio_nr, "Nr. of GPIO to use in this LKM");
MODULE_PARM_DESC(dev_name, "Device name to use in this LKM");

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("gpio_nr = %u\n", gpio_nr);
	printk("device_name = %s\n", dev_name);
	printk("Greetings, Kernel!\n");
	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("Goodbye, Kernel\n");
}



module_init(ModuleInit);
module_exit(ModuleExit);

