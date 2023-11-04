#ifndef INTRU_H
#define INTRU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class intru;
}

class intru : public QWidget
{
    Q_OBJECT

public:
    explicit intru(QWidget *parent = nullptr);
    ~intru();

private:
    Ui::intru *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllIndU();
    void BackIndU();
    void dbconnect();
};

#endif // INTRU_H
