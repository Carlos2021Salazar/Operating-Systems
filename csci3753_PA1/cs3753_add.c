#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>
asmlinkage long sys_cs3753_add(int number1,int number2,int* result)
{
	int sum = number1 + number2;	
	int *temp_result = &sum;
	copy_to_user(result, temp_result, 4);
	printk(KERN_ALERT "The summation is %d\n", sum);	
	return 0;
}
