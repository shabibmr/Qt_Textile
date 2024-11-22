#include "modeofpaystore.h"
#include "ui_modeofpaystore.h"

#include <QDebug>
#include "customwidgets/Validator/myvalidator.h"

#include <QMessageBox>

ModeofPayStore::ModeofPayStore(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeofPayStore)
{
    ui->setupUi(this);
    showSelectLedgerWidget(false);
    showSplitWidget(false);
    ui->modeOfServiceWidget->hide();

    ui->roundOffLabel->hide();
    ui->roundOffLineEdit->hide();

    ui->label_6->hide();
    ui->amountChange4Line->hide();
    //    ui->splitToolButton->hide();
    ui->complimentarycashToolButton->hide();

    //    hideDiscounts();


    ui->zomatoWidget->hide();
    ui->carriageToolButton->hide();
    ui->talabatToolButton->hide();
    //    ui->zomatoToolButton->hide();
    ui->DeliverooToolButton->hide();

    if(!CounterSettingsDataModel::TypeOfTrade.contains("Rest",Qt::CaseInsensitive)){
        ui->complimentarycashToolButton->hide();
        ui->zomatoToolButton->hide();
    }

    //ui->deliveryToolButton->setCheckable(true);
    //ui->storeToolButton->setCheckable(true);
    ui->billCopytextEdit->setReadOnly(true);
    ui->deliveryToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->deliveryToolButton->setAutoRaise(true);
    ui->storeToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->storeToolButton->setAutoRaise(true);

    ui->noOfCopiesSpinBox->setValue(CounterSettingsDataModel::AllOtherPrintCopies);
    ui->modeOfServiceWidget->hide();
    //    QString html = "<html><body><img src=:/images/test1.jpg>";
    //    ui->billCopytextEdit->setHtml(html);
    //    ui->billCopytextEdit->setReadOnly(true);

    if(CounterSettingsDataModel::TypeOfTrade!="Restaurant"){
        //        ui->zomatoToolButton->setHidden(true);
        ui->paxSpinBox->setHidden(true);
        ui->label_3->setHidden(true);
    }



    //ServiceMode mode;
    // qDebug()<<QString::number(ServiceModeValue::Delivery);
}

ModeofPayStore::~ModeofPayStore()
{
    delete ui;
}

void ModeofPayStore::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
    ui->discountPercentLineEdit->setText(QString::number(voucher->discountPercent,'f',2));
    ui->discountAmountLineEdit->setText(QString::number(voucher->discountinAmount,'f',2));
    ui->subTotalLineEdit->setText(QString::number(voucher->subTotal,'f',2));
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));
    ui->narrationTextEdit->setPlainText(voucher->narration);
    ui->paxSpinBox->setValue(voucher->numBoxes);
    ui->discountAmountLineEdit->setValidator(new MyValidator(0,voucher->grandTotal,2,this));
    ui->discountPercentLineEdit->setValidator(new MyValidator(0,100,2,this));
    //    if(voucher->ModeOfService == ServiceMode::Delivery){
    //        on_deliveryToolButton_clicked();
    //    }
    //    else{
    //        on_storeToolButton_clicked();
    //    }
    setBillPreview();

}

void ModeofPayStore::hideDiscounts()
{
    ui->discountAmountLineEdit->hide();
    ui->discountAmountLabel->hide();
    ui->discountPercentLabel->hide();
    ui->discountPercentLineEdit->hide();


}

void ModeofPayStore::on_cashToolButton_clicked()
{
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);

    //    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID("0x2x21");
    //    voucher->ledgerObject.LedgerName ="zomato";

    //    delete LedgerHelper;
    modeOfPayCompleted();
}

void ModeofPayStore::on_cardToolButton_clicked()
{
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    BankList = LedgerHelper->getAllLedgersUnderGroupByName("BANK ACCOUNTS");
    ui->ledgerComboBox->clear();
    if(BankList.size()>1){
        showSelectLedgerWidget(true);
        for(int i=0;i<BankList.size();i++){
            ui->ledgerComboBox->addItem(BankList[i].LedgerName);
        }
    }
    else if(BankList.size()==1){
        voucher->ledgerObject = BankList[0];
        qDebug()<<"Card : "<<BankList[0].LedgerName;
        modeOfPayCompleted();
    }
    else{
        QMessageBox Box;
        Box.setText("No Bank Account(s) Created");
        Box.exec();
    }
    qDebug()<<voucher->ledgerObject.LedgerName;
    ui->ledgerComboBox->setCurrentText(voucher->ledgerObject.LedgerName);

}

