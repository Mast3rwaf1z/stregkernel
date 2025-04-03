#pragma once

#include "linux/net.h"
#include <linux/delay.h>

#include "integrations/settings.h"
#include "utils.h"
#include "tcp.h"

#define STD_SIZE 1024

static int get_csrf_token(struct socket* sock, char* buffer) {
    char http_req[STD_SIZE] = {0};
    char recv_buf[STD_SIZE*4] = {0};

    snprintf(http_req, sizeof(http_req),
        "GET /1/ HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Connection: close\r\n\r\n",
        domain,
        port
    );

    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) {
            total_read += bytes_read;
        }
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';
    
    if(get_string_between("name=\"csrfmiddlewaretoken\" value=\"", "\"", recv_buf, buffer)) {
        pr_err(PRINT_FMT "failed to parse csrf token\n");
        return 1;
    }
    return 0;
}

static int send_quickbuy(struct socket* sock, const char* query, const char* csrf_token) {
    char http_req[STD_SIZE] = {0};
    char recv_buf[STD_SIZE*4] = {0};
    char post_body[STD_SIZE] = {0};

    snprintf(post_body, sizeof(post_body), "csrfmiddlewaretoken=%s&quickbuy=%s", csrf_token, query);

    snprintf(http_req, sizeof(http_req),
        "POST /1/sale/ HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Cookie: csrftoken=%s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        domain,
        port,
        csrf_token,
        strlen(post_body),
        post_body
    );

    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) {
            total_read += bytes_read;
        }
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';

    return 0;
}

int get_balance(struct socket* sock, char* buffer, const char* csrf_token) {
    char http_req[STD_SIZE] = {0};
    char recv_buf[STD_SIZE*4] = {0};

    char post_body[STD_SIZE] = {0};


    snprintf(post_body, sizeof(post_body), "csrfmiddlewaretoken=%s&quickbuy=%s", csrf_token, username);

    snprintf(http_req, sizeof(http_req),
        "POST /1/sale/ HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Cookie: csrftoken=%s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        domain,
        port,
        csrf_token,
        strlen(post_body),
        post_body
    );

    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) {
            total_read += bytes_read;
        }
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';

    if(get_string_between("<h4>", "</h4>", recv_buf, buffer)) {
        pr_err(PRINT_FMT "failed to get balance\n");
        return 1;
    }
    return 0;

}

int get_history(struct socket* sock, char* buffer, const char* csrf_token) {
    char http_req[STD_SIZE] = {0};
    char recv_buf[STD_SIZE] = {0};

    snprintf(http_req, sizeof(http_req),
        "GET /1/user/%d/ HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Cookie: csrftoken=%s\r\n"
        "Connection: close\r\n\r\n",
        user_id,
        domain,
        port,
        csrf_token
    );

    tcp_send(sock, http_req, strlen(http_req));
    memset(recv_buf, 0, sizeof(recv_buf));
    
    tcp_recv_index(sock, recv_buf, STD_SIZE, 3);

    pr_info("%s\n", recv_buf);

    if(get_string_between("<table class=\"default\">", "</table>", recv_buf, buffer)) {
        pr_err(PRINT_FMT "failed to get history\n");
        return 1;
    }
    return 0;
}

int get_products(struct socket* sock, char* buffer, const char* csrf_token) {
    char http_req[STD_SIZE] = {0};
    char recv_buf[STD_SIZE] = {0};

    snprintf(http_req, sizeof(http_req),
        "GET /1/ HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Connection: close\r\n\r\n",
        domain,
        port
    );

    tcp_send(sock, http_req, strlen(http_req));
    memset(recv_buf, 0, sizeof(recv_buf));
    
    tcp_recv_index(sock, recv_buf, STD_SIZE, 4);

    pr_info("%s\n", recv_buf);

    if(get_string_between("<tr>", "</tr>", recv_buf, buffer)) {
        pr_err(PRINT_FMT "failed to get history\n");
        return 1;
    }
    return 0;
}
