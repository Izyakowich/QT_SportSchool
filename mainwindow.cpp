#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_errorLabel = new QLabel();
    m_errorLabel->setStyleSheet("QLabel { color : red; }");

    connect(ui->btnAuth, SIGNAL(clicked(bool)), this, SLOT(Auth()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Auth(){
    if(!dbconn.isOpen())
    {
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль
        dbconn.setDatabaseName("AIS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        if(!dbconn.open()){
            QMessageBox::warning(this, "Ошибка", "Ошибка подкючения");
            return;
            }
    }
    else{
    // Если соединение уже открыто, то сообщить об этом
        m_errorLabel->setText("Connect is already open...");
    }

    //QString login = ui->comboBox->text();
    QString password = ui->lePassword->text();

    //if (login == "admin" && password == "111"){
    if (password == "111"){
        this->close();
        adminpanel *adminP = new adminpanel();
        adminP->show();
    //}else if (login == "user" && password == "222"){
    }else if (password == "222"){
        this->close();
        userpanel *userP = new userpanel();
        userP->show();
    }else{
        QMessageBox::warning(this, "Ошибка", "Неверный логин или пароль");
    }
    return;
}
