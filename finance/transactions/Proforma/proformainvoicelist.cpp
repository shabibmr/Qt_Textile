#include "proformainvoicelist.h"
#include "ui_proformainvoicelist.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
ProformaInvoiceList::ProformaInvoiceList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProformaInvoiceList)
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

ProformaInvoiceList::~ProformaInvoiceList()
{
    delete ui;
}


void ProformaInvoiceList::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void ProformaInvoiceList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void ProformaInvoiceList::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateTimeModel(fromDateTime,toDateTime);
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
        total+= model->record(i).value(7).toFloat();
    }
    //    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);


}

void ProformaInvoiceList::on_createNewPushButton_clicked()
{
    editor = new ProformaInvoiceEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}

void ProformaInvoiceList::on_okPushButton_clicked()
{
    setTableView();
}

void ProformaInvoiceList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void ProformaInvoiceList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    editor = new ProformaInvoiceEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher( voucher);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}
