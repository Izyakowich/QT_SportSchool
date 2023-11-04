#include "trainer.h"
#include "ui_trainer.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

trainer::trainer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::trainer)
{
    ui->setupUi(this);
    connect(ui->btnAddTrA, SIGNAL(clicked(bool)), this, SLOT(AddTrA()));
    connect(ui->btnDelTrA, SIGNAL(clicked(bool)), this, SLOT(DelTrA()));
    connect(ui->btnSelectAllTrA, SIGNAL(clicked(bool)), this, SLOT(SelectAllTrA()));
    connect(ui->btnBackTrA, SIGNAL(clicked(bool)), this, SLOT(BackTrA()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

trainer::~trainer()
{
    delete ui;

}

void trainer::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teTrainerA->append("SQL drivers:");
        ui->teTrainerA->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teTrainerA->append("Connect is open...");
        else
            {
            ui->teTrainerA->append("Error of connect:");
            ui->teTrainerA->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teTrainerA->append("Connect is already open...");

    // Количество столбцов
    ui->twTrainerA->setColumnCount(3);
    // Возможность прокрутки
    ui->twTrainerA->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twTrainerA->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twTrainerA->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twTrainerA->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->twTrainerA->setHorizontalHeaderItem(1,new QTableWidgetItem("Номер телефона"));
    ui->twTrainerA->setHorizontalHeaderItem(2,new QTableWidgetItem("Информация"));

    // Последний столбец растягивается при изменении размера формы
    ui->twTrainerA->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twTrainerA->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twTrainerA->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void trainer::AddTrA(){
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
    QString sqlstr = "insert into trainer(id_trainer,fio,phone_num,inf) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    //query.bindValue(0,ui->leId->text());

    query.bindValue(1,ui->leFio->text());
    query.bindValue(2,ui->lePhone->text());
    query.bindValue(3,ui->leInfo->text());

    // Если тип поля отличается от строкового, то преобразовать его
    //query.bindValue(3,ui->leInn->text().toLongLong());
    // Выполнить запрос
    if( !query.exec() )
    {
        ui->teTrainerA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teTrainerA->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    SelectAllTrA();
    return;
}

void trainer::SelectAllTrA(){
    ui->twTrainerA->clearContents();

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
        ui->twTrainerA->setRowCount( query.size());
    }
    else{
        ui->twTrainerA->setRowCount(0);
    }
    ui->teTrainerA->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twTrainerA->setItem(i,0,new QTableWidgetItem(query.value("fio").toString()));
        ui->twTrainerA->setItem(i,1,new QTableWidgetItem(query.value("phone_num").toString()));
        ui->twTrainerA->setItem(i,2,new QTableWidgetItem(query.value("inf").toString()));

        i++;
    }
    return;
}

void trainer::DelTrA(){
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
    int currow = ui->twTrainerA->currentRow();
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
    QString sqlstr = "delete from trainer where fio = '" + ui->twTrainerA->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teTrainerA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teTrainerA->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllTrA();
    return;
}

void trainer::BackTrA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}

void trainer::on_twTrainerA_cellClicked(int row, int column)
{
    ui->leFio->setText(ui->twTrainerA->item(row,0)->text());
    ui->lePhone->setText(ui->twTrainerA->item(row,1)->text());
    ui->leInfo->setText(ui->twTrainerA->item(row,2)->text());
}


void trainer::on_bntEdit_clicked()
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

        int currow = ui->twTrainerA->currentRow();

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

        //QString fio_up=ui->leId->text();

        QString phone_up=ui->leFio->text();

        QString email_up=ui->lePhone->text();

        QString salary_up=ui->leInfo->text();


        QSqlQuery query(dbconn);

        // Создать строку запроса.

        // Вместо подготовки запроса и передачи параметров значение параметра

        // конкатенируется со строкой запроса

        // Обратите,что строковое значение помещается в одинарные кавычки

        // Значение выбирается из компонента таблицы методом item(row,col)

        QString sqlstr = "UPDATE trainer SET phone_num='"+email_up+"',inf='"+salary_up+"' where fio='" + ui->twTrainerA->item(currow,0)->text() + "'";



        // Выполнить строку запроса и проверить его успешность

        if( !query.exec(sqlstr) )

        {

        ui->teTrainerA->append( query.lastQuery());

        QMessageBox::critical(this,"GEG",query.lastError().text());

        return;

        }

        // Вывести сообщение об удалении строки

        ui->teTrainerA->append( QString("Update %1 rows").arg(query.numRowsAffected()) );

        // Обновить содержимое компонента таблицы

        //leClear();

        SelectAllTrA();
}

