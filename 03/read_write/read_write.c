#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("Register a device number and implement some callback functions");

/* data buffer */
static char buff[255];
static int buff_ptr; /* buffer pointer */


/* device && device class variables */
static dev_t device_number;
static struct class *personal_class;
static struct cdev personal_device;

#define DRIVER_NAME "dummy"
#define DRIVER_CLASS "PersonalModuleClass"

/* @brief read data out of the buffer */
static ssize_t driver_read(struct file *FILE, char *user_buffer, size_t count, loff_t *offs) {
	int to_copy,not_copied,calculated;
	/* Get amount of data to copy */
	to_copy = min((unsigned long)count, (unsigned long)buff_ptr);

	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, buff, to_copy);



	/* Calculate data */
	calculated = to_copy - not_copied;

    return calculated;
}

/* @brief write data to buffer */
static ssize_t driver_write(struct file *FILE, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, calculated;

	/* Get amount of data to copy */
	to_copy = min((unsigned long)count, (unsigned long)buff_ptr);

	/* copy data to user */
	not_copied = copy_from_user(buff, user_buffer, to_copy);
	buff_ptr = to_copy;

	/* calculate data */
	calculated = to_copy - not_copied;

	return calculated;	
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
    printk("dev_nr - close was called!\n");
    return 0;
}

static struct file_operations fops = {  // Corrected structure type
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
};

#define MODULE_MAJOR 90

/**
 * @brief function call occurs when the module is loaded into the Kernel
 */
static int __init ModuleInit(void) {
	//int retval;
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if( alloc_chrdev_region(&device_number, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", device_number >> 20, device_number & 0xfffff);

	/* Create device class */
	if((personal_class = class_create(DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(personal_class, NULL, device_number, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&personal_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&personal_device, device_number, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}

	return 0;
AddError:
	device_destroy(personal_class, device_number);
FileError:
	class_destroy(personal_class);
ClassError:
	unregister_chrdev_region(device_number, 1);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	cdev_del(&personal_device);
	device_destroy(personal_class, device_number);
	class_destroy(personal_class);
	unregister_chrdev_region(device_number, 1);
	printk("Goodbye, Kernel\n");
}


module_init(ModuleInit);
module_exit(ModuleExit);

