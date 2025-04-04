
#include "linux/fs.h"

int perform_balance(char* buffer);

ssize_t balance(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset);
