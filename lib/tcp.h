#pragma once

#include <linux/net.h>


static int tcp_send(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    pr_info("Sending this: %s\n", buf);
    return kernel_sendmsg(sock, &msg, &vec, 1, length);
}

static int tcp_recv(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    int ret = kernel_recvmsg(sock, &msg, &vec, 1, length, 0);
    pr_info("Received this: %s\n", buf);
    return ret;
}
