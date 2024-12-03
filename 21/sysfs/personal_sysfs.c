#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/uaccess.h>

/* Meta information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("simple Linux driver with sysfs");

static struct class *personal_class;
static struct device *personal_device;

// Sysfs attribute: Read
static ssize_t personal_attr_show(struct device *dev, struct device_attribute *attr, char *buf)
{
    int value = 123; // Some value to display
    return sprintf(buf, "%d\n", value);
}

// Sysfs attribute: Write
static ssize_t personal_attr_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    int value;
    if (kstrtoint(buf, 10, &value) < 0)
        return -EINVAL;

    pr_info("Received value: %d\n", value);
    return count;
}

// Declare the sysfs attribute
static DEVICE_ATTR(personal_attr, S_IRUGO | S_IWUSR, personal_attr_show, personal_attr_store);

// Driver initialization function
static int __init personal_driver_init(void)
{
    int ret;

    // Create the class (Updated for compatibility with kernel 6.11.x)
    personal_class = class_create("personal_driver_class");
    if (IS_ERR(personal_class)) {
        pr_err("Failed to create class\n");
        return PTR_ERR(personal_class);
    }

    // Create the device
    personal_device = device_create(personal_class, NULL, 0, NULL, "personal_device");
    if (IS_ERR(personal_device)) {
        pr_err("Failed to create device\n");
        class_destroy(personal_class);
        return PTR_ERR(personal_device);
    }

    // Create the sysfs attribute
    ret = device_create_file(personal_device, &dev_attr_personal_attr);
    if (ret) {
        pr_err("Failed to create sysfs attribute\n");
        device_destroy(personal_class, 0);
        class_destroy(personal_class);
        return ret;
    }

    pr_info("Driver initialized and sysfs attribute created\n");
    return 0;
}

// Driver cleanup function
static void __exit personal_driver_exit(void)
{
    // Remove the sysfs attribute
    device_remove_file(personal_device, &dev_attr_personal_attr);

    // Destroy the device and class
    device_destroy(personal_class, 0);
    class_destroy(personal_class);

    pr_info("Driver exited and cleaned up\n");
}

module_init(personal_driver_init);
module_exit(personal_driver_exit);

