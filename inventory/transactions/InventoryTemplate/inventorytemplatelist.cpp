#include "inventorytemplatelist.h"
#include "ui_inventorytemplatelist.h"

InventoryTemplateList::InventoryTemplateList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventoryTemplateList)
{
    ui->setupUi(this);
}

InventoryTemplateList::~InventoryTemplateList()
{
    delete ui;
}
