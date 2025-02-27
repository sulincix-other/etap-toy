#include <stdbool.h>
extern "C" {
    char* readfile(char* path);
    bool totp_auth(const char* secret, const char* token);
}