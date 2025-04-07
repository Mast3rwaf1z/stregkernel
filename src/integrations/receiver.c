#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/socket.h>
#include <linux/file.h>

#include "lib/integrations/receiver.h"

ssize_t get_socket(struct file *file, const char __user *buf, size_t len, loff_t *ppos) {
    struct socket *sock;
    int fd;

    if (copy_from_user(&fd, buf, sizeof(int)))
        return -EFAULT;

    struct fd fdesc = fdget(fd);
    if (!fdesc.file) return -EBADF;

    sock = sock_from_file(fdesc.file);
    if (!sock) return -ENOTSOCK;

    tls_socket = *sock;  // Save for global use
    printk(KERN_INFO "TLS socket saved.\n");

    fdput(fdesc);
    return sizeof(int);
}
