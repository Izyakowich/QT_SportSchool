#include "dialog.h"
#include "ui_dialog.h"
#include "indtra.h"

dialog::dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
}

dialog::~dialog()
{
    delete ui;
}


void dialog::on_btnIndTrA_clicked()
{
    this->close();
    indtra *indtrA = new indtra(this);
    indtrA->show();
    return;
}
