#pragma once

#include "linux/fs.h"

int perform_products(char* buffer);

ssize_t products(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset);
