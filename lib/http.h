#pragma once

#include <linux/inet.h>
#include <linux/net_namespace.h>

#include "linux/net.h"
#include "linux/printk.h"
#include "integrations/settings.h"

static int init_socket(struct socket* sock) {
    address_struct.sin_addr.s_addr = in_aton(address);
    address_struct.sin_port = htons(port);

    if (kernel_connect(sock, (struct sockaddr*)&address_struct, sizeof(address_struct), 0)) {
        pr_err(PRINT_FMT "Connect failed\n"); 
        sock_release(sock);
        return 1;
    }

    return 0;
}


