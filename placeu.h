#ifndef PLACEU_H
#define PLACEU_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class placeu;
}

class placeu : public QWidget
{
    Q_OBJECT

public:
    explicit placeu(QWidget *parent = nullptr);
    ~placeu();

private:
    Ui::placeu *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllPlU();
    void BackPlU();
    void dbconnect();

};

#endif // PLACEU_H
