#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#ifndef SHA1
#define SHA1 2
#endif


static char* (*get_totp)(const char*,int,int,int,void*);

bool totp_auth(const char* secret, const char* token){
    if (get_totp){
        char* generated = get_totp(secret, 6, 30, SHA1, NULL);
        return strncmp(generated, token, 6) == 0;
    } else {
        fprintf(stderr, "Warning: %s\n","libcotp not found.");
    }
    return false;
}

char* readfile(char* path) {
    FILE *file;
    long file_size;
    char *buffer;

    file = fopen(path, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}


void cotp_init(){
    void *handle = dlopen("libcotp.so", RTLD_LAZY);;
    if(handle){
        get_totp = dlsym(handle, "get_totp");
    } else {
        fprintf(stderr, "Warning: %s\n","libcotp not found.");
    }
}

