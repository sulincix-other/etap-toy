#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdbool.h>


#ifndef SERVER_SOCK_FILE
#define SERVER_SOCK_FILE "/dev/etap-toy"
#define SOCKET_BUFFER_LENGTH 4
#endif
int soc_client_fd;
struct sockaddr_un client_from;
struct sockaddr_un client_addr;

static bool sock_init = false;

void client_init() {
    soc_client_fd = socket(PF_UNIX, SOCK_DGRAM, 0);
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, SERVER_SOCK_FILE);

    bind(soc_client_fd, (struct sockaddr *) &client_addr, sizeof(client_addr));
    connect(soc_client_fd, (struct sockaddr *) &client_addr, sizeof(client_addr));

}
void uinput_send(int type, int code, int value){
    if(!sock_init){
        client_init();
        sock_init = true;
    }
    int buff[SOCKET_BUFFER_LENGTH];
    buff[0] = type;
    buff[1] = code;
    buff[2] = value;
    buff[3] = 0;
    printf("%d %d %d\n",buff[0], buff[1], buff[2]);
    if (send(soc_client_fd, buff, sizeof(int)*SOCKET_BUFFER_LENGTH, 0) == -1) {
        perror("send");
    }
}

