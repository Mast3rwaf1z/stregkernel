#pragma once

#include <linux/net_namespace.h>
#include <linux/inet.h>
#include <linux/net.h>

#include "tcp.h"

static void release_socket(struct socket* sock) {
    sock_release(sock);
}

// Prepare structs
static int prepare_socket(struct socket* sock, struct sockaddr* addr, ssize_t size) {
    int ret;

    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) {
        pr_err("Socket creation failed\n");
        return ret;
    }

    ret = kernel_connect(sock, addr, size, 0);
    if (ret < 0) {
        pr_err("Failed to connect\n");
        release_socket(sock);
        return ret;
    }
    pr_info("Prepared socket\n");
    return 0;
}

static int prepare_addr(struct sockaddr_in* addr, ssize_t size) {
    memset(addr, 0, size);
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = htonl(0x7F000001);
    addr->sin_port = htons(8000);
    return 0;
}

static int prepare_http_request(char* request, char* endpoint, ssize_t size) {
    int ret = snprintf(request, size,
        "GET %s HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Connection: close\r\n\r\n",
        endpoint
    );
    pr_info("Prepared http request\n");
    return ret;
}

// HTTP helper functions
static int get_content_between(char* endpoint, char* buffer, char* start_string, char end_char) {
    struct socket *sock;
    struct sockaddr_in addr;
    char recv_buf[4096];
    char http_req[1024];
    int ret;

    ret = prepare_addr(&addr, sizeof(addr));
    if (ret < 0) {
        pr_err("Failed to prepare address\n");
        return ret;
    }

    ret = prepare_socket(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        pr_err("Failed to prepare socket\n");
        return ret;
    }
    ret = prepare_http_request(http_req, endpoint, sizeof(http_req));
    if (ret < 0) {
        pr_err("Failed to prepare http request");
        return ret;
    }
    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) total_read += bytes_read;
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';
    
    pr_info("GET Response: %.200s\n", recv_buf);

    char *token_start = strstr(recv_buf, start_string);
    if (token_start) {
        token_start += strlen(start_string);
        char *token_end = strchr(token_start, end_char);
        if (token_end) {
            size_t token_len = token_end - token_start;
            strncpy(buffer, token_start, token_len);
            buffer[token_len] = '\0';
            pr_info("Parsed string: %s\n", buffer);
        }
    } else {
        pr_err("string not found!\n");
        release_socket(sock);
    }

    release_socket(sock);
    pr_info("Found string\n");
    return 0;

}
