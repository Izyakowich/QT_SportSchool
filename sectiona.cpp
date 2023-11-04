#include "sectiona.h"
#include "ui_sectiona.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

sectiona::sectiona(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sectiona)
{
    ui->setupUi(this);
    connect(ui->btnAddA, SIGNAL(clicked(bool)), this, SLOT(AddA()));
    connect(ui->btnDelA, SIGNAL(clicked(bool)), this, SLOT(DelA()));
    connect(ui->btnSelectAllA, SIGNAL(clicked(bool)), this, SLOT(SelectAllA()));
    connect(ui->btnBackA, SIGNAL(clicked(bool)), this, SLOT(BackA()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

sectiona::~sectiona()
{
    delete ui;
}

void sectiona::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teSectionA->append("SQL drivers:");
        ui->teSectionA->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teSectionA->append("Connect is open...");
        else
            {
            ui->teSectionA->append("Error of connect:");
            ui->teSectionA->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teSectionA->append("Connect is already open...");

    // Количество столбцов
    ui->twSectionA->setColumnCount(3);
    // Возможность прокрутки
    ui->twSectionA->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twSectionA->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twSectionA->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twSectionA->setHorizontalHeaderItem(0,new QTableWidgetItem("Название"));
    ui->twSectionA->setHorizontalHeaderItem(1,new QTableWidgetItem("Место"));
    ui->twSectionA->setHorizontalHeaderItem(2,new QTableWidgetItem("Длительность"));

    // Последний столбец растягивается при изменении размера формы
    ui->twSectionA->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twSectionA->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twSectionA->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void sectiona::AddA(){
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
        QString sqlstr = "insert into sections(id_section,name_section,place, len) values(?,?,?,?)";
        // Подготовить запрос
        query.prepare(sqlstr);
        // Передать параметры из полей ввода в запрос
       // query.bindValue(0,ui->leId->text());

        query.bindValue(1,ui->leName->text());
        query.bindValue(2,ui->lePlace->text());
        query.bindValue(3,ui->leLen->text());
        // Если тип поля отличается от строкового, то преобразовать его
        //query.bindValue(3,ui->leInn->text().toLongLong());
        // Выполнить запрос
        if( !query.exec() )
        {
            ui->teSectionA->append( query.lastQuery());
            QMessageBox::critical(this,"Error",query.lastError().text());
            return;
        }
        // Если запрос выполнен, то вывести сообщение одобавлении строки
        ui->teSectionA->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
        // и обновить записи в компоненте таблицы
        SelectAllA();
    return;
}

void sectiona::SelectAllA(){
    ui->twSectionA->clearContents();

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
        ui->twSectionA->setRowCount( query.size());
    }
    else{
        ui->twSectionA->setRowCount(0);
    }
    ui->teSectionA->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twSectionA->setItem(i,0,new QTableWidgetItem(query.value("name_section").toString()));
        ui->twSectionA->setItem(i,1,new QTableWidgetItem(query.value("place").toString()));
        ui->twSectionA->setItem(i,2,new QTableWidgetItem(query.value("len").toString()));

        i++;
    }
    return;
}

void sectiona::DelA(){
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
    int currow = ui->twSectionA->currentRow();
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
    QString sqlstr = "delete from sections where name_section = '" + ui->twSectionA->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teSectionA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teSectionA->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllA();
    return;
}

void sectiona::BackA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}

void sectiona::on_twSectionA_cellClicked(int row, int column)
{
    ui->leName->setText(ui->twSectionA->item(row,0)->text());
    ui->lePlace->setText(ui->twSectionA->item(row,1)->text());
    ui->leLen->setText(ui->twSectionA->item(row,2)->text());
}


void sectiona::on_bntEdit_clicked()
{
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

    int currow = ui->twSectionA->currentRow();

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

    //QString fio_up=ui->leFio->text();

    //QString phone_up=ui->lePhone->text();

    QString email_up=ui->lePlace->text();

    QString salary_up=ui->leLen->text();


    QSqlQuery query(dbconn);

    // Создать строку запроса.

    // Вместо подготовки запроса и передачи параметров значение параметра

    // конкатенируется со строкой запроса

    // Обратите,что строковое значение помещается в одинарные кавычки

    // Значение выбирается из компонента таблицы методом item(row,col)

    //QString sqlstr = "UPDATE sections SET trainer='"+email_up+"',srudent='"+salary_up+"' where sections='" + ui->twSectionA->item(currow,0)->text() + "'";
    QString sqlstr = "UPDATE sections SET place='"+email_up+"',len='"+salary_up+"' where name_section='" + ui->twSectionA->item(currow,0)->text() + "'";



    // Выполнить строку запроса и проверить его успешность

    if( !query.exec(sqlstr) )

    {

    ui->teSectionA->append( query.lastQuery());

    QMessageBox::critical(this,"GEG",query.lastError().text());

    return;

    }

    // Вывести сообщение об удалении строки

    ui->teSectionA->append( QString("Update %1 rows").arg(query.numRowsAffected()) );

    // Обновить содержимое компонента таблицы

    //leClear();

    SelectAllA();
}

