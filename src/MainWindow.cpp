#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

#include "toy.h"

#include <sys/time.h>
#include <linux/uinput.h>

#include <stdio.h>

#define MOVE_TRESHOLD butsize


static long utime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

float scale = 1;


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QScreen *screen = QGuiApplication::primaryScreen();
    scale = screen->size().height() / 1080.0;
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
    do_move(settings->value("last-position").toPoint());
    initFlags();
    show();
}

void MainWindow::initFlags(){
    setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::Tool
                      | Qt::WindowStaysOnTopHint
                      | Qt::WindowSystemMenuHint
                      | Qt::WindowDoesNotAcceptFocus
                      | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::onButtonClicked() {
    setShowMainWindow(false);
    doRightClick(BTN_RIGHT);
}

void MainWindow::longPressDetect(){
    if(move_lock){
        long_pressed = true;
        doLongPress();
    }
}

void MainWindow::setHide(bool status){
    hide_lock = false;
    if(status){
        hide();
    } else {
        show();
    }
    hide_lock = true;
}

void MainWindow::do_move(QPoint newpos){
    move(newpos);
    if(toys != nullptr){
        toys->move(newpos - QPoint(toys->width() / 2, toys->height() / 2));
    }
}

#define newpos (mouseEvent->globalPosition().toPoint() - \
    QPoint(width() / 2, height() / 2))

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    pressed = (event->type() == QEvent::MouseButtonPress);
    if(event->type() == QEvent::Hide || event->type() == QEvent::WindowStateChange){
        if(hide_lock){
            hide();
            initFlags();
            show();
        }
    }
    //printf("%d\n", event->type());
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    if (event->type() == QEvent::MouseButtonPress) {
        _time = utime();
        // long press detection
        timer->start(LONG_PRESS_TIMEOUT);
        move_lock = true;
        lastPos = mouseEvent->globalPosition();
    } else if (event->type() == QEvent::MouseButtonRelease) {
        float delay = utime() - _time;
        if (delay < 150 && !long_pressed) {
            onButtonClicked();
        }
        long_pressed = false;
        timer->stop();
        if(settings != nullptr) {
            settings->setValue("last-position", newpos);
        }
    } else if (event->type() == QEvent::MouseMove) {
        if (QLineF(lastPos, mouseEvent->globalPosition()).length() > MOVE_TRESHOLD){
            _time = 0;
            long_pressed = false;
            timer->stop();
            move_lock = false;
        }
        if(!move_lock){
            do_move(newpos);
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
