#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <math.h>
#include "../common.h"

#define SERVER_PORT 9666

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "not enough args\n");
        exit(-1);
    }
    double x = atof(argv[1]);
    func_type ft = atof(argv[2]);
    int sock;
    struct sockaddr_in addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
    calc_struct cs = {x, ft};
    send(sock, &cs, sizeof(cs), 0);
    double result;
    recv(sock, &result, sizeof(result), 0);
    close(sock);
    if (isnan(result))
    {
        fprintf(stderr, "failed to get result\n");
        exit(-1);
    }
    printf("%g\n", result);
    return 0;
}
