#pragma once

#include <linux/inet.h>
#include <linux/net_namespace.h>

#include "http.h"
#include "tcp.h"

static int get_csrf_token(char* buffer) {
    int ret;

    ret = get_content_between("/", buffer, "name=\"csrfmiddlewaretoken\" value=\"", '"');
    if (ret < 0) {
        pr_err("Failed to get csrf token\n");
        return ret;
    }
    return 0;
}

static int send_query(char* query, char* token) {
    struct socket *sock;
    struct sockaddr_in addr;
    char recv_buf[4096];
    char http_req[1024];
    int ret;

    ret = prepare_addr(&addr, sizeof(addr));

    ret = prepare_socket(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        pr_err("Failed to prepare socket\n");
        return ret;
    }

    char post_body[512];
    snprintf(post_body, sizeof(post_body), "quickbuy=%s&csrfmiddlewaretoken=%s", query, token);

    snprintf(http_req, sizeof(http_req),
        "POST /1/sale/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Cookie: csrftoken=%s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        token,
        strlen(post_body), post_body);

    tcp_send(sock, http_req, strlen(http_req));
    memset(recv_buf, 0, sizeof(recv_buf));
    tcp_recv(sock, recv_buf, sizeof(recv_buf) - 1);
    pr_info("POST Response: %.200s\n", recv_buf);
    return 0;
}


static void send_multibuy(char *query) {
    char csrf_token[128] = {0};
    int ret;

    // GET CSRF Token
    ret = get_csrf_token(csrf_token);
    if (ret < 0) {
        pr_err("Failed to get csrf token\n");
        return;
    }


    // Send multibuy query
    ret = send_query(query, csrf_token);
    if (ret < 0) {
        pr_err("Failed to send multibuy query");
        return;
    }
}

static void get_balance(char* message) {
    struct socket *sock;
    struct sockaddr_in addr;
    int ret;

    ret = prepare_addr(&addr, sizeof(addr));

    ret = prepare_socket(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        pr_err("Failed to create socket\n");
        return;
    }
    ret = get_content_between("/1/sale/1/", message, "har ", ' ');
    if (ret < 0) {
        pr_err("Failed to get content\n");
        return;
    }
}
