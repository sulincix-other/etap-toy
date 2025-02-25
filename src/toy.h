extern float scale;

extern "C" {
    #include "toy/toy_button.h"
    #include "toy/right_click.h"
    void uinput_send(int type, int code, int value);
}
