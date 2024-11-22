#include "debitnote.h"
#include "ui_debitnote.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
debitNote::debitNote(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::debitNote)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new DebitNoteVoucherDatabaseHelper();
//    ui->tableWidget->setColumnCount(6);

//    int i=0;

//    QTableWidgetItem* SLNO = new QTableWidgetItem;
//    SLNO->setText("#");
//    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

//    QTableWidgetItem* dateItem = new QTableWidgetItem;
//    dateItem->setText("Date");
//    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

//    QTableWidgetItem* LedgerName = new QTableWidgetItem;
//    LedgerName->setText("Ledger");
//    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

//    QTableWidgetItem* ReqNo = new QTableWidgetItem;
//    ReqNo->setText("No.");
//    ui->tableWidget->setHorizontalHeaderItem(i++,ReqNo);

//    QTableWidgetItem* Amount = new QTableWidgetItem;
//    Amount->setText("Amount");
//    ui->tableWidget->setHorizontalHeaderItem(i++,Amount);

//    QTableWidgetItem* deleteItem = new QTableWidgetItem;
//    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
//    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
//    ui->tableWidget->setColumnWidth(0,25);
//    ui->tableWidget->setColumnWidth(5,25);

//    ui->tableWidget->hideColumn(0);
//    ui->tableWidget->setShowGrid(false);
     ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();
}

debitNote::~debitNote()
{
    delete ui;
}

void debitNote::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void debitNote::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void debitNote::setTableView()
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
        total+= model->record(i).value(6).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
//    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
}

void debitNote::on_createNewPushButton_clicked()
{
    addDebitNoteWidget = new DebitNoteVoucherEditor();
    addDebitNoteWidget->setWindowFlags(Qt::Window);
    addDebitNoteWidget ->showMaximized();
    QObject::connect(addDebitNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}

//void debitNote::on_tableWidget_doubleClicked(const QModelIndex &index)
//{
//    int r = ui->tableWidget->item(index.row(),0)->text().toInt()-1;
//    qDebug()<<"value of r"<<r;
//    addDebitNoteWidget = new DebitNoteVoucherEditor(this);
//    addDebitNoteWidget->setWindowFlags(Qt::Window);
//    addDebitNoteWidget->showVoucher(dbhelper->getVoucherByVoucherNoAsPtr(voucherList[r].voucherNumber));
//    addDebitNoteWidget->showMaximized();
//    QObject::connect(addDebitNoteWidget,SIGNAL(closing()),this,SLOT(setTable()));
//}

//void debitNote::setTable()
//{
//    float totalAmount =0;
//    QDate dateFrom = ui->fromDateEdit->date();
//    QDate dateTo = ui->toDateEdit->date();
//    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
//    qDebug()<<"Size = "<<voucherList.size();
//    int cnt =0;
//    int i=0;
//    ui->tableWidget->setRowCount(0);
//    for(GeneralVoucherDataObject obj:voucherList){
//        ui->tableWidget->insertRow(cnt);
//        i=0;
//        QTableWidgetItem* SLNO = new QTableWidgetItem;
//        SLNO->setText(QString::number(cnt+1));
//        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,SLNO);

//        QTableWidgetItem* dateItem = new QTableWidgetItem;
//        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
//        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,dateItem);

//        QTableWidgetItem* LedgerName = new QTableWidgetItem;
//        LedgerName->setText(obj.ledgerObject.LedgerName);
//        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,LedgerName);

//        QTableWidgetItem* ReqNo = new QTableWidgetItem;
//        ReqNo->setText(obj.voucherNumber);
//        ReqNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);

//        ReqNo->setFlags(ReqNo->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,ReqNo);

//        QTableWidgetItem* Amount = new QTableWidgetItem;
//        Amount->setText(QString::number(obj.grandTotal,'f',2));
//        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
//        ui->tableWidget->setItem(cnt,i++,Amount);
//        totalAmount += obj.grandTotal;
//        ui->totalAmountLineEdit->setText(QString::number(totalAmount,'f',2));

//        QToolButton* delButton = new QToolButton;
//        delButton->setIcon(QIcon(":/icons/trash.ico"));
//        delButton->setProperty("row",cnt);
//        delButton->setAutoRaise(true);
//        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
//        ui->tableWidget->setCellWidget(cnt,i++,delButton);
//        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

//        cnt++;
//    }
//}

void debitNote::on_okPushButton_clicked()
{
    setTableView();
}

void debitNote::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void debitNote::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    addDebitNoteWidget = new DebitNoteVoucherEditor(this);
    addDebitNoteWidget->setWindowFlags(Qt::Window);
    addDebitNoteWidget->showVoucher(voucher);
    addDebitNoteWidget->showMaximized();
    QObject::connect(addDebitNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}
