#include <linux/fs.h>
#include <linux/types.h>

#include "../http.h"

#define MAX_QUERY 1024

static ssize_t quickbuy_w(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    char query[MAX_QUERY] = {0};
    if (len >= MAX_QUERY) {
        len = MAX_QUERY - 1;
    }
    if (copy_from_user(query, buffer, len)) {
        return -EFAULT;
    }
    query[len] = '\0';
    pr_info("Received command: %s\n", query);

    perform_http_request(query);
    return len;
}
