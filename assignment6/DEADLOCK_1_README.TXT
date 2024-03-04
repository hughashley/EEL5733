If the driver is in mode 1 and a second process attempts to open the device file,
the user space program will wait in a locked state for the other process to release the resource.
Depending on what the other process is, and what is done by it, this may never come.  Test 1 checks for this condition.