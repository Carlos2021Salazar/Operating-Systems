Contact Information
Name: Carlos Salazar
ID: 102274535
Email: Casa0641@colorado.edu

----->		Files:		<-----

LKM simple character device driver implementation
a. simple_char_driver.c

simple user space testing application for reading and writing to a device file. 
b. test_Driver.c

simple Makefile to compile simple_char_driver.c to a module object.
c. Makefile

---->	RUN PROGRAM 	<----
To install or uninstall the module:

sudo insmod simple_char_driver.ko
sudo rmmod simple_char_driver
To compile and run the test Program:
make -C /lib/modules/$(uname -r)/build M=$PWD modules
gcc test_Driver.c
./a.out
Begin to input/output
dmesg or,
sudo tail /var/log/syslog