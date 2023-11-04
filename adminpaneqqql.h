#ifndef ADMINPANEQQQL_H
#define ADMINPANEQQQL_H

#include <QWidget>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QPushButton>
#include <QDialog>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    Ui::Form *ui;

public slots:
    void IndTrA();
    void TrainerA();
    void SectionA();
    void VisitingA();
    void StudentA();
    void PlaceA();
};

#endif // ADMINPANEQQQL_H
