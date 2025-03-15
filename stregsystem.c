
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <linux/string.h>
#include <net/net_namespace.h>  // Needed for init_net

#define DEVICE_NAME "stregsystem"
#define CLASS_NAME "streg"
#define MAX_BUFFER 1024

static int major_number;
static struct class* streg_class = NULL;
static struct device* streg_device = NULL;
static char command_buffer[MAX_BUFFER] = {0};

// Function prototypes
static int tcp_send(struct socket *sock, char *buf, size_t length);
static int tcp_recv(struct socket *sock, char *buf, size_t length);
static void perform_http_request(char *command);

static int tcp_send(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    return kernel_sendmsg(sock, &msg, &vec, 1, length);
}

static int tcp_recv(struct socket *sock, char *buf, size_t length) {
    struct msghdr msg = {};
    struct kvec vec = {
        .iov_base = buf,
        .iov_len = length
    };
    return kernel_recvmsg(sock, &msg, &vec, 1, length, 0);
}

static void perform_http_request(char *command) {
    struct socket *sock;
    struct sockaddr_in saddr;
    char recv_buf[4096];
    char http_req[1024];
    char csrf_token[128] = {0};
    int ret;

    // ---------- GET CSRF Token ----------
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) { pr_err("Socket create failed\n"); return; }

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(0x7F000001); // 127.0.0.1
    saddr.sin_port = htons(8000);

    ret = kernel_connect(sock, (struct sockaddr*)&saddr, sizeof(saddr), 0);
    if (ret < 0) { pr_err("Connect failed\n"); goto out_release; }

    snprintf(http_req, sizeof(http_req),
        "GET /1/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Connection: close\r\n\r\n");

    tcp_send(sock, http_req, strlen(http_req));
    int bytes_read, total_read = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    do {
        bytes_read = tcp_recv(sock, recv_buf + total_read, sizeof(recv_buf) - total_read - 1);
        if (bytes_read > 0) total_read += bytes_read;
    } while (bytes_read > 0);
    recv_buf[total_read] = '\0';
    
    pr_info("GET Response: %.200s\n", recv_buf);

    char *token_start = strstr(recv_buf, "name=\"csrfmiddlewaretoken\" value=\"");
    if (token_start) {
        token_start += strlen("name=\"csrfmiddlewaretoken\" value=\"");
        char *token_end = strchr(token_start, '"');
        if (token_end) {
            size_t token_len = token_end - token_start;
            strncpy(csrf_token, token_start, token_len);
            csrf_token[token_len] = '\0';
            pr_info("Parsed CSRF token: %s\n", csrf_token);
        }
    } else {
        pr_err("CSRF token not found!\n");
        goto out_release;
    }

    sock_release(sock);

    // ---------- POST with Command ----------
    ret = sock_create_kern(&init_net, AF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (ret < 0) { pr_err("Socket create failed\n"); return; }

    ret = kernel_connect(sock, (struct sockaddr*)&saddr, sizeof(saddr), 0);
    if (ret < 0) { pr_err("Connect failed\n"); goto out_release; }

    char post_body[512];
    snprintf(post_body, sizeof(post_body), "quickbuy=%s&csrfmiddlewaretoken=%s", command, csrf_token);

    snprintf(http_req, sizeof(http_req),
        "POST /1/sale/ HTTP/1.1\r\n"
        "Host: 127.0.0.1:8000\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n\r\n"
        "%s",
        strlen(post_body), post_body);

    tcp_send(sock, http_req, strlen(http_req));
    memset(recv_buf, 0, sizeof(recv_buf));
    tcp_recv(sock, recv_buf, sizeof(recv_buf) - 1);
    pr_info("POST Response: %.200s\n", recv_buf);

out_release:
    sock_release(sock);
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len >= MAX_BUFFER) len = MAX_BUFFER - 1;
    if (copy_from_user(command_buffer, buffer, len)) return -EFAULT;
    command_buffer[len] = '\0';
    pr_info("Received command: %s\n", command_buffer);

    perform_http_request(command_buffer);
    return len;
}

static struct file_operations fops = { .write = dev_write };

static int __init stregsystem_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    streg_class = class_create(CLASS_NAME);
    streg_device = device_create(streg_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    pr_info("stregsystem loaded on /dev/%s (major: %d)\n", DEVICE_NAME, major_number);
    return 0;
}

static void __exit stregsystem_exit(void) {
    device_destroy(streg_class, MKDEV(major_number, 0));
    class_unregister(streg_class);
    class_destroy(streg_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("stregsystem unloaded\n");
}

module_init(stregsystem_init);
module_exit(stregsystem_exit);
MODULE_LICENSE("GPL");
