#pragma once

#include "linux/fs.h"
#include "linux/printk.h"

#include "../variables.h"

// localhost as default address, we need this to be configurable when this project is mature enough for like kubernetes... or something????
#define ADDR_SIZE 1024
static char address[ADDR_SIZE] = "127.0.0.1";
// Port 8080 for some reason???
int port = 8080;


static ssize_t set_address(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    if (len >= ADDR_SIZE) {
        len = ADDR_SIZE - 1;
    }
    if (copy_from_user(address, buffer, len)) {
        pr_warn("failed to set address\n");
        return -1;
    }
    address[len] = '\0';
    pr_info(PRINT_FMT "set address to: %s", address);
    return len;
}


static ssize_t set_port(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    return 0;
}
