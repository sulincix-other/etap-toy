#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <libevdev-1.0/libevdev/libevdev-uinput.h>

#ifndef SERVER_SOCK_FILE
#define SERVER_SOCK_FILE "/dev/etap-toy"
#define SOCKET_BUFFER_LENGTH 4
#endif
int sock;

void socket_send(int *buf){
    if (send(sock, buf, SOCKET_BUFFER_LENGTH* sizeof(int), 0) < 0) {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    struct sockaddr_un server_address;
    int *buf = malloc(SOCKET_BUFFER_LENGTH * sizeof(int));

    // Create a Unix socket
    sock = socket(PF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Clear the server_address structure
    memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    strncpy(server_address.sun_path, SERVER_SOCK_FILE, sizeof(server_address.sun_path) - 1);

    // Connect to the Unix socket server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(struct sockaddr_un)) < 0) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Send data
    buf[0] = atoi(argv[1]);
    buf[1] = atoi(argv[2]);
    buf[2] = atoi(argv[3]);
    socket_send(buf);
    // Close the socket
    close(sock);
    return 0;
}
