#pragma once

#include "linux/socket.h"
#include <linux/net.h>

#include "variables.h"

static int tcp_send(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    return kernel_sendmsg(sock, &msg, &vec, 1, length);
}

static int tcp_recv(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {.msg_flags = MSG_WAITALL};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    int res = kernel_recvmsg(sock, &msg, &vec, 1, length, MSG_WAITALL);
    if (res == -EFAULT) {
        pr_err(PRINT_FMT "EFAULT\n");
    }
    else if (res == -EAGAIN) {
        pr_err(PRINT_FMT "EAGAIN\n");
    }
    return res;
}

int tcp_recv_index(struct socket *sock, char *buf, size_t buf_size, int index) {
    struct msghdr msg = {.msg_flags = MSG_WAITALL};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = buf_size
    };
    int res;
    for (int i = 0; i < index; i++) {
        res = kernel_recvmsg(sock, &msg, &vec, 1, buf_size, MSG_WAITALL);
    }
    return res;

}
