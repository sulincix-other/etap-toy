#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

#include "toy.h"

#include <stdio.h>


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
    this->move(0,0);
    show();
}

void MainWindow::onButtonClicked() {
    doRightClick();
}

void MainWindow::longPressDetect(){
    if(pressed){
        long_pressed = true;
        doLongPress();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    pressed = (event->type() == QEvent::MouseButtonPress);
    if (event->type() == QEvent::MouseButtonPress) {
        _time = time(NULL);
        // long press detection
        timer->start(LONG_PRESS_TIMEOUT);
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (time(NULL) - _time < 150 && !long_pressed) {
            onButtonClicked();
        }
        long_pressed = false;
        timer->stop();
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        _time = 0;
        if (mouseEvent->buttons() & Qt::LeftButton) {
            move(mouseEvent->globalPosition().toPoint() - QPoint(width() / 2, height() / 2));
            return true;
        }
        long_pressed = false;
        timer->stop();
    }
    return QMainWindow::eventFilter(obj, event);
}
