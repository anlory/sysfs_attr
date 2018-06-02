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






static struct attribute *sysfs_xb_attr[] = {
    &node_1.attr,
    &node_2.attr,
    NULL
};

static const struct attribute_group sysfs_xb_attr_group = {
    .attrs = sysfs_xb_attr,
};

static const struct attribute_group sysfs_xb_attr_group_1 = {
    .attrs = sysfs_xb_attr,
	.name = "sub_attr",
};










static int __init kobject_attr_init(void)
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
module_init(kobject_attr_init);

static void __exit kobject_attr__exit(void)
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
module_exit(kobject_attr__exit);

MODULE_LICENSE("GPL");
