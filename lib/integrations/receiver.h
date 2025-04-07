#pragma once

#include "linux/net.h"

static struct socket tls_socket;

ssize_t get_socket(struct file *f, const char __user *buffer, size_t len, loff_t *offset);
