#include "contactlistwidgetitem.h"
#include "ui_contactlistwidgetitem.h"

ContactListWidgetItem::ContactListWidgetItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactListWidgetItem)
{
    ui->setupUi(this);
    nameFont.setPointSize(12);
}

ContactListWidgetItem::~ContactListWidgetItem()
{
    delete ui;
}

void ContactListWidgetItem::setData(QString ContactName, QString CompanyName, QString phoneNumber, int id)
{
    this->itemID = id;
    ui->companyName->setFont(nameFont);
    ui->companyName->setStyleSheet("color : red;");
    ui->companyName->setText(CompanyName);
    ui->ContactName->setText(ContactName);

    ui->phone->setStyleSheet("color : blue");
    ui->phone->setText(phoneNumber);

}

int ContactListWidgetItem::getID()
{
    return itemID;
}

QString ContactListWidgetItem::getItemName()
{
    return ui->companyName->text();
}
