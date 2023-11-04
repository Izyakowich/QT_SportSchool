#ifndef PLACEA_H
#define PLACEA_H

#include <QWidget>
#include <QSqlDatabase>

namespace Ui {
class placea;
}

class placea : public QWidget
{
    Q_OBJECT

public:
    explicit placea(QWidget *parent = nullptr);
    ~placea();

private:
    Ui::placea *ui;
    QSqlDatabase dbconn;

public slots:
    void SelectAllPlA();
    void DelPlA();
    void AddPlA();
    void BackPlA();
    void dbconnect();
};

#endif // PLACEA_H
