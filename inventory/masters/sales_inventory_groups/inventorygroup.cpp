#include "inventorygroup.h"
#include "ui_inventorygroup.h"
#include "login/loginvalues.h"
#include <QMessageBox>

inventorygroup::inventorygroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventorygroup)
{
    ui->setupUi(this);
    qDebug("Setting Window title");
    this->setWindowTitle("Inventory Groups");
    qDebug()<<"inventorygroup constructor";
    dbhelper = new SalesInventoryGroupsDatabaseHelper();
//    ui->tableWidget->setColumnCount(4);
//    ui->tableWidget->hideColumn(2);
//    settable();
//    if(!(LoginValues::getUserName().contains("naseem",Qt::CaseInsensitive) ||
//           LoginValues::getUserName().contains("shabib",Qt::CaseInsensitive)) ){
//        ui->tableWidget->hideColumn(3);
//    }
    setTableView();
}

inventorygroup::~inventorygroup()
{
    delete ui;
}

void inventorygroup::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void inventorygroup::delButtonClicked(bool v)
{
    int x= sender()->property("row").toInt();
    dbhelper->deleteSalesInventoryGroup(groupList[x].GroupID);
    setTableView();
}

void inventorygroup::on_createNewPushButton_clicked()
{
    addGroupWidget = new AddItemGroups(this);
    addGroupWidget->setWindowFlags(Qt::Window);
    addGroupWidget->show();

    QObject::connect(addGroupWidget,&AddItemGroups::closing,
                     this,&inventorygroup::setTableView);
}

//void inventorygroup::settable()
//{
//    ui->tableWidget->setRowCount(0);
//    groupList = dbhelper->getALLgroupObjects();


//    QTableWidgetItem* GroupName = new QTableWidgetItem;
//    GroupName->setText("Group Name");
//    ui->tableWidget->setHorizontalHeaderItem(0,GroupName);

//    QTableWidgetItem* parentName = new QTableWidgetItem;
//    parentName->setText("Parent");
//    ui->tableWidget->setHorizontalHeaderItem(1,parentName);

//    QTableWidgetItem* deleteItem = new QTableWidgetItem;
//    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
//    ui->tableWidget->setHorizontalHeaderItem(3,deleteItem);

//    ui->tableWidget->setColumnWidth(3,25 );


//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

//    //qDebug()<<groupObjs.size();
//    int r=0;
//    InventoryItemGroupDataModel obj;

//    for(int i=0;i<groupList.size();i++){
//        obj=groupList[i];
//        if(obj.GroupName.contains(x,Qt::CaseInsensitive)){
//            ui->tableWidget->insertRow(r);
//            QTableWidgetItem* GroupName = new QTableWidgetItem;
//            GroupName->setText(obj.GroupName);
//            ui->tableWidget->setItem(r,0,GroupName);

//            QTableWidgetItem* parentName = new QTableWidgetItem;
//            parentName->setText(obj.ParentGroupName);
//            //        qDebug()<<"parent Name = "<<obj.parentGroupName;
//            ui->tableWidget->setItem(r,1,parentName);
//            QTableWidgetItem* hide = new QTableWidgetItem;
//            hide->setText(QString::number(i));
//            ui->tableWidget->setItem(r,2,hide);

//            QToolButton* delButton = new QToolButton;
//            delButton->setIcon(QIcon(":/icons/trash.ico"));
//            delButton->setProperty("row",i);
//            delButton->setAutoRaise(true);
//            ui->tableWidget->setCellWidget(r,3,delButton);
//            QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));


//            r++;
//        }
    //    }
//}

void inventorygroup::setTableView()
{
    invGroupModel = dbhelper->getAllInventoryGroupsModel();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->setModel(invGroupModel);
    ui->tableView->hideColumn(0);

}



void inventorygroup::searchTextChanged(QString text)
{
    x=text;
    setTableView();
}

//void inventorygroup::on_tableWidget_doubleClicked(const QModelIndex &index)
//{
//    int r = index.row();
//    int col = ui->tableWidget->item(r,2)->text().toInt();
//    addGroupWidget = new AddItemGroups(this);
//    addGroupWidget->setWindowFlags(Qt::Window);
//    addGroupWidget->setTitle("Update Inventory Group");
//    addGroupWidget->edit(groupList[col]);
//    addGroupWidget->show();
//    QObject::connect(addGroupWidget,SIGNAL(closing()),this,SLOT(settable()));
//}

void inventorygroup::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    QString groupID = invGroupModel->record(r).value(0).toString();

    addGroupWidget = new AddItemGroups(this);
    addGroupWidget->setWindowFlags(Qt::Window);
    addGroupWidget->setTitle("Update Group");
    addGroupWidget->edit(dbhelper->getInvGroupPtrByID(groupID));
    addGroupWidget->show();
    QObject::connect(addGroupWidget,&AddItemGroups::closing,
                     this,&inventorygroup::setTableView);
   \
}
