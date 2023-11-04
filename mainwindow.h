#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QPushButton>
#include <QWidget>
#include <adminpanel.h>
#include <userpanel.h>
#include <QDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QLineEdit>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *m_errorLabel;
    QSqlDatabase dbconn;
//    adminpanel *admin_panel;
//    userpanel *user_panel;

public slots:
    void Auth();
};
#endif // MAINWINDOW_H
