#include "modeofpay.h"
#include "selectitemswidget.h"
#include "ui_modeofpay.h"

#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QComboBox>
#include "customwidgets/Validator/myvalidator.h"
#include <QMessageBox>
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/Misc/loading.h"
#include "finance/transactions/LedgerView/ledgerwisevouchertableview.h"


ModeOfPay::ModeOfPay(GeneralVoucherDataObject *v,LedgerMasterDatabaseHelper *LHelper,SalesOrderDatabaseHelper *orderHelper ,
                     bool update,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeOfPay)
{
    ui->setupUi(this);
    this->setWindowTitle("Mode of Pay");

    this->setObjectName("Modeofpay");
    this->LedgerHelper = LHelper;
    this->voucher = v;
    this->orderHelper = orderHelper;
    this->setStyleSheet("QWidget#Modeofpay{background-color:#e0f7fa}");

    showSelectLedgerWidget(false);
    showSplitWidget(false);
    ui->modeOfServiceWidget->hide();

    ui->roundOffLabel->hide();
    ui->roundOffLineEdit->hide();

    ui->label_6->hide();
    ui->amountChange4Line->hide();
    //    ui->splitToolButton->hide();
    bool hidCompliment =  ConfigurationSettingsDatabaseHelper::getValue(hideComplimentaryButton,true).toBool();
    bool hidLoyality =  ConfigurationSettingsDatabaseHelper::getValue(hideLoyalityButton,true).toBool();

    if(hidCompliment)
        ui->complimentarycashToolButton->hide();
    if(hidLoyality)
        ui->loyalityToolButton->hide();
    //        hideDiscounts();

    ui->zomatoWidget->hide();

    //ui->talabatToolButton->hide();
    //    ui->zomatoToolButton->hide();

    hideLedgerwiseFlag =  ConfigurationSettingsDatabaseHelper::getValue(allowEmptyAddress,true).toBool();

    restrictDiscounts();


    // mask ui->discountPercentLineEdit to accept only "5" , "10" or "15";


    ui->cashToolButton->hide();
    ui->cardToolButton->hide();

    if(hideLedgerwiseFlag)
        ui->ledgersButton->hide();

    // ui->deliveryToolButton->setCheckable(true);
    // ui->storeToolButton->setCheckable(true);
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

    font.setPointSize(11);
    setFavButtons();

    mainLedgerWidget = new GMLedgerLineMaterial(LedgerHelper,LedgerHelper->getLedgerQueryModel(),&voucher->ledgerObject);

    mainLedgerWidget->setLabel("Customer Name");
    mainLedgerWidget->setParent(this);
    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");
    mainLedgerWidget->setFixedWidth(200);
    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
            this,[=](LedgerMasterDataModel led){
                if(led.LedgerID.length()<1)
                    return;

                setBillPreview();
                qDebug()<<"76 > Mode Pay led name : "<<led.LedgerName;


            });
    QFont font("Arial",12);
    delivery = new GMDateTimeEdit(&voucher->DeliveryDate,"dd-MM hh:mm ap");
    delivery->setFont(font);
    font.setBold(true);
    QLabel *lbl = new QLabel("Pick Up ",this);
    lbl->setFont(font);

    advancePaidAmount = new QLineEdit(this);
    advancePaidAmount->setPlaceholderText("Enter Advance Paid");
    advancePaidAmount->setValidator(new MyValidator(0,9999,3,this));
    advancePaidAmount->setFixedWidth(200);
    advancePaidAmount->setFixedHeight(30);

    setBillPreview();


    ui->formLayout_2->addRow(lbl,delivery);
    ui->searchLay->addWidget(mainLedgerWidget);
    ui->searchLay->addWidget(advancePaidAmount);

    resetWidgets();
    installEventFilter(this);
    qDebug()<<"106";
    //ServiceMode mode;
    // qDebug()<<QString::number(ServiceModeValue::Delivery);
}

ModeOfPay::~ModeOfPay()
{
    delete ui;
}

