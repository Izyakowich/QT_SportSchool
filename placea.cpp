#include "placea.h"
#include "ui_placea.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

placea::placea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::placea)
{
    ui->setupUi(this);
    connect(ui->btnAddPlA, SIGNAL(clicked(bool)), this, SLOT(AddPlA()));
    connect(ui->btnDelPlA, SIGNAL(clicked(bool)), this, SLOT(DelPlA()));
    connect(ui->btnSelectAllPlA, SIGNAL(clicked(bool)), this, SLOT(SelectAllPlA()));
    connect(ui->btnBackPlA, SIGNAL(clicked(bool)), this, SLOT(BackPlA()));
    connect(ui->btnConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

placea::~placea()
{
    delete ui;
}

void placea::AddPlA(){
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
        QString sqlstr = "insert into place(id_place,cabinet,address) values(?,?,?)";
        // Подготовить запрос
        query.prepare(sqlstr);
        // Передать параметры из полей ввода в запрос
        query.bindValue(0,ui->leId->text());

        query.bindValue(1,ui->lePlaceA->text());
        query.bindValue(2,ui->leAdresPlA->text());

        // Если тип поля отличается от строкового, то преобразовать его
        //query.bindValue(3,ui->leInn->text().toLongLong());
        // Выполнить запрос
        if( !query.exec() )
        {
            ui->tePlaceA->append( query.lastQuery());
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        // Если запрос выполнен, то вывести сообщение одобавлении строки
        ui->tePlaceA->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
        // и обновить записи в компоненте таблицы
        SelectAllPlA();
    return;
}

void placea::DelPlA(){
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
    int currow = ui->twPlaceA->currentRow();
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
    QString sqlstr = "delete from place where cabinet = '" + ui->twPlaceA->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->tePlaceA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->tePlaceA->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllPlA();
    return;
}

void placea::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->tePlaceA->append("SQL drivers:");
        ui->tePlaceA->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->tePlaceA->append("Connect is open...");
        else
            {
            ui->tePlaceA->append("Error of connect:");
            ui->tePlaceA->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->tePlaceA->append("Connect is already open...");

    // Количество столбцов
    ui->twPlaceA->setColumnCount(2);
    // Возможность прокрутки
    ui->twPlaceA->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twPlaceA->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twPlaceA->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twPlaceA->setHorizontalHeaderItem(0,new QTableWidgetItem("Кабинет"));
    ui->twPlaceA->setHorizontalHeaderItem(1,new QTableWidgetItem("Адрес"));

    // Последний столбец растягивается при изменении размера формы
    ui->twPlaceA->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twPlaceA->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twPlaceA->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void placea::SelectAllPlA(){
    ui->twPlaceA->clearContents();

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
        ui->twPlaceA->setRowCount( query.size());
    }
    else{
        ui->twPlaceA->setRowCount(0);
    }
    ui->tePlaceA->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twPlaceA->setItem(i,0,new QTableWidgetItem(query.value("cabinet").toString()));
        ui->twPlaceA->setItem(i,1,new QTableWidgetItem(query.value("address").toString()));

        i++;
    }
    return;
}

void placea::BackPlA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}
