
#include "../http.h"
#include "linux/fs.h"
#include "linux/net.h"

#include "../variables.h"
#include "../stregsystem.h"

int perform_balance(char* buffer) {
    struct socket *sock;

    if (sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock)) {
        pr_err(PRINT_FMT "Socket create failed\n"); 
        return 1;
    }

    if(init_socket(sock)) {
        pr_err(PRINT_FMT "Failed to initialize socket\n");
        return 1;
    }

    if(get_balance(sock, buffer)) {
        pr_err(PRINT_FMT "Failed to send query\n");
        return 1;
    }
    sock_release(sock);
    return 0;
}

static ssize_t balance(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    char balance[1024] = {0};
    
    perform_balance(balance);

    int len = strlen(balance); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, balance, len)) { 
        pr_info("we finished displaying your balance\n"); 
        ret = 0; 
    } else { 
        pr_info("reading balance... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}
