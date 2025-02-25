#include <QtWidgets>
#include <QPainter>
#include <QMap>

#include "../toy.h"


QMap<qint64, ToyButton> buttons;

ToyButton::ToyButton(QWidget *parent) : QWidget(parent) {
    show();
}

extern "C" {
    void toy_button_init(){
        
    }
}
