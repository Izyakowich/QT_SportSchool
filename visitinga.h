#ifndef VISITINGA_H
#define VISITINGA_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class visitinga;
}

class visitinga : public QWidget
{
    Q_OBJECT

public:
    explicit visitinga(QWidget *parent = nullptr);
    ~visitinga();

private:
    Ui::visitinga *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllViA();
    void DelViA();
    void AddViA();
    void BackViA();
    void dbconnect();

private slots:
    void on_bntEdit_clicked();
    void on_twVisitA_cellClicked(int row, int column);
};

#endif // VISITINGA_H
