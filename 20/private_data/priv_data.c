#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("example using private data for file descriptors");

#define DEVNR 64
#define DEVNRNAME "personal_cdev"

static struct cdev personal_cdev;

static int personal_open(struct inode *inode, struct file *file)
{
	char *ptr;
	ptr = kzalloc(256, GFP_KERNEL);
	if(!ptr)
		return -ENOMEM;
	printk("priv_data - Allocated 256 bytes of memory\n");
	file->private_data = ptr;
	return 0;
}

static ssize_t personal_read(struct file *file, char __user *user_buffer, size_t len, loff_t *off)
{
	char *buffer;
	int not_copied, to_copy = (len < 256) ? len : 256;

	buffer = (char *) file->private_data;
	if(!buffer)
		return -1;

	printk("priv_data - personal_Read called, *off: %lld\n", *off);

	if(*off >= to_copy) 
		return 0;

	not_copied = copy_to_user(user_buffer, buffer, to_copy);

	*off += to_copy - not_copied;

	return to_copy - not_copied;
}

static ssize_t personal_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *off)
{
	char *buffer;
	int not_copied, to_copy = (len < 256) ? len : 256;

	buffer = (char *) file->private_data;
	if(!buffer)
		return -1;

	printk("priv_data - personal_write called\n");


	not_copied = copy_from_user(buffer, user_buffer, to_copy);


	return to_copy - not_copied;
}

static int personal_close(struct inode *inode, struct file *file) 
{
	char *ptr;
	ptr = (char *) file->private_data;
	if(ptr)
		kfree(ptr);
	printk("priv_data - freed 256 bytes\n");
	return 0;
}

struct file_operations fops = {
	.open = personal_open,
	.release = personal_close, 
	.read = personal_read,
	.write = personal_write,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init personal_init(void) {
	int status;
	dev_t devnr = MKDEV(DEVNR, 0);

	status = register_chrdev_region(devnr, 1, DEVNRNAME);
	if(status < 0) {
		printk("priv_data - Error regiserting device number!\n");
		return status;
	}

	cdev_init(&personal_cdev, &fops);
	personal_cdev.owner = THIS_MODULE;

	status = cdev_add(&personal_cdev, devnr, 1);
	if(status < 0) {
		printk("priv_data - Error adding cdev\n");
		unregister_chrdev_region(devnr, 1);
		return status;
	}
	printk("priv_data - Registered device number 64 0, created cdev\n");

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit personal_exit(void) {
	dev_t devnr = MKDEV(DEVNR, 0);
	unregister_chrdev_region(devnr, 1);
	cdev_del(&personal_cdev);
	printk("priv_data - Registered device number 64 0, created cdev\n");
}

module_init(personal_init);
module_exit(personal_exit);
