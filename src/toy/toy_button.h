#include <QLabel>

extern "C" {
    void doLongPress();
    void toy_reload();
    void toy_button_init();
}

class ToyButton : public QWidget {
public:
    ToyButton(QWidget *parent, QString path);
    QString type;
    void dummy();
    void setImage(QString path);
    QString actionValue;
    void action();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLabel *img;
};

class ToyWindow : public QMainWindow {
public:
    QMap<QString, ToyButton*> buttons;
    ToyWindow(QWidget *parent);
};

extern ToyWindow *toys;