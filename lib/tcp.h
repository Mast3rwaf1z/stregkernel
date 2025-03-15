#pragma once

#include <linux/net.h>


static int tcp_send(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    return kernel_sendmsg(sock, &msg, &vec, 1, length);
}

static int tcp_recv(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    return kernel_recvmsg(sock, &msg, &vec, 1, length, 0);
}
