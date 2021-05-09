#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/init.h>
#include <linux/module.h>

typedef struct {
	pid_t pid;
	char name[16];
} proc_info;

typedef struct {
	long studentID;
	proc_info proc;
	proc_info parent_proc;
	proc_info oldest_child_proc;
} procinfos;

struct task_struct* task;
struct task_struct* task_child;
struct list_head * list;
int pid = 1;
struct list_head* head;

int iterate_init(void) {
	printk(KERN_INFO "%s", "LOADING MODULE\n");
	//To do: Handle pid = - 1;
	//if(pid == -1) pid = get_pid();
	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	printk("[%d]--------------[%s]\n", task->pid, task->comm);
	printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);

	head = &task->children;
	task_child = list_entry(head->next, struct task_struct, sibling);
	printk("[%d]--------------[%s]\n", task_child->pid, task_child->comm);
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
