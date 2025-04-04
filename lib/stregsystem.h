#pragma once

#include "linux/net.h"
#include <linux/delay.h>

#include "integrations/settings.h"
#include "utils.h"
#include "tcp.h"

#define STD_SIZE 1024

static int post(struct socket* sock, const char* endpoint, const char* body) {
    char http_request[STD_SIZE];

    pr_info("POST BODY: %s\n", body);

    snprintf(http_request, sizeof(http_request),
        "POST %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        endpoint,
        domain,
        port,
        strlen(body),
        body
    );

    tcp_send(sock, http_request, strlen(http_request));

    return 0;
}

static int get(struct socket* sock, const char* endpoint, char* buffer, const size_t length, const size_t offset) {
    char http_request[STD_SIZE] = {0};
    char post_body[STD_SIZE] = {0};

    pr_info("GET ENDPOINT: %s\n", endpoint);

    snprintf(http_request, sizeof(http_request), 
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        endpoint,
        domain,
        port,
        strlen(post_body),
        post_body
    );

    tcp_send(sock, http_request, STD_SIZE);
    tcp_recv(sock, buffer, offset);
    tcp_recv(sock, buffer, length);
    pr_info(PRINT_FMT "GET RESPONSE: %s\n", buffer);
    return 0;
}

static int send_quickbuy(struct socket* sock, const char* query) {
    char body[STD_SIZE];

    snprintf(body, sizeof(body), "{\"member_id\":%d, \"buystring\": \"%s\", \"room\":%d}", member_id, query, room_id);

    return post(sock, "/api/sale", body);
}

int get_balance(struct socket* sock, char* buffer) {
    char response[STD_SIZE] = {0};
    char endpoint[STD_SIZE] = {0};
    
    pr_info("test\n");

    snprintf(endpoint, sizeof(endpoint), "/api/member/balance?member_id=%d", member_id);

    pr_info("test\n");
    if(get(sock, endpoint, response, STD_SIZE, 700)) {
        pr_err(PRINT_FMT "Failed to send GET request\n");
        return 1;
    }
    pr_info("test\n");
    
    if(get_string_between("{", "}", response, buffer)) {
        pr_err(PRINT_FMT "failed to get balance\n");
        return 1;
    }
    pr_info("test\n");
    return 0;
}

int get_history(struct socket* sock, char* buffer) {
    char response[STD_SIZE] = {0};
    char endpoint[STD_SIZE] = {0};

    snprintf(endpoint, sizeof(endpoint), "/api/member/sales?member_id=%d", member_id);

    if(get(sock, endpoint, response, STD_SIZE, 700)) {
        pr_err(PRINT_FMT "Failed to send GET request\n");
        return 1;
    }
    
    if(get_string_between("{", "]}", response, buffer)) {
        pr_err(PRINT_FMT "failed to get history\n");
        return 1;
    }
    return 0;
}

int get_products(struct socket* sock, char* buffer) {
    char response[STD_SIZE] = {0};
    char endpoint[STD_SIZE] = {0};

    snprintf(endpoint, sizeof(endpoint), "/api/products/active_products?room_id=%d", room_id);

    if(get(sock, endpoint, response, STD_SIZE, 700)) {
        pr_err(PRINT_FMT "Failed to send GET request\n");
        return 1;
    }
    
    if(get_string_between("{", "}}", response, buffer)) {
        pr_err(PRINT_FMT "failed to get products\n");
        return 1;
    }
    return 0;
}
