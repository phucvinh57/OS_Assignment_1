#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

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

SYSCALL_DEFINE2(get_proc_info, pid_t, pid, struct procinfos*, info) {
//asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos* info) {
	// To do
	struct task_struct* task;
	struct task_struct* oldest_child;
	struct list_head* list;
	struct procinfos temp;

	if(pid < -1 || info == NULL) return EINVAL;
	if(pid == -1) task = current;
	if(pid == 0) task = pid_task(find_vpid(1), PIDTYPE_PID)->parent;
	if(pid >= 1) task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task == NULL) return EINVAL;

	temp.studentID = 1915940;
	printk("[%d]--------------[%s]\n", task->pid, task->comm);
	temp.proc.pid = task->pid;
	strcpy(temp.proc.name, task->comm);

	if(task == task->parent) {
		printk("This process has no parent\n");
		temp.parent_proc.pid = -1;
		strcpy(temp.parent_proc.name, "null");
	} else {
		printk("[%d]--------------[%s]\n", task->parent->pid, task->parent->comm);
		temp.parent_proc.pid = task->parent->pid;
		strcpy(temp.parent_proc.name, task->parent->comm);
	}

	list = &task->children;
	if(list_empty_careful(list)) {
		printk("This process has no child\n");
		temp.oldest_child_proc.pid = -1;
		strcpy(temp.oldest_child_proc.name, "NULL"); 
	}
	else {
		oldest_child = list_entry(list->next, struct task_struct, sibling);
		printk("[%d]--------------[%s]\n", oldest_child->pid, oldest_child->comm);
		temp.oldest_child_proc.pid = oldest_child->pid;
		strcpy(temp.oldest_child_proc.name, oldest_child->comm);
	}

	if (copy_to_user(info, &temp, sizeof(struct procinfos)) == 0) {
		printk("Copy data successfully\n");
	}
	else {
		printk("Copy fail\n");
		return EINVAL;
	}
	return 0;
}
