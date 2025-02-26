#include <QLabel>

extern "C" {
    void doLongPress();
    void toy_reload();
    void toy_button_init();
}
class ToyButton : public QWidget {
public:
    ToyButton(QWidget *parent, QString path);
    void dummy();
    void setImage(QString path);
    QString command;
private:
    QLabel *img;
};

class ToyWindow : public QMainWindow {
public:
    QMap<QString, ToyButton*> buttons;
    ToyWindow(QWidget *parent);
};

extern ToyWindow *toys;