#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luka Stefanovic");
MODULE_DESCRIPTION("A simple implementation for the mmap syscall");

#define PERSONAL_MAJOR 64
#define DEVNAME "personal-dev"

static void *personal_data;

static ssize_t personal_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offs)
{
	int not_copied, to_copy = (len > PAGE_SIZE) ? PAGE_SIZE : len;
	not_copied = copy_to_user(user_buffer, personal_data, to_copy);
	return to_copy - not_copied;
}

static ssize_t personal_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offs)
{
	int not_copied, to_copy = (len > PAGE_SIZE) ? PAGE_SIZE : len;
	not_copied = copy_from_user(personal_data, user_buffer, to_copy);
	return to_copy - not_copied;
}

static int personal_mmap(struct file *file, struct vm_area_struct *vma)
{
	int status;

	vma->vm_pgoff = virt_to_phys(personal_data) >> PAGE_SHIFT;

	status = remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff, 
				 vma->vm_end - vma->vm_start, vma->vm_page_prot);
	if(status) {
		printk("personal_mmap - Error remap_pfn_range: %d\n", status);
		return -EAGAIN;
	}
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = personal_read,
	.write = personal_write,
	.mmap = personal_mmap,
};

int __init personal_init(void)
{
	int status;

	printk("personal_mmap - Hello!\n");
	personal_data = kzalloc(PAGE_SIZE, GFP_DMA);
	if(!personal_data)
		return -ENOMEM;

	printk("personal_mmap - I have allocated a page (%ld Bytes)\n", PAGE_SIZE);
	printk("personal_mmap - PAGESHIFT: %d\n", PAGE_SHIFT);

	status = register_chrdev(PERSONAL_MAJOR, DEVNAME, &fops);
	if(status < 0) {
		printk("personal_mmap - Error registering device number!\n");
		kfree(personal_data);
		return status;
	}

	return 0;
}

void __exit personal_exit(void)
{
	if(personal_data)
		kfree(personal_data);
	unregister_chrdev(PERSONAL_MAJOR, DEVNAME);
	printk("personal_mmap - Goodbye\n");
}

module_init(personal_init);
module_exit(personal_exit);
