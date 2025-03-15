#pragma once

#include <linux/inet.h>
#include <linux/net_namespace.h>

#include "tcp.h"

static void perform_http_request(char *command) {
    struct socket *sock;
    struct sockaddr_in saddr;
    char recv_buf[4096];
    char http_req[1024];
    char csrf_token[128] = {0};
    int ret;

    // ---------- GET CSRF Token ----------
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) { pr_err("Socket create failed\n"); return; }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(0x7F000001); // 127.0.0.1
    saddr.sin_port = htons(8000);

    ret = kernel_connect(sock, (struct sockaddr*)&saddr, sizeof(saddr), 0);
    if (ret < 0) { pr_err("Connect failed\n"); goto out_release; }

    snprintf(http_req, sizeof(http_req),
        "GET /1/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Connection: close\r\n\r\n");

    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) total_read += bytes_read;
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';
    
    pr_info("GET Response: %.200s\n", recv_buf);

    char *token_start = strstr(recv_buf, "name=\"csrfmiddlewaretoken\" value=\"");
    if (token_start) {
        token_start += strlen("name=\"csrfmiddlewaretoken\" value=\"");
        char *token_end = strchr(token_start, '"');
        if (token_end) {
            size_t token_len = token_end - token_start;
            strncpy(csrf_token, token_start, token_len);
            csrf_token[token_len] = '\0';
            pr_info("Parsed CSRF token: %s\n", csrf_token);
        }
    } else {
        pr_err("CSRF token not found!\n");
        goto out_release;
    }

    sock_release(sock);

    // ---------- POST with Command ----------
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) { pr_err("Socket create failed\n"); return; }

    ret = kernel_connect(sock, (struct sockaddr*)&saddr, sizeof(saddr), 0);
    if (ret < 0) { pr_err("Connect failed\n"); goto out_release; }

    char post_body[512];
    snprintf(post_body, sizeof(post_body), "quickbuy=%s&csrfmiddlewaretoken=%s", command, csrf_token);

    snprintf(http_req, sizeof(http_req),
        "POST /1/sale/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        strlen(post_body), post_body);

    tcp_send(sock, http_req, strlen(http_req));
    memset(recv_buf, 0, sizeof(recv_buf));
    tcp_recv(sock, recv_buf, sizeof(recv_buf) - 1);
    pr_info("POST Response: %.200s\n", recv_buf);

out_release:
    sock_release(sock);
}
