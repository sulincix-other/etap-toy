#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void single_instance_init(){
    if(!getenv("USER")){
        exit(1);
    }
    char buffer[1024];
    strcpy(buffer,"/tmp/eta-toy-");
    strcat(buffer,getenv("USER"));
    strcat(buffer,".lock");
    int pid_file = open(buffer, O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno){
            puts("Another instance is already running");
            exit(1);
        }
    }
}
