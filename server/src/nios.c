#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "../include/nios.h"

// 初始化服务器
int server_init(NIO_Server *server) {
    server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server->listen_fd) {
        perror("socket creation failed");
        return -1;
    }

    // 设置端口可重用
    int opt = 1;
    setsockopt(server->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(server->port ? server->port : DEFAULT_PORT);

    if (bind(server->listen_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        return -2;
    }

    // 开始监听
    if (listen(server->listen_fd, SOMAXCONN) < 0) {
        perror("listen failed");
        return -3;
    }

    printf("Server listening on Port %d\n", server->port);
    return 0;
}
