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
int *socket_read() {
    int *buff = malloc(SOCKET_BUFFER_LENGTH * sizeof(int));
    recvfrom(soc_server_fd, buff, SOCKET_BUFFER_LENGTH * sizeof(int), 0, (struct sockaddr * ) & server_from, & fromlen);
    return buff;
}


static void emit(int fd, int type, int code, int val) {
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;

    if (write(fd, &ie, sizeof(ie)) < 0)
        fprintf(stderr,"error: write()");
}

int fd;

void uinput_init() {
    if ((fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK)) < 0)
		fprintf(stderr,"error: open");
	// enable synchronization
	if (ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
		fprintf(stderr,"error: ioctl UI_SET_EVBIT EV_SYN");

	// enable 1 button
	if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		fprintf(stderr,"error: ioctl UI_SET_EVBIT EV_KEY");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_TOOL_PEN) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_STYLUS) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_STYLUS2) < 0)
		fprintf(stderr,"error: ioctl UI_SET_KEYBIT");

	// enable 2 main axes + pressure (absolute positioning)
	if (ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
		fprintf(stderr,"error: ioctl UI_SET_EVBIT EV_ABS");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_X) < 0)
		fprintf(stderr,"error: ioctl UI_SETEVBIT ABS_X");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
		fprintf(stderr,"error: ioctl UI_SETEVBIT ABS_Y");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_PRESSURE) < 0)
		fprintf(stderr,"error: ioctl UI_SETEVBIT ABS_PRESSURE");

        {
          struct uinput_abs_setup abs_setup;
          struct uinput_setup setup;

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_X;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 3840;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 1;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            fprintf(stderr,"error: UI_ABS_SETUP ABS_X");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_Y;
          abs_setup.absinfo.value = 0;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 2160;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 1;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            fprintf(stderr,"error: UI_ABS_SETUP ABS_Y");

          memset(&abs_setup, 0, sizeof(abs_setup));
          abs_setup.code = ABS_PRESSURE;
          abs_setup.absinfo.value = 1;
          abs_setup.absinfo.minimum = 0;
          abs_setup.absinfo.maximum = 1;
          abs_setup.absinfo.fuzz = 0;
          abs_setup.absinfo.flat = 0;
          abs_setup.absinfo.resolution = 0;
          if (ioctl(fd, UI_ABS_SETUP, &abs_setup) < 0)
            fprintf(stderr,"error: UI_ABS_SETUP ABS_PRESSURE");

          memset(&setup, 0, sizeof(setup));
          snprintf(setup.name, UINPUT_MAX_NAME_SIZE, "Amogus Mouse Device");
          setup.id.bustype = BUS_VIRTUAL;
          setup.id.vendor  = 0x1;
          setup.id.product = 0x1;
          setup.id.version = 2;
          setup.ff_effects_max = 0;
          if (ioctl(fd, UI_DEV_SETUP, &setup) < 0)
            fprintf(stderr,"error: UI_DEV_SETUP");

          if (ioctl(fd, UI_DEV_CREATE) < 0)
            fprintf(stderr,"error: ioctl");
        }
}

int main(int argc, char** argv){
    (void)argc; (void)argv;
    uinput_init();
    socket_init();
    int *buf;
    puts("Ready");
    while(1){
        buf = socket_read();
        printf("%d %d %d\n",buf[0], buf[1], buf[2]);
        emit(fd, buf[0], buf[1], buf[2]);
    }
    return 0;
}
