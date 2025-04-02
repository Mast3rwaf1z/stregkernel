
#include "linux/fs.h"

static char* BALANCE = "100 kr. :)";

static ssize_t balance(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    int len = strlen(BALANCE); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, BALANCE, len)) { 
        pr_info("we finished displaying your balance\n"); 
        ret = 0; 
    } else { 
        pr_info("reading balance... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}
