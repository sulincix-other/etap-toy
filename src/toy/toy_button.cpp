#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>

#include <QtWidgets>
#include <QPainter>
#include <QMap>
#include <stdio.h>

#include <linux/uinput.h>

#include <math.h>

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
        QString("background-color: #69313131;") +
        QString("border: 3px solid #69696969;") +
        QString("border-radius: ")+QString::number(butsize*2)+QString("px;")
    );
    move->setFixedSize(butsize*4, butsize*4);
}


ToyButton::ToyButton(QWidget *parent, QString path) : QWidget(parent) {
    img = new QLabel("", this);
    img->setStyleSheet(QString("background-color: none;"));
    img->setFixedSize(butsize, butsize);
    setFixedSize(butsize, butsize);
    setImage(path);
    installEventFilter(this);
    show();
}

void ToyButton::setImage(QString path) {
    QIcon icon = QIcon(path);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(butsize, butsize)));
    img->setPixmap(pixmap);
}

void ToyButton::action(){
    puts(type.toStdString().c_str());
    if(type == "command"){
        (void)system((actionValue+"&").toStdString().c_str());
        setShowMainWindow(true);
    } else if(type == "click"){
        if(actionValue == "right"){
            doRightClick(BTN_RIGHT);
        } else if(actionValue == "middle"){
            doRightClick(BTN_MIDDLE);
        } else if(actionValue == "left"){
            doRightClick(BTN_LEFT);
        }
    } else if(type == "lock"){
        doLockScreen();
    }
    toys->hide();
}

bool ToyButton::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonRelease) {
        action();
    }
    return QWidget::eventFilter(obj, event);
}



ToyWindow *toys;

void toy_button_init(){
    toys = new ToyWindow(NULL);
    QSettings settings(":/etap-toy.conf", QSettings::IniFormat);
    const QStringList childGroups = settings.childGroups();
    foreach (const QString &str, childGroups){
        settings.beginGroup(str);
        QString icon = settings.value("icon").toString();
        toys->buttons[str] = new ToyButton(toys, icon);
        toys->buttons[str]->actionValue = settings.value("value").toString();
        toys->buttons[str]->type = settings.value("type").toString();
        settings.endGroup();

    }
    toy_reload();
}

void toy_reload(){
    float total = toys->buttons.count();
    float i=0;
    for (auto it = toys->buttons.begin(); it != toys->buttons.end(); ++it) {
        int x = butsize*2 + (sin((2*i*M_PI) / total) * butsize * 1.2) - (butsize/2);
        int y = butsize*2 + (cos((2*i*M_PI) / total) * butsize * 1.2) - (butsize/2);
        printf("%f %f %f\n", total, i, M_PI);
        toys->buttons[it.key()]->move(x,y);
        i++;
    }
}

void doLongPress(){
    if(toys != nullptr){
        setShowMainWindow(false);
        toys->show();
    }
    puts("Long press!");
}
