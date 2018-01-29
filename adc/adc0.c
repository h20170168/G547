#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/slab.h>                 
#include<linux/uaccess.h>            
#include <linux/ioctl.h>
#include <linux/random.h>

#define WR_VALUE _IOW('a',0,int*)
#define RD_VALUE _IOR('a',1,int*)

int value = 0;
int number=0;
unsigned int i;
dev_t dev = 0;
static struct class *dev_class;
static struct cdev adc_cdev;

static int __init adc_driver_init(void);
static void __exit adc_driver_exit(void);
static int adc_open(struct inode *inode, struct file *file);
static int adc_release(struct inode *inode, struct file *file);
static ssize_t adc_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t adc_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long adc_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
      
       
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = adc_read,
        .write          = adc_write,
        .open           = adc_open,
        .unlocked_ioctl = adc_ioctl,
        .release        = adc_release,
};



static int adc_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Opened!\n");
        return 0;
}

static int adc_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Device File Closed!\n");
        return 0;
}

static ssize_t adc_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Read Function\n");
        return 0;
}
static ssize_t adc_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Write function\n");
        return 0;
}

static long adc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{	
         switch(cmd) {
                case WR_VALUE:
                        copy_from_user(&value ,(unsigned int*) arg, sizeof(value));
                        printk(KERN_INFO "channel selected = %d\n", value);
                        break;
                case RD_VALUE:
			number=value;
			switch(number)
			{     case 0: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
				      printk(KERN_INFO "adc value = %d\n", i);                  
                        	      copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
                              case 1: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
				      printk(KERN_INFO "adc value = %d\n", i);                   
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
			      case 2: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
				       printk(KERN_INFO "adc value = %d\n", i);                   
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
 			      case 3: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
                                       printk(KERN_INFO "adc value = %d\n", i);                   
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
			      case 4: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
                                       printk(KERN_INFO "adc value = %d\n", i);                   
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
                              case 5: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024; 
                                      printk(KERN_INFO "adc value = %d\n", i);                  
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
                              case 6: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024;
                                       printk(KERN_INFO "adc value = %d\n", i);                   
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
                              case 7: get_random_bytes(&i,sizeof(i)); 
				      i=i%1024; 
                                       printk(KERN_INFO "adc value = %d\n", i);                  
                        	       copy_to_user((unsigned int*) arg, &i, sizeof(i));
                                      break;
                                   }
        }
        return 0;
}





static int __init adc_driver_init(void)
{
        
        if((alloc_chrdev_region(&dev, 0, 1, "adc_Dev")) <0){
                printk(KERN_INFO "Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));


 
        cdev_init(&adc_cdev,&fops);
        adc_cdev.owner = THIS_MODULE;
        adc_cdev.ops = &fops;

       
        if((cdev_add(&adc_cdev,dev,1)) < 0){
            printk(KERN_INFO "Cannot add the device to the system\n");
            goto cls1;
        }

      
        if((dev_class = class_create(THIS_MODULE,"adc0")) == NULL){
            printk(KERN_INFO "Cannot create the struct class\n");
            goto cls1;
        }

        
        if((device_create(dev_class,NULL,dev,NULL,"adc0")) == NULL){
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

void __exit adc_driver_exit(void)
{
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&adc_cdev);
        unregister_chrdev_region(dev, 1);
    printk(KERN_INFO "Device Driver Removed!\n");
}

module_init(adc_driver_init);
module_exit(adc_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vaibhav jain");
MODULE_DESCRIPTION("adc");      
   
