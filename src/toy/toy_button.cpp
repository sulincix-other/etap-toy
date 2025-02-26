#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>

#include <QtWidgets>
#include <QPainter>
#include <QMap>
#include <stdio.h>

#include "../toy.h"

ToyWindow::ToyWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::Tool
                      | Qt::WindowStaysOnTopHint
                      | Qt::WindowSystemMenuHint
                      | Qt::WindowDoesNotAcceptFocus
                      | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(butsize*4, butsize*4);

    QLabel *move = new QLabel("", this);
    move->setStyleSheet(
        QString("background-color: #31313131;") +
        QString("border: 3px solid #31696969;") +
        QString("border-radius: ")+QString::number(butsize*2)+QString("px;")
    );
    move->setFixedSize(butsize*4, butsize*4);
}


ToyButton::ToyButton(QWidget *parent, QString path) : QWidget(parent) {
    QLabel *img = new QLabel("", this);
    img->setStyleSheet(QString("background-color: none;"));
    img->setFixedSize(butsize, butsize);
    setFixedSize(butsize, butsize);
    QIcon icon = QIcon(path);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(butsize, butsize)));
    img->setPixmap(pixmap);
    show();
}

void ToyButton::dummy(){
        puts("Ü");
}

ToyWindow *toys;

void toy_button_init(){
    toys = new ToyWindow(NULL);
    toys->buttons["test"] = new ToyButton(toys, ":images/move-icon.svg");
    toys->buttons["test"]->dummy();
}

void doLongPress(){
    if(toys != nullptr){
        toys->show();
    }
    puts("Long press!");
}