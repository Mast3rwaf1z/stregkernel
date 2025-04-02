#pragma once

#include "linux/fs.h"
#include "linux/in.h"
#include "linux/socket.h"
#include "linux/printk.h"

#include "../variables.h"

// localhost as default address, we need this to be configurable when this project is mature enough for like kubernetes... or something????
#define SETTINGS_SIZE 1024
static struct sockaddr_in address_struct = {
    .sin_family = AF_INET
};
static char address[SETTINGS_SIZE] = "127.0.0.1";
// Little workaround for balance config
static char username[SETTINGS_SIZE] = "tester";
// Port 8080 for some reason???
static int port = 8080;


static ssize_t set_address(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if (len >= SETTINGS_SIZE) {
        len = SETTINGS_SIZE - 1;
    }
    if (copy_from_user(address, buffer, len)) {
        pr_warn("failed to set address\n");
        return -1;
    }
    address[len] = '\0';
    pr_info(PRINT_FMT "set address to: %s", address);
    return len;
}


static ssize_t get_address(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    int len = strlen(address); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, address, len)) { 
        pr_info("we finished displaying the address\n"); 
        ret = 0; 
    } else { 
        pr_info("reading address... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}

// User functions are primarily to configure the 
static ssize_t set_username(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if (len >= SETTINGS_SIZE) {
        len = SETTINGS_SIZE - 1;
    }
    if (copy_from_user(username, buffer, len)) {
        pr_warn("failed to set username\n");
        return -1;
    }
    username[len] = '\0';
    pr_info(PRINT_FMT "set username to: %s", username);
    return len;
}

static ssize_t get_username(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    int len = strlen(username); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, username, len)) { 
        pr_info("we finished displaying the username\n"); 
        ret = 0; 
    } else { 
        pr_info("reading username... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}

static ssize_t set_port(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    char temp[SETTINGS_SIZE] = {0};
    if (len >= SETTINGS_SIZE) {
        len = SETTINGS_SIZE - 1;
    }
    if (copy_from_user(temp, buffer, len)) {
        pr_warn("failed to set port\n");
        return -1;
    }
    temp[len] = '\0';
    if(kstrtoint(temp, 0, &port)) {
        pr_warn(PRINT_FMT "Failed to parse int %s\n", temp);
        return -1;
    }
    pr_info(PRINT_FMT "set port to: %d", port);
    return len;
}

static ssize_t get_port(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset) { 
    char temp[SETTINGS_SIZE] = {0};
    snprintf(temp, sizeof(temp), "%d", port);
    
    int len = strlen(temp); 
    ssize_t ret = len; 
    if (*offset >= len || copy_to_user(buffer, temp, len)) { 
        pr_info("we finished displaying the port\n"); 
        ret = 0; 
    } else { 
        pr_info("reading port... %s\n", file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 
    return ret; 
}
