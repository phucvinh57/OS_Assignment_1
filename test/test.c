#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/module.h>
#include <asm/current.h>
struct task_struct* task;
struct task_struct* task_child;
struct list_head * list;
int pid = -1;
struct list_head* head;

int iterate_init(void) {
	printk(KERN_INFO "%s", "LOADING MODULE\n");
	//To do: Handle pid = - 1;
	if(pid == -1) task = current;
	else task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task == NULL) printk("PID not found !!\n");
	else {
		printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);
		printk("[%d]--------------[%s]\n", task->pid, task->comm);
		head = &task->children;
		if(head->next == NULL) printk("NULL child process\n");
		task_child = list_entry(head->next, struct task_struct, sibling);
		printk("[%d]--------------[%s]\n", task_child->pid, task_child->comm);
	}
	return 0;
}
void cleanup_exit(void) {
	printk(KERN_INFO "%s", "REMOVING MODULE\n");
}
module_init(iterate_init);
module_exit(cleanup_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ITERATE TSDSDS");
MODULE_AUTHOR("ASDAD");
