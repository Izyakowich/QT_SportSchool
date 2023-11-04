#ifndef SECTIONU_H
#define SECTIONU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class sectionu;
}

class sectionu : public QWidget
{
    Q_OBJECT

public:
    explicit sectionu(QWidget *parent = nullptr);
    ~sectionu();

private:
    Ui::sectionu *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllU();
    void BackU();
    void dbconnect();

};

#endif // SECTIONU_H
