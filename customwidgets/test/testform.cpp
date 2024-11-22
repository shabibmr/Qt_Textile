#include "testform.h"
#include "ui_testform.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include "customwidgets/mydelegate.h"

#include "inventory/masters/sales_inventory_items/additems.h"
#include <QCompleter>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "customwidgets/Basic/gmhovertoolbutton.h"

TestForm::TestForm(QWidget *parent) : QWidget(parent),ui(new Ui::TestForm)
{
    ui->setupUi(this);
    sqlModel = new QSqlQueryModel();
    QSqlTableModel *sqlTable = new QSqlTableModel(this,QSqlDatabase::database(DatabaseValues::connectionString));


    QString qStr;
    sqlTable->setTable(SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name);
    sqlTable->select();

    //    DatabaseHelper *db = new DatabaseHelper;
    //    if(!db->db.isOpen()){
    //        qDebug()<<"DB NOT open";
    //    }

    sqlModel->setQuery("SELECT Item_ID,Item_Name,Price FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name ,QSqlDatabase::database(DatabaseValues::connectionString));
    dbhelper = new SalesInventoryItemDatabaseHelper();

    sqlModel = dbhelper->getInventoryItemsQueryModel();
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(sqlModel);


    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->setItemDelegate(new LedgerReportDelegate());
    ui->tableView->hideColumn(0);

    ui->listView->setModel(sqlModel);
    ui->listView->setItemDelegate(new Mydelegate());

    QCompleter *comp = new QCompleter(this);

    comp->setModel(sqlModel);
    qDebug()<<"Count = "<<comp->completionCount();
    comp->popup()->setItemDelegate(new Mydelegate());

    ui->testline->setCompleter(comp);

    //    ui->comboBox->setModel(sqlModel);
    qDebug()<<"Completiong col = "<<comp->completionColumn();
    comp->setCompletionColumn(1);
//    comp->popup()->setItemDelegate(new Mydelegate());
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->comboBox->setModel(sqlModel);
    ui->comboBox->setItemDelegate(new Mydelegate());
//    ui->comboBox->setCompleter(comp);
    ui->comboBox->setEditable(true);

    GMItemSearchLite* search = new GMItemSearchLite(sqlModel);
    QToolButton* butt = new QToolButton;
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    butt->setContentsMargins(0,0,0,0);
    ui->verticalLayout_2->setContentsMargins(0,0,0,0);
    lay->addWidget(search);
    butt->setLayout(lay);
    butt->setMinimumWidth(250);
    ui->verticalLayout_2->addWidget(butt);

    GMHoverToolButton *B1 = new GMHoverToolButton ();
    B1->setText("Hatim");
    ui->verticalLayout_2->addWidget(B1);
}

TestForm::~TestForm()
{
    delete ui;
}

void TestForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    int col = index.column();

    qDebug()<<sqlModel->record(row).value(0).toString();
    AddItems* addItem = new AddItems(this);
    addItem->setWindowFlags(Qt::Window);
    addItem->setItem(dbhelper->getInventoryItemByIDasPtr(sqlModel->record(row).value(0).toString()));
    addItem->edit();
    addItem->show();
}

void TestForm::on_lineEdit_textChanged(const QString &arg1)
{
    //    sort->setFilterKeyColumn();
    QRegExp regExp(ui->lineEdit->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);
}
