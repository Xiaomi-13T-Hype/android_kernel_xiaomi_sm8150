/*
 * N0KZ / E404 Kernel Attributes Driver
 * Author: Xiaomi-13T-Hype | モトテーパー
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/n0kz_attributes.h>

struct n0kz_attributes n0kz_data = {
	.kgsl_skip_zeroing = 0,
	.avoid_dirty_pte = 0,
};
EXPORT_SYMBOL_GPL(n0kz_data);

#define N0KZ_ATTR_RW(name) \
static ssize_t name##_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) \
{ \
	return sprintf(buf, "%d\n", n0kz_data.name); \
} \
static ssize_t name##_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) \
{ \
	int val; \
	if (kstrtoint(buf, 10, &val)) \
		return -EINVAL; \
	n0kz_data.name = val; \
	return count; \
} \
static struct kobj_attribute name##_attr = __ATTR(name, 0664, name##_show, name##_store);

N0KZ_ATTR_RW(kgsl_skip_zeroing);
N0KZ_ATTR_RW(avoid_dirty_pte);

static struct attribute *n0kz_attrs[] = {
	&kgsl_skip_zeroing_attr.attr,
	&avoid_dirty_pte_attr.attr,
	NULL
};

static struct attribute_group n0kz_attr_group = {
	.attrs = n0kz_attrs,
};

static struct kobject *n0kz_kobj;
static struct kobject *e404_kobj;

static int __init n0kz_attributes_init(void)
{
	int retval;

	/* Create /sys/kernel/n0kz_attributes */
	n0kz_kobj = kobject_create_and_add("n0kz_attributes", kernel_kobj);
	if (n0kz_kobj) {
		retval = sysfs_create_group(n0kz_kobj, &n0kz_attr_group);
		if (retval)
			pr_warn("n0kz_attributes: failed to create n0kz_attributes group: %d\n", retval);
	}

	/* Create /sys/kernel/e404 */
	e404_kobj = kobject_create_and_add("e404", kernel_kobj);
	if (e404_kobj) {
		retval = sysfs_create_group(e404_kobj, &n0kz_attr_group);
		if (retval)
			pr_warn("n0kz_attributes: failed to create e404 group: %d\n", retval);
	}

	return 0;
}

static void __exit n0kz_attributes_exit(void)
{
	if (e404_kobj)
		kobject_put(e404_kobj);
	if (n0kz_kobj)
		kobject_put(n0kz_kobj);
}

module_init(n0kz_attributes_init);
module_exit(n0kz_attributes_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xiaomi-13T-Hype | モトテーパー");
MODULE_DESCRIPTION("N0KZ / E404 Kernel Attributes Driver");