void ModeofPayStore::on_creditToolButton_clicked()
{
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    ui->ledgerComboBox->clear();
    BankList = LedgerHelper->getAllLedgersUnderGroupByName("SUNDRY DEBTORS");
    if(BankList.size()>1){
        showSelectLedgerWidget(true);
        for(int i=0;i<BankList.size();i++){
            ui->ledgerComboBox->addItem(BankList[i].LedgerName);
        }
    }
    else if(BankList.size()==1){
        voucher->ledgerObject = BankList[0];
        modeOfPayCompleted();
    }
    else{
        QMessageBox Box;
        Box.setText("No-Debtors Created");
        Box.exec();
    }
    ui->ledgerComboBox->setCurrentText(voucher->ledgerObject.LedgerName);


}

void ModeofPayStore::on_splitToolButton_clicked()
{
    if(voucher->grandTotal<=0)
        return;
    PaymentSplitWidget* paywid = new PaymentSplitWidget(this);
    paywid->setWindowTitle("Enter Payment Details");
    paywid->setWindowFlags(Qt::Window);
    paywid->setAttribute(Qt::WA_DeleteOnClose);
    paywid->setVoucher(voucher);
    paywid->setLedList(splitLeds);
    paywid->show();
    QObject::connect(paywid,SIGNAL(paymentSplit(QList<LedgerMasterDataModel>)),
                     this,SLOT(set_Splits(QList<LedgerMasterDataModel>)));
    return;

    showSplitWidget(true);
    ui->creditLabel->hide();
    ui->creditLineEdit->hide();
}

void ModeofPayStore::set_Splits(QList<LedgerMasterDataModel> ledList)
{
    splitLeds = ledList;
    voucher->ledgersList = ledList;
    //    qDebug()<<"led list sizr "<<ledList.size();
    //    qDebug()<<"First led";
    //    qDebug()<<ledList[0].LedgerName;
    calcTotals();
    voucher->paymentSplit = true;
    voucher->ledgerObject = ledList[0];
    voucher->ledgerObject.LedgerName="Split Pay";
    QString narration = "<table width ='100%'>";
    for(int i=0;i<ledList.size();i++){
        narration +="<tr><td>";
        narration += ledList[i].LedgerName + "</td><td>" +QString::number(ledList[i].amount,'f',2) +"</td></tr>";
    }
    narration += "</table>";
    voucher->narration = narration;
    ui->narrationTextEdit->setPlainText(voucher->narration);
    modeOfPayCompleted();
}

void ModeofPayStore::on_complimentarycashToolButton_clicked()
{
    modeOfPayCompleted();
}

void ModeofPayStore::setPotafoPrice()
{
    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.price = voucher->InventoryItems[i].BaseItem.price_2;
    }
}

void ModeofPayStore::on_zomatoToolButton_clicked()
{
    voucher->ledgerObject.LedgerName ="potafo";

    // add discount;

    setPotafoPrice();
    calcTotals();
    on_discountPercentLineEdit_textChanged("12.5");

    ui->paylabel->setText("Payment Mode");
    carriageFlag=false;
    ui->zomatoWidget->show();
}

void ModeofPayStore::on_splitOKPushButton_clicked()
{
    QString creditAmount = ui->creditLineEdit->displayText();
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;

    if(creditAmount != "")
    {
        showSelectLedgerWidget(true);
    }
    else
    {
        float cashval = ui->cashLineEdit->text().toFloat();
        float cardval = ui->cashLineEdit->text().toFloat();
        if(cashval>0 && cardval >0){
            LedgerMasterDataModel cashledger = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
            LedgerMasterDataModel cardledger = LedgerHelper->getLedgerObjectByID("0x5x21x2");

        }
        else{
            return;
        }
    }
}

void ModeofPayStore::on_ledgerOKPushButton_clicked()
{

    int i = ui->ledgerComboBox->currentIndex();
    voucher->ledgerObject =BankList[i];
    modeOfPayCompleted();
}

void ModeofPayStore::showZomato(bool v)
{
    ui->zomatoToolButton->setHidden(!v);
}

void ModeofPayStore::showPax(bool v)
{
    ui->label_3->setHidden(!v);
    ui->paxSpinBox->setHidden(!v);
}

void ModeofPayStore::showComplimentary(bool v)
{
    ui->complimentarycashToolButton->setHidden(!v);
}

void ModeofPayStore::showSplitWidget(bool v)
{
    ui->splitWidget->setHidden(!v);
}

