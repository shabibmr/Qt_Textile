#include "chequewidget.h"

ChequeWidget::ChequeWidget(GeneralVoucherDataObject *voucher, QWidget *parent) :
    QWidget(parent)
{

    dbHelper = new ChequeDetailsDatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper();

    this->setFixedWidth(700);
    this->setWindowTitle("Cheque Entry");
    this->setContentsMargins(0,0,0,0);
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:#ffffff}");

    this->voucher = voucher;


    setWidgets();
}

ChequeWidget::~ChequeWidget()
{
}


void ChequeWidget::setWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    appBar = new AppBarWidget("Cheque Entry");

    instrumentDateWidget = new GMDateEdit(&voucher->chequeEntry->InstrumentDate);
    instrumentNumberWidget = new GMMaterialLineEdit(&voucher->chequeEntry->ChequeNo);
    instrumentNumberWidget->setLabel("Cheque No.");
    payeeNameWidget = new GMMaterialLineEdit(&voucher->chequeEntry->LedgerName);
    payeeNameWidget->setLabel("Payee");
    payeeNameWidget->setDisabled(true);
    bankNameWidget = new GMMaterialLineEdit(&voucher->chequeEntry->BankName);
    bankNameWidget->setLabel("Payee Bank");
    branchWidget = new GMMaterialLineEdit(&voucher->chequeEntry->Branch);
    branchWidget->setLabel("Branch");
    amountWidget = new GMMaterialNumberEdit(&voucher->chequeEntry->Amount);
    amountWidget->setLabel("Amount");
    amountWidget->setDisabled(true);
//    paymentTypeWidget = new GMMaterialLineEdit(&voucher->chequeEntry->ins)

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QPointer<QVBoxLayout> vLayout = new QVBoxLayout(this);
    this->setLayout(vLayout);

    QPointer<QHBoxLayout> hLay = new QHBoxLayout(this);

    QPointer<QVBoxLayout> lay1 = new QVBoxLayout(this);
    QPointer<QVBoxLayout> lay2 = new QVBoxLayout(this);

    vLayout->addWidget(appBar);
    vLayout->addLayout(hLay);

    hLay->addLayout(lay1);
    hLay->addLayout(lay2);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    lay1->addWidget(new QLabel("Date: "));
    lay1->addWidget(instrumentDateWidget);
    lay1->addWidget(instrumentNumberWidget);
    lay1->addWidget(bankNameWidget);
    lay1->addWidget(branchWidget);

    lay2->addWidget(payeeNameWidget);
    lay2->addWidget(amountWidget);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(appBar->backButton, &QtMaterialIconButton::clicked, this, &ChequeWidget::closeWidget);
    connect(saveButton, &QtMaterialFloatingActionButton::clicked, this, [=](){
        emit save();
        this->close();
    });
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void ChequeWidget::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    instrumentDateWidget->setVariable(&voucher->chequeEntry->InstrumentDate);
    instrumentNumberWidget->setVariable(&voucher->chequeEntry->ChequeNo);
    bankNameWidget->setVariable(&voucher->chequeEntry->BankName);
    branchWidget->setVariable(&voucher->chequeEntry->Branch);
    payeeNameWidget->setVariable(&voucher->chequeEntry->LedgerName);
    amountWidget->setVariable(&voucher->chequeEntry->Amount);

}

void ChequeWidget::closeWidget()
{
    this->close();
    emit closing();

}
