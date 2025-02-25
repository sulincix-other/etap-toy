#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>

#include <unistd.h>
#include <sys/un.h>

#define O_RDONLY 00
#define O_WRONLY 01
#define O_RDWR 02

#ifndef SERVER_SOCK_FILE
#define SERVER_SOCK_FILE "/dev/etap-toy"
#define SOCKET_BUFFER_LENGTH 4
#endif

#include <sys/stat.h>
#include <sys/socket.h>

int soc_server_fd;
struct sockaddr_un server_from;
struct sockaddr_un server_addr;
socklen_t fromlen;

void socket_init() {
    int ok = 1;
    fromlen = sizeof(server_from);

    if ((soc_server_fd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        ok = 0;
    }

    bind(soc_server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (ok) {
        memset( & server_from, 0, sizeof(server_addr));
        server_from.sun_family = AF_UNIX;
        strcpy(server_from.sun_path, SERVER_SOCK_FILE);
        unlink(SERVER_SOCK_FILE);
        if (bind(soc_server_fd, (struct sockaddr * ) & server_from, sizeof(server_from)) < 0) {
            perror("bind");
            ok = 0;
        }
    }
    chmod(SERVER_SOCK_FILE, S_IWUSR | S_IWGRP | S_IWOTH);
}
int *socket_read() {
    int *buff = malloc(SOCKET_BUFFER_LENGTH * sizeof(int));
    recvfrom(soc_server_fd, buff, SOCKET_BUFFER_LENGTH * sizeof(int), 0, (struct sockaddr * ) & server_from, & fromlen);
    return buff;
}



struct libevdev *dev;
struct libevdev_uinput *uidev;
void uinput_init(){
    int err;
    struct input_absinfo absinfo_x;
    struct input_absinfo absinfo_y;
    absinfo_x.maximum = 3840;
    absinfo_x.resolution = 1;
    absinfo_y.maximum = 2160;
    absinfo_y.resolution = 1;

    dev = libevdev_new();
    libevdev_set_name(dev, "Amogus mouse");
    libevdev_enable_event_type(dev, EV_ABS);
    libevdev_enable_event_type(dev, EV_KEY);
    libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
    libevdev_enable_event_code(dev, EV_ABS, ABS_X, &absinfo_x);
    libevdev_enable_event_code(dev, EV_ABS, ABS_Y, &absinfo_y);

    err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    if (err != 0) exit(err);

    sleep(1);
}

int main(int argc, char** argv){
    (void)argc; (void)argv;
    uinput_init();
    socket_init();
    int *buf;
    puts("Ready");
    while(1){
        buf = socket_read();
        libevdev_uinput_write_event(uidev, buf[0], buf[1], buf[2]);
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        printf("%d %d %d\n",buf[0], buf[1], buf[2]);
    }
    return 0;
}
