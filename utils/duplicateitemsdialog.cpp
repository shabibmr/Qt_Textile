#include "duplicateitemsdialog.h"
#include "ui_duplicateitemsdialog.h"

DuplicateItemsDialog::DuplicateItemsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicateItemsDialog)
{
    ui->setupUi(this);

    QLabel *label = new QLabel;
    label->setText("Duplicates found! Select the item to keep");

    ui->verticalLayout->addWidget(label);
}

DuplicateItemsDialog::~DuplicateItemsDialog()
{
    delete ui;
}

void DuplicateItemsDialog::addItemToLayout(QString itemId, QString itemName)
{
    QtMaterialFlatButton *button = new QtMaterialFlatButton(itemName);
    connect(button, &QtMaterialFlatButton::clicked, this, [=](){
       emit itemSelected(itemId) ;
        this->close();
    });

    ui->verticalLayout->addWidget(button);
}

void DuplicateItemsDialog::showDuplicateItems(QString currName, QString refName)
{
    QLabel *label = new QLabel;
    label->setText(" Duplicates found! \n  Current name   : " + currName + " \n"+
                   " Reference name : " + refName + " \n "+
                   " Keep Reference item? ");
}
