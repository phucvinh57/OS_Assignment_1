#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <asm/current.h>
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
struct task_struct* oldest_child;
struct list_head* list;
asmlinkage long sys_get_proc_info(pid_t pid, procinfos* info) {
	// To do
	if(pid == -1) task = current;
	else task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task == NULL) return EINVAL;

	info->studentID = 1915940;

	printk("[%d]--------------[%s]\n", task->pid, task->comm);
	info->proc.pid = task->pid;
	strcpy(info->proc.name, task->comm);

	printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);
	info->parent_proc.pid = task->parent->pid;
	strcpy(info->parent_proc.name, task->parent->comm);

	list = &task->children;
	oldest_child = list_entry(list->next, struct task_struct, sibling);
	printk("[%d]--------------[%s]\n", oldest_child->pid, oldest_child->comm);
	info->oldest_child_proc.pid = oldest_child->pid;
	strcpy(info->oldest_child_proc.name, oldest_child->comm);

	return 0;
}
