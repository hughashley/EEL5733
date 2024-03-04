# EEL5733 Advanced System Programming
# Assignment 4 Makefile
# to run this, type $make 
# the input should be processed with input/output redirection:
#
#			./location_updater {buffer size} < input.txt


all:	location_updater

location_updater: location_updater.c
	gcc -o  location_updater location_updater.c -pthread 
clean:
	rm -rf location_updater
