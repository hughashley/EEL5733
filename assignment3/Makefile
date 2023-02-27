# EEL4732/5733 Advanced System Programming
# Assignment 2 Makefile
# to run this, type $make 
# the input should be processed with input/output redirection:
#			./email_filter < input.txt
#			./calendar_filter < ${output_from_email_filter}.txt
#			./location_updater < input.txt
# These are not recommended:
#		./email_filter input.txt
#		fopen("input.txt","r"); // in ${email_filter calendar_filter location_updater}.c 
# BTW: fwrite is totally fine as we need it to generate output.txt file.

all:	eft_handler

eft_handler: eft_handler.c
	gcc -o  eft_handler eft_handler.c -pthread 
clean:
	rm -rf eft_handler
