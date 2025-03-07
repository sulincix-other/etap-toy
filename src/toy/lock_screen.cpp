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
#include <QPalette>
#include <QPixmap>

#include <stdbool.h>

#include "../toy.h"

class LockScreen : public QMainWindow {
public:
    QScreen *screen;
    QWidget *area;
    QWidget *dialog;
    QLabel *label;
    QLineEdit *passwordLineEdit;

    LockScreen() {
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    screen = QGuiApplication::primaryScreen();
    this->setFixedSize(screen->size());

    area = new QWidget(this);
    area->setStyleSheet("background-color: black"); // Semi-transparent background
    area->setFixedSize(screen->size());

    dialog = new QWidget(area);
    dialog->setStyleSheet("background-color: #ff313131; border-radius: "+QString::number(10*scale)+"px;"); // Rounded corners
    dialog->setFixedSize(400*scale, 200*scale); // Fixed size for the dialog
    dialog->move((screen->size().width() - dialog->size().width()) / 2,
                 (screen->size().height() - dialog->size().height()) / 2);

    label = new QLabel("");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "color: red;"
        "font-size: "+QString::number(18*scale)+"px;"
    );

    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText(_("Enter Password"));
    passwordLineEdit->setStyleSheet(
        "background-color: #232323;"
        "color: white;"
        "font-size: "+QString::number(18*scale)+"px;"
        "padding: "+QString::number(10*scale)+"px;"
    );

    QPushButton *okButton = new QPushButton(_("Unlock"));
    okButton->setStyleSheet(
        "background-color: #4CAF50;"
        "color: black;"
        "border: none;"
        "font-size: "+QString::number(18*scale)+"px;"
        "border-radius: "+QString::number(5*scale)+"px;"
        "padding: "+QString::number(10*scale)+"px;"
    );

    connect(okButton, &QPushButton::clicked, this, &LockScreen::auth);


    QPushButton *kbdButton = new QPushButton("");
    QIcon icon = QIcon(":images/keyboard.svg");
    kbdButton->setIcon(icon);
    kbdButton->setIconSize(QSize(butsize, butsize));
    kbdButton->setFixedSize(butsize*1.3, butsize*1.2);
    kbdButton->setStyleSheet(
        "background-color: #2196F3;"
        "color: white;"
        "border: none;"
        "font-size: "+QString::number(18*scale)+"px;"
        "border-radius: "+QString::number(5*scale)+"px;"
        "padding: "+QString::number(10*scale)+"px;"
    );


    connect(kbdButton, &QPushButton::clicked, this, &LockScreen::openEtaKeyboard);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LockScreen::auth);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addWidget(okButton);
    mainLayout->addWidget(label);
    mainLayout->addWidget(kbdButton); // Add the command button to the layout
    dialog->setLayout(mainLayout);
    }

    // Example slot for the command button
    void openEtaKeyboard() {
        (void)system("eta-keyboard show &");
    }


    void resizeEvent(QResizeEvent *event) override {
        (void)event;
        this->setFixedSize(screen->size());
        area->setFixedSize(screen->size());
    }

    void auth() {
        const char* username = getenv("USER");
        if(username == nullptr){
            return;
        }
        std::string passwordString = passwordLineEdit->text().toStdString();
        const char* password = passwordString.c_str();
        bool isOk = false;
        if (auth_totp()){
            isOk = true;
        }else if (pam_auth(username, password)) {
            isOk = true;
        } else {
            updateLabel(_("Incorrect password. Please try again."));
        }
        if (isOk){
            hide();
            setShowMainWindow(true);
        }
        passwordLineEdit->setText("");
        updateLabel("");
    }

    bool auth_totp(){
#ifdef COTP
        QProcess process;
        process.start("etap-totp", QStringList() << passwordLineEdit->text());
        process.waitForFinished();
        return process.exitCode() == 0;
#else
       return false;
#endif
    }

    int updateLabel(const char* msg) {
        label->setText(QString(msg));
        return 0;
    }
};

static LockScreen *lock;

static int update_label(const char* msg) {
    return lock->updateLabel(msg);
}

void lock_screen_init() {
    lock = new LockScreen();
    pam_show_cb = update_label;
}

void doLockScreen() {
    lock->showFullScreen();
}

