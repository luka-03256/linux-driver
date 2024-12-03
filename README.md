# Linux Driver Tutorial

This repository contains a series of examples for learning how to write and work with Linux Kernel modules and drivers.

## Content

In this repository, you can find examples for the following topics:

1. **Simple Kernel Module**  
   Learn how to write a basic kernel module that can be inserted and removed from the kernel.

2. **Device Numbers and Device Files**  
   Understand how to allocate device numbers and create device files for communication between user space and kernel space.

3. **Create Device File in Driver and Callbacks**  
   Learn to create a device file within the driver and implement necessary callback functions like open, close, and read.

4. **Timer in Linux Kernel Modules**  
   Learn how to implement a timer in a kernel module to perform periodic tasks.

5. **High Resolution Timer in Linux Kernel Modules**  
   Use high-resolution timers for tasks requiring more precise timing in your kernel module.

6. **Using Parameters in a Linux Kernel Module**  
   Pass parameters to kernel modules at load time to customize behavior.

7. **IOCTL in a Linux Kernel Module**  
   Learn to implement IOCTL commands for communication between user space and kernel space for device control.

8. **Threads in a Linux Kernel Module**  
   Implement kernel threads for performing tasks concurrently within the kernel.

9. **Sending a Signal from a Linux Kernel Module to a Userspace Application**  
   Learn how to send signals from the kernel to user-space applications to notify or trigger events.

10. **The poll Callback**  
   Implement the `poll` callback to allow user-space programs to check if a device is ready for reading or writing.

11. **Waitqueues in a Linux Kernel Module**  
   Use waitqueues to make the kernel thread sleep until some condition is met (e.g., data becomes available).

12. **Create Procfs Entries from a Linux Kernel Module**  
   Learn how to create entries in `/proc` filesystem to expose kernel information to user space.

13. **Create Sysfs Entries from a Linux Kernel Module**  
   Create sysfs entries to interact with kernel parameters through the `/sys` filesystem.

14. **Dynamical Memory Management in a Linux Kernel Module**  
   Learn to manage memory dynamically within the kernel, including allocation and deallocation.

15. **Misc Device**  
   Work with a miscellaneous device to interact with kernel resources in a simpler way.

16. **Mutex for Exclusive Access to Shared Resources**  
   Learn how to use mutexes to ensure exclusive access to shared kernel resources, preventing race conditions.

17. **Completions for Synchronization**  
   Use completion variables to synchronize kernel tasks and control execution order.

18. **Accessing Files from a Linux Driver**  
   Learn how to interact with files from within the kernel, such as opening, reading, and writing to them.

19. **The mmap Callback**  
   Implement the `mmap` callback to allow user-space programs to directly map kernel memory to user space.

20. **Linked Lists**  
   Learn to implement and manage linked lists within the kernel, useful for managing dynamic data structures.

21. **Registering Device Numbers, Read and Write Callback in Character Devices (Take 2)**  
   Explore more advanced techniques in registering device numbers and implementing read/write callbacks in character devices.

22. **Private Data in `struct file`**  
   Learn how to store private data in `struct file` for maintaining state information specific to a file or device.

23. **Sysfs Class**  
   Understand how to create a sysfs class, a way to organize and manage sysfs entries logically.

24. **Kernel Log Levels**  
   Learn how to use different kernel log levels (e.g., `KERN_INFO`, `KERN_ERR`) for logging messages with different severities.

---

## Requirements

### Installing Kernel Headers

To build and use these drivers, you must have the appropriate kernel headers installed for your distribution. On Debian-based systems (like Ubuntu), you can install the kernel headers by running:

```bash
  sudo apt-get install linux-headers-$(uname -r)
```


## Step-by-Step Guide to Install Everything

1\. Install Required
Dependencies You'll need to install some tools and libraries to compile
kernel modules. The most common dependencies are the kernel headers.

On Debian-based distributions (like Ubuntu or Kali), run the following
command:

```bash
sudo apt update sudo apt install build-essential
linux-headers-$(uname -r) make gcc build-essential
```
A meta-package that installs essential tools for building packages. linux-headers-$(uname -r): Installs the kernel headers for your current kernel version. 
make: A build automation tool that will be used to compile the kernel module.
gcc: The GNU Compiler Collection. 
<br/><br/><br/>

2\. Clone the Repository If you haven't
cloned the repository yet, use the following command:

Copy code
```bash  
git clone https://github.com/luka-03256/linux-driver.git
```
<br/><br/><br/>

3\. Navigate to the Desired Module Directory After cloning the
repository, navigate to the directory of the module you want to work on.
For example:

Copy code
```bash  
cd linux-driver/01 # for the Simple Kernel Module 
``` 

<br/><br/><br/>

4.Build the Kernel Module In each tutorial directory, you'll find a
Makefile. Run the following command to compile the kernel module:

Copy code
```bash 
    make
```
This will compile the .c source code files into a .ko (kernel object) file.
<br/><br/><br/>

5\. Insert the Kernel Module After building the kernel module, insert it
into the kernel using insmod:

Copy code

```bash
    sudo insmod module_name.ko Replace module_name.ko 
```
with the name of the kernel object file generated from the make command
(e.g., simple_module.ko).
<br/><br/><br/>

6\. Check the Kernel Logs After inserting the kernel module, you can
check the kernel logs to confirm it was loaded successfully:

Copy code

```bash  
    dmesg | tail
```
This will display the last few lines of kernel messages.
<br/><br/><br/>

7\. Remove the Kernel Module If you need to remove the module (e.g., to
test or reload it), use the rmmod command:

Copy code

```bash  
    sudo rmmod module_name 
```
<br/><br/><br/>


8. Use the Module Many of the
examples will involve creating device files or interacting with the
module from user-space. For example, to create a device file:

Copy code

```bash 
    sudo mknod /dev/my_device c major_number minor_number
```

Replace major_number and minor_number with the actual numbers assigned to your device.
<br/><br/><br/>


9\. Automate Module Loading (Optional) If you want your kernel module to
load automatically at boot, you can add it to the /etc/modules file or
create a systemd service. This step is optional and only needed if you
want the module to persist across reboots.



## Common Issues and Troubleshooting Kernel Mismatch If you encounter
Issues during the make process due to mismatched kernel versions, check
your kernel version:

Copy code

```bash
    uname -r 
```
Make sure the headers for your running kernel version are installed:

Copy code

```bash 
    sudo apt install linux-headers-$(uname -r) 
```

Missing Kernel
Headers If you see an error like fatal error: linux/module.h: No such
file or directory, it means the kernel headers are missing. Install them
with:

Copy code

```bash 
    sudo apt install linux-headers-$(uname -r)
```

Permissions:<br/>
For commands like insmod, rmmod, and creating device files, you need
root privileges. Use sudo to ensure you have the necessary permissions.


