#pragma once

#include "linux/socket.h"
#include <linux/net.h>

int tcp_send(struct socket *sock, char *buf, size_t length);

int tcp_recv(struct socket *sock, char *buf, size_t length);
