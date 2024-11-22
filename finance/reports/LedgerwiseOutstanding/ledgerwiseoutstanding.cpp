#include "ledgerwiseoutstanding.h"
#include "ui_ledgerwiseoutstanding.h"

LedgerwiseOutstanding::LedgerwiseOutstanding(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedgerwiseOutstanding)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->tableWidget->setColumnCount(7);

    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* dateItem = new QTableWidgetItem;
    dateItem->setText("Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

    QTableWidgetItem* VchrType = new QTableWidgetItem;
    VchrType->setText("Voucher Type");
    ui->tableWidget->setHorizontalHeaderItem(i++,VchrType);

    QTableWidgetItem* VchrNo = new QTableWidgetItem;
    VchrNo->setText("Voucher No.");
    ui->tableWidget->setHorizontalHeaderItem(i++,VchrNo);

    QTableWidgetItem* Debit = new QTableWidgetItem;
    Debit->setText("Debit");
    ui->tableWidget->setHorizontalHeaderItem(i++,Debit);

    QTableWidgetItem* Credit = new QTableWidgetItem;
    Credit->setText("Credit");
    ui->tableWidget->setHorizontalHeaderItem(i++,Credit);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setShowGrid(false);

    setTable();
    this->setFocus();

}

LedgerwiseOutstanding::~LedgerwiseOutstanding()
{
    delete ui;
}

void LedgerwiseOutstanding::setTable()
{
    QDate dateFrom = ui->fromDateDateEdit->date();
    QDate dateTo = ui->toDateDateEdit->date();
//    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
    int cnt =0;
    int i=0;
    ui->tableWidget->setRowCount(0);
    for(GeneralVoucherDataObject obj:voucherList){
        ui->tableWidget->insertRow(cnt);
        i=0;

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,SLNO);

        QTableWidgetItem* dateItem = new QTableWidgetItem;
        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,dateItem);

        QTableWidgetItem* LedgerName = new QTableWidgetItem;
//        LedgerName->setText(obj.Contact.CompanyName);
        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,LedgerName);

        QTableWidgetItem* VchrType = new QTableWidgetItem;
//        VchrType->setText(obj.voucherNumber);
        VchrType->setFlags(VchrType->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,VchrType);

        QTableWidgetItem* VchrNo = new QTableWidgetItem;
        VchrNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
//        VchrNo->setText(obj.status);
        VchrNo->setFlags(VchrNo->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,VchrNo);

        QTableWidgetItem* Debit = new QTableWidgetItem;
//        Debit->setText(QString::number(obj.grandTotal,'f',2));
        Debit->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
        Debit->setFlags(Debit->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Debit);

        QTableWidgetItem* Credit = new QTableWidgetItem;
//        Credit->setText(QString::number(obj.grandTotal,'f',2));
        Credit->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
        Credit->setFlags(Credit->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Credit);

        cnt++;
    }
}
