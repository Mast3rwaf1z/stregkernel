#include "lib/integrations/flogo.h"

ssize_t flogo(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    int len = strlen(FLOGO); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, FLOGO, len)) { 
        ret = 0; 
    } else { 
        *offset += len; 
    } 
    return ret; 
} 