void ModeofPayStore::showSelectLedgerWidget(bool v)
{
    ui->searchLedgerWidget->setHidden(!v);
}

void ModeofPayStore::modeOfPayCompleted()
{
    voucher->numBoxes = ui->paxSpinBox->text().toInt();
    voucher->NoOfCopies = ui->noOfCopiesSpinBox->text().toInt();
    voucher->narration = ui->narrationTextEdit->toPlainText();
    this->close();
    emit modeOfPaySignal();

}

void ModeofPayStore::on_backButton_clicked()
{
    this->close();
}

void ModeofPayStore::setBillPreview()
{
    calcTotals();
    CounterPrinterHelper pHelper;
    //    qDebug()<<"setting printer";
    QString html = pHelper.getHtml(voucher);
    ui->billCopytextEdit->setHtml(html);
    //    qDebug()<<"Document Created";


}

void ModeofPayStore::on_cashPaidLineEdit_textChanged(const QString &arg1)
{
    float total = ui->grandTotalLineEdit->text().toFloat();
    float paidAmount = ui->cashPaidLineEdit->text().toFloat();
    float balance = paidAmount - total;
    ui->balanceLineEdit->setText(QString::number(balance,'f',2));
    voucher->PaidAmount = paidAmount;
    voucher->BalanceAmount = balance;

}

void ModeofPayStore::on_discountPercentLineEdit_textChanged(const QString &arg1)
{
    ui->discountAmountLineEdit->blockSignals(true);
    ui->roundOffLineEdit->blockSignals(true);

    float discountPercent = arg1.toFloat();
    voucher->discountPercent = discountPercent;

    float discountAmount = (voucher->subTotal * voucher->discountPercent)/100;
    qDebug()<<"Disocunt Amount = "<<discountAmount;
    voucher->discountinAmount = discountAmount;
    ui->discountAmountLineEdit->setText(QString::number(voucher->discountinAmount,'f',2));

    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;

    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));

    voucher->taxTotalAmount = voucher->grossTotal * 0/100;
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));

    voucher->grandTotal = voucher->grossTotal + voucher->taxTotalAmount;
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));

    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage = voucher->discountPercent;
        voucher->InventoryItems[i].BaseItem.discountinAmount =
            voucher->InventoryItems[i].BaseItem.quantity *
            voucher->InventoryItems[i].BaseItem.price *
            voucher->InventoryItems[i].BaseItem.discountPercentage/100;
    }

    ui->discountAmountLineEdit->blockSignals(false);
    ui->roundOffLineEdit->blockSignals(false);
    setBillPreview();
}

void ModeofPayStore::calcTotals()
{
   TransactionCalculator::VoucherCalculateSales(voucher);
}

float ModeofPayStore::getRoundOff(float r,int multiple){

    //    return r;
    int no = (int)r;

    float fraction = r-no;
    if(fraction <0.125){
        fraction = 0;
    }
    else if(fraction>=0.125 && fraction <0.375)
        fraction = .25;
    else if(fraction>=0.375 && fraction<0.625){
        fraction = .5;
    }
    else if(fraction>=0.625 && fraction<0.875){
        fraction = .75;
    }

    else if(fraction>=0.875){
        fraction = 1;
    }

    qDebug()<<"round amount "<<fraction+no;
    return fraction+no;
}

void ModeofPayStore::on_discountAmountLineEdit_textChanged(const QString &arg1)
{
    qDebug()<<"Discount Amount Changed";
    ui->discountPercentLineEdit->blockSignals(true);
    ui->roundOffLineEdit->blockSignals(true);

    ui->discountPercentLineEdit->clear();

    float discountAmount = arg1.toFloat();
    voucher->discountinAmount = discountAmount;

    voucher->discountPercent = discountAmount / voucher ->subTotal *100 ;
    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;

    voucher->taxTotalAmount = voucher->grossTotal * 0/100;
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));

    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));
    voucher->grandTotal = voucher->grossTotal + voucher->taxTotalAmount;
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));

    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage = voucher->discountPercent;
        voucher->InventoryItems[i].BaseItem.discountinAmount =
                voucher->InventoryItems[i].BaseItem.quantity *
                voucher->InventoryItems[i].BaseItem.price *
                voucher->InventoryItems[i].BaseItem.discountPercentage/100;
    }
    ui->discountPercentLineEdit->blockSignals(false);
    ui->roundOffLineEdit->blockSignals(false);

    setBillPreview();

}


