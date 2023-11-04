#include "visitinga.h"
#include "ui_visitinga.h"
#include "adminpanel.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>

visitinga::visitinga(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::visitinga)
{
    ui->setupUi(this);
    connect(ui->btnAddVisitA, SIGNAL(clicked(bool)), this, SLOT(AddViA()));
    connect(ui->btnDelVisitA, SIGNAL(clicked(bool)), this, SLOT(DelViA()));
    connect(ui->btnSelectAllVisitA, SIGNAL(clicked(bool)), this, SLOT(SelectAllViA()));
    connect(ui->btnBackVisitA, SIGNAL(clicked(bool)), this, SLOT(BackViA()));
    connect(ui->bntConnect, SIGNAL(clicked(bool)), this, SLOT(dbconnect()));

}

visitinga::~visitinga()
{
    delete ui;
}

void visitinga::dbconnect()
{
    if(!dbconn.isOpen())
    {
        // Если соединение не открыто, то вывести список доступных драйверов БД
        // (вывод в поле teResult, метод append добавляет строки).
        ui->teVisitA->append("SQL drivers:");
        ui->teVisitA->append(QSqlDatabase::drivers().join(","));
        // Создать глобальную переменную для установки соединения с БД
        dbconn=QSqlDatabase::addDatabase("QPSQL");
        // Установить параметры соединения: имя БД, адрес хоста, логин и пароль пользователя, порт (если отличается от стандартного)
        dbconn.setDatabaseName("AlS");
        dbconn.setHostName("localhost");
        dbconn.setUserName("postgres");
        dbconn.setPassword("123456789");
        // Открыть соединениe и результат вывести в окно вывода
        if( dbconn.open() )
            ui->teVisitA->append("Connect is open...");
        else
            {
            ui->teVisitA->append("Error of connect:");
            ui->teVisitA->append(dbconn.lastError().text());
        }
    }
    else
        // Если соединение уже открыто, то сообщить об этом
        ui->teVisitA->append("Connect is already open...");

    // Количество столбцов
    ui->twVisitA->setColumnCount(3);
    // Возможность прокрутки
    ui->twVisitA->setAutoScroll(true);
    // Режим выделения ячеек - только одна строка
    ui->twVisitA->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twVisitA->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Заголовки таблицы
    ui->twVisitA->setHorizontalHeaderItem(0,new QTableWidgetItem("Секция"));
    ui->twVisitA->setHorizontalHeaderItem(1,new QTableWidgetItem("Тренер"));
    ui->twVisitA->setHorizontalHeaderItem(2,new QTableWidgetItem("Ученик"));

    // Последний столбец растягивается при изменении размера формы
    ui->twVisitA->horizontalHeader()->setStretchLastSection(true);
    // Разрешаем сортировку пользователю
    ui->twVisitA->setSortingEnabled(true);
    // Запрет на изменение ячеек таблицы при отображении
    ui->twVisitA->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
void visitinga::AddViA(){
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
    QString sqlstr = "insert into visiting(id_visiting, sections, trainer, srudent) values(?,?,?,?)";
    // Подготовить запрос
    query.prepare(sqlstr);
    // Передать параметры из полей ввода в запрос
    //query.bindValue(0,ui->leId->text());

    query.bindValue(1,ui->leSection->text());
    query.bindValue(2,ui->leTrainer->text());
    query.bindValue(3,ui->leStudent->text());

    // Если тип поля отличается от строкового, то преобразовать его
    //query.bindValue(3,ui->leInn->text().toLongLong());
    // Выполнить запрос
    if( !query.exec() )
    {
        ui->teVisitA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Если запрос выполнен, то вывести сообщение одобавлении строки
    ui->teVisitA->append( QString("AddRead %1rows").arg(query.numRowsAffected()) );
    // и обновить записи в компоненте таблицы
    SelectAllViA();
    return;
}

void visitinga::SelectAllViA(){
    ui->twVisitA->clearContents();

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
        ui->twVisitA->setRowCount( query.size());
    }
    else{
        ui->twVisitA->setRowCount(0);
    }
    ui->teVisitA->append( QString("Read %1 rows").arg(query.size()));
    // Прочитать в цикле все строки результата (курсора)
    // и вывести их в компонент таблицы
    int i=0;
    while(query.next())
    {
        ui->twVisitA->setItem(i,0,new QTableWidgetItem(query.value("sections").toString()));
        ui->twVisitA->setItem(i,1,new QTableWidgetItem(query.value("trainer").toString()));
        ui->twVisitA->setItem(i,2,new QTableWidgetItem(query.value("srudent").toString()));

        i++;
    }
    return;
}

void visitinga::DelViA(){
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
    int currow = ui->twVisitA->currentRow();
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
    QString sqlstr = "delete from visiting where sections = '" + ui->twVisitA->item(currow,0)->text() + "'";
    // Выполнить строку запроса и проверить его успешность
    if( !query.exec(sqlstr) )
    {
        ui->teVisitA->append( query.lastQuery());
        QMessageBox::critical(this,"Error",query.lastError().text());
        return;
    }
    // Вывести сообщение об удалении строки
    ui->teVisitA->append( QString("Del %1 rows").arg(query.numRowsAffected()) );
    // Обновить содержимое компонента таблицы
    SelectAllViA();
    return;
}

void visitinga::BackViA(){
    this->close();
    adminpanel *admin_panel = new adminpanel();
    admin_panel->show();
    return;
}

void visitinga::on_bntEdit_clicked()
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

        int currow = ui->twVisitA->currentRow();

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

        QString phone_up=ui->leSection->text();

        QString email_up=ui->leTrainer->text();

        QString salary_up=ui->leStudent->text();


        QSqlQuery query(dbconn);

        // Создать строку запроса.

        // Вместо подготовки запроса и передачи параметров значение параметра

        // конкатенируется со строкой запроса

        // Обратите,что строковое значение помещается в одинарные кавычки

        // Значение выбирается из компонента таблицы методом item(row,col)

        QString sqlstr = "UPDATE visiting SET trainer='"+email_up+"',srudent='"+salary_up+"' where sections='" + ui->twVisitA->item(currow,0)->text() + "'";



        // Выполнить строку запроса и проверить его успешность

        if( !query.exec(sqlstr) )

        {

        ui->teVisitA->append( query.lastQuery());

        QMessageBox::critical(this,"GEG",query.lastError().text());

        return;

        }

        // Вывести сообщение об удалении строки

        ui->teVisitA->append( QString("Update %1 rows").arg(query.numRowsAffected()) );

        // Обновить содержимое компонента таблицы

        //leClear();

        SelectAllViA();

}


void visitinga::on_twVisitA_cellClicked(int row, int column)
{
    ui->leSection->setText(ui->twVisitA->item(row,0)->text());
    ui->leTrainer->setText(ui->twVisitA->item(row,1)->text());
    ui->leStudent->setText(ui->twVisitA->item(row,2)->text());

}

