#include <QApplication>
#include "MainWindow.h"

extern "C" {
    extern void ctx_init();
}

int main(int argc, char *argv[]) {
    setenv("QT_QPA_PLATFORM", "xcb;wayland",1);
    //Force ignore system dpi
    setenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0", 1);
    setenv("QT_ENABLE_HIGHDPI_SCALING", "0", 1);
    setenv("QT_SCALE_FACTOR", "1", 1);

    QApplication app(argc, argv);
    MainWindow window;
    ctx_init();
    window.show();
    return app.exec();
}
