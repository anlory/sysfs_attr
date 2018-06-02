#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>


static struct kobject *k_obj = NULL;
static struct kobject *k_obj_1 = NULL;

static char s_value[20] = {0};


static ssize_t device_attr_show(struct device_driver *driver, char *buf)
{
	return sprintf(buf, "%s\n", driver->name);

}
static ssize_t device_attr_store(struct device_driver *driver, const char *buf,
		 size_t count)
{
	unsigned int val;
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



ssize_t class_attr_show(struct class *class, struct class_attribute *attr,
		char *buf)
{
	return sprintf(buf, "%s\n", attr->attr.name);
}
ssize_t class_attr_store(struct class *class, struct class_attribute *attr,
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
    &node_1.attr,
    &node_2.attr,
    NULL
};

static struct attribute *sysfs_xb_attr1[] = {
    &dev_attr_node_1.attr,
    &dev_attr_node_2.attr,
    &dev_attr_node_3.attr,
    &dev_attr_node_4.attr,
    NULL
};


static const struct attribute_group sysfs_xb_attr_group = {
    .attrs = sysfs_xb_attr,
};

static const struct attribute_group sysfs_xb_attr_group_1 = {
    .attrs = sysfs_xb_attr,
	.name = "sub_attr",
};
static const struct attribute_group sysfs_xb_attr_group_2 = {
    .attrs = sysfs_xb_attr1,
	.name = "sub_attr",
};



static struct class_attribute xb_class_attrs[] = {
	__ATTR(node_1, S_IWUSR|S_IRUGO, class_attr_show, class_attr_store),
	__ATTR(node_2, S_IWUSR|S_IRUGO, class_attr_show, class_attr_store),
	__ATTR_NULL,
};



static struct class xb_class = {
    .name = "xb_class",
    .class_attrs = xb_class_attrs,
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


static struct miscdevice misc_xb_dev = {  
    .minor = MISC_DYNAMIC_MINOR,  
    .name = "xb_attr", //此名称将显示在/dev目录下面  
    .fops = &misc_xb_fops,  
};  


static int __init device_attr_demo_init(void)
{
	int i;
/* 1. 在sys下创建目录，和节点 脱离设备。 sysfs_create_group  */
//	/sys/xb/
    if ((k_obj = kobject_create_and_add("xb", NULL)) == NULL ) {
        pr_err("sysfs_demo sys node create error \n");
        goto err;
    }

    if(sysfs_create_group(k_obj, &sysfs_xb_attr_group) ) {
        pr_err("sysfs_create_group failed\n");
        goto err1;
    }

    if ((k_obj_1 = kobject_create_and_add("xb2", k_obj)) == NULL ) {
        pr_err("hwinfo sys node create error \n");
        goto err2;
    }

    if(sysfs_create_group(k_obj_1, &sysfs_xb_attr_group_1) ) {
        pr_err("sysfs_create_group failed\n");
        goto err3;
    }

/*2. 使用class_register方式创建节点，脱离设备*/
//	/sys/class/misc/xb_attr/

	class_register(&xb_class);
/**************************************/
/*3.在设备目录下创建节点。  */

	misc_register(&misc_xb_dev);

	//1.数组形式创建
	sysfs_create_group(&misc_xb_dev.this_device->kobj, &sysfs_xb_attr_group_2);
#if 0

	//2.逐个添加   ，此方法不适用misc dev
	for (i = 0; i < ARRAY_SIZE(misc_xb_attrs); i++) {
		device_create_file(misc_xb_dev.this_device, &misc_xb_attrs[i]);
	}
#endif	
    pr_info("enter.\n");
    return 0;
err3:
    kobject_put(k_obj_1);
err2:
    sysfs_remove_group(k_obj, &sysfs_xb_attr_group);
err1:
    kobject_put(k_obj);
err:
    return -1;
}
module_init(device_attr_demo_init);

static void __exit device_attr_demo_exit(void)
{
    pr_info("enter.\n");

    if (k_obj) {
        sysfs_remove_group(k_obj, &sysfs_xb_attr_group);
        if (k_obj_1) {
            sysfs_remove_group(k_obj_1, &sysfs_xb_attr_group_1);
            kobject_put(k_obj_1);
        }
        kobject_put(k_obj);
    }

}
module_exit(device_attr_demo_exit);

MODULE_LICENSE("GPL");
