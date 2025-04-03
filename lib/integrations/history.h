
#include "../http.h"
#include "linux/fs.h"
#include "linux/net.h"

#include "../variables.h"
#include "../stregsystem.h"

int perform_history(char* buffer) {
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
    if(get_history(sock, buffer, csrf_token)) {
        pr_err("Failed to send query\n");
        return 1;
    }
    sock_release(sock);
    return 0;
}

static ssize_t history(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    char history[1024*4] = {0};
    
    perform_history(history);

    int len = strlen(history); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, history, len)) { 
        pr_info("we finished displaying your history\n"); 
        ret = 0; 
    } else { 
        pr_info("reading balance... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}
