#include "studenta.h"
#include "ui_studenta.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

studenta::studenta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::studenta)
{
    ui->setupUi(this);
    connect(ui->btnAddStA, SIGNAL(clicked(bool)), this, SLOT(AddStA()));
    connect(ui->btnDelStA, SIGNAL(clicked(bool)), this, SLOT(DelStA()));
    connect(ui->btnSelectAllStA, SIGNAL(clicked(bool)), this, SLOT(SelectAllStA()));
    connect(ui->btnBackStA, SIGNAL(clicked(bool)), this, SLOT(BackStA()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

studenta::~studenta()
{
    delete ui;
}

void studenta::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teStudent->append("SQL drivers:");
        ui->teStudent->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teStudent->append("Connect is open...");
        else
            {
            ui->teStudent->append("Error of connect:");
            ui->teStudent->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teStudent->append("Connect is already open...");

    // Количество столбцов
    ui->twStudent->setColumnCount(4);
    // Возможность прокрутки
    ui->twStudent->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twStudent->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twStudent->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twStudent->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twStudent->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер телефона"));
    ui->twStudent->setHorizontalHeaderItem(2,new QTableWidgetItem("Дата рождения"));
    ui->twStudent->setHorizontalHeaderItem(3,new QTableWidgetItem("Информация"));

    // Последний столбец растягивается при изменении размера формы
    ui->twStudent->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twStudent->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twStudent->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void studenta::AddStA(){
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    QSqlQuery query(dbconn);
    // Создать строку запроса
    QString sqlstr = "insert into student(id_student,fio,phone_num,bthd_date,inf) values(?,?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    //query.bindValue(0,ui->leId->text());

    query.bindValue(1,ui->leFio->text());
    query.bindValue(2,ui->lePhone->text());
    query.bindValue(3,ui->leBd->text());
    query.bindValue(4,ui->leInfo->text());

    // Если тип поля отличается от строкового, то преобразовать его
    //query.bindValue(3,ui->leInn->text().toLongLong());
    // Выполнить запрос
    if( !query.exec() )
    {
        ui->teStudent->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teStudent->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    SelectAllStA();
    return;
}

void studenta::SelectAllStA(){
    ui->twStudent->clearContents();

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
        ui->twStudent->setRowCount( query.size());
    }
    else{
        ui->twStudent->setRowCount(0);
    }
    ui->teStudent->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twStudent->setItem(i,0,new QTableWidgetItem(query.value("fio").toString()));
        ui->twStudent->setItem(i,1,new QTableWidgetItem(query.value("phone_num").toString()));
        ui->twStudent->setItem(i,2,new QTableWidgetItem(query.value("bthd_date").toString()));
        ui->twStudent->setItem(i,3,new QTableWidgetItem(query.value("inf").toString()));

        i++;
    }
    return;
}

void studenta::DelStA(){
    if( !dbconn.isOpen() )
    {
        dbconnect();
        if( !dbconn.isOpen() )
        {
            QMessageBox::critical(this,"Error",dbconn.lastError().text());
            return;
        }
    }
    // Получить номер выбранной строки в компоненте таблицы
    int currow = ui->twStudent->currentRow();
    // Если он меньше 0 (строка не выбрана), то
    // сообщение об ошибке и выход из функции
    if( currow < 0 )
    {
        QMessageBox::critical(this,"Error","Not selected row!");
        return;
    }
    // Спросить у пользователя подтверждение удаления записи
    // Используется статический метод QMessageBox::question
    // для задания вопроса, который возвращает код нажатой кнопки
    if( QMessageBox::question(this,"Delete","Delete row?", QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)
        return;
    // Создать объект запроса
    QSqlQuery query(dbconn);
    // Создать строку запроса.
    // Вместо подготовки запроса и передачи параметров значение параметра
    // конкатенируется со строкой запроса
    // Обратите,что строковое значение помещается в одинарные кавычки
    // Значение выбирается из компонента таблицы методом item(row,col)
    QString sqlstr = "delete from student where fio = '" + ui->twStudent->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teStudent->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teStudent->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllStA();
    return;
}

void studenta::BackStA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}

void studenta::on_twStudent_cellClicked(int row, int column)
{
    ui->leFio->setText(ui->twStudent->item(row,0)->text());
    ui->lePhone->setText(ui->twStudent->item(row,1)->text());
    ui->leBd->setText(ui->twStudent->item(row,2)->text());
    ui->leInfo->setText(ui->twStudent->item(row,3)->text());

}


void studenta::on_bntEdit_clicked()
{
    // Подключение к БД

        if( !dbconn.isOpen() )

        {

        dbconnect();

        if( !dbconn.isOpen() )

        {

        QMessageBox::critical(this,"Error",dbconn.lastError().text());

        return;

        }

        }

        // Получить номер выбранной строки в компоненте таблицы

        int currow = ui->twStudent->currentRow();

        // Если он меньше 0 (строка не выбрана), то

        // сообщение об ошибке и выход из функции

        if( currow < 0 )

        {

        QMessageBox::critical(this,"Error","Not selected row!");

        return;

        }

        // Спросить у пользователя подтверждение удаления записи

        // Используется статический метод QMessageBox::question

        // для задания вопроса, который возвращает код нажатой кнопки

        if( QMessageBox::question(this,"Update","Update row?",

        QMessageBox::Cancel,QMessageBox::Ok)==QMessageBox::Cancel)

        return;

        // Создать объект запроса

        QString fio_up=ui->leFio->text();

        QString phone_up=ui->lePhone->text();

        QString email_up=ui->leBd->text();

        QString salary_up=ui->leInfo->text();


        QSqlQuery query(dbconn);

        // Создать строку запроса.

        // Вместо подготовки запроса и передачи параметров значение параметра

        // конкатенируется со строкой запроса

        // Обратите,что строковое значение помещается в одинарные кавычки

        // Значение выбирается из компонента таблицы методом item(row,col)

        QString sqlstr = "UPDATE student SET bthd_date='"+email_up+"',phone_num='"+phone_up+"',inf='"+salary_up+"' where fio='" + ui->twStudent->item(currow,0)->text() + "'";



        // Выполнить строку запроса и проверить его успешность

        if( !query.exec(sqlstr) )

        {

        ui->teStudent->append( query.lastQuery());

        QMessageBox::critical(this,"GEG",query.lastError().text());

        return;

        }

        // Вывести сообщение об удалении строки

        ui->teStudent->append( QString("Update %1 rows").arg(query.numRowsAffected()) );

        // Обновить содержимое компонента таблицы

        //leClear();

        SelectAllStA();

}

