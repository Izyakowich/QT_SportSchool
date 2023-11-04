#include "userpanel.h"
#include "ui_userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>

UserPanel::UserPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPanel)
{
    ui->setupUi(this);
    connect(ui->btnIndTrU, SIGNAL(clicked(bool)), this, SLOT(IndTrU()));
    connect(ui->btnTrainerU, SIGNAL(clicked(bool)), this, SLOT(TrainerU()));
    connect(ui->btnSectionU, SIGNAL(clicked(bool)), this, SLOT(SectionU()));
    connect(ui->btnVisitingU, SIGNAL(clicked(bool)), this, SLOT(VisitingU()));
    connect(ui->btnStudentU, SIGNAL(clicked(bool)), this, SLOT(StudentU()));
    connect(ui->btnPlaceU, SIGNAL(clicked(bool)), this, SLOT(PlaceU()));
}

