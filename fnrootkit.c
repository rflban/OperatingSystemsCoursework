#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Faris Nabiev ICS7-73B");
MODULE_DESCRIPTION("LKM for subject \"operating systems\" coursework. Implementation of a rootkit");

#define MODULE_NAME "fnrootkit"

static int __init fnrootkit_init(void) {
    printk(KERN_INFO "fnrootkit: module have loaded.\n");
    return 0;
}

static void __exit fnrootkit_exit(void) {
    printk(KERN_INFO "fnrootkit: module have unloaded.\n");
}

module_init(fnrootkit_init);
module_exit(fnrootkit_exit);
