#include "MainWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>

#include <stdio.h>

#define butsize 48

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::Tool
                      | Qt::X11BypassWindowManagerHint
                      | Qt::WindowStaysOnTopHint
                      | Qt::WindowSystemMenuHint
                      | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QLabel *move = new QLabel("Test", this);
    QIcon icon = QIcon(":images/move-icon.svg");
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(butsize, butsize)));
    move->setPixmap(pixmap);
    move->setStyleSheet(QString("background-color: none;"));
    move->setFixedSize(butsize, butsize);

    setFixedSize(butsize, butsize);


    installEventFilter(this);
}

void MainWindow::onButtonClicked() {
    puts("Hello World");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        _time = time(NULL);
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (time(NULL) - _time < 150) {
            onButtonClicked();
        }
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        _time = 0;
        if (mouseEvent->buttons() & Qt::LeftButton) {
            move(mouseEvent->globalPosition().toPoint() - QPoint(width() / 2, height() / 2));
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
