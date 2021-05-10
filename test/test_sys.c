#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/pid.h>
#include <linux/sched/signal.h>
static long PID;

MODULE_LICENSE("GPL");
module_param(PID, long, S_IRUSR);
MODULE_PARM_DESC(PID, "Pid of the process");

struct task_struct* task;
struct task_struct* oldest_child;
struct list_head* list;
int init_iterate(void) {
	// To do
	printk("Kernel module starts\n");
	if(PID == -1) task = current;
	else task = pid_task(find_vpid(PID), PIDTYPE_PID);
	if(task == NULL) {
		printk("Cannot find process\n");
		return 0;
	}
	printk("[%d]--------------[%s]\n", task->pid, task->comm);
	printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);
	list = &task->children;
	oldest_child = list_entry(list->next, struct task_struct, sibling);
	printk("[%d]--------------[%s]\n", oldest_child->pid, oldest_child->comm);
	return 0;
}
void clean_exit(void) {
	printk("Kernel module ends\n");
}
module_init(init_iterate);
module_exit(clean_exit);
