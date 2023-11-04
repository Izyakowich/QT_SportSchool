#include "intru.h"
#include "ui_intru.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

intru::intru(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::intru)
{
    ui->setupUi(this);

    //connect(ui->btnSelectAllIndU, SIGNAL(clicked(bool)), this, SLOT(SelectAllIndU()));
    connect(ui->btnBackIndU, SIGNAL(clicked(bool)), this, SLOT(BackIndU()));
    connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

intru::~intru()
{
    delete ui;
}

void intru::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teIndU->append("SQL drivers:");
        ui->teIndU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teIndU->append("Connect is open...");
        else
            {
            ui->teIndU->append("Error of connect:");
            ui->teIndU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teIndU->append("Connect is already open...");

    // Количество столбцов
    ui->twIndU->setColumnCount(5);
    // Возможность прокрутки
    ui->twIndU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twIndU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twIndU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twIndU->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twIndU->setHorizontalHeaderItem(1,new QTableWidgetItem("Тренер"));
    ui->twIndU->setHorizontalHeaderItem(2,new QTableWidgetItem("Ученик"));
    ui->twIndU->setHorizontalHeaderItem(3,new QTableWidgetItem("День"));
    ui->twIndU->setHorizontalHeaderItem(4,new QTableWidgetItem("Время"));

    // Последний столбец растягивается при изменении размера формы
    ui->twIndU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twIndU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twIndU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllIndU();
}

void intru::SelectAllIndU(){
    // Очистить содержимое компонента
         ui->twIndU->clearContents();
         // Создать объект запроса с привязкой к установленному соединению
         QSqlQuery query(dbconn);
         // Создать строку запроса на выборку данных
         QString sqlstr = "select * from ind_train";
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
             ui->twIndU->setRowCount( query.size());
         }
         else{
             ui->twIndU->setRowCount(0);
         }
         ui->teIndU->append( QString("Read %1 rows").arg(query.size()));
         // Прочитать в цикле все строки результата (курсора)
         // и вывести их в компонент таблицы
         int i=0;
         while(query.next())
         {
             ui->twIndU->setItem(i,0,new QTableWidgetItem(query.value("name_train").toString()));
             ui->twIndU->setItem(i,1,new QTableWidgetItem(query.value("trainer").toString()));
             ui->twIndU->setItem(i,2,new QTableWidgetItem(query.value("student").toString()));
             ui->twIndU->setItem(i,3,new QTableWidgetItem(query.value("days").toString()));
             ui->twIndU->setItem(i,4,new QTableWidgetItem(query.value("time").toString()));

             i++;
         }
}

void intru::BackIndU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
