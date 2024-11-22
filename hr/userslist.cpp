#include "userslist.h"
#include "ui_userslist.h"

UsersList::UsersList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersList)
{
    ui->setupUi(this);
    dbhelper = new UserProfileDatabaseHelper();
    int i=0;

    ui->tableWidget->setColumnCount(4);

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* groupName = new QTableWidgetItem;
    groupName->setText("Employee Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,groupName);

    QTableWidgetItem* designation = new QTableWidgetItem;
    designation->setText("Designation");
    ui->tableWidget->setHorizontalHeaderItem(i++,designation);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    //    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(3,25);

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setShowGrid(false);
    setTable();

}

UsersList::~UsersList()
{
    delete ui;

}

void UsersList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void UsersList::on_createNewPushButton_clicked()
{
    AddUserProfileWidget = new AddUserProfile(this);
    AddUserProfileWidget->setWindowFlags(Qt::Window);
    AddUserProfileWidget->show();
    QObject::connect(AddUserProfileWidget,SIGNAL(closing()),this,SLOT(setTable()));

}

void UsersList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
        int r = ui->tableWidget->item(index.row(),0)->text().toInt()-1;
        AddUserProfileWidget = new AddUserProfile(this);
        AddUserProfileWidget->setWindowFlags(Qt::Window);
        AddUserProfileWidget->showVoucher( *voucherList[r]);
        AddUserProfileWidget->show();
        QObject::connect(AddUserProfileWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

void UsersList::setTable()
{
    int cnt =0;
    int i=0;
    ui->tableWidget->setRowCount(0);
    voucherList = dbhelper->getAllUsersAsPtr();
    for(int j =0;j<voucherList.size();j++){

        UserProfileDataModel obj=*voucherList[j];
        ui->tableWidget->insertRow(cnt);
        i=0;
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,SLNO);

        QTableWidgetItem* groupName = new QTableWidgetItem;
        groupName->setText(obj.Name);
        groupName->setFlags(groupName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,groupName);

        QTableWidgetItem* Designation = new QTableWidgetItem;
        Designation->setText(obj.Designation);
        Designation->setFlags(groupName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Designation);


        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",cnt);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->tableWidget->setCellWidget(cnt,i++,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        cnt++;
    }
}

void UsersList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
}
