#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <linux/tls.h>
#include <linux/net.h>

int send_fd(int sock_fd, int fd_to_send) {
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    char buf[CMSG_SPACE(sizeof(fd_to_send))], dummy = '*';
    struct iovec io = {
        .iov_base = &dummy,
        .iov_len = sizeof(dummy)
    };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));

    memcpy(CMSG_DATA(cmsg), &fd_to_send, sizeof(fd_to_send));

    return sendmsg(sock_fd, &msg, 0);
}

int main(int argc, char* argv[]) {
    char* addr;
    switch (argc) {
        case 1: {
            addr = "localhost:8080";
            break;
        }
        default:
        case 2: {
            addr = argv[1];
            break;
        }
    }
    // Connect to TLS server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    struct hostent *server = gethostbyname(addr);
    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(443)
    };
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // You could set up kTLS here with setsockopt(), but for now we just pass the connected fd.

    // Open the device node
    int dev = open("/dev/fklub/settings/socket", O_WRONLY);
    if (dev < 0) { perror("open /dev/fklub/settings/socket"); exit(1); }

    if (send_fd(dev, sockfd) < 0) {
        perror("send_fd");
        exit(1);
    }

    close(sockfd);
    close(dev);

    return 0;
}

