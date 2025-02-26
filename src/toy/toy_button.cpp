#include <QtWidgets>
#include <QPainter>
#include <QMap>
#include <stdio.h>

#include "../toy.h"


QMap<qint64, ToyButton> buttons;

ToyButton::ToyButton(QWidget *parent) : QWidget(parent) {
    show();
}

extern "C" {
    void toy_button_init(){
        ToyButton *toy = new ToyButton(NULL);
        (void)toy;
    }
}

void doLongPress(){
    puts("Long press!");
}