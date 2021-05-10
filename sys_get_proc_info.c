#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/unistd.h>
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

asmlinkage long sys_get_proc_info(pid_t pid, struct procinfos* info) {
	struct task_struct* task;
	struct task_struct* oldest_child;
	struct list_head* list;
	struct procinfos temp;
	int res;
	printk("Finding...\n");
	printk("PID must be find is %d\n", pid);
	for_each_process(task) {
		if(task->pid == pid) {
			printk("[%d ------------ [%s]", task->pid, task->comm);
			temp.studentID = 1915940;

			temp.proc.pid = task->pid;
			strcpy(temp.proc.name, task->comm);
			temp.parent_proc.pid = task->parent->pid;
			strcpy(temp.parent_proc.name, task->parent->comm);

			list = &task->children;
			oldest_child = list_entry(list->next, struct task_struct, sibling);

			info->oldest_child_proc.pid = oldest_child->pid;
			strcpy(info->oldest_child_proc.name, oldest_child->comm);
			res = copy_to_user(info, &temp, sizeof(temp));
			if(res == 0) printk("Copy data to user successfully !\n");
			else printk("Copy data to user FAILED !\n");
			printk("Find out\n");
			return 0;
		}
	}
	
	printk("Cannot find process\n");
	return EINVAL;
}
