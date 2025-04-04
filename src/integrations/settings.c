
#include "linux/fs.h"
#include "linux/socket.h"
#include "linux/printk.h"

#include "lib/integrations/settings.h"
#include "lib/variables.h"

ssize_t set_str(const char* __user source, char* target, size_t len) {
    if (len >= SETTINGS_SIZE) {
        len = SETTINGS_SIZE - 1;
    }
    if (copy_from_user(target, source, len)) {
        pr_warn("failed to set setting\n");
        return -1;
    }
    target[len] = '\0';
    pr_info(PRINT_FMT "set setting to: %s", target);
    return len;
}

ssize_t get_str(const char* source, char* __user target, loff_t* offset) {
    int len = strlen(source); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(target, source, len)) { 
        ret = 0; 
    } else { 
        *offset += len; 
    } 
    pr_info(PRINT_FMT "Returned setting: %s", source);
    return ret;
}

ssize_t set_int(const char* __user source, int* target, size_t len) {
    char temp[SETTINGS_SIZE] = {0};
    if (len >= SETTINGS_SIZE) {
        len = SETTINGS_SIZE - 1;
    }
    if (copy_from_user(temp, source, len)) {
        pr_warn(PRINT_FMT "failed to set setting\n");
        return -1;
    }
    temp[len] = '\0';
    if(kstrtoint(temp, 0, target)) {
        pr_warn(PRINT_FMT "Failed to parse int %s\n", temp);
        return -1;
    }
    pr_info(PRINT_FMT "set setting to: %d", *target);
    return len;
}

ssize_t get_int(const int* source, char* __user target, loff_t* offset) {
    char temp[SETTINGS_SIZE] = {0};
    snprintf(temp, sizeof(temp), "%d", *source);
    
    int len = strlen(temp); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(target, temp, len)) { 
        ret = 0; 
    } else { 
        *offset += len; 
    } 
    pr_info(PRINT_FMT "Returned setting: %d", *source);
    return ret; 
}

ssize_t set_address(struct file* __, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, address, len);
}


ssize_t get_address(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(address, buffer, offset);
}

ssize_t set_domain(struct file *filep, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, domain, len);
}


ssize_t get_domain(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(domain, buffer, offset);
}

ssize_t set_username(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, username, len);
}

ssize_t get_username(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(username, buffer, offset);
}

ssize_t set_port(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_int(buffer, &port, len);
}

ssize_t get_port(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_int(&port, buffer, offset);
}

ssize_t set_member_id(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_int(buffer, &member_id, len);
}

ssize_t get_member_id(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_int(&member_id, buffer, offset);
}

ssize_t set_room_id(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_int(buffer, &room_id, len);
}

ssize_t get_room_id(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_int(&room_id, buffer, offset);
}
