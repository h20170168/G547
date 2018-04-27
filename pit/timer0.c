#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h>            
#include <linux/ioctl.h>
 
#define WR_VALUE _IOW('a',0,int*)
#define RD_VALUE _IOR('a',1,int*)


#define channel0 	0x40
#define channel1 	0x41
#define channel2 	0x42
#define controlword 	0x43
#define counter		0b0000010010101001

int i=0;
int delay;
unsigned long val,current_count_value;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev timer_cdev;

static int __init timer_driver_init(void);
static void __exit timer_driver_exit(void);
static int timer_open(struct inode *inode, struct file *file);
static int timer_release(struct inode *inode, struct file *file);
static long timer_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
      
       
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .open           = timer_open,
        .unlocked_ioctl = timer_ioctl,
        .release        = timer_release,
};



static int timer_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened!\n");
        return 0;
}

static int timer_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed!\n");
        return 0;
}


int sleep(void)    
{
	outb_p(0x34, controlword);  //setting 8254 to work as rate generator
	outb_p(counter & 0xff,channel0); //low byte loaded
	outb_p(counter>>8,channel0);       //high byte loaded
	return 0;
}

unsigned long read_counter_value(void)
{
	outb_p(0x00,controlword); //setting latch command
        val=inb(channel0);
	val=(val | (inb(channel0)<<8)); //reading current counter value
	return val;
}

static long timer_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{	
         switch(cmd) {
                case WR_VALUE:
                        delay=0;
                        copy_from_user(&i ,(unsigned int*) arg, sizeof(i));
			while(delay<i)  //creating delay set by the user
			{                        
				//sleep();
				current_count_value=read_counter_value();
				
				if(current_count_value==1)
				{delay++;
				}			
			}	                        
			break;
                case RD_VALUE:
			//reading current counter value
		     current_count_value=read_counter_value();
			printk(KERN_ALERT "value of timer=%d\n\n",current_count_value);                
              	      copy_to_user((unsigned int*) arg, &current_count_value, sizeof(current_count_value));
                      break;                    
        		}
        	return 0;
}


static int __init timer_driver_init(void)
{
       sleep(); //start 1ms rate generator
        if((alloc_chrdev_region(&dev, 0, 1, "timer_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));


 
        cdev_init(&timer_cdev,&fops);
        timer_cdev.owner = THIS_MODULE;
        timer_cdev.ops = &fops;

       
        if((cdev_add(&timer_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto cls1;
        }

      
        if((dev_class = class_create(THIS_MODULE,"timer0")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto cls1;
        }

        
        if((device_create(dev_class,NULL,dev,NULL,"timer0")) == NULL){
            printk(KERN_INFO "Cannot create the Device 1\n");
            goto devi;
        }
        printk(KERN_INFO "Device Driver Inserted!\n");
    return 0;

devi:
        class_destroy(dev_class);
cls1:
        unregister_chrdev_region(dev,1);
        return -1;
}


void __exit timer_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&timer_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Removed!\n");
}

module_init(timer_driver_init);
module_exit(timer_driver_exit);

 


MODULE_LICENSE("GPL");
MODULE_AUTHOR("vaibhav");
MODULE_DESCRIPTION("8254 pit");