void ModeOfPay::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
    resetWidgets();

}

void ModeOfPay::resetWidgets()
{
    delivery->setVariable(&voucher->DeliveryDate);
    ui->discountPercentLineEdit->blockSignals(true);
    ui->discountAmountLineEdit->blockSignals(true);
    ui->discountPercentLineEdit->setText(QString::number(voucher->discountPercent,'f',2));
    ui->discountAmountLineEdit->setText(QString::number(voucher->discountinAmount,'f',2));
    ui->subTotalLineEdit->setText(QString::number(voucher->subTotal,'f',2));
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));
    ui->narrationTextEdit->setPlainText(voucher->narration);
    ui->paxSpinBox->setValue(voucher->numBoxes);
    ui->discountAmountLineEdit->setValidator(new MyValidator(0,voucher->grandTotal,2,this));
    ui->discountPercentLineEdit->setValidator(new MyValidator(0,100,2,this));
    ui->discountPercentLineEdit->blockSignals(false);
    ui->discountAmountLineEdit->blockSignals(false);

    mainLedgerWidget->setItem(&voucher->ledgerObject);

    setBillPreview();

}

void ModeOfPay::hideDiscounts()
{
    ui->discountAmountLineEdit->hide();
    ui->discountAmountLabel->hide();
    ui->discountPercentLabel->hide();
    ui->discountPercentLineEdit->hide();
}

void ModeOfPay::on_cashToolButton_clicked()
{

    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);


    modeOfPayCompleted();
}

void ModeOfPay::on_cardToolButton_clicked()
{

    BankList = LedgerHelper->getAllLedgersUnderGroupByName("BANK ACCOUNTS");
    //    ui->ledgerComboBox->clear();
    if(BankList.size()>1){

        for(int i=0;i<BankList.size();i++){
            //            ui->ledgerComboBox->addItem(BankList[i].LedgerName);
        }
        voucher->ledgerObject = BankList[0];
        showSelectLedgerWidget(true);

    }
    else if(BankList.size()==1){
        voucher->ledgerObject = BankList[0];
        voucher->BillingName = BankList[0].LedgerName;
        modeOfPayCompleted();
        return;
    }
    else{
        QMessageBox Box;
        Box.setText("No Bank Account(s) Created");
        Box.exec();
    }


}

void ModeOfPay::on_creditToolButton_clicked()
{
    //    ui->ledgerComboBox->clear();
    BankList = LedgerHelper->getAllLedgersUnderGroupByName("SUNDRY DEBTORS");
    if(BankList.size()>0){
        showSelectLedgerWidget(true);
        //        for(int i=0;i<BankList.size();i++){
        //            ui->ledgerComboBox->addItem(BankList[i].LedgerName);
        //        }
        //        voucher->ledgerObject = BankList[0];
        //        voucher->BillingName = BankList[0].LedgerName;
        //        showSelectLedgerWidget(true);
    }
    // else if(BankList.size()==1){
    //     voucher->ledgerObject = BankList[0];
    //     BillwiseRecordDataModel *bill = new BillwiseRecordDataModel();
    //     bill->Amount = voucher->grandTotal;
    //     bill->VoucherDate = voucher->VoucherDate;
    //     bill->VoucherPrefix = voucher->VoucherPrefix;
    //     bill->VoucherNo = voucher->voucherNumber;
    //     bill->DebitAmount = voucher->grandTotal;
    //     bill->VoucherType = voucher->voucherType;
    //     bill->LedgerID = voucher->ledgerObject.LedgerID;
    //     voucher->ledgerObject.mapList.append(bill);
    //     modeOfPayCompleted();
    // }
    else{
        QMessageBox Box;
        Box.setText("No-Debtors Created");
        Box.exec();
    }
    //    ui->ledgerComboBox->setCurrentText(voucher->ledgerObject.LedgerName);

}

void ModeOfPay::on_splitToolButton_clicked()
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

}

