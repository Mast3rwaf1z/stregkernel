#pragma once

#include <linux/stat.h>

#include "variables.h"
#include "stregsystem.h"

#define DEV_CLASS_MODE ((umode_t)(S_IRUGO|S_IWUGO))

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len >= MAX_BUFFER) len = MAX_BUFFER - 1;
    if (copy_from_user(multibuy_query, buffer, len)) return -EFAULT;
    multibuy_query[len] = '\0';
    pr_info("Received command: %s\n", multibuy_query);

    send_multibuy(multibuy_query);
    return len;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t length, loff_t *offset) {
    
    char message[1024];
    get_balance(message);

    copy_to_user(buffer, message, sizeof(message));
    return 2;
}

static char* devnode(const struct device *dev, umode_t *mode) {
    *mode = DEV_CLASS_MODE;
    return NULL;
}

static struct file_operations fops = { .write = dev_write, .read = dev_read };

static int stregsystem_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    streg_class = class_create(CLASS_NAME);
    streg_class->devnode = devnode;
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


