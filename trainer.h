#ifndef TRAINER_H
#define TRAINER_H

#include <QWidget>
#include <QSqlDatabase>


namespace Ui {
class trainer;
}

class trainer : public QWidget
{
    Q_OBJECT

public:
    explicit trainer(QWidget *parent = nullptr);
    ~trainer();

private:
    Ui::trainer *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllTrA();
    void DelTrA();
    void AddTrA();
    void BackTrA();
    void dbconnect();

private slots:
    void on_twTrainerA_cellClicked(int row, int column);
    void on_bntEdit_clicked();
};

#endif // TRAINER_H