void ModeOfPay::set_Splits(QList<LedgerMasterDataModel> ledList)
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

void ModeOfPay::restrictDiscounts()
{

    QString restrictDiscount =  ConfigurationSettingsDatabaseHelper::getValue(allowedDiscounts,"-1").toString();

    if(restrictDiscount=="-1"){
        return;
    }
    else {
        QComboBox *dbox = new QComboBox(this);

        dbox->setMinimumHeight(30);
        //        dbox->lineEdit()->setFont(QFont("Arial",12,1));

        QList<QString> discountList = restrictDiscount.split(",");

        foreach (QString v, discountList) {

            dbox->addItem(v,v.toFloat());

        }
        ui->discountPercentLabel->hide();
        ui->discountPercentLineEdit->hide();
        QLabel *lbl = new QLabel("Discount %",this);
        QFont fnnt; fnnt.setPointSize(12); fnnt.setBold(true);
        lbl->setFont(fnnt);
        dbox->setFont(fnnt);
        ui->formLayout_3->insertRow(1,lbl,dbox);
        connect(dbox,&QComboBox::currentTextChanged,this,[=](QString index){
            ui->discountPercentLineEdit->setText(QString::number(dbox->currentData().toFloat(),'f',2));
        });

        ui->discountAmountLineEdit->setReadOnly(true);
        //        ui->discountPercentLineEdit->setReadOnly(true);
    }


}

void ModeOfPay::on_complimentarycashToolButton_clicked()
{
    ui->discountPercentLineEdit->setText("100.00");
    ui->narrationTextEdit->setPlainText("Complimentary");
    modeOfPayCompleted();
}


void ModeOfPay::on_loyalityToolButton_clicked()
{
    ui->discountPercentLineEdit->setText("100.00");
    ui->narrationTextEdit->setPlainText("Loyality");
    modeOfPayCompleted();
}

void ModeOfPay::LedgerSelectedFromFav()
{
    sender()->blockSignals(true);
    QLabel w;

    w.setMinimumSize(400, 400);
    Loading *loading = new Loading(&w);
    QTimer::singleShot(4000, loading, SLOT(hide()));
    w.show();



    QString id = sender()->property("id").toString();
    QString name = sender()->property("name").toString();
    float disc = sender()->property("discount").toFloat();
    int priceid = sender()->property("PriceId").toInt();
    float discLimit = sender()->property("discLimit").toFloat();

    if(priceid>0)
        voucher->ModeOfService = priceid;

    qDebug()<<"New Price : "<<voucher->priceListId;
    voucher->ledgerObject= LedgerHelper->getLedgerObjectByID( id);
    if(priceid>0 && !updateMode){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"Default Price List Found","Do you Update Price",
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            emit(resetPrice());
        }
        else{

        }
    }
    //    if(voucher->discountPercent == 0 ){

    if(disc > 0){
        if(discLimit > 0){
            float disc1 = discLimit * 100 / voucher->subTotal;
            if(disc1 < disc)
                voucher->discountPercent = disc1;
            else {
                voucher->discountPercent = disc;
            }
            qDebug()<<Q_FUNC_INFO<<__LINE__<<disc<<discLimit<<disc1<<voucher->subTotal<<voucher->discountPercent;
        }
        else
            voucher->discountPercent = disc;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<disc<<discLimit<<voucher->subTotal<<voucher->discountPercent;
    }
    //    }
    ui->discountPercentLineEdit->setText(QString::number(voucher->discountPercent));

    modeOfPayCompleted();


}

void ModeOfPay::setPotafoPrice()
{
    return;
    //    for(int i=0;i<voucher->InventoryItems.size();i++){
    //        voucher->InventoryItems[i].BaseItem.price = voucher->InventoryItems[i].BaseItem.price_2;
    //    }
}

