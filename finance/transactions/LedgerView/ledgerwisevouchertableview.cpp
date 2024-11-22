#include "ledgerwisevouchertableview.h"
#include "ui_ledgerwisevouchertableview.h"

LedgerwiseVoucherTableView::LedgerwiseVoucherTableView(GeneralVoucherDataObject *v, LedgerMasterDatabaseHelper *ledHelper, QWidget *parent)
    :ui(new Ui::LedgerwiseVoucherTableView)
{
    ui->setupUi(this);
    this->setWindowTitle("Ledgers");
    this->voucher = v;
    this->ledgerHelper = ledHelper;
    initGlobalObjects();
}

LedgerwiseVoucherTableView::~LedgerwiseVoucherTableView()
{
    delete ui;
}

void LedgerwiseVoucherTableView::initGlobalObjects()
{
    lis<<"#"<<"Ledgers"<<"Dr"<<"Cr";

    DataTableWidget = new LedgerwiseTableView(voucher,3);
    DataTableWidget->setItemDelegate(new GMLedgerTransactionDelegate(
                                         ledgerHelper,ledgerHelper->getAllLedgersModel()));


    model = new GMLedgerItemModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);

    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    ui->verticalLayout->addWidget(DataTableWidget);
    connect(model,&GMLedgerItemModel::updateValues,[=]{
        emit ledUpdated();
    });
}

void LedgerwiseVoucherTableView::updateData()
{
//    qDebug()<<"Led count : "<<voucher->ledgersList.size();
    model->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void LedgerwiseVoucherTableView::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
    resetWidgets();
}

void LedgerwiseVoucherTableView::resetWidgets()
{
    model = new GMLedgerItemModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);

    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    ui->verticalLayout->addWidget(DataTableWidget);
    connect(model,&GMLedgerItemModel::updateValues,[=]{
        emit ledUpdated();
    });
}


void LedgerwiseVoucherTableView::closeEvent(QCloseEvent *event)
{
    this->hide();
}
