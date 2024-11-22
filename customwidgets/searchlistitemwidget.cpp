#include "searchlistitemwidget.h"
#include "ui_searchlistitemwidget.h"

SearchListItemWidget::SearchListItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchListItemWidget)
{
    ui->setupUi(this);

    nameFont.setPointSize(12);
}

SearchListItemWidget::~SearchListItemWidget()
{
    delete ui;
}

void SearchListItemWidget::setData(QString itemName, QString brandName, QString modelNo,QString id)
{
    this->itemID = id;
    ui->nameLabel->setFont(nameFont);
    ui->nameLabel->setStyleSheet("color : red;");
    ui->nameLabel->setText(itemName);
    ui->brandLabel->setText(brandName);
    ui->modelLabel->setText(modelNo);
}

QString SearchListItemWidget::getID()
{
    return itemID;
}

QString SearchListItemWidget::getItemName()
{
    return ui->nameLabel->text();
}
