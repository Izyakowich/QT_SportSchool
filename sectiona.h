#ifndef SECTIONA_H
#define SECTIONA_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class sectiona;
}

class sectiona : public QWidget
{
    Q_OBJECT

public:
    explicit sectiona(QWidget *parent = nullptr);
    ~sectiona();

private:
    Ui::sectiona *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllA();
    void DelA();
    void AddA();
    void BackA();
    void dbconnect();

private slots:
    void on_twSectionA_cellClicked(int row, int column);
    void on_bntEdit_clicked();
};

#endif // SECTIONA_H
