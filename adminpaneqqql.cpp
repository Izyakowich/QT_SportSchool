#include "adminpanel.h"
#include "ui_adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDialog>

AdminPanel::AdminPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);
    connect(ui->btnIndTrA, SIGNAL(clicked(bool)), this, SLOT(IndTrA()));
    connect(ui->btnTrainerA, SIGNAL(clicked(bool)), this, SLOT(TrainerA()));
    connect(ui->btnSectionA, SIGNAL(clicked(bool)), this, SLOT(SectionA()));
    connect(ui->btnVisitingA, SIGNAL(clicked(bool)), this, SLOT(VisitingA()));
    connect(ui->btnStudentA, SIGNAL(clicked(bool)), this, SLOT(StudentA()));
    connect(ui->btnPlaceA, SIGNAL(clicked(bool)), this, SLOT(PlaceA()));
}

AdminPanel::~AdminPanel()
{
    delete ui;
}

