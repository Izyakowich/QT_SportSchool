#ifndef USERPANELLLL_H
#define USERPANELLLL_H

#include <QWidget>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QPushButton>
#include <QDialog>
#include "placeu.h"
#include "intru.h"
#include "sectionu.h"
#include "studentu.h"
#include "traineru.h"
#include "visitingu.h"
#include "mainwindow.h"

namespace Ui {
class userpanel;
}

class userpanel : public QWidget
{
    Q_OBJECT

public:
    explicit userpanel(QWidget *parent = nullptr);
    ~userpanel();

private:
    Ui::userpanel *ui;

    sectionu *sectionU;
    intru *indtrU;
    traineru *trainerU;
    visitingu *visitingU;
    studentu *studentU;
    placeu *placeU;
//    MainWindow *window;

private slots:
    void on_btnIndTrU_clicked();
    void on_btnTrainerU_clicked();
    void on_btnSectionU_clicked();
    void on_btnVisitingU_clicked();
    void on_btnStudentU_clicked();
    void on_btnPlaceU_clicked();
    void on_pushButton_clicked();
};

#endif // USERPANELLLL_H
