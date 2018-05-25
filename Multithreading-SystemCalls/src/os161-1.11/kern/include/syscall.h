#ifndef _SYSCALL_H_
#define _SYSCALL_H_

/*
 * Prototypes for IN-KERNEL entry points for system call implementations.
 */

int sys_reboot(int code);
int sys_write(int fd, userptr_t buf, size_t len, int *ret_val);
int sys_fork(struct trapframe *tf, int *ret_val);
void sys__exit(int exit_code);
int sys_getpid(pid_t *ret_val);
int sys_execv(userptr_t progname, userptr_t args);

//helpers
void begin_forkentry(void *data1, unsigned long data2);
#endif /* _SYSCALL_H_ */
