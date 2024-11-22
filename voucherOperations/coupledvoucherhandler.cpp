#include "coupledvoucherhandler.h"
#include <QScreen>
#include <QApplication>

CoupledVoucherHandler::CoupledVoucherHandler(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(300,250);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    this->setStyleSheet("QWidget#editor{background-color: #e0f7fa }");
    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#e0f7fa"));
    setAutoFillBackground(true);
    setPalette(pal);

    initDbHelpers();
}

CoupledVoucherHandler::~CoupledVoucherHandler()
{
    delete coupledVoucher;

}

void CoupledVoucherHandler::initDbHelpers()
{
    ledHelper = new LedgerMasterDatabaseHelper();
    accHelper = new AccountGroupMasterDatabaseHelper();


}

void CoupledVoucherHandler::setParentVoucher(GeneralVoucherDataObject *voucher)
{
    parentVoucher = voucher;
//    resetWidgets();
}

void CoupledVoucherHandler::resetWidgets()
{
    advancePaid->setVariable(&parentVoucher->advanceCash);
    mainLedgerWidget->setText(modeOfPay.LedgerName);
}

void CoupledVoucherHandler::setWidgets()
{
    QFont font;
    font.setPointSize(10);

    cashPaid = new QCheckBox();
    cashPaid->setText("Cash Paid");
    cashPaid->setFont(font);
    cashPaid->setMaximumSize(120,25);


    advancePaid = new GMMaterialNumberEdit(&parentVoucher->advanceCash);
    advancePaid->setLabel("Paid Amount");
    advancePaid->setStyleSheet("GMMaterialNumberEdit{background-color:#e0f7fa;}");
    advancePaid->setMaximumWidth(150);



    QObject::connect(advancePaid,&QLineEdit::editingFinished,
                     [=](){
        parentVoucher->PaidAmount = parentVoucher->advanceCash;
    });


    QStringList modeOfPayGroups;
    modeOfPayGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    modeOfPayGroups.append(accHelper->getGroupIDByName("Bank Accounts"));

    mainLedgerWidget = new GMLedgerLineMaterial(ledHelper,ledHelper->getLedgerQueryString(modeOfPayGroups),&modeOfPay);
    mainLedgerWidget->setLabel("Mode Of Payment");
    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");
    mainLedgerWidget->setText(modeOfPay.LedgerName);
    mainLedgerWidget->setMaximumWidth(200);


    connect(cashPaid, &QCheckBox::stateChanged, this, [=] (int checked){
        if(checked == 2){
            parentVoucher->advanceCash = parentVoucher->grandTotal;
            parentVoucher->PaidAmount = parentVoucher->advanceCash;
            advancePaid->setVariable(&parentVoucher->advanceCash);
            advancePaid->setEnabled(false);
        }
        else {
            advancePaid->setEnabled(true);
        }
    });


    //    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
    //            this,[=](LedgerMasterDataModel led){
    //        modeOfPay = led;
    //    });

    //    QVBoxLayout *vLayout = new QVBoxLayout;
    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);

    verticalLayout->addWidget(mainLedgerWidget);
    verticalLayout->addWidget(cashPaid);
    verticalLayout->addWidget(advancePaid);


}

