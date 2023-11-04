#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QDialog>
#include <QWidget>
#include "sectiona.h"
#include "trainer.h"
#include "visitinga.h"
#include "studenta.h"
#include "placea.h"
#include "indtra.h"
#include "mainwindow.h"

namespace Ui {
class adminpanel;
}

class adminpanel : public QWidget
{
    Q_OBJECT

public:
    explicit adminpanel(QWidget *parent = nullptr);
    ~adminpanel();

private:
    Ui::adminpanel *ui;

    indtra *indtrA;
    trainer *train;
    sectiona *sectionA;
    studenta *studentA;
    visitinga *visitingA;
    placea *placeA;
//    MainWindow *window;
//signals:
//    void firstwindow();

private slots:

    void on_btnIndTrA_clicked();
    void on_btnTrainerA_clicked();
    void on_btnSectionA_clicked();
    void on_btnVisitingA_clicked();
    void on_btnStudentA_clicked();
    void on_btnPlaceA_clicked();
    void on_pbBack_clicked();
};

#endif // ADMINPANEL_H
