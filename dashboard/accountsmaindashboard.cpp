#include "accountsmaindashboard.h"
#include "ui_accountsmaindashboard.h"

AccountsMainDashboard::AccountsMainDashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountsMainDashboard)
{
    ui->setupUi(this);

}

AccountsMainDashboard::~AccountsMainDashboard()
{
    delete ui;
}
