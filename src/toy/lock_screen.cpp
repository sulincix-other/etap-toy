#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QMainWindow>
#include <QMouseEvent>
#include <QScreen>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <stdbool.h>

#include "../toy.h"
class LockScreen : public QMainWindow {

public:
    QScreen *screen;
    QWidget *area;
    QWidget *dialog;
    QLabel *label;
    QLineEdit *passwordLineEdit;

    LockScreen(){
        this->setWindowFlags(Qt::WindowStaysOnTopHint
                      | Qt::FramelessWindowHint);
        this->setAttribute(Qt::WA_TranslucentBackground);
        area = new QWidget(this);
        dialog = new QWidget(area);
        area->setStyleSheet(QString("background-color: #ee313131;"));
        dialog->setStyleSheet(QString("background-color: none;"));
        screen = QGuiApplication::primaryScreen();
        this->setFixedSize(screen->size().width(), screen->size().height());
        area->setFixedSize(screen->size().width(), screen->size().height());
        dialog->setFixedSize(butsize * 6, butsize * 3);
        dialog->move(
            (screen->size().width() - dialog->size().width()) / 2,
            (screen->size().height() - dialog->size().height()) / 2
        );

        label = new QLabel("");
        passwordLineEdit = new QLineEdit();
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        QPushButton *okButton = new QPushButton("Unlock");

        connect(okButton, &QPushButton::clicked, this, &LockScreen::auth);

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->addWidget(passwordLineEdit);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(okButton);
        mainLayout->addLayout(buttonLayout);
        
        QHBoxLayout *labelLayout = new QHBoxLayout;
        labelLayout->addWidget(label);
        mainLayout->addLayout(labelLayout);

        dialog->setLayout(mainLayout);
        
        passwordLineEdit->setFixedSize(butsize*5, butsize);
        okButton->setFixedSize(butsize*3, butsize);
    }
    int updateLabel(const char* msg){
        label->setText(QString(msg));
        return 0;
    }

    void resizeEvent(QResizeEvent *event) override {
        (void)event;
        this->setFixedSize(screen->size().width(), screen->size().height());
        area->setFixedSize(screen->size().width(), screen->size().height());
    }
    void auth(){
        const char* username = getenv("USER");
        const char* password = passwordLineEdit->text().toStdString().c_str();
        puts(password);
        if(pam_auth(username, password)){
            hide();
        }
    }
    
};
static LockScreen *lock;

static int update_label(const char* msg){
    return lock->updateLabel(msg);
}

void lock_screen_init(){
    lock = new LockScreen();
    pam_show_cb = update_label;
    lock->showFullScreen();
}

void doLockScreen(){

}
