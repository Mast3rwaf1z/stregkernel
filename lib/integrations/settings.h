#pragma once

#include "asm/user_64.h"
#include "linux/fs.h"
#include "linux/in.h"
#include "linux/socket.h"
#include "linux/printk.h"

#include "../variables.h"

#define SETTINGS_SIZE 1024
static struct sockaddr_in address_struct = {
    .sin_family = AF_INET
};
// localhost as default address, we need this to be configurable when this project is mature enough for like kubernetes... or something????
static char address[SETTINGS_SIZE] = "127.0.0.1";
// Hack the HTTP header to think we know wtf DNS is, in reality we just know the IP address
static char domain[SETTINGS_SIZE] = "localhost";
// Little workaround for balance config
static char username[SETTINGS_SIZE] = "tester";
static int user_id = 1;
// Port 8080 for some reason???
static int port = 8080;

static ssize_t set_str(const char* __user source, char* target, size_t len) {
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

static ssize_t get_str(const char* source, char* __user target, loff_t* offset) {
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

static ssize_t set_int(const char* __user source, int* target, size_t len) {
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

static ssize_t get_int(const int* source, char* __user target, loff_t* offset) {
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

static ssize_t set_address(struct file* __, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, address, len);
}


static ssize_t get_address(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(address, buffer, offset);
}

static ssize_t set_domain(struct file *filep, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, domain, len);
}


static ssize_t get_domain(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(domain, buffer, offset);
}

// User functions are primarily to configure the 
static ssize_t set_username(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_str(buffer, username, len);
}

static ssize_t get_username(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_str(username, buffer, offset);
}

static ssize_t set_port(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_int(buffer, &port, len);
}

static ssize_t get_port(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_int(&port, buffer, offset);
}

static ssize_t set_user_id(struct file *f, const char __user *buffer, size_t len, loff_t* offset) {
    return set_int(buffer, &user_id, len);
}

static ssize_t get_user_id(struct file *f, char __user *buffer, size_t len, loff_t *offset) { 
    return get_int(&user_id, buffer, offset);
}
