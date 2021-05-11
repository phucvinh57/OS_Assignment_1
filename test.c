#include <get_proc_info.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
long get_proc_info(pid_t pid, procinfos* info);
int main(int argc, char* argv[]) {
	pid_t pid = atoi(argv[1]);
	printf("PID: %d\n", pid);
	procinfos info;
	if(get_proc_info(pid, &info) == 0) {
		printf("Student ID: %ld\n", info.studentID);
		printf("Parent process infos:\n");
		printf("\tPID: %d\n", info.parent_proc.pid);
		printf("\tName: %s\n", info.parent_proc.name);

		printf("Process infos:\n");
		printf("\tPID: %d\n", info.proc.pid);
		printf("\tName: %s\n", info.proc.name);

		printf("Oldest child process infos:\n");
		printf("\tPID: %d\n", info.oldest_child_proc.pid);
		printf("\tName: %s\n", info.oldest_child_proc.name);

	}
	else {
		printf("Cannot get information in struct procinfos\n");
	}
	sleep(1);
	return 0;
}
