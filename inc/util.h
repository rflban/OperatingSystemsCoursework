#ifndef OSCW_UTIL_H_
#define OSCW_UTIL_H_

#include <linux/types.h>
#include <linux/kernel.h>

static inline pid_t
str_to_pid(char *str) {
    return simple_strtoul(str, NULL, 10);
}

#endif // OSCW_UTIL_H_
