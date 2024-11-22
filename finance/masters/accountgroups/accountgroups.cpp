#include "accountgroups.h"
#include "ui_accountgroups.h"
#include <QTableWidgetItem>

accountGroups::accountGroups(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::accountGroups)
{
    ui->setupUi(this);
    dbhelper = new AccountGroupMasterDatabaseHelper();
//    accGroupsModel = new QSqlQueryModel();

    sort = new GMItemSortFilterProxyModel();
    setTableView();

}

accountGroups::~accountGroups()
{
    delete ui;
}

void accountGroups::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void accountGroups::on_createNewPushButton_clicked()
{
//    AddAccountGroupClickedSlot();
    newAccountGroup = new addAccountGroup(this);
    newAccountGroup->setWindowFlags(Qt::Window);
    newAccountGroup->show();

    QObject::connect(newAccountGroup,SIGNAL(closing()),this,SLOT(setTableView()));

}

void accountGroups::AddAccountGroupClickedSlot()
{
    newAccountGroup = new addAccountGroup(this);
    newAccountGroup->setWindowFlags(Qt::Window);
    newAccountGroup->show();

    QObject::connect(newAccountGroup,SIGNAL(closing()),this,SLOT(setTableView()));

}

void accountGroups::setTableView()
{
    accGroupsModel = dbhelper->getAllAccountsModel();

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(accGroupsModel);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //    ui->tableWidget->setItemDelegate(new LedgerReportDelegate());
    ui->tableView->hideColumn(0);

    ui->searchLine->setFocus();

}


//void accountGroups::on_tableWidget_doubleClicked(const QModelIndex &index)
//{
//    int r = index.row();
//    newAccountGroup = new addAccountGroup(this);
//    newAccountGroup->setWindowFlags(Qt::Window);
//    newAccountGroup->setTitle("Update Account Group");
//    newAccountGroup->editAccountGroup(groupObjs[r]);
//    newAccountGroup->show();

//    QObject::connect(newAccountGroup,SIGNAL(closing()),this,SLOT(setTable()));

//}

void accountGroups::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = sort->mapToSource(index).row();
    QString groupID = accGroupsModel->record(r).value(0).toString();

    newAccountGroup = new addAccountGroup(this);
    newAccountGroup->setWindowFlags(Qt::Window);
    newAccountGroup->setTitle("Update Account Group");
    newAccountGroup->editAccountGroup(dbhelper->getGroupPtrById(groupID));
    newAccountGroup->show();

    QObject::connect(newAccountGroup,SIGNAL(closing()),this,SLOT(setTableView()));

}

void accountGroups::on_searchLine_textChanged(const QString &arg1)
{
    QRegExp regExp(ui->searchLine->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);
}
