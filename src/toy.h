extern float scale;
void doRightClick();

extern "C" {
    void uinput_send(int type, int code, int value);
}