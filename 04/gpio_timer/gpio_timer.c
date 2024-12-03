#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("A simple kernel module to control GPIO 4 and a timer");

static struct timer_list personal_timer;  // Renamed timer structure

// Timer callback function
static void timer_callback(struct timer_list *t)
{
    printk(KERN_INFO "Timer expired, turning off LED\n");
    gpio_set_value(4, 0);  // Turn off the LED (GPIO 4)
}

// Module initialization function
static int __init ModuleInit(void)
{
    printk(KERN_INFO "Hello, Kernel!\n");

    // GPIO 4 initialization
    if (gpio_request(4, "rpi-gpio-4")) {
        printk(KERN_ERR "Can not allocate GPIO 4\n");
        return -1;
    }

    // Set GPIO 4 direction to output
    if (gpio_direction_output(4, 0)) {
        printk(KERN_ERR "Can not set GPIO 4 to output!\n");
        gpio_free(4);  // Free GPIO 4 before returning
        return -1;
    }

    // Turn LED on (GPIO 4)
    gpio_set_value(4, 1);
    printk(KERN_INFO "LED turned on (GPIO 4)\n");

    // Initialize and start the personal_timer
    timer_setup(&personal_timer, timer_callback, 0);
    mod_timer(&personal_timer, jiffies + msecs_to_jiffies(1000));  // Timer set to expire in 1 second

    return 0;
}

// Module exit function
static void __exit ModuleExit(void)
{
    del_timer_sync(&personal_timer);  // Delete timer if module is removed
    gpio_set_value(4, 0);             // Turn off LED
    gpio_free(4);                     // Free GPIO 4
    printk(KERN_INFO "Goodbye, Kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

