extern float scale;

#define butsize 48 * scale


#include "toy/toy_button.h"
#include "toy/right_click.h"
#include "toy/lock_screen.h"

extern "C" {
    void ctx_init();
    void setShowMainWindow(bool state);
    void uinput_send(int type, int code, int value);
    bool pam_auth(const char *username, const char *password);
    bool totp_auth(const char *secret, const char *token);

    extern int (*pam_show_cb)(const char* msg);
}
