#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
/*-init.h: required for initialization of module. 
-module.h: required to let kernel know that it is an LKM.
-linux/fs.h: required to get functions related to device driver coding. 
-asm/uaccess.h: required to get data from userspace to kernel and vice versa.*/
#define SIZE_BUFF 1024 
#define MY_DEVICE_NAME "Simple_Char_Driver" 

int openFlag = 0;
int closeFlag = 0;
char* buff_device;
ssize_t simple_char_driver_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
    int rbyte;
    int checkByte = SIZE_BUFF - *offset;
    // Check for the end of file
    if (checkByte == 0)
    {
        printk(KERN_ALERT "This is EOF");
        return checkByte;
    }
    // Gets bytes read
    rbyte = checkByte - copy_to_user(buffer, SIZE_BUFF + *offset, checkByte);
    printk(KERN_ALERT "Reading %d bytes\n", rbyte);
    //Will get end of file
    *offset += rbyte;
    return 0;
}
ssize_t simple_char_driver_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
    int writeBit = 0;
    int readTot;
    printk(KERN_ALERT "You are in %s function\n",__FUNCTION__);
    if((*offset >= SIZE_BUFF) || (length + *offset > SIZE_BUFF)) {return 1;}
    /*
    Begin to count edge case logic
    too few/much to write
    */
    if(*offset < writeBit)
    {
        *offset = writeBit;
        if(length + *offset  > SIZE_BUFF) {return 1;}
    }

    // to write, user writes in the userspace, so it needs to be copied from the userspace to the kernel space
    readTot = copy_from_user(buff_device + *offset, buffer, length);
    // copy_from_user we have dest-> buff_device, src-> userspace
    writeBit += length;
    printk(KERN_ALERT "Exiting %s function\n", __FUNCTION__);
    return length;
}

int simple_char_driver_open(struct inode *pinode, struct file *pfile)
{
    printk(KERN_ALERT "Device is open\n");
    openFlag++;
    //Global
    printk(KERN_ALERT "This device has been opened %d\n", openFlag);
    /*print the number of times this device has been opened until now*/
    return 0;
}

int simple_char_driver_close(struct inode *pinode, struct file *pfile)
{
    printk(KERN_ALERT "Device is closed\n");
    closeFlag++;
    //Global
    printk(KERN_ALERT "This device has been closed %d \n", closeFlag);
    /*print the number of times this device has been closed until now*/
    return 0;
}

loff_t simple_char_driver_seek(struct file *pfile, loff_t offset, int whence)
{
    loff_t posFlag = 0;
    printk(KERN_ALERT "You are in %s function\n",__FUNCTION__);

    switch(whence) 
    {

   		//Set, Current, End
        case 0: 
            posFlag = offset;
            break;
        case 1: 
            posFlag = pfile->f_pos + offset;
            break;
        case 2: 
            posFlag = SIZE_BUFF - offset;
            break;
        default:
            printk(KERN_ALERT "Whence isnt good!");
            break;
    }
    if(posFlag < 0) {posFlag = 0;} // if the position is < 0 set to 0

    if(posFlag > SIZE_BUFF) {posFlag = SIZE_BUFF;}
    //if the position is > SIZE_BUFF set SIZE_BUFF
    // pfile-> curr pos
    pfile->f_pos = posFlag;
    printk(KERN_ALERT "Position pointer for file is %d\n", posFlag);

    return 0;
}

struct file_operations simple_char_driver_file_operations = {              
    .owner   = THIS_MODULE,                     
    .open    = simple_char_driver_open,                  
    .release = simple_char_driver_close,                 
    .read    = simple_char_driver_read,                                      
    .write   = simple_char_driver_write,                 
    .llseek  = simple_char_driver_seek,                
};

static int simple_char_driver_init(void)
{
    printk(KERN_ALERT "You are in %s function\n",__FUNCTION__);
    //buff_device = kmalloc(SIZE_BUFF, GFP_KERNEL); 
    register_chrdev(240, MY_DEVICE_NAME, &simple_char_driver_file_operations);
    return 0;
}

static void simple_char_driver_exit(void)
{
    printk(KERN_ALERT "You are in %s function\n",__FUNCTION__);
    unregister_chrdev(240, MY_DEVICE_NAME);
}

module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);

