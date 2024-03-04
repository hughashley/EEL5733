Assignment 5
Hugh Ashley
EEL 5733

compiled and tested on:

Linux version 5.19.0-38-generic (buildd@bos02-arm64-044) (aarch64-linux-gnu-gcc-12 (Ubuntu 12.2.0-3ubuntu1) 12.2.0,
GNU ld (GNU Binutils for Ubuntu) 2.39) #39-Ubuntu SMP PREEMPT_DYNAMIC Fri Mar 17 17:36:31 UTC 2023

program complies with the instructions that were provided below for this assignment.

Instructions on how the module will be tested :

Copy the files char_driver.c, Makefile and userapp.c to a virtual linux machine
and follow the following steps:

1) Compile driver module : $ make

2) Load module : $ sudo insmod char_driver.ko NUM_DEVICES=<num_devices>

3) Test driver :
	1) Compile userapp : $ make app
	2) Run userapp : $ sudo ./userapp <device_number>			
		where device_number identifies the id number of the device to be tested.   

	Note : userapp has to be executed with sudo privilege as the device files
		   in /dev/ are created in the driver with root privileges.
		   
4) Unload module : $ sudo rmmod char_driver
