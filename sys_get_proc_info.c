#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <asm/current.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

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
procinfos temp;
int result;
pid_t kernelpid;
asmlinkage long sys_get_proc_info(pid_t pid, procinfos* info) {
	// To do
	result = copy_from_user(&kernelpid, &pid, sizeof(pid_t));
	if (result == 0) {
		printk("Copy kernelpid successfully\n");
	}
	else {
		printk("Copy kernel pid fail\n");
	}	
	if(kernelpid == -1) 
		task = current;
	else 
		task = pid_task(find_vpid(kernelpid), PIDTYPE_PID);
	if(task == NULL) 
		return EINVAL;

	temp.studentID = 1915940;

	printk("[%d]--------------[%s]\n", task->pid, task->comm);
	temp.proc.pid = task->pid;
	strcpy(temp.proc.name, task->comm);

	printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);
	temp.parent_proc.pid = task->parent->pid;
	strcpy(temp.parent_proc.name, task->parent->comm);

	list = &task->children;
	oldest_child = list_entry(list->next, struct task_struct, sibling);
	printk("[%d]--------------[%s]\n", oldest_child->pid, oldest_child->comm);
	temp.oldest_child_proc.pid = oldest_child->pid;
	strcpy(temp.oldest_child_proc.name, oldest_child->comm);

	result = copy_to_user(info, &temp, sizeof(procinfos));
	if (result == 0) {
		printk("Copy data successfully\n");
	}
	else {
		printk("Copy fail\n");
	}
	return 0;
}
