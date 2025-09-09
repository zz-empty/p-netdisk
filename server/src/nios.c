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

// 初始化epoll实例
static int init_epoll(NIO_Server *server) {
    server->epoll_fd = epoll_create1(0);
    if (-1 == server->epoll_fd) {
        perror("epoll creation failed");
        return -1;
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server->listen_fd;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->listen_fd, &ev) < 0) {
        perror("epoll_ctl add listen_fd failed");
        return -2;
    }

    return 0;
}

// 启动事件循环
void event_loop(NIO_Server *server) {
    if (init_epoll(server) < 0) {
        return;
    }

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int nready = epoll_wait(server->epoll_fd, events, MAX_EVENTS, -1);
        if (-1 == nready) {
            perror("epoll_wait error");
            break;
        }

        for (int i = 0; i < nready; ++i) {
            // 处理新连接，后续补充
            printf("Event received on fd %d\n", events[i].data.fd);
        }
    }
}


// 关闭服务器资源
void server_cleanup(NIO_Server *server) {
    if (server->listen_fd > 0) close(server->listen_fd);
    if (server->epoll_fd > 0) close(server->epoll_fd);
}
