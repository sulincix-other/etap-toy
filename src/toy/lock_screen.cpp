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
    dialog->setStyleSheet("background-color: #ff313131; border-radius: 10px;"); // Rounded corners
    dialog->setFixedSize(400, 200); // Fixed size for the dialog
    dialog->move((screen->size().width() - dialog->size().width()) / 2,
                 (screen->size().height() - dialog->size().height()) / 2);

    label = new QLabel("");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: red;"); // Error message color

    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setPlaceholderText("Enter Password");
    passwordLineEdit->setStyleSheet("color: white;");

    QPushButton *okButton = new QPushButton("Unlock");
    okButton->setStyleSheet("background-color: #4CAF50; color: black; border: none; border-radius: 5px; padding: 10px;");

    connect(okButton, &QPushButton::clicked, this, &LockScreen::auth);


    QPushButton *kbdButton = new QPushButton("Kbd");
    kbdButton->setFixedSize(80*scale, 30*scale);
    kbdButton->setStyleSheet("background-color: #2196F3; color: white; border: none; border-radius: 5px; padding: 5px;");


    connect(kbdButton, &QPushButton::clicked, this, &LockScreen::openEtaKeyboard);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(passwordLineEdit);
    mainLayout->addWidget(okButton);
    mainLayout->addWidget(label);
    mainLayout->addWidget(kbdButton); // Add the command button to the layout
    dialog->setLayout(mainLayout);
    }

    // Example slot for the command button
    void openEtaKeyboard() {
        system("eta-keyboard show &");
    }


    void resizeEvent(QResizeEvent *event) override {
        (void)event;
        this->setFixedSize(screen->size());
        area->setFixedSize(screen->size());
    }

    void auth() {
        const char* username = getenv("USER");
        std::string passwordString = passwordLineEdit->text().toStdString();
        const char* password = passwordString.c_str();
        bool isOk = false;
        if (totp_auth("amogus",password)){
            isOk = true;
        }else if (pam_auth(username, password)) {
            isOk = true;
        } else {
            updateLabel("Incorrect password. Please try again.");
        }
        if (isOk){
            hide();
            setShowMainWindow(true);
        }
        passwordLineEdit->setText("");
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

