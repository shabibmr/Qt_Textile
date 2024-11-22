#include "groupvouchers.h"
#include "ui_groupvouchers.h"

GroupVouchers::GroupVouchers(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupVouchers)
{
    ui->setupUi(this);
}

GroupVouchers::~GroupVouchers()
{
    delete ui;
}
