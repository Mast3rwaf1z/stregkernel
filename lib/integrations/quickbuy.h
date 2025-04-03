#include <linux/fs.h>
#include <linux/types.h>

#include "../http.h"
#include "../stregsystem.h"

#define MAX_QUERY 1024

static int perform_quickbuy(char *query) {
    struct socket *sock;
    char csrf_token[128] = {0};

    if (sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock)) {
        pr_err(PRINT_FMT "Socket create failed\n"); 
        return 1;
    }

    if(init_socket(sock)) {
        pr_err(PRINT_FMT "Failed to initialize socket\n");
        return 1;
    }

    // ---------- GET CSRF Token ---------
    if(get_csrf_token(sock, csrf_token)) {
        pr_err(PRINT_FMT "Failed to parse csrf token\n");
        sock_release(sock);
        return 1;
    }

    sock_release(sock);


    if (sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock)) {
        pr_err(PRINT_FMT "Socket create failed\n"); 
        return 1;
    }

    if(init_socket(sock)) {
        pr_err(PRINT_FMT "Failed to initialize socket\n");
        return 1;
    }

    // ---------- POST with Command ----------
    if(send_quickbuy(sock, query, csrf_token)) {
        pr_err("Failed to send query\n");
        return 1;
    }
    sock_release(sock);
    return 0;

}

static ssize_t quickbuy(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    char query[MAX_QUERY] = {0};
    if (len >= MAX_QUERY) {
        len = MAX_QUERY - 1;
    }
    if (copy_from_user(query, buffer, len)) {
        return -EFAULT;
    }
    query[len] = '\0';
    pr_info("Received command: %s\n", query);

    perform_quickbuy(query);
    return len;
}
