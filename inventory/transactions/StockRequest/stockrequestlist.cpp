#include "stockrequestlist.h"
#include "ui_stockrequestlist.h"

StockRequestList::StockRequestList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StockRequestList)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    model = new QSqlQueryModel;
    setTable();
}

StockRequestList::~StockRequestList()
{
    delete ui;
}

void StockRequestList::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper.getVouchersListByDateModel(fromDate, toDate);


    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView->hideColumn(2);

}

void StockRequestList::on_createNew_clicked()
{
    editor = new StockRequestEditor(this);
    editor->setWindowTitle("Stock Request Form");
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTable()));
}

void StockRequestList::on_OKButton_clicked()
{
    setTable();
}

void StockRequestList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();

    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(2).toString();
    editor = new StockRequestEditor(this);
    editor->setWindowTitle("Stock Request Form");
    editor->setWindowFlags(Qt::Window);
    qDebug()<<"Voucher  No:"<<voucherNo;
    obj = dbHelper.getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
    editor->showVoucher(obj);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTable()));

}
