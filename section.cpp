#include "section.h"
#include "ui_section.h"

section::section(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::section)
{
    ui->setupUi(this);
}

section::~section()
{
    delete ui;
}
