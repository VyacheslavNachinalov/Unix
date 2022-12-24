#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <asm/uaccess.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>
#include <linux/ioctl.h>
#include <stdio.h>

/*
#ifndef ”mydriverio.h”
#define ”mydriverio.h”
#endif
*/



int device_open (struct inode *myinode, struct file *myflie) {
    return 0;
}




int device_release (struct inode *, struct file *) {
    return 0;
}



static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t * offset) {
int bytes_read = 0; /* Number of bytes actually written to the buffer. If we're at the end of the message, * return 0 signifying end of file */
if (*msg_Ptr == 0) return 0;  /* Actually put the data into the buffer */
while (length && *msg_Ptr) {
/** The buffer is in the user data segment, not the kernel
* segment so "*" assignment won't work. We have to use
* put_user which copies data from the kernel data segment to
* the user data segment. */
put_user(*(msg_Ptr++), buffer++);
length--;
bytes_read++;
}
/* Most read functions return the number of bytes put into the buffer */
return bytes_read;
}



long myioctl (struct file *, unsigned int, unsigned long){
    return 0;
}



struct file_operations fops = {
.read = device_read,
.unlocked_ioctl = myioctl,
.open = device_open,
.release = device_release
};



int init_module(void) {
Major = register_chrdev(0, DEVICE_NAME, &fops);
if (Major < 0) {
printk(KERN_ALERT "Registering char device failed with %d\n", Major);
return Major;
}
printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
printk(KERN_INFO "the driver, create a dev file with\n");
printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major)
return 0;
}


void cleanup_module(void) {
unregister_chrdev(Major, DEVICE_NAME);
printk(KERN_ALERT "Сleanup_module OK \n");
}
