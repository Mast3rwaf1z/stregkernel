#pragma once


#include "http.h"
#include "variables.h"
#include "chrdev.h"


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len >= MAX_BUFFER) len = MAX_BUFFER - 1;
    if (copy_from_user(multibuy_query, buffer, len)) return -EFAULT;
    multibuy_query[len] = '\0';
    pr_info("Received command: %s\n", multibuy_query);

    perform_http_request(multibuy_query);
    return len;
}
static struct file_operations quickbuy_operations = { .write = dev_write };
static struct file_operations balance_operations = { .write = dev_write };

chrdev_wrapper quickbuy_wrapper;
chrdev_wrapper balance_wrapper;

static int stregkernel_init(void) {
    create_wrapper(&quickbuy_wrapper, NAMESPACE "/quickbuy", &quickbuy_operations);
    create_wrapper(&balance_wrapper, NAMESPACE "/balance", &balance_operations);

    pr_info("quickbuy loaded on /dev/fklub (major: %d)\n", quickbuy_wrapper.major);
    pr_info("balance loaded on /dev/fklub (major: %d)\n", balance_wrapper.major);
    return 0;
}

static void stregkernel_exit(void) {
    destroy_wrapper(&quickbuy_wrapper, NAMESPACE "/quickbuy");
    destroy_wrapper(&balance_wrapper, NAMESPACE "/balance");
    pr_info("stregkernel unloaded\n");
}


