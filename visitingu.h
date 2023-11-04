#ifndef VISITINGU_H
#define VISITINGU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class visitingu;
}

class visitingu : public QWidget
{
    Q_OBJECT

public:
    explicit visitingu(QWidget *parent = nullptr);
    ~visitingu();

private:
    Ui::visitingu *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllViU();
    void BackViU();
    void dbconnect();
    //void vis();
};

#endif // VISITINGU_H
