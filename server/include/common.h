#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

// 通用错误处理宏
#define ERROR_EXIT(msg) \
    do { \
        fprintf(stderr, "[ERROR] %s:%d - %s: %s\n", \
                __FILE__, __LINE__, msg, strerror(errno)); \
        exit(EXIT_FAILURE); \
    } while(0)

#define WARNING_LOG(msg) \
        fprintf(stderr, "[WARNING] %s:%d - %s\n", __FILE__, __LINE__, msg)

// 安全资源释放宏
#define SAVE_CLOSE(fd) \
    do { \
        if (fd >= 0) { \
            close(fd);\
            fd = -1; \
        }; \
    } while(0)

#endif  // COMMON_H
