#include "userpanel.h"
#include "ui_userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include "intru.h"
#include "placeu.h"
#include "sectionu.h"
#include "studentu.h"
#include "traineru.h"
#include "visitingu.h"
#include "mainwindow.h"

userpanel::userpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userpanel)
{
    ui->setupUi(this);
    //connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(on_pushButton_clicked()));

}

userpanel::~userpanel()
{
    delete ui;
}


void userpanel::on_btnIndTrU_clicked()
{
    this->close();
    indtrU = new intru();
    indtrU->show();
    return;
}


void userpanel::on_btnTrainerU_clicked()
{
    this->close();
    trainerU = new traineru();
    trainerU->show();
    return;
}


void userpanel::on_btnSectionU_clicked()
{
    this->close();
    sectionU = new sectionu();
    sectionU->show();
    return;
}


void userpanel::on_btnVisitingU_clicked()
{
    this->close();
    visitingU = new visitingu();
    visitingU->show();
    return;
}


void userpanel::on_btnStudentU_clicked()
{
    this->close();
    studentU = new studentu();
    studentU->show();
    return;
}


void userpanel::on_btnPlaceU_clicked()
{
    this->close();
    placeU = new placeu();
    placeU->show();
    return;
}


void userpanel::on_pushButton_clicked()
{
    this->close();
    MainWindow *window = new MainWindow();
    window->show();
    return;
}

