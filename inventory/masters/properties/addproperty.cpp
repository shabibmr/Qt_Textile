#include "addproperty.h"
#include "ui_addproperty.h"

AddProperty::AddProperty(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddProperty)
{
    ui->setupUi(this);
    this->setWindowTitle("Create Item Property");
    dbHelper = new PropertyDatabaseHelper();
}

AddProperty::~AddProperty()
{
    delete ui;
}

void AddProperty::on_buttonBox_accepted()
{

    if(property->PropertyID == 0){
        dbHelper ->insertProperty(property);
    }
    else{
        dbHelper ->updateProperty(property);
    }

    this->close();
    emit closing();
}

void AddProperty::on_buttonBox_rejected()
{
    this->close();
}

void AddProperty::edit(PropertyDataModel* obj)
{
   delete  property;
   property = obj;
}

void AddProperty::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}
