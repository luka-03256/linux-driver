#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("A simple LKM using High Resolution Timers");

/* hr timer */
static struct hrtimer personal_hr_timer;
u64 start_t;

static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer) {
    u64 now_t = jiffies;
    printk("start_t - now_t = %u\n", jiffies_to_msecs(now_t - start_t));
    return HRTIMER_NORESTART;
}

static int __init ModuleInit(void) {
    printk("Hello, Kernel!\n");
    hrtimer_init(&personal_hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    personal_hr_timer.function = &test_hrtimer_handler;
    start_t = jiffies;
    hrtimer_start(&personal_hr_timer, ms_to_ktime(100), HRTIMER_MODE_REL);
    return 0;
}

static void __exit ModuleExit(void) {
    hrtimer_cancel(&personal_hr_timer);
    printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

