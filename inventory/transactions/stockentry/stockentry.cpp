#include "stockentry.h"
#include "ui_stockentry.h"

StockEntry::StockEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockEntry)
{
    ui->setupUi(this);
    dbhelper = new StockEntryDatabaseHelper();
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());

    setTableView();
}

StockEntry::~StockEntry()
{
    delete ui;
}

void StockEntry::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void StockEntry::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void StockEntry::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateModel(fromDate,toDate);
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
    //     ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    //    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);


}

void StockEntry::on_createNewPushButton_clicked()
{
    addStockEntryWidget = new StockEntryEditor(this);
    addStockEntryWidget->setWindowFlags(Qt::Window);
    addStockEntryWidget->showMaximized();
    QObject::connect(addStockEntryWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void StockEntry::on_okPushButton_clicked()
{
    setTableView();
}

void StockEntry::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void StockEntry::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);

    addStockEntryWidget = new StockEntryEditor(this);
    addStockEntryWidget->setWindowFlags(Qt::Window);
    addStockEntryWidget->showVoucher(voucher);
    addStockEntryWidget->showMaximized();
    QObject::connect(addStockEntryWidget,SIGNAL(closing()),this,SLOT(setTableView()));

}
