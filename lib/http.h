#pragma once

#include <linux/inet.h>
#include <linux/net_namespace.h>

#include "linux/net.h"
#include "linux/printk.h"
#include "integrations/settings.h"

static int init_socket(struct socket* sock) {
    address_struct.sin_addr.s_addr = in_aton(address);
    address_struct.sin_port = htons(port);

    return 0;
}


