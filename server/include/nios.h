#ifndef NIOS_H
#define NIOS_H

#include <sys/epoll.h>

#define MAX_EVENTS 64
#define DEFAULT_PORT 9000

typedef struct {
    int listen_fd;      // 监听的socket描述符
    int epoll_fd;       // epoll实例描述符
    uint16_t port;      // 监听端口
} NIO_Server;

// 初始化服务器
int server_init(NIO_Server *server);

// 启动事件循环
void event_loop(NIO_Server *server);

// 关闭服务器资源
void server_cleanup(NIO_Server *server);

#endif