void CoupledVoucherHandler::createCoupledPaymentVoucher()
{
    //    if(modeOfPay.LedgerID.isEmpty()){
    //        QMessageBox box; box.setText("Mode Of Pay not entered. Payment Voucher not saved!"); box.exec();
    //        return;
    //    }

    qDebug()<<"save payment";

    PaymentVoucherDatabaseHelper *dbHelper = new PaymentVoucherDatabaseHelper();

    coupledVoucher = new GeneralVoucherDataObject();

    coupledVoucher->VoucherDate = QDate::currentDate();
    coupledVoucher->VoucherPrefix = LoginValues::voucherPrefix;
    coupledVoucher->voucherType =  GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);
    coupledVoucher->voucherNumber = dbHelper->getNextVoucherNo(parentVoucher->VoucherPrefix);
    coupledVoucher->status = QuotationStatus::Started;
    //    coupledVoucher->ledgerObject = ledHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
    coupledVoucher->ledgerObject = modeOfPay;
    coupledVoucher->grandTotal = parentVoucher->advanceCash;
    coupledVoucher->TransactionId = parentVoucher->TransactionId;
    coupledVoucher->SalesmanID = parentVoucher->SalesmanID;
    qDebug()<<coupledVoucher->SalesmanID<<parentVoucher->SalesmanID;

    coupledVoucher->Contact = parentVoucher->Contact;

    LedgerMasterDataModel led = parentVoucher->ledgerObject;
    led.amount = parentVoucher->advanceCash;
    led.drAmount = parentVoucher->advanceCash;

    // Billwise mapping

    BillwiseRecordDataModel* bill = new BillwiseRecordDataModel;
    bill->RefVoucherNo = parentVoucher->voucherNumber;
    bill->VoucherDate = parentVoucher->VoucherDate;
    bill->RefType = parentVoucher->voucherType;
    bill->RefPrefix = parentVoucher->VoucherPrefix;
    bill->LedgerID = parentVoucher->ledgerObject.LedgerID;
    bill->CreditAmount = 0;
    bill->DebitAmount = coupledVoucher->grandTotal;
    bill->MethodOfAdjustment = "AGAINST REFERENCE";
    bill->DueDate = parentVoucher->VoucherDate.addDays(parentVoucher->ledgerObject.Credit_Period);

    if(led.mapList.size() > 0)
        led.mapList.removeFirst();
    qDebug()<<"size : "<<led.mapList.size();
    led.mapList.append(bill);


    coupledVoucher->ledgersList.append(led);

    saveCoupledPaymentVoucher(EditorStatus::NewVoucher);
}

void CoupledVoucherHandler::createCoupledReceiptVoucher()
{
    //    if(modeOfPay.LedgerID.isEmpty()){
    //        QMessageBox box; box.setText("Mode Of Pay not entered. Receipt Voucher not saved!"); box.exec();
    //        return;
    //    }

    ReceiptVoucherDatabaseHelper *dbHelper = new ReceiptVoucherDatabaseHelper();

    coupledVoucher = new GeneralVoucherDataObject();

    coupledVoucher->VoucherDate = QDate::currentDate();
    coupledVoucher->VoucherPrefix = LoginValues::voucherPrefix;
    coupledVoucher->voucherType =  GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);
    coupledVoucher->voucherNumber = dbHelper->getNextVoucherNo(parentVoucher->VoucherPrefix);
    coupledVoucher->status = QuotationStatus::Started;
    coupledVoucher->ledgerObject = modeOfPay;
    coupledVoucher->grandTotal = parentVoucher->advanceCash;
    coupledVoucher->TransactionId = parentVoucher->TransactionId;
    coupledVoucher->SalesmanID = parentVoucher->SalesmanID;

    coupledVoucher->Contact = parentVoucher->Contact;

    LedgerMasterDataModel led = parentVoucher->ledgerObject;
    led.amount = parentVoucher->advanceCash;
    led.crAmount = parentVoucher->advanceCash;

    // Billwise mapping

    BillwiseRecordDataModel* bill = new BillwiseRecordDataModel;
    bill->RefVoucherNo = parentVoucher->voucherNumber;
    bill->VoucherDate = parentVoucher->VoucherDate;
    bill->RefType = parentVoucher->voucherType;
    bill->RefPrefix = parentVoucher->VoucherPrefix;
    bill->LedgerID = parentVoucher->ledgerObject.LedgerID;
    bill->CreditAmount = coupledVoucher->grandTotal;
    bill->DebitAmount = 0;
    bill->MethodOfAdjustment = "AGAINST REFERENCE";
    bill->DueDate = parentVoucher->VoucherDate.addDays(parentVoucher->ledgerObject.Credit_Period);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(led.mapList.size() > 0)
        led.mapList.removeFirst();
    qDebug()<<"size : "<<led.mapList.size();
    led.mapList.append(bill);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    coupledVoucher->ledgersList.append(led);
    saveCoupledReceiptVoucher(EditorStatus::NewVoucher);

}

bool CoupledVoucherHandler::retrieveCoupledReceiptVoucher()
{
    ReceiptVoucherDatabaseHelper *dbHelper = new ReceiptVoucherDatabaseHelper();

    QPair<QString, QString> pair;
    pair = dbHelper->getVoucherNoByTransactionId(parentVoucher->TransactionId);
    if(pair.first != "-1"){
        coupledVoucher=dbHelper->getVoucherByVoucherNoPtr(pair.first, pair.second);
        parentVoucher->advanceCash = coupledVoucher->ledgersList[0].amount;
        parentVoucher->PaidAmount = parentVoucher->advanceCash;
        parentVoucher->isCoupled = true;
        modeOfPay = coupledVoucher->ledgerObject;
        resetWidgets();

        return true;
    }
    else {
        return false;
    }
}

