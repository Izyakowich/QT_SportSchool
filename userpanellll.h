#ifndef USERPANELLLL_H
#define USERPANELLLL_H

#include <QWidget>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QPushButton>

namespace Ui {
class UserPanel;
}

class UserPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UserPanel(QWidget *parent = nullptr);
    ~UserPanel();

private:
    Ui::UserPanel *ui;

public slots:
    void IndTrU();
    void TrainerU();
    void SectionU();
    void VisitingU();
    void StudentU();
    void PlaceU();
};

#endif // USERPANELLLL_H
