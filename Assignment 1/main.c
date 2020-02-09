#include<linux/module.h>
//#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/device.h>
#include"chardev.h"
#include<linux/cdev.h>
#include<linux/init.h>
#include<linux/random.h>
#include<linux/uaccess.h>

uint16_t rand,x;
int channel,alignment;
static dev_t first; //global variable for the first device driver
static struct cdev c_dev;
static struct class *cls;

//STEP 4 : Driver Callback functions

static int my_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "mychar: open()\n");
	return 0;
}		
		
static int my_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "mychar: close()\n");
	return 0;
}		
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "mychar: read()\n");
	//x=1024;
	printk(KERN_INFO "Value of alignment: %d\n", alignment);
	get_random_bytes(&rand,2);
	x=(rand%1024)&0x07FF;
	printk(KERN_INFO "Data is %d\n",x);
	if(alignment==0)
	{
	printk(KERN_INFO "the output is left aligned = %u \n",x);
	copy_to_user(buf,&x,2);}
	if(alignment==1)
	{
	x=x<<6;
	//printk(KERN_INFO "Data in alignment 1 is %d\n",x);
	printk(KERN_INFO"the output is right aligned = %u \n",x);
	copy_to_user(buf,&x,2);
	}
	
	return 0;
}

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	switch(cmd){
		case IOCTL_SET_CHANNEL:
		get_user(channel, (int __user*)arg);
		printk(KERN_INFO "SET CHANNEL  to %d \n", channel);
		break;
		case IOCTL_SET_ALIGNMENT:
		get_user(alignment, (int __user*)arg);
		printk(KERN_INFO "SET ALIGNMENT to %d\n", alignment);
		break;
		default:
			return -ENOTTY;
	}return 0;
}

static struct file_operations fops = 
				{
					.owner = THIS_MODULE,
					.open = my_open,
					.release = my_close,
					.read = my_read,
				.unlocked_ioctl= my_ioctl
					
				};		
					
static __init int mychar_init(void)  /*Constructor*/
{
	printk(KERN_INFO "adc registered");
	//  STEP1: reserve <major,minor>
	if (alloc_chrdev_region(&first, 4, 1, "8 channel ADC") < 0)
	{
		return -1;
	}

	///step 2 creation of device file
	if((cls=class_create(THIS_MODULE, "chardrv"))==NULL)
	{
		unregister_chrdev_region(first,1);
		return -1;	
	}
	if(device_create(cls, NULL, first, NULL, "adc8")==NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;
	}
	printk(KERN_INFO "ADC created\n");
	
	//STEP 3- Link fops and cdev to the device node
	cdev_init(&c_dev,&fops);
	if(cdev_add(&c_dev, first, 1)==1)
	{
		device_destroy(cls, first);
		class_destroy(cls);
		unregister_chrdev_region(first,1);
		return -1;	
	}
	return 0;


	//printk(KERN_INFO "<Major, Minor>: <%d, %d> \n", MAJOR(first), MINOR(first));
	//return 0;
}

static __exit void mychar_exit(void) /*Destructor*/
{
	cdev_del(&c_dev);
	device_destroy(cls,first);
	class_destroy(cls);	
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "ADC unregistered");
}

module_init(mychar_init);
module_exit(mychar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ksG");
MODULE_DESCRIPTION("Our first assignment on character driver");

