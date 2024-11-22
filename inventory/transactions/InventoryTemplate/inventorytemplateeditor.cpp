#include "inventorytemplateeditor.h"
#include "ui_inventorytemplateeditor.h"

InventorytemplateEditor::InventorytemplateEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InventorytemplateEditor)
{
    ui->setupUi(this);
}

InventorytemplateEditor::~InventorytemplateEditor()
{
    delete ui;
}
