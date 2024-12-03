#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("Demonstration for lists in the kernel");

struct personal_data {
	struct list_head list;
	char text[64];
};

LIST_HEAD(personal_list);

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init personal_init(void) {
	struct personal_data *tmp, *next;
	struct list_head *ptr;

	printk("personal_list - Hello, Kernel!\n");

	/* Let's create a list with three elements */
	tmp = kmalloc(sizeof(struct personal_data), GFP_KERNEL);
	strcpy(tmp->text, "Hello World");
	list_add_tail(&tmp->list, &personal_list);

	tmp = kmalloc(sizeof(struct personal_data), GFP_KERNEL);
	strcpy(tmp->text, "Second Element");
	list_add_tail(&tmp->list, &personal_list);

	tmp = kmalloc(sizeof(struct personal_data), GFP_KERNEL);
	strcpy(tmp->text, "and the last element");
	list_add_tail(&tmp->list, &personal_list);

	list_for_each_prev(ptr, &personal_list) {
		tmp = list_entry(ptr, struct personal_data, list);
		printk("personal_list - Element text: %s\n", tmp->text);
	}

	/* Free memory */
	list_for_each_entry_safe(tmp, next, &personal_list, list) {
		list_del(&tmp->list);
		kfree(tmp);
	}
	printk("personal_list - Freeing memory done!\n");

	return 0;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit personal_exit(void) {
	printk("personal_list - Goodbye, Kernel\n");
}

module_init(personal_init);
module_exit(personal_exit);
