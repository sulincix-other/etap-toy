#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#define LONG_PRESS_TIMEOUT 450

class MainWindow : public QMainWindow {

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onButtonClicked();
    void longPressDetect();

private:
    int _time;
    QTimer *timer;
    QPointF lastPos;
    bool pressed = false;
    bool long_pressed = false;
};

#endif // MAINWINDOW_H
