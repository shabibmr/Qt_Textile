#include "previousquotationslist.h"
#include "ui_previousquotationslist.h"

PreviousQuotationsList::PreviousQuotationsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousQuotationsList)
{
    ui->setupUi(this);
    ui->createNewPushButton->hide();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    dbhelper = new OldQuotationDatabaseHelper();
    ui->tableWidget->setColumnCount(7);

    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* dateItem = new QTableWidgetItem;
    dateItem->setText("Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger");
    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

    QTableWidgetItem* ReqNo = new QTableWidgetItem;
    ReqNo->setText("No.");
    ui->tableWidget->setHorizontalHeaderItem(i++,ReqNo);

    QTableWidgetItem* ApprovalStatus = new QTableWidgetItem;
    ApprovalStatus->setText("Status");
    ui->tableWidget->setHorizontalHeaderItem(i++,ApprovalStatus);

    QTableWidgetItem* Amount = new QTableWidgetItem;
    Amount->setText("Amount");
    Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);

    ui->tableWidget->setHorizontalHeaderItem(i++,Amount);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(3,40);
    ui->tableWidget->setColumnWidth(6,25);


    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setShowGrid(false);
    if(!LoginValues::model.Quotes_Delete)
        ui->tableWidget->hideColumn(6);
    setTable();
    this->setFocus();

}

PreviousQuotationsList::~PreviousQuotationsList()
{
    delete ui;
}

void PreviousQuotationsList::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void PreviousQuotationsList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
    else if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){

    }

}

void PreviousQuotationsList::on_createNewPushButton_clicked()
{
    addQuotationObject = new PreviousQuotationWidget(this);
    addQuotationObject ->setWindowFlags(Qt::Window);
    addQuotationObject ->showMaximized();
    QObject::connect(addQuotationObject,SIGNAL(closing()),this,SLOT(setTable()));
}

void PreviousQuotationsList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = ui->tableWidget->item(index.row(),0)->text().toInt()-1;
    qDebug()<<"value of r"<<r;
    addQuotationObject = new PreviousQuotationWidget(this);
    addQuotationObject->setWindowFlags(Qt::Window);
    addQuotationObject->showVoucher( voucherList[r]);
    addQuotationObject->showMaximized();
    QObject::connect(addQuotationObject,SIGNAL(closing()),this,SLOT(setTable()));
}

void PreviousQuotationsList::setTable()
{
    QDate dateFrom = ui->fromDateDateEdit->date();
    QDate dateTo = ui->toDateDateEdit->date();
    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
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
        LedgerName->setText(obj.Contact.CompanyName);
        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,LedgerName);


        QTableWidgetItem* ReqNo = new QTableWidgetItem;
        ReqNo->setText(obj.voucherNumber);
        ReqNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);

        ReqNo->setFlags(ReqNo->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,ReqNo);


        QTableWidgetItem* status = new QTableWidgetItem;
        status->setTextAlignment(Qt::AlignHCenter |Qt::AlignVCenter);
        if(obj.ClientApprovalStatus == true) status->setText("Approved");

        else if (obj.ClientApprovalStatus == false) status->setText("Awaiting Approval");
        status->setFlags(status->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,status);


        QTableWidgetItem* Amount = new QTableWidgetItem;
        Amount->setText(QString::number(obj.grandTotal,'f',2));
        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Amount);

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",cnt);
        delButton->setAutoRaise(true);

        ui->tableWidget->setCellWidget(cnt,i++,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));
        qDebug()<<i;
        cnt++;
    }
}

void PreviousQuotationsList::on_okPushButton_clicked()
{
    setTable();
}

void PreviousQuotationsList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;

}

void PreviousQuotationsList::enterPressed()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    qDebug()<<"row :"<<index.row();
}

void PreviousQuotationsList::on_fromDateDateEdit_editingFinished()
{
    on_okPushButton_clicked();
}
