#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>


/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("A simple example for ioctl in a LKM");


/* Global variables for the threads */
static struct task_struct *kthreadTH1;
static struct task_struct *kthreadTH2;
static int th1 = 1, th2 = 2;


int thread_function(void *thread_nr) {
	unsigned int i = 0;
	int th_nr = *(int *) thread_nr;

	/*working loop */
	while(!kthread_should_stop()) {
		printk("kTrhead - Thread %d is executed! Counter val -> %d\n", th_nr, i++);
		msleep(th_nr * 1000); // milliseconds
	}
	printk("kThread - Thread %d finished execution\n", th_nr);
	return 0;
}


/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("kThread - Init threads\n");

	kthreadTH1 = kthread_create(thread_function, &th1, "kthread_1");
	if(kthreadTH1 != NULL){
		/* Let's start the thread */
		wake_up_process(kthreadTH1);
		printk("kthread - Thread 1 was created and is running now!\n");
	}
	else {
		printk("kthread - Thread 1 could not be created!\n");
		return -1;
	}
	kthreadTH2 = kthread_run(thread_function, &th2, "kthread_2");
	if(kthreadTH2 != NULL)
		printk("kthread - Thread 2 was created and is running now!\n");
	else {
		printk("kthread - Thread 2 could not be created!\n");
		kthread_stop(kthreadTH1);
		return -1;
	}
	printk("kthread - Both threads are running now!\n");

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	printk("kthread - Stop both threads\n");
	kthread_stop(kthreadTH1);
	kthread_stop(kthreadTH2);
}

module_init(ModuleInit);
module_exit(ModuleExit);
