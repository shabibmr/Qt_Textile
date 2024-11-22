#include "usergroupslist.h"
#include "ui_usergroupslist.h"
#include <QDebug>

UserGroupsList::UserGroupsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserGroupsList)
{
    ui->setupUi(this);
    dbhelper = new UserGroupDatabaseHelper();
    int i=0;
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->hideColumn(3);
    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* groupName = new QTableWidgetItem;
    groupName->setText("Group Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,groupName);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);



    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    //    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(2,25);

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setShowGrid(false);

    setTable();

}

UserGroupsList::~UserGroupsList()
{
    delete ui;
}

void UserGroupsList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void UserGroupsList::on_createNewPushButton_clicked()
{
    AddUserGroupWidget = new AddUserGroup(this);
    AddUserGroupWidget->setWindowFlags(Qt::Window);
    AddUserGroupWidget->show();
    QObject::connect(AddUserGroupWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

void UserGroupsList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    int id = ui->tableWidget->item(r,3)->text().toInt();
    AddUserGroupWidget = new AddUserGroup(this);
    AddUserGroupWidget->setWindowFlags(Qt::Window);
//    AddUserGroupWidget->showVoucher(*voucherList[r]);
    UserGroupDataModel userGroup = dbhelper->getUserGroup(id);
    AddUserGroupWidget->edit(&userGroup);
    AddUserGroupWidget->show();
    QObject::connect(AddUserGroupWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

void UserGroupsList::setTable()
{
    int cnt =0;
    int i=0;
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setRowCount(0);
    map = dbhelper->getUserGroups();

    for(int id:map.keys()){
        ui->tableWidget->insertRow(cnt);
        int i=0;
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,SLNO);

        QTableWidgetItem* groupName = new QTableWidgetItem;
        groupName->setText(map.value(id));
        groupName->setFlags(groupName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,groupName);

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",id);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->tableWidget->setCellWidget(cnt,i++,delButton);

        QTableWidgetItem* id1 = new QTableWidgetItem;
        id1->setText(QString::number(id));
        id1->setFlags(groupName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,id1);


        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        cnt++;
    }
    ui->tableWidget->setSortingEnabled(true);
}

void UserGroupsList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}
