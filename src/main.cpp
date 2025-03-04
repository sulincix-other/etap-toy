#include <QApplication>
#include <QSettings>
#include "MainWindow.h"

#include <stdlib.h>
#include <locale.h>
#include <libintl.h>

#define _(String) gettext(String)

extern "C" {
    extern void ctx_init();
    void setShowMainWindow(bool state);
    extern QSettings *settings;
    void settings_init();
}

static MainWindow *window;

int main(int argc, char *argv[]) {
    // translation part
    const char *systemLanguage = getenv("LANG");
    if (systemLanguage != nullptr) {
        bindtextdomain("etap-toy", "/usr/share/locale");
        setlocale(LC_ALL, systemLanguage);
        textdomain("etap-toy");
    } else {
        fprintf(stderr, "WARNING: LANG environment variable not set.\n");
    }


    setenv("QT_QPA_PLATFORM", "xcb;wayland",1);
    // Force ignore system dpi
    setenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0", 1);
    setenv("QT_ENABLE_HIGHDPI_SCALING", "0", 1);
    setenv("QT_SCALE_FACTOR", "1", 1);

    QApplication app(argc, argv);
    settings_init();
    window = new MainWindow();
    ctx_init();
    window->show();
    // Load settings
    return app.exec();
}

void setShowMainWindow(bool state){
    if(state){
        window->show();
    } else {
        window->hide();
    }
}
