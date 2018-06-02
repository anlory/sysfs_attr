#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>





static ssize_t device_attr_show(struct device_driver *driver, char *buf)
{
	pr_info("Enter...");
	return sprintf(buf, "%s\n", driver->name);

}
static ssize_t device_attr_store(struct device_driver *driver, const char *buf,
		 size_t count)
{
	unsigned int val;
	pr_info("Enter...");
    //需要获取的信息从buf中拿。
    sscanf(buf, "%u", &val);
    pr_info("ATTR: %s   %d\n", driver->name,val);

	return count;

}

ssize_t kobject_attr_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	return sprintf(buf, "%s\n", attr->attr.name);
}
ssize_t kobject_attr_store(struct kobject *kobj, struct kobj_attribute *attr,
		 const char *buf, size_t count)
{
	unsigned int val;
    //需要获取的信息从buf中拿。
    sscanf(buf, "%u", &val);
    pr_info("ATTR: %s   %d\n", attr->attr.name,val);

	return count;

}


static struct kobj_attribute node_1 =__ATTR(node_1, S_IWUSR|S_IRUGO, kobject_attr_show, kobject_attr_store);
static struct kobj_attribute node_2 =__ATTR(node_2, S_IWUSR|S_IRUGO, kobject_attr_show, kobject_attr_store);





static DEVICE_ATTR(node_1, S_IWUSR | S_IRUGO, device_attr_show, device_attr_store);
static DEVICE_ATTR(node_2, S_IWUSR | S_IRUGO, device_attr_show, device_attr_store);
static DEVICE_ATTR(node_3, S_IWUSR | S_IRUGO, device_attr_show, device_attr_store);
static DEVICE_ATTR(node_4, S_IWUSR | S_IRUGO, device_attr_show, device_attr_store);



static struct attribute *sysfs_xb_attr[] = {
    &dev_attr_node_1.attr,
    &dev_attr_node_2.attr,
    &dev_attr_node_3.attr,
    &dev_attr_node_4.attr,
    NULL
};

static struct attribute *sysfs_xb_attr1[] = {
    &node_1.attr,
    &node_2.attr,
    NULL
};

static const struct attribute_group sysfs_xb_attr_group = {
    .attrs = sysfs_xb_attr,
	.name  = "dev_xb",
};

static const struct attribute_group sysfs_xb_attr_group1 = {
    .attrs = sysfs_xb_attr1,
	.name = "kobj_xb",
};





static struct device_attribute misc_xb_attrs[] = {
	__ATTR(node_5, S_IRUGO | S_IWUSR, device_attr_show, device_attr_store),
	__ATTR(node_6, S_IRUGO | S_IWUSR, device_attr_show, device_attr_store),
	__ATTR_NULL,
};


static int misc_xb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)  
{  
    printk ("misc  misc_xb_ioctl\n");  
    return 0;  
}  
  
static int misc_xb_open(struct inode *inode, struct file *filp)  
{  
    printk ("misc   misc_xb_open\n");  
    return 0;  
}  
static int misc_xb_release(struct inode *inode, struct file *filp)  
{  
    printk ("misc misc_xb_release\n");  
    return 0;  
}  
  
static ssize_t misc_xb_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)  
{  
      printk ("misc misc_xb_read\n");  
      return 1;  
}  

static struct file_operations misc_xb_fops =  
{  
    .owner   = THIS_MODULE,  
    .read    = misc_xb_read,  
    .compat_ioctl   = misc_xb_ioctl,  
    .open    = misc_xb_open,  
    .release = misc_xb_release  
};  


static struct miscdevice misc = {  
    .minor = MISC_DYNAMIC_MINOR,  
    .name = "xb_attr", //此名称将显示在/dev目录下面  
    .fops = &misc_xb_fops,  
};  


static int __init device_attr_init(void)
{
	int i;

	misc_register(&misc);
	//1.数组形式创建
	sysfs_create_group(&misc.this_device->kobj, &sysfs_xb_attr_group);
	sysfs_create_group(&misc.this_device->kobj, &sysfs_xb_attr_group1);
#if 0
	//2.逐个添加  不适用于misc_dev,由于此类设备不存在parent节点
	for (i = 0; i < ARRAY_SIZE(misc_xb_attrs); i++) {
		device_create_file(&misc.this_device,
					 &misc_xb_attrs[i]);
	}
#endif	
    pr_info("enter.\n");
    return 0;
}
module_init(device_attr_init);

static void __exit device_attr_exit(void)
{
    pr_info("enter.\n");

}
module_exit(device_attr_exit);

MODULE_LICENSE("GPL");
