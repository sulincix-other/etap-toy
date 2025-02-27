#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <cotp.h>


bool totp_auth(const char* secret, const char* token){
    char* generated = get_totp(strdup(secret), 6, 30, SHA1, NULL);
    return strncmp(token, generated, 6) == 0;
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
    while(buffer[file_size-1] == '\n'){
        buffer[file_size-1] = '\0';
        file_size--;
    }
    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}


int main(int argc, char** argv){
    char* secret = readfile("/etc/etap.secret");
    if(!secret){
        setuid(0);
        fprintf(stderr, "Error: %s\n","secret not found.");
        system("uuidgen | base32 > /etc/etap.secret;");
        system("; chmod 600 /etc/etap.secret ; chown root:root /etc/etap.secret");
        return 1;
    }
    system("; chmod 600 /etc/etap.secret ; chown root:root /etc/etap.secret");
    if (argc < 2){
        fprintf(stderr, "Error: %s\n","token not found.");
        return 1;
    }
    if (strlen(argv[1]) != 6){
        fprintf(stderr, "Error: %s\n","invalid token.");
        return 1;
    }
    // read /etc/etap.secret as root
    setuid(0);
    if (totp_auth(secret, argv[1])){
        fprintf(stderr, "Authentication Successfully\n");
        return 0;
    }
    fprintf(stderr, "Authentication Failed\n");
    return 1;
}

