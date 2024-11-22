#include "workorderlist.h"
#include "ui_workorderlist.h"

WorkOrderList::WorkOrderList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkOrderList)
{
    ui->setupUi(this);

    ui->fromDateEdit->setDate(QDate::currentDate().addYears(-1));
    ui->toDateEdit->setDate(QDate::currentDate());
    dbHelper = new WorkOrderDatabaseHelper();


    setTableView();
}

WorkOrderList::~WorkOrderList()
{
    delete ui;
}

void WorkOrderList::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();

    QDate toDate = ui->toDateEdit->date();

    model = dbHelper->getVoucherListByDateModel(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}


void WorkOrderList::on_okButton_clicked()
{
    setTableView();
}

void WorkOrderList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"get voucher"<<voucherNo<<voucherPrefix;
    GeneralVoucherDataObject *voucher = dbHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

//    editor = new WorkOrderEditor(this);
//    editor->setWindowFlags(Qt::Window);
//    editor->showVoucher(voucher);
//    editor->showMaximized();
//    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));

}

void WorkOrderList::on_createNew_clicked()
{
//    editor = new WorkOrderEditor(this);
//    editor->setWindowFlags(Qt::Window);
////    editor->showVoucher(voucher);
//    editor->showMaximized();
//    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));

}
