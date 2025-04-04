#include <linux/net.h>
#include <linux/seq_file_net.h>

#include "lib/integrations/products.h"
#include "lib/variables.h"
#include "lib/http.h"
#include "lib/stregsystem.h"

int perform_products(char* buffer) {
    struct socket *sock;

    if (sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock)) {
        pr_err(PRINT_FMT "Socket create failed\n"); 
        return 1;
    }

    if(init_socket(sock)) {
        pr_err(PRINT_FMT "Failed to initialize socket\n");
        return 1;
    }

    if(get_products(sock, buffer)) {
        pr_err(PRINT_FMT "Failed to send query\n");
        return 1;
    }
    sock_release(sock);
    return 0;
}

ssize_t products(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    char history[1024*4] = {0};
    
    perform_products(history);

    int len = strlen(history); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, history, len)) { 
        ret = 0; 
    } else { 
        *offset += len; 
    } 
    return ret; 
}
