#include "adminpanel.h"
#include "ui_adminpanel.h"
#include "sectiona.h"
#include "placea.h"
#include "studenta.h"
#include "visitinga.h"
#include "mainwindow.h"

adminpanel::adminpanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminpanel)
{
    ui->setupUi(this);
    //connect(ui->pbBack, SIGNAL(clicked(bool)), this, SLOT(on_pbBack_clicked()));

}

adminpanel::~adminpanel()
{
    delete ui;
}


void adminpanel::on_btnIndTrA_clicked()
{
    this->close();
    indtrA = new indtra();
    indtrA->show();
    return;
}


void adminpanel::on_btnTrainerA_clicked()
{
    this->close();
    train = new trainer();
    train->show();
    return;
}


void adminpanel::on_btnSectionA_clicked()
{
    this->close();
    sectionA = new sectiona();
    sectionA->show();
    return;

}


void adminpanel::on_btnVisitingA_clicked()
{
    this->close();
    visitingA = new visitinga();
    visitingA->show();
    return;
}


void adminpanel::on_btnStudentA_clicked()
{
    this->close();
    studentA = new studenta();
    studentA->show();
    return;
}


void adminpanel::on_btnPlaceA_clicked()
{
    this->close();
    placeA = new placea();
    placeA->show();
    return;
}


void adminpanel::on_pbBack_clicked()
{
    this->close();
    MainWindow *window = new MainWindow();
    window->show();
    return;

}