void ModeOfPay::on_zomatoToolButton_clicked()
{

    QString id = LedgerHelper->getLedgerIDByName("Zomato");
    if(id.length()<1){
        QMessageBox box; box.setText("No Such Ledger Exist"); box.exec();
        return;
    }
    voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(id);
    voucher->ledgerObject.LedgerName ="Zomato";
    setPotafoPrice();
    calcTotals();
    modeOfPayCompleted();
}

void ModeOfPay::on_splitOKPushButton_clicked()
{
    QString creditAmount = ui->creditLineEdit->displayText();


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

void ModeOfPay::on_ledgerOKPushButton_clicked()
{
    qDebug()<<"Ok pressed Led Name "<<voucher->ledgerObject.LedgerName;
    PricelistDatabaseHelper priceHelper;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->ledgerObject.defaultPriceListID.length()>0)
            voucher->InventoryItems[i].BaseItem.price = priceHelper.getPriceByItemID(voucher->InventoryItems[i].BaseItem.ItemID,voucher->ledgerObject.defaultPriceListID);
    }

    if(voucher->ledgerObject.discountPercent > 0){
        if(voucher->ledgerObject.Credit_Limit > 0){
            float disc1 = voucher->ledgerObject.Credit_Limit * 100 / voucher->subTotal;
            if(disc1 < voucher->ledgerObject.discountPercent)
                voucher->discountPercent = disc1;
            else {
                voucher->discountPercent = voucher->ledgerObject.discountPercent;
            }
        }
        else
            voucher->discountPercent = voucher->ledgerObject.discountPercent;
    }
    ui->discountPercentLineEdit->setText(QString::number(voucher->discountPercent));

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->subTotal<<voucher->ledgerObject.discountPercent<<voucher->discountPercent<<voucher->ledgerObject.Credit_Limit;
    //    }

    modeOfPayCompleted();
}

void ModeOfPay::showZomato(bool v)
{
    //    ui->zomatoToolButton->setHidden(!v);
}

void ModeOfPay::showPax(bool v)
{
    ui->label_3->setHidden(!v);
    ui->paxSpinBox->setHidden(!v);
}

void ModeOfPay::showComplimentary(bool v)
{
    ui->complimentarycashToolButton->setHidden(!v);
}

void ModeOfPay::showSplitWidget(bool v)
{
    ui->splitWidget->setHidden(!v);
}

void ModeOfPay::showSelectLedgerWidget(bool v)
{
    ui->searchLedgerWidget->setHidden(!v);
}

void ModeOfPay::modeOfPayCompleted()
{
    voucher->numBoxes = ui->paxSpinBox->text().toInt();
    voucher->NoOfCopies = ui->noOfCopiesSpinBox->text().toInt();
    voucher->narration = ui->narrationTextEdit->toPlainText();
    emit modeOfPaySignal();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->discountPercent<<voucher->discountinAmount;
    this->close();

}

void ModeOfPay::on_backButton_clicked()
{
    this->close();
}

void ModeOfPay::setBillPreview()
{
    calcTotals();
    CounterPrinterHelper pHelper;
    //    qDebug()<<"setting printer";
    QString html = pHelper.getHtml(voucher);
    ui->billCopytextEdit->setHtml(html);
    //    qDebug()<<"Document Created";


}

void ModeOfPay::on_cashPaidLineEdit_textChanged(const QString &arg1)
{
    float total = voucher->grandTotal;
    float paidAmount = ui->cashPaidLineEdit->text().toFloat();
    float balance = paidAmount - total;
    ui->balanceLineEdit->setText(QString::number(balance,'f',2));
    voucher->PaidAmount = paidAmount;
    voucher->BalanceAmount = balance;
}

