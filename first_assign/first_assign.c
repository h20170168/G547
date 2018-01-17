#include<linux/init.h>
#include<linux/module.h>

int hello_world(void)
{
	printk(KERN_ALERT "hello world");
	return 0;
}

void bye_world(void)
{
	printk(KERN_ALERT "bye world.");
	
}

module_init(hello_world);
module_exit(bye_world);
