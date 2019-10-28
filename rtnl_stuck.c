#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/sysctl.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("hiboma <hiroyan@gmail.com>");
MODULE_DESCRIPTION("This module is PoC");

static struct ctl_table_header *rtnl_stuck_sysctl_header;

static int min = 1;
static int max = INT_MAX;
static int rtnl_stuck_msleep_seconds = 1;

int rtnl_stuck_sysctl_handler(struct ctl_table *table, int write,
	void __user *buffer, size_t *length, loff_t *ppos)
{
	int ret;

	ret = proc_dointvec_minmax(table, write, buffer, length, ppos);
	if (ret)
		return ret;

	if (write) {
		pr_info("I will sleep for %d seconds", rtnl_stuck_msleep_seconds);
		rtnl_lock();
		msleep(1000 * rtnl_stuck_msleep_seconds);
		rtnl_unlock();
	}

	return 0;
}

static struct ctl_table rtnl_stuck_ctl_table[] = {
	{
		.procname	= "rtnl_stuck",
		.data		= &rtnl_stuck_msleep_seconds,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= &rtnl_stuck_sysctl_handler,
		.extra1		= &min,
		.extra2		= &max,
	},
	{},
};

static int __init rtnl_stuck_init(void)
{
	rtnl_stuck_sysctl_header = register_sysctl_table(rtnl_stuck_ctl_table);
	if (!rtnl_stuck_sysctl_header)
		return -ENOMEM;

	return 0;
}

static void __exit rtnl_stuck_exit(void)
{
	unregister_sysctl_table(rtnl_stuck_sysctl_header);
}

module_init(rtnl_stuck_init);
module_exit(rtnl_stuck_exit);
