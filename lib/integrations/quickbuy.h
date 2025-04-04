#include <linux/fs.h>

#define MAX_QUERY 1024

int perform_quickbuy(char *query);

ssize_t quickbuy(struct file *filep, const char __user *buffer, size_t len, loff_t *offset);
