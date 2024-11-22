#include "serviceregisterlist.h"
#include "ui_serviceregisterlist.h"

ServiceRegisterList::ServiceRegisterList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceRegisterList)
{
    ui->setupUi(this);
}

ServiceRegisterList::~ServiceRegisterList()
{
    delete ui;
}