void ModeOfPay::on_discountPercentLineEdit_textChanged(const QString &arg1)
{
    ui->discountAmountLineEdit->blockSignals(true);
    ui->roundOffLineEdit->blockSignals(true);

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    float discountPercent = arg1.toFloat();
    voucher->discountPercent = discountPercent;

    float discountAmount = (voucher->subTotal * voucher->discountPercent)/100;
    qDebug()<<"Disocunt Amount = "<<discountAmount;
    //    voucher->discountinAmount = discountAmount;
    //    ui->discountAmountLineEdit->setText(QString::number(voucher->discountinAmount,'f',2));

    //    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;

    //    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));

    //    voucher->taxTotalAmount = voucher->grossTotal * 0/100;
    //    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));


    //    voucher->grandTotal = voucher->grossTotal + voucher->taxTotalAmount;
    //    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));


    //    voucher->BalanceAmount  = voucher->PaidAmount - voucher->grandTotal;
    //    ui->balanceLineEdit->setText(QString::number(voucher->BalanceAmount,'f',2));


    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage = voucher->discountPercent;
        voucher->InventoryItems[i].BaseItem.discountinAmount =
            voucher->InventoryItems[i].BaseItem.quantity *
            voucher->InventoryItems[i].BaseItem.price *
            voucher->InventoryItems[i].BaseItem.discountPercentage/100;
    }

    TransactionCalculator::VoucherCalculateSales(voucher);
    ui->discountAmountLineEdit->setText(QString::number(voucher->discountinAmount,'f',2));
    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));
    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));
    ui->balanceLineEdit->setText(QString::number(voucher->BalanceAmount,'f',2));

    ui->discountAmountLineEdit->blockSignals(false);
    ui->roundOffLineEdit->blockSignals(false);
    setBillPreview();
}

void ModeOfPay::calcTotals()
{
    TransactionCalculator::VoucherCalculateSales(voucher);

}

