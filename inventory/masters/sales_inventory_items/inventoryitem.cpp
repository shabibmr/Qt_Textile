#include "inventoryitem.h"
#include "ui_inventoryitem.h"
#include "login/loginvalues.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QSqlTableModel>
#include <QSqlRecord>
#include "updateplulisttextfileclass.h"


inventoryItem::inventoryItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventoryItem)
{
    ui->setupUi(this);

    dbHelper = new SalesInventoryItemDatabaseHelper();

    sqlModel = new QSqlQueryModel();
    sort = new GMItemSortFilterProxyModel();
    x="";

    settable();
    QScrollBar *verticalScroll = ui->tableWidget->verticalScrollBar();
    QScrollBar *horizontalScroll = ui->tableWidget->horizontalScrollBar();
    int valueV = verticalScroll->value();



    ui->tableWidget->setItemDelegate(new ReportCommonDelegate());


    ui->lineEdit->setFocus();
    setTabOrder(ui->lineEdit,ui->createNewPushButton);

}

inventoryItem::~inventoryItem()
{
    delete ui;
}

void inventoryItem::searchTextChanged(QString text)
{
    x=text;
    settable();
}

void inventoryItem::setFocusonSearchLine()
{
    ui->lineEdit->setFocus();
}

void inventoryItem::setLineFocus()
{
    ui->lineEdit->setFocus();
}

void inventoryItem::on_createNewPushButton_clicked()
{

        additemWidget = new AddItems(this);
        additemWidget->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
        additemWidget->setAttribute(Qt::WA_DeleteOnClose);
        additemWidget->show();
        QObject::connect(additemWidget,SIGNAL(closing()),this,SLOT(settable()));

    //    itemEditor = new InventoryItemEditor(this);
    //    itemEditor->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
    //    itemEditor->setAttribute(Qt::WA_DeleteOnClose);
    //    itemEditor->show();
    //    QObject::connect(itemEditor,SIGNAL(closing()),this,SLOT(settable()));

}

void inventoryItem::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void inventoryItem::deleteitemAdd(){
    //    delete additemWidget;
}


void inventoryItem::settable()
{
    //    itemsList = dbHelper->getALLInventoryObjectsAsPtr();

    //ui->tableWidget->setModel(dbHelper->getInventoryItemsQueryModel());

    sqlModel = dbHelper->getInventoryItemsQueryModel();
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(sqlModel);
    sort->sort(1);

    ui->tableWidget->setModel(sort);
    ui->tableWidget->setSortingEnabled(true);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //    ui->tableWidget->setItemDelegate(new LedgerReportDelegate());
    ui->tableWidget->hideColumn(0);
    ui->tableWidget->hideColumn(4);
    ui->lineEdit->setFocus();


}

void inventoryItem::delButtonClicked()
{
    int x= sender()->property("row").toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete","Are you sure you want to Delete "+itemsList[x]->ItemName,
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes)
        dbHelper->deleteSalesInventoryItem(itemsList[x]->ItemID);
    settable();
}

void inventoryItem::on_lineEdit_textChanged(const QString &arg1)
{
    QRegExp regExp(ui->lineEdit->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);

}



void inventoryItem::on_tableWidget_clicked(const QModelIndex &index)
{

}

void inventoryItem::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    int col = index.column();


    if(getBackFlag == 0)
    {

        additemWidget = new AddItems(this);
        additemWidget->setWindowFlags(Qt::Window);
        additemWidget->setAttribute(Qt::WA_DeleteOnClose);
        additemWidget->setItem(dbHelper->getInventoryItemByIDasPtr(sqlModel->record(row).value(0).toString()));
        additemWidget->edit();
        QObject::connect(additemWidget,SIGNAL(closing()),this,SLOT(settable()));
        additemWidget->show();

        //        itemEditor = new InventoryItemEditor(this);
        //        itemEditor->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
        //        itemEditor->setAttribute(Qt::WA_DeleteOnClose);
        //        itemEditor->editItem(dbHelper->getInventoryItemByIDasPtr(sqlModel->record(row).value(0).toString()));
        //        itemEditor->show();
        //        QObject::connect(itemEditor,SIGNAL(closing()),this,SLOT(settable()));
    }

    else if(getBackFlag == 1 ){

        CompoundItemDataObject obj;
        obj.BaseItem = dbHelper->getInventoryItemByID(sqlModel->record(row).value(0).toString());
        obj.BaseItem.quantity =1;
        emit SelectedItem(obj);
        this->close();
    }
}

void inventoryItem::on_uploadPLU_clicked()
{
    UpdatePLUListTextFileClass *updateObject = new UpdatePLUListTextFileClass;
    //    updateObject->UpdateFile("d:\\jhma\\PLU.txt");
    updateObject->UpdateFile("PLU.txt");
    delete updateObject;
}

void inventoryItem::on_printPLU_clicked()
{
    UpdatePLUListTextFileClass *updateObject = new UpdatePLUListTextFileClass;
    //    updateObject->UpdateFile("d:\\jhma\\PLU.txt");
    updateObject->printPLU();
    delete updateObject;
}
