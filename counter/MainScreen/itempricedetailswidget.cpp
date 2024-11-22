#include "itempricedetailswidget.h"
#include "ui_itempricedetailswidget.h"

ItemPriceDetailsWidget::ItemPriceDetailsWidget(CompoundItemDataObject *Item,
                                               SalesInventoryItemDatabaseHelper *itemHelper,
                                               QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemPriceDetailsWidget)
{
    ui->setupUi(this);
    this->item = Item;
    this->itemHelper = itemHelper;

    setWidgets();
}

ItemPriceDetailsWidget::~ItemPriceDetailsWidget()
{
    delete ui;
}

void ItemPriceDetailsWidget::setWidgets()
{
    stdCost = itemHelper->getCostByItem(item->BaseItem.ItemID);
    lpp = itemHelper->getLastPurchPrice(item->BaseItem.ItemID, QDate::currentDate());
    lsp = itemHelper->getLastSalesPrice(item->BaseItem.ItemID, QDate::currentDate());

    stdCostEdit = new GMMaterialNumberEdit(&stdCost);
    stdCostEdit->setLabel("Std Cost");
    lppEdit = new GMMaterialNumberEdit(&lpp);
    lppEdit->setLabel("Last Purchase Price");
    lspEdit = new GMMaterialNumberEdit(&lsp);
    lspEdit->setLabel("Last Sales Price");

    ui->horizontalLayout->addWidget(stdCostEdit);
    ui->horizontalLayout->addWidget(lppEdit);
    ui->horizontalLayout->addWidget(lspEdit);
}

void ItemPriceDetailsWidget::setValues()
{
    stdCost *= item->BaseItem.uomObject.convRate;
    lpp *= item->BaseItem.uomObject.convRate;
    lsp *= item->BaseItem.uomObject.convRate;

    stdCostEdit->setVariable(&stdCost);
    lppEdit->setVariable(&lpp);
    lspEdit->setVariable(&lsp);
}
