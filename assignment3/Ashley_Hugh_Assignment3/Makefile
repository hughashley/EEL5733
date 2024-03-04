# EEL4732/5733 Advanced System Programming
# Assignment 3 Makefile
# to run this, type $make 
# the input should be processed with input/output redirection:
#			./eft_handler {max threads} < input.txt
# These are not recommended:
#		./eft_handler input.txt
#	


all:	eft_handler

eft_handler: eft_handler.c
	gcc -o  eft_handler eft_handler.c -pthread 
clean:
	rm -rf eft_handler
