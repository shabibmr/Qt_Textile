#include "quotaitonslist.h"
#include "ui_quotaitonslist.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

QuotaitonsList::QuotaitonsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuotaitonsList)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new ProformaDatabaseHelper();
    //    ui->tableWidget->setColumnCount(6);

    int i=0;

    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();
}

QuotaitonsList::~QuotaitonsList()
{
    delete ui;
}


void QuotaitonsList::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void QuotaitonsList::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void QuotaitonsList::on_createNewPushButton_clicked()
{

    editor = new QuotationEditor(this);

    editor->setObjectName("Editor");

//    editor->setStyleSheet("QWidget#Editor {border-top-left-radius:15px;border-top-right-radius:5px;}");
//    editor->setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}


void QuotaitonsList::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

//    model = ledgerHelper->getSalesReport(fromDateTime,toDateTime);
    model = dbhelper->getVoucherListByDateTimeModel(fromDateTime, toDateTime);
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
        total += model->record(i).value(7).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
}

void QuotaitonsList::on_okPushButton_clicked()
{
    setTableView();
}

void QuotaitonsList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
}

void QuotaitonsList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"Pref : "<<voucherPrefix;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    editor = new QuotationEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher(voucher);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));

}