void ModeofPayStore::on_roundOffLineEdit_textChanged(const QString &arg1)
{
    ui->discountPercentLineEdit->blockSignals(true);
    ui->discountAmountLineEdit->blockSignals(true);
    ui->cashPaidLineEdit->clear();
    ui->balanceLineEdit->clear();

    float roundOffAmount = arg1.toFloat();
    voucher->roundOff = roundOffAmount;

    voucher->grandTotal = (voucher->grossTotal + voucher->taxTotalAmount) - voucher->roundOff;
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));

    ui->discountPercentLineEdit->blockSignals(false);
    ui->discountAmountLineEdit->blockSignals(false);
    setBillPreview();
}


void ModeofPayStore::on_deliveryToolButton_toggled(bool checked)
{

}

void ModeofPayStore::on_storeToolButton_toggled(bool checked)
{

}

void ModeofPayStore::on_deliveryToolButton_clicked()
{
    return;
    ui->storeToolButton->blockSignals(true);

    voucher->ModeOfService = ServiceMode::Delivery;
    ui->storeToolButton->setIcon(QIcon(":/ions/check.ico"));
    ui->deliveryToolButton->setChecked(true);
    ui->deliveryToolButton->setIcon(QIcon(":/icons/check.ico"));


    ui->storeToolButton->blockSignals(false);
}

void ModeofPayStore::on_storeToolButton_clicked()
{
    return;
    ui->deliveryToolButton->blockSignals(true);
    //    voucher->ModeOfService = ServiceMode::Store;
    ui->storeToolButton->setIcon(QIcon(":/icons/check.ico"));
    ui->deliveryToolButton->setChecked(false);
    ui->deliveryToolButton->setIcon(QIcon(":/ions/check.ico"));

    ui->deliveryToolButton->blockSignals(false);
}

void ModeofPayStore::on_zomatoCashButton_clicked()
{
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);

    if(carriageFlag == true){
        voucher->ledgerObject.LedgerName ="Talabat";
    }
    else{
        voucher->ledgerObject.LedgerName ="Potafo";
    }
    modeOfPayCompleted();
}

void ModeofPayStore::on_zomatoCardButton_clicked()
{

    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    if(carriageFlag == true){
        QString id = LedgerHelper->getLedgerIDByName("Talabat");
        voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(id);
        voucher->ledgerObject.LedgerName ="Talabat";

    }
    else{
        voucher->ledgerObject = LedgerHelper->getLedgerObjectByID("0x5x19x17");
        voucher->ledgerObject.LedgerName ="Potafo";

    }
    calcTotals();
    modeOfPayCompleted();
}

void ModeofPayStore::hideTotalWidget()
{
    ui->totalWidget->hide();
}

void ModeofPayStore::on_amountChange4Line_returnPressed()
{
    float reqAmt = ui->amountChange4Line->text().toFloat();
    if(voucher->taxTotalAmount>0 && CounterSettingsDataModel::inclusiveVat)
        reqAmt = reqAmt *1 * 0.952381;
    float subTotal = voucher->subTotal;
    float reqDisc = subTotal - reqAmt;
    voucher->taxTotalAmount = reqAmt * .05;
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->discountAmountLineEdit->setText(QString::number(reqDisc,'f',2));
}

void ModeofPayStore::on_createLedger_clicked()
{

}

void ModeofPayStore::on_carriageToolButton_clicked()
{
    //    return;
    qDebug()<<"Carriage Clicked";
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    QString id = LedgerHelper->getLedgerIDByName("Carriage");
    qDebug()<<"Carriage id = "<<id;
    if(id.length()<1)
        return;
    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(id);
    voucher->ledgerObject.LedgerName ="Carriage";
    modeOfPayCompleted();
}

void ModeofPayStore::on_talabatToolButton_clicked()
{
    ui->paylabel->setText("Talabat Payment");
    carriageFlag = true;

    voucher->ledgerObject.LedgerName ="talabat";

    ui->zomatoWidget->show();

}

void ModeofPayStore::on_DeliverooToolButton_clicked()
{
    qDebug()<<"Deliveroo Clicked";
    LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
    QString id = LedgerHelper->getLedgerIDByName("Deliveroo");
    qDebug()<<"Deliveroo id = "<<id;
    if(id.length()<1){
        QMessageBox box; box.setText("Deliveroo Account not Added"); box.exec();
        return;
    }
    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(id);
    voucher->ledgerObject.LedgerName ="Deliveroo";
    modeOfPayCompleted();
}
