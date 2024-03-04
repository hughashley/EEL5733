#include <linux/module.h>	/* for modules */
#include <linux/fs.h>		/* file_operations */
#include <linux/uaccess.h>	/* copy_(to,from)_user */
#include <linux/init.h>		/* module_init, module_exit */
#include <linux/slab.h>		/* kmalloc */
#include <linux/cdev.h>		/* cdev utilities */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/ioctl.h>
#include <linux/semaphore.h>

#define MYDEV_NAME "mycdrv"

#define ramdisk_size (size_t) (16 * PAGE_SIZE) // ramdisk size 

//NUM_DEVICES defaults to 3 unless specified during insmod
static int NUM_DEVICES = 3;
static int my_major, my_minor = 0;
static struct class *my_class;

#define CDRV_IOC_MAGIC 'Z'
#define ASP_CLEAR_BUF _IO(CDRV_IOC_MAGIC, 1)

module_param(NUM_DEVICES, int, S_IRUGO);


struct ASP_mycdrv {
	struct cdev dev;
	char *ramdisk;
	unsigned long size;
	struct semaphore sem;
	int devNo;

};


struct ASP_mycdrv *my_devices;


static int mycdrv_open(struct inode *inode, struct file *file)
{
	struct ASP_mycdrv *dev; /* device information */

	dev = container_of(inode->i_cdev, struct ASP_mycdrv, dev);
	file->private_data = dev; /* for other methods */
	if ( (file->f_flags & O_ACCMODE) == O_WRONLY) {
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;
		//my_trim(dev); /* ignore errors */
		up(&dev->sem);
	}

	return 0;          /* success */
}
static int mycdrv_release(struct inode *inode, struct file *file)
{
	return 0;
}
static loff_t mycdrv_llseek(struct file *file, loff_t off, int orig)
{
	struct ASP_mycdrv *dev = file->private_data;
	loff_t testpos;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	switch(orig) {
	  case SEEK_SET: /* SEEK_SET */
		testpos = off;
		break;

	  case SEEK_CUR: /* SEEK_CUR */
		testpos = file->f_pos + off;
		break;

	  case SEEK_END: /* SEEK_END */
		testpos = dev->size + off;
		break;

	  default: /* can't happen */
		return -EINVAL;
	}
	if (testpos < 0) return -EINVAL;
	testpos = testpos < ramdisk_size ? testpos : ramdisk_size;
	testpos = testpos >= 0 ? testpos : 0;
	file->f_pos = testpos;
	pr_info("Seeking to pos=%ld\n", (long)testpos);
	up(&dev->sem);
	return testpos;
}
long mycdrv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int retval;
	struct ASP_mycdrv *dev = file->private_data;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	pr_info("1\n");
	switch(cmd){

	case ASP_CLEAR_BUF:

		if (! capable (CAP_SYS_ADMIN))
			return -EPERM;
		kfree(dev->ramdisk);
		dev->ramdisk = kmalloc(ramdisk_size, GFP_ATOMIC);
		break;

	default:
		return -ENOTTY;
	}

	up(&dev->sem);
	return retval;


}

static ssize_t mycdrv_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos)
{
	struct ASP_mycdrv *dev = file->private_data;
	int nbytes;

	if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;

	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
		return 0;
	}

	nbytes = lbuf - copy_to_user(buf, dev->ramdisk + *ppos, lbuf);
	*ppos += nbytes;
	pr_info("\n READING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);
	up(&dev->sem);
	return nbytes;
}
static ssize_t mycdrv_write(struct file *file, const char __user * buf, size_t lbuf,
	     loff_t * ppos)
{
	int nbytes;
	struct ASP_mycdrv *dev = file->private_data;; /* device information */


	if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;

	if ((lbuf + *ppos) > ramdisk_size) {
		pr_info("trying to read past end of device,"
			"aborting because this is just a stub!\n");
		return 0;
	}
	nbytes = lbuf - copy_from_user(dev->ramdisk + *ppos, buf, lbuf);
	*ppos += nbytes;
	pr_info("\n WRITING function, nbytes=%d, pos=%d\n", nbytes, (int)*ppos);

	up(&dev->sem);
	return nbytes;
}
static const struct file_operations mycdrv_fops = {
	.owner = THIS_MODULE,
	.read = mycdrv_read,
	.write = mycdrv_write,
	.open = mycdrv_open,
	.release = mycdrv_release,
	.llseek = mycdrv_llseek,
	.unlocked_ioctl = mycdrv_ioctl,
};


static void my_setup_cdev(struct ASP_mycdrv *dev, int index)
{
	int err;
	dev->devNo = MKDEV(my_major, my_minor + index);

	cdev_init(&dev->dev, &mycdrv_fops);
	dev->dev.owner = THIS_MODULE;
	dev->dev.ops = &mycdrv_fops;
	printk(KERN_NOTICE "adding: mycdrv%d major: %d minor: %d\n", index, my_major, my_minor+index);
	err = cdev_add(&dev->dev, dev->devNo, 1);


	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding mycdrv%d", err, index);
}
void my_cleanup_module(void)
{

	if (my_devices) {
		for (int i = 0; i < NUM_DEVICES; i++) {
			dev_t devno = MKDEV(my_major, my_minor+i);
			cdev_del(&my_devices[i].dev);
			unregister_chrdev_region(devno, NUM_DEVICES);
			device_destroy(my_class, my_devices[i].devNo);
			printk(KERN_NOTICE "removing: mycdrv%d major: %d minor: %d\n", i, my_major, my_minor+i);

		}
		class_destroy(my_class);
		kfree(my_devices);

	}

}

static int __init my_init(void)
{

	int result, i;
	dev_t first = 0;
	my_class = class_create(THIS_MODULE, "my_class");



	if (my_major) {
		first = MKDEV(my_major, my_minor);

		result = register_chrdev_region(first, NUM_DEVICES, MYDEV_NAME);
	} else {
		result = alloc_chrdev_region(&first, my_minor, NUM_DEVICES,
				MYDEV_NAME);
		my_major = MAJOR(first);
	}
	if (result < 0) {
		printk(KERN_WARNING "mycdrv: can't get major %d\n", my_major);
		return result;
	}



	my_devices = kmalloc(NUM_DEVICES * sizeof(struct ASP_mycdrv), GFP_KERNEL);
	if (!my_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}
	memset(my_devices, 0, NUM_DEVICES * sizeof(struct ASP_mycdrv));

	for (i = 0; i < NUM_DEVICES; i++) {

		my_devices[i].ramdisk = kmalloc(ramdisk_size, GFP_KERNEL);
		my_setup_cdev(&my_devices[i], i);
		sema_init(&my_devices[i].sem, 1);
		device_create(my_class, NULL, my_devices[i].devNo, NULL, "mycdrv%d", i);

	}


	return 0;

	fail:
		my_cleanup_module();
		return result;
}


static void __exit my_exit(void)
{
my_cleanup_module();
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Hugh Ashley");
MODULE_LICENSE("GPL v2");
