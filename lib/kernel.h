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

static int stregkernel_init(void) {
    quickbuy_major_number = register_chrdev(0, "fklub/" QUICKBUY_NAME, &fops);
    balance_major_number = register_chrdev(0, "fklub/" BALANCE_NAME, &fops);
    streg_class = class_create(CLASS_NAME);
    quickbuy_device = device_create(streg_class, NULL, MKDEV(quickbuy_major_number, 0), NULL, "fklub/" QUICKBUY_NAME);
    balance_device = device_create(streg_class, NULL, MKDEV(balance_major_number, 0), NULL, "fklub/" BALANCE_NAME);
    pr_info("quickbuy loaded on /dev/fklub (major: %d)\n", quickbuy_major_number);
    pr_info("balance loaded on /dev/fklub (major: %d)\n", balance_major_number);
    return 0;
}

static void stregkernel_exit(void) {
    device_destroy(streg_class, MKDEV(quickbuy_major_number, 0));
    device_destroy(streg_class, MKDEV(balance_major_number, 0));
    class_unregister(streg_class);
    class_destroy(streg_class);
    unregister_chrdev(quickbuy_major_number, QUICKBUY_NAME);
    unregister_chrdev(balance_major_number, BALANCE_NAME);
    pr_info("stregkernel unloaded\n");
}


