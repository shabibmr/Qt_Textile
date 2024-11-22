#include "receiptsmaindash.h"
#include "ui_receiptsmaindash.h"

ReceiptsMainDash::ReceiptsMainDash(
        LedgerHelper *ledgerHelper,
        QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiptsMainDash)
{
    ui->setupUi(this);
    dbhelper= ledgerHelper;
}

ReceiptsMainDash::~ReceiptsMainDash()
{
    delete ui;
}
