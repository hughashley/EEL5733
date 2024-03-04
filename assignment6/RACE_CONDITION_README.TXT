
this region of code takes the resource, checks the mode, then increments the number of threads/processes using the node. 
from the assignment prompt this mode should not allow more than one process to access the device node and can result in unexpected behavior such as a deadlock.
int e2_open(struct inode *inode, struct file *filp)
{
    struct e2_dev *devc = container_of(inode->i_cdev, struct e2_dev, cdev);
    filp->private_data = devc;
    down_interruptible(&devc->sem1);
    if (devc->mode == MODE1) {
        devc->count1++;
        up(&devc->sem1);
        down_interruptible(&devc->sem2);
        return 0;
    }
    else if (devc->mode == MODE2) {
        devc->count2++;
    }
    up(&devc->sem1);
    return 0;
}


mode 1 concurrent writing to the file relies on the open function to only allow one process to open the resource.
this functionality does not always prevent multiple threads or processes from opening the resource resulting in the potential of a a deadlock.

static ssize_t e2_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct e2_dev *devc;
	ssize_t ret = 0;
	devc = filp->private_data;
	down_interruptible(&devc->sem1);
	if (devc->mode == MODE1) {
		up(&devc->sem1);
        if (*f_pos + count > ramdisk_size) {
            printk("Trying to read past end of buffer!\n");
            return ret;
        }
        ret = count - copy_from_user(devc->ramdisk, buf, count);
	}
	else {
        if (*f_pos + count > ramdisk_size) {
            printk("Trying to read past end of buffer!\n");
            up(&devc->sem1);
            return ret;
        }
        ret = count - copy_from_user(devc->ramdisk, buf, count);
		up(&devc->sem1);
	}
	return ret;
}





the below region allows a deadlock state if multiple processes/threads are attempting to change the mode. both will wait for the other to free.
		case E2_IOCMODE2:
				down_interruptible(&(devc->sem1));
				if (devc->mode == MODE2) {
					up(&devc->sem1);
					break;
				}
				if (devc->count1 > 1) {
					while (devc->count1 > 1) {
						up(&devc->sem1);
					    wait_event_interruptible(devc->queue1, (devc->count1 == 1));
						down_interruptible(&devc->sem1);
					}
				}
				devc->mode = MODE2;
             devc->count1--;
             devc->count2++;
				up(&devc->sem2);
				up(&devc->sem1);
				break;
				
				
Similar to the region to change to mode 2, the region to change to mode 1 contains the same fault where if two processes are racing to change a mode it can result in a deadlock				
						case E2_IOCMODE1:
				down_interruptible(&devc->sem1);
				if (devc->mode == MODE1) {
				   up(&devc->sem1);
				   break;
				}
				if (devc->count2 > 1) {
				   while (devc->count2 > 1) {
				       up(&devc->sem1);
				       wait_event_interruptible(devc->queue2, (devc->count2 == 1));
				       down_interruptible(&devc->sem1);
				   }
				}
				devc->mode = MODE1;
		      devc->count2--;
		      devc->count1++;
				down_interruptible(&devc->sem2);
				up(&devc->sem1);
				break;
				