#include "indtra.h"
#include "ui_indtra.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

//extern int rowIndex = 1;

indtra::indtra(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::indtra)
{
    ui->setupUi(this);
    connect(ui->btnAddIndA, SIGNAL(clicked(bool)), this, SLOT(AddIndA()));
    connect(ui->btnDelIndA, SIGNAL(clicked(bool)), this, SLOT(DelIndA()));
    connect(ui->btnSelectAllIndA, SIGNAL(clicked(bool)), this, SLOT(SelectAllIndA()));
    connect(ui->btnBackIndA, SIGNAL(clicked(bool)), this, SLOT(BackIndA()));
    connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

indtra::~indtra()
{
    delete ui;
}


int indtra::countRows() {
    // Создать объект запроса с привязкой к установленному соединению
    QSqlQuery query(dbconn);
    // Создать строку запроса на выборку данных
    QString sqlstr = "select * from ind_train";
    // Выполнить запрос и поверить его успешность
    if( !query.exec(sqlstr) )
    {
        QMessageBox::critical(this,"Error", query.lastError().text());
        return 0;
    }

    int count = 0;
    while (query.next()) {
        count++;
    }
    return count;
}

void indtra::AddIndA(){
    //rowIndex = countRows() + 1;
    // Подключиться к БД
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
        QString sqlstr = "insert into ind_train(id_train, trainer,student,days,time,name_train) values(?,?,?,?,?,?)";
        // Подготовить запрос
        query.prepare(sqlstr);
        // Передать параметры из полей ввода в запрос
        query.bindValue(0,ui->leNew->text());

        query.bindValue(1,ui->leTrainer->text());
        query.bindValue(2,ui->leStudent->text());
        query.bindValue(3,ui->leDay->text());
        query.bindValue(4, ui->leTime->text());
        query.bindValue(5, ui->leName->text());
        // Если тип поля отличается от строкового, то преобразовать его
        //query.bindValue(3,ui->leInn->text().toLongLong());
        // Выполнить запрос
        if( !query.exec() )
        {
            ui->teIndA->append( query.lastQuery());
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        // Если запрос выполнен, то вывести сообщение одобавлении строки
        ui->teIndA->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
        // и обновить записи в компоненте таблицы
        SelectAllIndA();
    return;
}

void indtra::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teIndA->append("SQL drivers:");
        ui->teIndA->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teIndA->append("Connect is open...");
        else
            {
            ui->teIndA->append("Error of connect:");
            ui->teIndA->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teIndA->append("Connect is already open...");

    // Количество столбцов
    ui->twIndA->setColumnCount(6);
    // Возможность прокрутки
    ui->twIndA->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twIndA->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twIndA->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twIndA->setHorizontalHeaderItem(0,new QTableWidgetItem("Номер записи"));

    ui->twIndA->setHorizontalHeaderItem(1,new QTableWidgetItem("Название"));
    ui->twIndA->setHorizontalHeaderItem(2,new QTableWidgetItem("Тренер"));
    ui->twIndA->setHorizontalHeaderItem(3,new QTableWidgetItem("ученик"));
    ui->twIndA->setHorizontalHeaderItem(4,new QTableWidgetItem("День"));
    ui->twIndA->setHorizontalHeaderItem(5,new QTableWidgetItem("Время"));
    // Последний столбец растягивается при изменении размера формы
    ui->twIndA->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twIndA->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twIndA->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void indtra::SelectAllIndA(){

//        // Очистить содержимое компонента
        ui->twIndA->clearContents();
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
            ui->twIndA->setRowCount( query.size());
        }
        else{
            ui->twIndA->setRowCount(0);
        }
        ui->teIndA->append( QString("Read %1 rows").arg(query.size()));
        // Прочитать в цикле все строки результата (курсора)
        // и вывести их в компонент таблицы
        int i=0;
        while(query.next())
        {
            ui->twIndA->setItem(i,0,new QTableWidgetItem(query.value("id_train").toString()));

            ui->twIndA->setItem(i,1,new QTableWidgetItem(query.value("name_train").toString()));
            ui->twIndA->setItem(i,2,new QTableWidgetItem(query.value("trainer").toString()));
            ui->twIndA->setItem(i,3,new QTableWidgetItem(query.value("student").toString()));
            ui->twIndA->setItem(i,4,new QTableWidgetItem(query.value("days").toString()));
            ui->twIndA->setItem(i,5,new QTableWidgetItem(query.value("time").toString()));

            i++;
        }
}

void indtra::DelIndA(){
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
    int currow = ui->twIndA->currentRow();
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
    QString sqlstr = "delete from ind_train where id_train = '" + ui->twIndA->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teIndA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teIndA->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllIndA();
    return;
}

void indtra::BackIndA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}

