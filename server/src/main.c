#include <stdio.h>
#include "../include/nios.h"

int main()
{
    NIO_Server server = {};
    server.port = DEFAULT_PORT;

    if (server_init(&server) < 0) {
        fprintf(stderr, "Server initialization failed\n");
        return 1;
    }

    printf("Starting event loop...\n");
    event_loop(&server);

    server_cleanup(&server);
    return 0;
}
