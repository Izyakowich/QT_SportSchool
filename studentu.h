#ifndef STUDENTU_H
#define STUDENTU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class studentu;
}

class studentu : public QWidget
{
    Q_OBJECT

public:
    explicit studentu(QWidget *parent = nullptr);
    ~studentu();

private:
    Ui::studentu *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllU();
    void BackStU();
    void dbconnect();

};

#endif // STUDENTU_H
