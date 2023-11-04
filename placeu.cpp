#include "placeu.h"
#include "ui_placeu.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

placeu::placeu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::placeu)
{
    ui->setupUi(this);

   //connect(ui->btnSelectAllPlU, SIGNAL(clicked(bool)), this, SLOT(SelectAllPlU()));
    connect(ui->btnBackPlU, SIGNAL(clicked(bool)), this, SLOT(BackPlU()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

placeu::~placeu()
{
    delete ui;
}

void placeu::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->tePlaceU->append("SQL drivers:");
        ui->tePlaceU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->tePlaceU->append("Connect is open...");
        else
            {
            ui->tePlaceU->append("Error of connect:");
            ui->tePlaceU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->tePlaceU->append("Connect is already open...");

    // Количество столбцов
    ui->twPlaceU->setColumnCount(2);
    // Возможность прокрутки
    ui->twPlaceU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twPlaceU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twPlaceU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twPlaceU->setHorizontalHeaderItem(0,new QTableWidgetItem("Кабинет"));
    ui->twPlaceU->setHorizontalHeaderItem(1,new QTableWidgetItem("Адрес"));

    // Последний столбец растягивается при изменении размера формы
    ui->twPlaceU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twPlaceU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twPlaceU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllPlU();
}
void placeu::SelectAllPlU(){
    ui->twPlaceU->clearContents();

    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from place";
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
        ui->twPlaceU->setRowCount( query.size());
    }
    else{
        ui->twPlaceU->setRowCount(0);
    }
    ui->tePlaceU->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twPlaceU->setItem(i,0,new QTableWidgetItem(query.value("cabinet").toString()));
        ui->twPlaceU->setItem(i,1,new QTableWidgetItem(query.value("address").toString()));

        i++;
    }
    return;
}

void placeu::BackPlU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
