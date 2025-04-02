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

static int tcp_recv_all(struct socket *sock, char *recv_buf, size_t length) {
    int bytes_read, total_read = 0;
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) {
            total_read += bytes_read;
        }
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';
    return total_read;
}
