#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

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
asmlinkage long sys_get_proc_info(pid_t pid, procinfos* info) {
	// To do
	for_each_process(task) {
		
	}
}
