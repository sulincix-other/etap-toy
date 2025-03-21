#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QMainWindow>
#include <QMouseEvent>
#include <QScreen>

#include "../toy.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>

static int click_button = BTN_RIGHT;

class RightClick : public QMainWindow {

public:
    QScreen *screen;
    QWidget *area;

    RightClick(){
        this->setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::Tool
                      | Qt::X11BypassWindowManagerHint
                      | Qt::WindowStaysOnTopHint
                      | Qt::WindowSystemMenuHint
                      | Qt::WindowDoesNotAcceptFocus
                      | Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
        this->installEventFilter(this);
        area = new QWidget(this);
        area->setStyleSheet(QString("background-color: #31A00000;"));
        screen = QGuiApplication::primaryScreen();
        this->setFixedSize(screen->size().width(), screen->size().height());
        area->setFixedSize(screen->size().width(), screen->size().height());
    }

    void resizeEvent(QResizeEvent *event) override {
        (void)event;
        this->setFixedSize(screen->size().width(), screen->size().height());
        area->setFixedSize(screen->size().width(), screen->size().height());
    }

    bool eventFilter(QObject *obj, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonRelease) {
            // Read mouse release event
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            int x = mouseEvent->globalPosition().x();
            int y = mouseEvent->globalPosition().y();
            // hide window
            hide();
            usleep(30000);
            // move mouse event position
            uinput_send(EV_ABS, ABS_X, (x *3840) / screen->size().width());
            uinput_send(EV_ABS, ABS_Y, (y * 2160) / screen->size().height());
            // execute right click down
            uinput_send(EV_KEY, click_button, 1);
            usleep(100000);
            // execute right click up
            uinput_send(EV_KEY, click_button, 0);
            setShowMainWindow(true);
        }
        return QMainWindow::eventFilter(obj, event);
    }
private:
    QProcess proc1;
    QProcess proc2;
    QStringList args1;
    QStringList args2;

};

static RightClick *rc;

void right_click_init(){
    rc = new RightClick();
}

void doRightClick(int but){
    click_button = but;
    rc->show();
}
