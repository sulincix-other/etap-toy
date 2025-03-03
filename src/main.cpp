#include <QApplication>
#include "MainWindow.h"

#include <stdlib.h>
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)

extern "C" {
    extern void ctx_init();
    void setShowMainWindow(bool state);
}

static MainWindow *window;

int main(int argc, char *argv[]) {
    setenv("QT_QPA_PLATFORM", "xcb;wayland",1);
    //Force ignore system dpi
    setenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0", 1);
    setenv("QT_ENABLE_HIGHDPI_SCALING", "0", 1);
    setenv("QT_SCALE_FACTOR", "1", 1);

    QApplication app(argc, argv);
    window = new MainWindow();
    ctx_init();
    window->show();
    return app.exec();
}

void setShowMainWindow(bool state){
    if(state){
        window->show();
    } else {
        window->hide();
    }
}
