#include "paymentsplitwidget.h"
#include "ui_paymentsplitwidget.h"



PaymentSplitWidget::PaymentSplitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentSplitWidget)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(2);
    ledgerHelper = new LedgerMasterDatabaseHelper();
    allLedgersPtr = ledgerHelper->getAllLedgersAsPtr();

    voucher = new GeneralVoucherDataObject();

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger Name");
    ui->tableWidget->setHorizontalHeaderItem(0,LedgerName);

    QTableWidgetItem* ledAmount = new QTableWidgetItem;
    ledAmount->setText("Amount");
    ui->tableWidget->setHorizontalHeaderItem(1,ledAmount);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);



}

PaymentSplitWidget::~PaymentSplitWidget()
{
    //delete ui;
}

void PaymentSplitWidget::on_cancelButton_clicked()
{

    this->close();
}

void PaymentSplitWidget::on_OKButton_clicked()
{

    float bal = ui->balanceLine->text().toFloat();
    float amt = ui->amountLine->text().toFloat();

    if( bal != amt ){
        QMessageBox box;
        box.setText("Amount Should be Equal to "+QString::number(amt,'f',2));
        box.exec();
        return;
    }    
    else {
        this->close();
        emit paymentSplit(voucher->ledgersList);
        qDebug()<<"Closing Split widget";

    }
}

void PaymentSplitWidget::setTable()
{
    ui->tableWidget->setRowCount(voucher->ledgersList.size());
    qDebug()<<"set table size = "<<voucher->ledgersList.size();
    int rCnt;
    float total=0;
    for(rCnt=0;rCnt<voucher->ledgersList.size();rCnt++){


        QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
        QPointer<QWidget> SearchWid = new QWidget;
        QPointer<QVBoxLayout> lay = new QVBoxLayout();
        lay->setContentsMargins(0,0,0,0);
        lay->setSpacing(0);
        lay->addWidget(ledSearch);
        SearchWid->setLayout(lay);
        ledSearch->setText(ledgerHelper->getLedgerNameByID(voucher->ledgersList[rCnt].LedgerID));
        //        qDebug()<<"Led name "<<rCnt<<voucher->ledgersList[rCnt].LedgerName;
        //        qDebug()<<"Led id"<<rCnt<<ledgerHelper->getLedgerNameByID(voucher->ledgersList[rCnt].LedgerID);
        ledSearch->setProperty("row",rCnt);
        SearchWid->setProperty("row",rCnt);
        ui->tableWidget->setCellWidget(rCnt,0,SearchWid);
        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);

        total = total+voucher->ledgersList[rCnt].drAmount;
        if(voucher->ledgersList[rCnt].drAmount>=0){
            amountEdit->setText(QString::number(voucher->ledgersList[rCnt].drAmount,'f',2));
        }
        else{
            amountEdit->setText(QString::number(voucher->ledgersList[rCnt].crAmount*-1,'f',2));
        }

        ui->tableWidget->setCellWidget(rCnt,1,amountEdit);
        amountEdit->setAlignment(Qt::AlignRight);
        amountEdit->setProperty("row",rCnt);
        QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));
        QObject::connect(ledSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                         this,SLOT(getLedgerDetails(LedgerMasterDataModel)));




    }
    ui->tableWidget->insertRow(rCnt);


    QPointer<GMLedgerSearch> ledSearch = new GMLedgerSearch(allLedgersPtr);
    QPointer<QWidget> SearchWid = new QWidget;
    QPointer<QVBoxLayout> lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(ledSearch);
    SearchWid->setLayout(lay);
    ledSearch->setProperty("row",rCnt);
    SearchWid->setProperty("row",rCnt);
    ui->tableWidget->setCellWidget(rCnt,0,SearchWid);

    QLineEdit *amountEdit = new QLineEdit;
    amountEdit ->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    amountEdit->setFrame(QFrame::NoFrame);
    amountEdit->setProperty("row",rCnt);
    amountEdit->setAlignment(Qt::AlignRight);
    amountEdit->setReadOnly(true);
    ledSearch->setFocus();
    ledSearch->selectAll();
    QObject::connect(amountEdit,SIGNAL(editingFinished()),this,SLOT(updateLedgerValue()));
    ui->tableWidget->setCellWidget(rCnt,1,amountEdit);

    QObject::connect(ledSearch,SIGNAL(SelectedLedger(LedgerMasterDataModel)),
                     this,SLOT(getLedgerDetails(LedgerMasterDataModel)));

    ui->balanceLine->setText(QString::number(total,'f',2));


}

void PaymentSplitWidget::updateLedgerValue()
{
    qDebug()<<"update Led Value";
    int row = sender()->property("row").toInt();

    if(row<=voucher->ledgersList.size()){
        qDebug()<<"num ledgers"<<voucher->ledgersList.size();
        float amount = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,1))
                ->text().toFloat();
        if(amount >=0){
            voucher->ledgersList[row].drAmount = amount;
        }
        else{

            voucher->ledgersList[row].crAmount = -amount;
        }
        voucher->ledgersList[row].amount = amount;


    }
    setTable();
    return;
}

void PaymentSplitWidget::appendLedger(LedgerMasterDataModel item, int row)
{
    openNewWidgetLedger = true;
    qDebug()<<"524 row ="<<row;
    //    QLineEdit* line = qobject_cast<QLineEdit*> (ui->tableWidget->cellWidget(row,1));

    //    item.amount = line->text().toFloat();
    //    item.drAmount = item.amount;
    //    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
    //    qDebug()<<"$$$$$$$$$$$$$$edited row sub tot= "<<item.amount;
    //    qDebug()<<"$$$$$$$$$$$$$$$$$$$$";

    qDebug()<<"524 row count = "<<ui->tableWidget->rowCount();
    if(voucher->ledgersList.size() == row ){
        voucher->ledgersList.append(item);
        qDebug()<<"item appended";
    }
    else{
        voucher->ledgersList[row]=item;
        qDebug()<<"item Inserted at "<<row;
    }
}

void PaymentSplitWidget::getLedgerDetails(LedgerMasterDataModel led)
{

    qDebug()<<"444"<<"Ledger Selected"<<led.LedgerName;
    int row = sender()->property("row").toInt();
    led.isInvoiceItem = false;
    appendLedger(led,row);
    setTable();
    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,1))->setFocus();
    qobject_cast< QLineEdit*>(ui->tableWidget->cellWidget(row,1))->selectAll();
    return;
}

void PaymentSplitWidget::setLedList(QList<LedgerMasterDataModel> ledList)
{
    voucher->ledgersList = ledList;
    setTable();
}

void PaymentSplitWidget::setVoucher(GeneralVoucherDataObject *value)
{
    // delete voucher;
    voucher = value;

    ui->amountLine->setText(QString::number(value->grandTotal,'f',2));
    ui->balanceLine->setText("0.00");
    setTable();

}
