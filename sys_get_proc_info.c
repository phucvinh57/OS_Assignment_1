#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <asm/current.h>
#include <linux/uaccess.h>

struct proc_info {
	pid_t pid;
	char name[16];
};

struct procinfos {
	long studentID;
	struct proc_info proc;
	struct proc_info parent_proc;
	struct proc_info oldest_child_proc;
};


struct task_struct* task;
struct task_struct* oldest_child;
struct list_head* list;
struct procinfos temp;
pid_t kernelpid;
asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos* info) {
	// To do
	if(__get_user(kernelpid, &pid) == 0)
		printk("Copy kernelpid successfully\n");
	else {
		printk("Copy kernel pid fail\n");
		return -EFAULT;
	}

	printk("Param value: %d\n", kernelpid);
	if(kernelpid == -1) task = current;
	else task = pid_task(find_vpid(kernelpid), PIDTYPE_PID);
	if(task == NULL) {
		printk("Cannot find process\n");
		return EINVAL;
	}
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

	if (copy_to_user(info, &temp, sizeof(struct procinfos)) == 0) {
		printk("Copy data successfully\n");
	}
	else {
		printk("Copy fail\n");
		return -EFAULT;
	}
	return 0;
}
