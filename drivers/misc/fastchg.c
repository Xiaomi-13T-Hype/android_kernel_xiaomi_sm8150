/*
 * Force Fast Charge Driver
 * Author: Xiaomi-13T-Hype | モトテーパー
 *
 * SPDX-License-Identifier: GPL-2.0
 */

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fastchg.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>

int force_fast_charge = 0;
EXPORT_SYMBOL_GPL(force_fast_charge);

static int __init get_fastcharge_opt(char *ffc)
{
	if (strcmp(ffc, "1") == 0)
		force_fast_charge = 1;
	else
		force_fast_charge = 0;
	return 1;
}
__setup("ffc=", get_fastcharge_opt);

static ssize_t force_fast_charge_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", force_fast_charge);
}

static ssize_t force_fast_charge_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	int val = 0;

	if (kstrtoint(buf, 10, &val) == 0) {
		force_fast_charge = (val > 0) ? 1 : 0;
	}

	return count;
}

static struct kobj_attribute force_fast_charge_attribute =
	__ATTR(force_fast_charge, 0664, force_fast_charge_show, force_fast_charge_store);

static struct attribute *force_fast_charge_attrs[] = {
	&force_fast_charge_attribute.attr,
	NULL,
};

static struct attribute_group force_fast_charge_attr_group = {
	.attrs = force_fast_charge_attrs,
};

static struct kobject *force_fast_charge_kobj;

static int __init force_fast_charge_init(void)
{
	int retval;

	force_fast_charge_kobj = kobject_create_and_add("fast_charge", kernel_kobj);
	if (!force_fast_charge_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(force_fast_charge_kobj, &force_fast_charge_attr_group);
	if (retval)
		kobject_put(force_fast_charge_kobj);

	return retval;
}

static void __exit force_fast_charge_exit(void)
{
	if (force_fast_charge_kobj)
		kobject_put(force_fast_charge_kobj);
}

module_init(force_fast_charge_init);
module_exit(force_fast_charge_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xiaomi-13T-Hype | モトテーパー");
MODULE_DESCRIPTION("USB Force Fast Charge Driver");
