#define pr_fmt(fmt) KBUILD_MODNAME ": %s: " fmt, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>



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


static struct class_attribute xb_class_attrs[] = {
	__ATTR(node_1, S_IWUSR|S_IRUGO, class_attr_show, class_attr_store),
	__ATTR(node_2, S_IWUSR|S_IRUGO, class_attr_show, class_attr_store),
	__ATTR_NULL,
};


static struct class xb_class = {
    .name = "xb_class",
    .class_attrs = xb_class_attrs,
};



static int __init class_attr_init(void)
{

    pr_info("enter.\n");
	class_register(&xb_class);

    return 0;
}
module_init(class_attr_init);

static void __exit class_attr_exit(void)
{
    pr_info("enter.\n");
}
module_exit(class_attr_exit);

MODULE_LICENSE("GPL");
