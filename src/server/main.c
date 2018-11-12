#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <zconf.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include "../common.h"

#define SERVER_PORT 9666

void signal_handler(int sig);

int listener;

int main()
{
    signal(SIGINT, signal_handler);

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
    {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    listen(listener, 5);
    int sock;
    while (1)
    {
        sock = accept(listener, NULL, NULL);
        if (sock < 0)
        {
            perror("accept");
            exit(3);
        }
        printf("%d: connected\n", sock);
        while (1)
        {
            int bytes_read;
            calc_struct cs;
            bytes_read = recv(sock, &cs, sizeof(cs), 0);
            if (bytes_read <= 0) break;
            printf("%d: params = { x: %g,  ft: %i }\n", sock, cs.x, cs.ft);
            double result = NAN;
            switch (cs.ft)
            {
                case FC_EXP:
                    result = exp(cs.x);
                    break;
                case FC_SIN:
                    result = sin(cs.x);
                    break;
                case FC_COS:
                    result = cos(cs.x);
                    break;
            }
            send(sock, &result, sizeof(result), 0);
        }
        close(sock);
        printf("%d: connection closed\n", sock);
    }
    return 0;
}

void signal_handler(int sig)
{
    close(listener);
    printf("Server stopped\n");
    exit(0);
}
