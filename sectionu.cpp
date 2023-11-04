#include "sectionu.h"
#include "ui_sectionu.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

sectionu::sectionu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sectionu)
{
    ui->setupUi(this);

   // connect(ui->btnSelectAllU, SIGNAL(clicked(bool)), this, SLOT(SelectAllU()));
    connect(ui->btnBackU, SIGNAL(clicked(bool)), this, SLOT(BackU()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

sectionu::~sectionu()
{
    delete ui;
}

void sectionu::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teSectionU->append("SQL drivers:");
        ui->teSectionU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teSectionU->append("Connect is open...");
        else
            {
            ui->teSectionU->append("Error of connect:");
            ui->teSectionU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teSectionU->append("Connect is already open...");

    // Количество столбцов
    ui->twSectionU->setColumnCount(3);
    // Возможность прокрутки
    ui->twSectionU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twSectionU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twSectionU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twSectionU->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twSectionU->setHorizontalHeaderItem(1,new QTableWidgetItem("Место"));
    ui->twSectionU->setHorizontalHeaderItem(2,new QTableWidgetItem("Длительность"));

    // Последний столбец растягивается при изменении размера формы
    ui->twSectionU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twSectionU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twSectionU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllU();
}
void sectionu::SelectAllU(){
    ui->twSectionU->clearContents();

    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from sections";
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
        ui->twSectionU->setRowCount( query.size());
    }
    else{
        ui->twSectionU->setRowCount(0);
    }
    ui->teSectionU->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twSectionU->setItem(i,0,new QTableWidgetItem(query.value("name_section").toString()));
        ui->twSectionU->setItem(i,1,new QTableWidgetItem(query.value("place").toString()));
        ui->twSectionU->setItem(i,2,new QTableWidgetItem(query.value("len").toString()));

        i++;
    }
    return;
}

void sectionu::BackU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
