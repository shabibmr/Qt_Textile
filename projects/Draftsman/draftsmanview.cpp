#include "draftsmanview.h"
#include "ui_draftsmanview.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
draftsManview::draftsManview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::draftsManview)
{
    ui->setupUi(this);

    ui->dateFrom->setDate(QDate::currentDate().addYears(-1));
    ui->dateTo->setDate(QDate::currentDate());
    dbHelper = new QuotationDatabaseHelper();
    ui->tableWidget->setColumnCount(7);
    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* Date = new QTableWidgetItem;
    Date->setText("Quote Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,Date);

    QTableWidgetItem* uom = new QTableWidgetItem;
    uom->setText("Quote No");
    ui->tableWidget->setHorizontalHeaderItem(i++,uom);

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Customer Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,ItemName);


    QTableWidgetItem* ModelNumber = new QTableWidgetItem;
    ModelNumber->setText("Status");
    ui->tableWidget->setHorizontalHeaderItem(i++,ModelNumber);

    QTableWidgetItem* uploaded = new QTableWidgetItem;
    uploaded->setText("Files Count");

    ui->tableWidget->setHorizontalHeaderItem(i++,uploaded);


    ui->tableWidget->hideColumn(6);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(5,80);

    ui->tableWidget->resizeRowsToContents();

    setTable();
}

draftsManview::~draftsManview()
{
    delete ui;
}

void draftsManview::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r=ui->tableWidget->item(index.row(),6)->text().toInt();

    QuoteViewWidget = new QuoteView(this);
    QuoteViewWidget->setWindowFlags(Qt::Window);
    QString vNO=voucherList[r].voucherNumber;
    GeneralVoucherDataObject obj = dbHelper->getVoucherByVoucherNo(vNO);
    QuoteViewWidget->showVoucher(obj);
    QuoteViewWidget->showMaximized();
    QObject::connect(QuoteViewWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

void draftsManview::setTable()
{
    QDate dateFrom = ui->dateFrom->date();
    QDate dateTo = ui->dateTo->date();
    voucherList = dbHelper->getVoucherListByDate(dateFrom,dateTo);
    int cnt =0;
    int i=0;
    ui->tableWidget->setRowCount(0);

    QStringList statusList;

    int k=0;
    for(GeneralVoucherDataObject obj:voucherList){
        i=0;
        if(obj.status==QuotationStatus::SentToDraftsMan || obj.status==QuotationStatus::DraftsCompleted
                || obj.status == QuotationStatus::DraftsStarted){

            ui->tableWidget->insertRow(cnt);
            QTableWidgetItem* SLNO = new QTableWidgetItem;
            SLNO->setText(QString::number(cnt+1));
            SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,SLNO);

            QTableWidgetItem* dateItem = new QTableWidgetItem;
            dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
            dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,dateItem);


            QTableWidgetItem* ReqNo = new QTableWidgetItem;
            ReqNo->setText(obj.voucherNumber);
            ReqNo->setTextAlignment(Qt::AlignHCenter |Qt::AlignVCenter);
            ReqNo->setFlags(ReqNo->flags()^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,ReqNo);


            QTableWidgetItem* LedgerName = new QTableWidgetItem;
            LedgerName->setText(obj.Contact.CompanyName);
            LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,LedgerName);


            QTableWidgetItem* status = new QTableWidgetItem;
            status->setTextAlignment(Qt::AlignHCenter |Qt::AlignVCenter);
            status->setText(QuotationStatus::getStatusString(obj.status));
            status->setFlags(status->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,status);
            ui->tableWidget->resizeRowsToContents();

            int fileCount = dbHelper->getFileCount(obj.voucherNumber);


            QTableWidgetItem* itemsUploaded = new QTableWidgetItem;
            itemsUploaded->setText(QString::number(fileCount));
            itemsUploaded->setTextAlignment(Qt::AlignHCenter |Qt::AlignVCenter);
            itemsUploaded->setFlags(itemsUploaded->flags()^ Qt::ItemIsEditable);
            ui->tableWidget->setItem(cnt,i++,itemsUploaded);

            QTableWidgetItem* hidden = new QTableWidgetItem;
            hidden->setText(QString::number(k));
            ui->tableWidget->setItem(cnt,i++,hidden);

        cnt++;
        }
        k++;

    }

}

void draftsManview::on_okButton_clicked()
{
    setTable();
}
