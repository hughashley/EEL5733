# EEL4732/5733 Advanced System Programming
# Assignment 1 Makefile
# to run this, type $make 
# the input should be processed with input/output redirection:
#			./email_filter < input.txt
#			./calendar_filter < ${output_from_email_filter}.txt
#			./location_updater < input.txt
# These are not recommended:
#		./email_filter input.txt
#		fopen("input.txt","r"); // in ${email_filter calendar_filter location_updater}.c 
# BTW: fwrite is totally fine as we need it to generate output.txt file.

all:	email_filter calendar_filter location_updater

email_filter: email_filter.c
	gcc -o email_filter email_filter.c
calendar_filter: calendar_filter.c
	gcc -o calendar_filter calendar_filter.c
location_updater: location_updater.c
	gcc -o location_updater location_updater.c
clean:
	rm -rf email_filter calendar_filter location_updater
