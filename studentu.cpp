#include "studentu.h"
#include "ui_studentu.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

studentu::studentu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studentu)
{
    ui->setupUi(this);
    //connect(ui->btnSelectAllStU, SIGNAL(clicked(bool)), this, SLOT(SelectAllU()));
    connect(ui->btnBackStU, SIGNAL(clicked(bool)), this, SLOT(BackStU()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

studentu::~studentu()
{
    delete ui;
}

void studentu::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teStudentU->append("SQL drivers:");
        ui->teStudentU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teStudentU->append("Connect is open...");
        else
            {
            ui->teStudentU->append("Error of connect:");
            ui->teStudentU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teStudentU->append("Connect is already open...");

    // Количество столбцов
    ui->twStudentU->setColumnCount(2);
    // Возможность прокрутки
    ui->twStudentU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twStudentU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twStudentU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twStudentU->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twStudentU->setHorizontalHeaderItem(1,new QTableWidgetItem("Дата рождения"));
    // Последний столбец растягивается при изменении размера формы
    ui->twStudentU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twStudentU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twStudentU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllU();
}

void studentu::SelectAllU(){
    ui->twStudentU->clearContents();

    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from student";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return;
    }
    // Если запрос активен (успешно завершен),
    // то вывести сообщение о прочитанном количестве строк в окно вывода
    // и установить количество строк для компонента таблицы
    if( query.isActive()){
        ui->twStudentU->setRowCount( query.size());
    }
    else{
        ui->twStudentU->setRowCount(0);
    }
    ui->teStudentU->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twStudentU->setItem(i,0,new QTableWidgetItem(query.value("fio").toString()));
        ui->twStudentU->setItem(i,1,new QTableWidgetItem(query.value("bthd_date").toString()));
        i++;
    }
    return;
}

void studentu::BackStU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
