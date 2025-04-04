#pragma once

#include "linux/fs.h"
#include "linux/in.h"
#include "linux/socket.h"

#define SETTINGS_SIZE 1024
static struct sockaddr_in address_struct = {
    .sin_family = AF_INET
};
// localhost as default address, we need this to be configurable when this project is mature enough for like kubernetes... or something????
static char address[SETTINGS_SIZE] = "127.0.0.1";
// Hack the HTTP header to think we know wtf DNS is, in reality we just know the IP address
static char domain[SETTINGS_SIZE] = "localhost";
// Little workaround for balance config
static char username[SETTINGS_SIZE] = "tester";
static int member_id = 1;
static int room_id = 1;
// Port 8080 for some reason???
static int port = 8080;

ssize_t set_str(const char* __user source, char* target, size_t len);

ssize_t get_str(const char* source, char* __user target, loff_t* offset);

ssize_t set_int(const char* __user source, int* target, size_t len);

ssize_t get_int(const int* source, char* __user target, loff_t* offset);

ssize_t set_address(struct file* __, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_address(struct file *f, char __user *buffer, size_t len, loff_t *offset);

ssize_t set_domain(struct file *filep, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_domain(struct file *f, char __user *buffer, size_t len, loff_t *offset);

ssize_t set_username(struct file *f, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_username(struct file *f, char __user *buffer, size_t len, loff_t *offset);

ssize_t set_port(struct file *f, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_port(struct file *f, char __user *buffer, size_t len, loff_t *offset);

ssize_t set_member_id(struct file *f, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_member_id(struct file *f, char __user *buffer, size_t len, loff_t *offset);

ssize_t set_room_id(struct file *f, const char __user *buffer, size_t len, loff_t* offset);

ssize_t get_room_id(struct file *f, char __user *buffer, size_t len, loff_t *offset);
