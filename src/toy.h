extern float scale;

#define butsize 48 * scale


#include "toy/toy_button.h"
#include "toy/right_click.h"

extern "C" {
    void ctx_init();
    void uinput_send(int type, int code, int value);
}
