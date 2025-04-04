
#include "linux/fs.h"

int perform_history(char* buffer);

ssize_t history(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset);
