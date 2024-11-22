#include "receiptnotes.h"
#include "ui_receiptnotes.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

ReceiptNotes::ReceiptNotes( bool hideCreate, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiptNotes)
{
    ui->setupUi(this);
    ui->createNewPushButton->setHidden(hideCreate);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    dbhelper = new ReceiptNoteDatabaseHelper;
    setTableView();

}

ReceiptNotes::~ReceiptNotes()
{
    delete ui;
}

void ReceiptNotes::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void ReceiptNotes::on_createNewPushButton_clicked()
{
    AddReceiptNoteWidget = new ReceiptNoteVoucherEditor(this);
    AddReceiptNoteWidget->setWindowFlags(Qt::Window);
    AddReceiptNoteWidget->showMaximized();
    QObject::connect(AddReceiptNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void ReceiptNotes::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    AddReceiptNoteWidget = new ReceiptNoteVoucherEditor(this);
    AddReceiptNoteWidget->setWindowFlags(Qt::Window);
    AddReceiptNoteWidget->showVoucher(voucher);
    AddReceiptNoteWidget->showMaximized();
    QObject::connect(AddReceiptNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

void ReceiptNotes::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
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
        total+= model->record(i).value(6).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    //    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
}
