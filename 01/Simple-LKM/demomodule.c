#include <linux/module.h>
#include <linux/init.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("demo LKM");

/**
 * @brief function call occurs when the module is loaded into the Kernel
 */
static int __init ModuleInit(void) {
    printk("Greeting message to kernel!\n");
    return 0;
}

/**
 * @brief function call occurs when the module is removed from the Kernel
 */
static void __exit ModuleExit(void) {
    printk("Goodbye, kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

