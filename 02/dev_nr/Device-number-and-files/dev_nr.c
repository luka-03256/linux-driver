#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("Registers a device nr. and implements some callback functions");

/**
 * @brief function is being called when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
    printk("dev_nr - open was called!\n");
    return 0;
}

static int driver_close(struct inode *device_file, struct file *instance) {
    printk("dev_nr - close was called!\n");
    return 0;
}

static struct file_operations fops = {  // Corrected structure type
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
};

#define MODULE_MAJOR 90

/**
 * @brief function call occurs when the module is loaded into the Kernel
 */
static int __init ModuleInit(void) {
    int retval;
    printk("Greeting message to kernel!\n");
    /* register device number */
    retval = register_chrdev(MODULE_MAJOR, "dev_nr", &fops);

    if (retval == 0) {
        printk("dev_nr - registered Device number Major: %d, Minor: %d\n", MODULE_MAJOR, 0);
    } else if (retval > 0) {
        printk("dev_nr - registered Device number Major: %d, Minor: %d\n", retval >> 20, retval & 0xfffff);
    } else {
        printk("Could not register device number!\n");
        return -1;
    }

    return 0;
}

/**
 * @brief function call occurs when the module is removed from the Kernel
 */
static void __exit ModuleExit(void) {
    unregister_chrdev(MODULE_MAJOR, "dev_nr");
    printk("Goodbye, kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

