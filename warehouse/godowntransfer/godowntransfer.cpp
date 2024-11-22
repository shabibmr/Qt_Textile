#include "godowntransfer.h"
#include "ui_godowntransfer.h"

GodownTransfer::GodownTransfer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownTransfer)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    dbhelper = new GodownTransferDatabaseHelper;
    setTableView();
}

GodownTransfer::~GodownTransfer()
{
    delete ui;
}

void GodownTransfer::setHeading(QString label)
{

}

void GodownTransfer::setTableView()
{
    qDebug()<<"GT 27";
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    qDebug()<<"GT 36";
    model = dbhelper->getVoucherListByDateModel(fromDate,toDate);
    qDebug()<<"GT 37";
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(4).toFloat();
    }
//    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
//    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    qDebug()<<"GT 55";

}

void GodownTransfer::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void GodownTransfer::on_createNewPushButton_clicked()
{
    AddGodownTransferWidget = new GodownTransferEditor(this);
//    AddGodownTransferWidget = new AddGodownTransfer(this);
    AddGodownTransferWidget->setWindowFlags(Qt::Window);
    AddGodownTransferWidget->showMaximized();
    connect(AddGodownTransferWidget, &GodownTransferEditor::closing, this, &GodownTransfer::setTableView);
}

void GodownTransfer::on_okPushButton_clicked()
{
    setTableView();
}

void GodownTransfer::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
//    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    AddGodownTransferWidget = new GodownTransferEditor();
//    AddGodownTransferWidget = new AddGodownTransfer(this);
    AddGodownTransferWidget->setWindowFlags(Qt::Window);
    AddGodownTransferWidget->showVoucher(voucher);
    AddGodownTransferWidget->showMaximized();
    QObject::connect(AddGodownTransferWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}
