#include "visitingu.h"
#include "ui_visitingu.h"
#include "userpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

visitingu::visitingu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::visitingu)
{
    ui->setupUi(this);
   // connect(ui->btnSelectAllVisitU, SIGNAL(clicked(bool)), this, SLOT(SelectAllViU()));
    connect(ui->btnBackVisitU, SIGNAL(clicked(bool)), this, SLOT(BackViU()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

visitingu::~visitingu()
{
    delete ui;
}

void visitingu::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teVisitU->append("SQL drivers:");
        ui->teVisitU->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teVisitU->append("Connect is open...");
        else
            {
            ui->teVisitU->append("Error of connect:");
            ui->teVisitU->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teVisitU->append("Connect is already open...");

    // Количество столбцов
    ui->twVisitU->setColumnCount(3);
    // Возможность прокрутки
    ui->twVisitU->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twVisitU->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twVisitU->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twVisitU->setHorizontalHeaderItem(0,new QTableWidgetItem("Секция"));
    ui->twVisitU->setHorizontalHeaderItem(1,new QTableWidgetItem("Тренер"));
    ui->twVisitU->setHorizontalHeaderItem(2,new QTableWidgetItem("Ученик"));

    // Последний столбец растягивается при изменении размера формы
    ui->twVisitU->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twVisitU->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twVisitU->setEditTriggers(QAbstractItemView::NoEditTriggers);
    SelectAllViU();
}


void visitingu::SelectAllViU(){
    ui->twVisitU->clearContents();

    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from visiting";
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
        ui->twVisitU->setRowCount( query.size());
    }
    else{
        ui->twVisitU->setRowCount(0);
    }
    ui->teVisitU->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twVisitU->setItem(i,0,new QTableWidgetItem(query.value("sections").toString()));
        ui->twVisitU->setItem(i,1,new QTableWidgetItem(query.value("trainer").toString()));
        ui->twVisitU->setItem(i,2,new QTableWidgetItem(query.value("srudent").toString()));

        i++;
    }
    return;
}

void visitingu::BackViU(){
    this->close();
    userpanel *user_panel = new userpanel();
    user_panel->show();
    return;
}
//void visitingu::vis(){
//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery("SELECT * FROM visiting WHERE column = :value");
//    model->bindValue(":value", ui->leStudent->text());
//    ui->tableView->setModel(model);
//    SelectAllViU();
//}
