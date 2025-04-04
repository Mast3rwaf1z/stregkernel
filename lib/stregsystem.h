#pragma once

#include "linux/net.h"

#define STD_SIZE 1024

int post(struct socket* sock, const char* endpoint, const char* body);

int get(struct socket* sock, const char* endpoint, char* buffer, const size_t length, const size_t offset);

int send_quickbuy(struct socket* sock, const char* query);

int get_balance(struct socket* sock, char* buffer);

int get_history(struct socket* sock, char* buffer);

int get_products(struct socket* sock, char* buffer);
