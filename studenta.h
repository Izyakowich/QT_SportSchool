#ifndef STUDENTA_H
#define STUDENTA_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class studenta;
}

class studenta : public QWidget
{
    Q_OBJECT

public:
    explicit studenta(QWidget *parent = nullptr);
    ~studenta();

private:
    Ui::studenta *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllStA();
    void DelStA();
    void AddStA();
    void BackStA();
    void dbconnect();

private slots:
    void on_twStudent_cellClicked(int row, int column);
    void on_bntEdit_clicked();
};

#endif // STUDENTA_H
