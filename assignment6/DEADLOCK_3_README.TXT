Deadlock test 3 simulates an attempt to perform multiple reads while in mode 1.
this can cause the driver to lock up because there are multiple programs/threads with the device
node open in mode 1.