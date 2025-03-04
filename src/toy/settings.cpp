#include <QSettings>


extern "C" {
    QSettings *settings;
    void settings_init();
}

void settings_init(){
    if(settings == nullptr) {
        QString settingsFile = QString(getenv("HOME"))+"/.config/etap-toy.ini";
        qDebug() << settingsFile;
        settings = new QSettings(settingsFile, QSettings::NativeFormat);
    }
}
