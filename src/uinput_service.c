#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


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
char * socket_read() {
    char * buff = malloc(SOCKET_BUFFER_LENGTH * sizeof(char));
    recvfrom(soc_server_fd, buff, SOCKET_BUFFER_LENGTH, 0, (struct sockaddr * ) & server_from, & fromlen);
    return buff;
}


static void emit(int fd, int type, int code, int val) {
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, & ie, sizeof(ie));
}

int fd;

void uinput_init(void) {
    system("modprobe uinput");
    struct uinput_setup usetup;

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    ioctl(fd, UI_SET_EVBIT, EV_SYN);
	  ioctl(fd, UI_SET_EVBIT, EV_KEY);
	  ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
	  ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
	  ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);

    ioctl(fd, UI_SET_EVBIT, EV_ABS);
	  ioctl(fd, UI_SET_ABSBIT, ABS_X);
	  ioctl(fd, UI_SET_ABSBIT, ABS_Y);

    memset( & usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x3169;
    usetup.id.product = 0x3169;
    usetup.id.version = 31;
    strcpy(usetup.name, "Amogus Mouse");

    ioctl(fd, UI_DEV_SETUP, & usetup);
    ioctl(fd, UI_DEV_CREATE);

    sleep(1);
}

int main(int argc, char** argv){
    (void)argc; (void)argv;
    uinput_init();
    socket_init();
    char *buf;
    puts("Ready");
    while(1){
        buf = socket_read();
        printf("%d %d %d\n",buf[0], buf[1], buf[2]);
        emit(fd, buf[0], buf[1], buf[2]);
        emit(fd, EV_SYN, SYN_REPORT, 0);
        usleep(30000);
    }
    return 0;
}
