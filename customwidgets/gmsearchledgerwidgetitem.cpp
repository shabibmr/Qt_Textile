#include "gmsearchledgerwidgetitem.h"
#include "ui_gmsearchledgerwidgetitem.h"

GMSearchLedgerWidgetItem::GMSearchLedgerWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMSearchLedgerWidgetItem)
{
    ui->setupUi(this);
}

GMSearchLedgerWidgetItem::~GMSearchLedgerWidgetItem()
{
    delete ui;
}

QString GMSearchLedgerWidgetItem::getLedgerID()
{
    return ledgerID;
}

QString GMSearchLedgerWidgetItem::getLedgerName()
{
    return ui->NameLabel->text();
}

void GMSearchLedgerWidgetItem::setData(QString LedgerID, QString LedgerName, QString Group)
{
    ledgerID = LedgerID;
    ui->NameLabel->setText(LedgerName);
  //  ui->groupLabel->setStyleSheet("color:grey");
    ui->groupLabel->setText(Group);
}
