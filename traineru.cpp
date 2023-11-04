#include "traineru.h"
#include "ui_traineru.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

traineru::traineru(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::traineru)
{
    ui->setupUi(this);
    //connect(ui->btnSelectAllU, SIGNAL(clicked(bool)), this, SLOT(SelectAllU()));
    connect(ui->btnBackU, SIGNAL(clicked(bool)), this, SLOT(BackU()));
    connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

traineru::~traineru()
{
    delete ui;
}

void traineru::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teTrainerU->append("SQL drivers:");
        ui->teTrainerU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teTrainerU->append("Connect is open...");
        else
            {
            ui->teTrainerU->append("Error of connect:");
            ui->teTrainerU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teTrainerU->append("Connect is already open...");

    // Количество столбцов
    ui->twTrainerU->setColumnCount(3);
    // Возможность прокрутки
    ui->twTrainerU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twTrainerU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twTrainerU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twTrainerU->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twTrainerU->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер телефона"));
    ui->twTrainerU->setHorizontalHeaderItem(2,new QTableWidgetItem("Информация"));

    // Последний столбец растягивается при изменении размера формы
    ui->twTrainerU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twTrainerU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twTrainerU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllU();
}


void traineru::SelectAllU(){
    ui->twTrainerU->clearContents();

    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from trainer";
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
        ui->twTrainerU->setRowCount( query.size());
    }
    else{
        ui->twTrainerU->setRowCount(0);
    }
    ui->teTrainerU->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twTrainerU->setItem(i,0,new QTableWidgetItem(query.value("fio").toString()));
        ui->twTrainerU->setItem(i,1,new QTableWidgetItem(query.value("phone_num").toString()));
        ui->twTrainerU->setItem(i,2,new QTableWidgetItem(query.value("inf").toString()));

        i++;
    }
    return;
}


void traineru::BackU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