void CoupledVoucherHandler::updateCoupledReceiptVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(coupledVoucher == nullptr){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        createCoupledReceiptVoucher();
    }
    else {
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        coupledVoucher->ledgersList[0].amount = parentVoucher->advanceCash;
        coupledVoucher->ledgersList[0].crAmount = parentVoucher->advanceCash;
        coupledVoucher->Contact = parentVoucher->Contact;
        coupledVoucher->ledgerObject = modeOfPay;
        coupledVoucher->SalesmanID = parentVoucher->SalesmanID;
        saveCoupledReceiptVoucher(EditorStatus::EditVoucher);
    }

}

void CoupledVoucherHandler::saveCoupledReceiptVoucher(int editorStatus)
{

    qDebug()<<Q_FUNC_INFO<<__LINE__<<editorStatus;

    QPointer<ReceiptVoucherEditor> editor = new ReceiptVoucherEditor(new ReceiptVoucherDatabaseHelper);
    TransactionCalculator::VoucherCalculateLedgersTotal(coupledVoucher);
    editor->setVoucher(coupledVoucher);
    editor->setEditorStatus(editorStatus);
    editor->saveVoucher();

}

bool CoupledVoucherHandler::retrieveCoupledPaymentVoucher()
{
    PaymentVoucherDatabaseHelper *dbHelper = new PaymentVoucherDatabaseHelper();

    QPair<QString, QString> pair;
    pair = dbHelper->getVoucherNoByTransactionId(parentVoucher->TransactionId);
    if(pair.first != "-1"){
        coupledVoucher = dbHelper->getVoucherByVoucherNoPtr(pair.first, pair.second);
        parentVoucher->advanceCash = -1 * coupledVoucher->ledgersList[0].amount;
        parentVoucher->PaidAmount = parentVoucher->advanceCash;
        parentVoucher->isCoupled = true;
        modeOfPay = coupledVoucher->ledgerObject;
        resetWidgets();
        return true;
    }
    else
        return false;

}

void CoupledVoucherHandler::updateCoupledPaymentVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<coupledVoucher;
    if(coupledVoucher == nullptr){
        createCoupledPaymentVoucher();
    }
    else {
        coupledVoucher->ledgerObject = modeOfPay;
        coupledVoucher->ledgersList[0].amount = parentVoucher->advanceCash;
        coupledVoucher->ledgersList[0].drAmount = parentVoucher->advanceCash;
        coupledVoucher->Contact = parentVoucher->Contact;
        coupledVoucher->SalesmanID = parentVoucher->SalesmanID;
        saveCoupledPaymentVoucher(EditorStatus::EditVoucher);
    }

}

void CoupledVoucherHandler::saveCoupledPaymentVoucher(int editorStatus)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    PaymentVoucherEditor *editor = new PaymentVoucherEditor(new PaymentVoucherDatabaseHelper);
    TransactionCalculator::VoucherCalculateLedgersTotal(coupledVoucher);
    editor->setVoucher(coupledVoucher);
    editor->setEditorStatus(editorStatus);
//    editor->showVoucher();
    editor->saveVoucher();
}

bool CoupledVoucherHandler::checkModeOfPayEntered()
{

    if(modeOfPay.LedgerID.isEmpty())
        return false;
    else return true;
}

void CoupledVoucherHandler::saveCoupledVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesOrder))
        updateCoupledReceiptVoucher();
    else if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseOrder))
        updateCoupledPaymentVoucher();
}

bool CoupledVoucherHandler::retrieveCoupledVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesOrder)){

        return retrieveCoupledReceiptVoucher();
    }
    else if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseOrder))
        return retrieveCoupledPaymentVoucher();
    else
        return false;
}

void CoupledVoucherHandler::createCoupledVoucher()
{
    if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::SalesOrder)){

         createCoupledReceiptVoucher();
    }
    else if(GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseVoucher)
            || GMVoucherTypes::compareVoucherType(parentVoucher->voucherType, GMVoucherTypes::PurchaseOrder))
         createCoupledPaymentVoucher();

}