float ModeOfPay::getRoundOff(float r,int multiple){

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

void ModeOfPay::on_discountAmountLineEdit_textChanged(const QString &arg1)
{
    qDebug()<<"Discount Amount Changed";
    ui->discountPercentLineEdit->blockSignals(true);
    ui->roundOffLineEdit->blockSignals(true);

    //    ui->discountPercentLineEdit->clear();
    voucher->discountinAmount = arg1.toFloat();

    voucher->discountPercent = voucher->discountinAmount / voucher ->subTotal *100 ;

    //    voucher->grossTotal = voucher->subTotal - voucher->discountinAmount;

    //    voucher->taxTotalAmount = voucher->grossTotal * 0/100;

    //    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));

    //    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));
    //    voucher->grandTotal = voucher->grossTotal + voucher->taxTotalAmount;
    //    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));

    //    voucher->BalanceAmount  = voucher->PaidAmount - voucher->grandTotal;
    //    ui->balanceLineEdit->setText(QString::number(voucher->BalanceAmount,'f',2));

    for(int i=0;i<voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.discountPercentage = voucher->discountPercent;
        voucher->InventoryItems[i].BaseItem.discountinAmount =
            voucher->InventoryItems[i].BaseItem.quantity *
            voucher->InventoryItems[i].BaseItem.price *
            voucher->InventoryItems[i].BaseItem.discountPercentage/100;
    }

    TransactionCalculator::VoucherCalculateSales(voucher);
    ui->discountPercentLineEdit->setText(QString::number(voucher->discountPercent,'f',2));

    ui->discountPercentLineEdit->blockSignals(false);
    ui->roundOffLineEdit->blockSignals(false);

    ui->vATLineEdit->setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->subTotalLineEdit->setText(QString::number(voucher->grossTotal,'f',2));
    ui->grandTotalLineEdit->setText(QString::number(voucher->grandTotal,'f',2));
    ui->balanceLineEdit->setText(QString::number(voucher->BalanceAmount,'f',2));

    setBillPreview();

}


void ModeOfPay::on_roundOffLineEdit_textChanged(const QString &arg1)
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


void ModeOfPay::hideTotalWidget()
{
    ui->totalWidget->hide();
}

void ModeOfPay::on_amountChange4Line_returnPressed()
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

void ModeOfPay::on_createLedger_clicked()
{

}

void ModeOfPay::setFavButtons()
{
    QSqlQueryModel *model = LedgerHelper->getLedForModeofPay();
    qDebug()<<"Led fav count : "<<model->rowCount();
    for(int i=0;i<model->rowCount();i++){
        QToolButton* button = new QToolButton(ui->widget);
        button->setMinimumWidth(120);
        button->setMaximumWidth(180);
        button->setMinimumHeight(50);
        button->setText(model->record(i).value(1).toString());
        //        button->setAutoRaise(true);
        button->setProperty("id",model->record(i).value(0).toString());
        button->setProperty("name",model->record(i).value(1).toString());
        button->setProperty("discount",model->record(i).value(2).toFloat());
        button->setProperty("PriceId",model->record(i).value(3).toInt());
        button->setProperty("discLimit", model->record(i).value(4).toFloat());

        button->setFont(font);
        button->setText(model->record(i).value(1).toString());
        ui->favLay->addWidget(button);
        //        ui->horizontalLayout->addWidget(button);
        QObject::connect(button,&QToolButton::clicked,this,
                         &ModeOfPay::LedgerSelectedFromFav);
    }
}

void ModeOfPay::keyPressEvent(QKeyEvent *event)
{
    switch (event->key ()) {
    case Qt::Key_F1:{
        on_cashToolButton_clicked();
        ;
        break;}
    case Qt::Key_F2:{
        on_cashToolButton_clicked();
        break;}
    case Qt::Key_Escape:{
        on_backButton_clicked();
        break;
    }
    case Qt::Key_F9 :{
        openItemsSplit();
        break;
    }
    case Qt::Key_Return:
    case Qt::Key_Enter:{

        break;
    }
    default:break;
    }
}

void ModeOfPay::on_ledgersButton_clicked()
{
    //    QWidget *widget = new QWidget(this);

    LedgerwiseVoucherTableView *table = new LedgerwiseVoucherTableView(voucher,LedgerHelper,this);
    //    QVBoxLayout *lay = new QVBoxLayout(widget);
    //    lay->addWidget(table);
    //    widget->setLayout(lay);
    table->setWindowFlags(Qt::Popup);
    int swidth = qApp->primaryScreen()->geometry().width()* 0.7;
    int  sheight = qApp->primaryScreen()->geometry().height()* 0.7;
    table->resize(swidth,sheight);
    table->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, table->size(),
                                           qApp->primaryScreen()->geometry()));

    connect(table,&LedgerwiseVoucherTableView::ledUpdated,[=](){
        //        qDebug()<<"Update model at voucher";
        setBillPreview();
    });

    table->show();
}


void ModeOfPay::openItemsSplit()
{
    SelectItemsWidget *widget = new SelectItemsWidget(voucher,this);
    widget->setVoucher(voucher);
    widget->setWindowFlag(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();

    connect(widget,&SelectItemsWidget::selectedRows,this,[=](QList<int> rows){

        ui->splitItemsButton->hide();
        /*
         * 1. move lines at each rows to a new inv list.
         * */
        QList<CompoundItemDataObject> sItems;
        QList<CompoundItemDataObject> oItems;
        for(int i=0;i<voucher->InventoryItems.length();i++){
            if(rows.contains(i)){
                sItems.append(voucher->InventoryItems[i]);
            }
            else{
                oItems.append(voucher->InventoryItems[i]);
            }
        }

        voucher->InventoryItems = oItems;
        voucher->reference = voucher->reference+"_1";
        TransactionCalculator::VoucherCalculateSales(voucher);
        orderHelper->updateVoucher(voucher,voucher->voucherNumber);

        voucher->InventoryItems = sItems;
        voucher->voucherNumber = orderHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->reference = voucher->reference+"_2";
        voucher->TransactionId = DatabaseHelper::getUUID();

        TransactionCalculator::VoucherCalculateSales(voucher);
        orderHelper->insertVoucher(voucher);
        resetWidgets();
        resetTable();

    });

}


void ModeOfPay::on_splitItemsButton_clicked()
{
    openItemsSplit();
}

