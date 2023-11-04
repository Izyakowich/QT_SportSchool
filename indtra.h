#ifndef INDTRA_H
#define INDTRA_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class indtra;
}

class indtra : public QWidget
{
    Q_OBJECT

public:
    explicit indtra(QWidget *parent = nullptr);
    ~indtra();

private:
    Ui::indtra *ui;
    QSqlDatabase dbconn;

protected:
    //void showEvent(QShowEvent *event) override;


private slots:
    void SelectAllIndA();
    void DelIndA();
    void AddIndA();
    void BackIndA();
    //void clearInf();
    int countRows();
    void dbconnect();

};

#endif // INDTRA_H
