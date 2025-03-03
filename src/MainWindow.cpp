#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

#include "toy.h"

#include <linux/uinput.h>

#include <stdio.h>

#define MOVE_TRESHOLD butsize / 3

float scale = 1;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QScreen *screen = QGuiApplication::primaryScreen();
    scale = screen->size().height() / 1080.0;
    setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::Tool
                      | Qt::WindowStaysOnTopHint
                      | Qt::WindowSystemMenuHint
                      | Qt::WindowDoesNotAcceptFocus
                      | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QLabel *move = new QLabel("Test", this);
    QIcon icon = QIcon(":images/move-icon.svg");
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(butsize, butsize)));
    move->setPixmap(pixmap);
    move->setStyleSheet(QString("background-color: none;"));
    move->setFixedSize(butsize, butsize);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setFixedSize(butsize, butsize);

    // long press detect
    timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, this, [=](){longPressDetect();});

    installEventFilter(this);
    this->move(
        (screen->size().width() - butsize) / 2,
        (screen->size().height() - butsize) / 2
    );
    show();
}

void MainWindow::onButtonClicked() {
    setShowMainWindow(false);
    doRightClick(BTN_RIGHT);
}

void MainWindow::longPressDetect(){
    if(pressed){
        long_pressed = true;
        doLongPress();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    pressed = (event->type() == QEvent::MouseButtonPress);
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress) {
        _time = time(NULL);
        // long press detection
        timer->start(LONG_PRESS_TIMEOUT);
        lastPos = mouseEvent->globalPosition();
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (time(NULL) - _time < 150 && !long_pressed) {
            onButtonClicked();
        }
        long_pressed = false;
        timer->stop();
    } else if (event->type() == QEvent::MouseMove) {
        QPoint newpos = mouseEvent->globalPosition().toPoint() - QPoint(width() / 2, height() / 2);
        move(newpos);
        if(toys !=nullptr){
            toys->move(newpos - QPoint(toys->width() / 2, toys->height() / 2));
        }
        if (QLineF(lastPos, mouseEvent->globalPosition()).length() > MOVE_TRESHOLD){
            _time = 0;
            long_pressed = false;
            timer->stop();
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
