#pragma once


#include "variables.h"
#include "http.h"

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len >= MAX_BUFFER) len = MAX_BUFFER - 1;
    if (copy_from_user(multibuy_query, buffer, len)) return -EFAULT;
    multibuy_query[len] = '\0';
    pr_info("Received command: %s\n", multibuy_query);

    perform_http_request(multibuy_query);
    return len;
}

static struct file_operations fops = { .write = dev_write };

static int stregsystem_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    streg_class = class_create(CLASS_NAME);
    streg_device = device_create(streg_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    pr_info("stregsystem loaded on /dev/%s (major: %d)\n", DEVICE_NAME, major_number);
    return 0;
}

static void stregsystem_exit(void) {
    device_destroy(streg_class, MKDEV(major_number, 0));
    class_unregister(streg_class);
    class_destroy(streg_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("stregsystem unloaded\n");
}


