#ifndef TRAINERU_H
#define TRAINERU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class traineru;
}

class traineru : public QWidget
{
    Q_OBJECT

public:
    explicit traineru(QWidget *parent = nullptr);
    ~traineru();

private:
    Ui::traineru *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllU();
    void BackU();
    void dbconnect();

};

#endif // TRAINERU_H
