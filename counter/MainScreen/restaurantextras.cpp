#include "restaurantextras.h"
#include "ui_restaurantextras.h"

RestaurantExtras::RestaurantExtras(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantExtras)
{
    ui->setupUi(this);

    setMaximumSize(400,400);
}

RestaurantExtras::~RestaurantExtras()
{
    delete ui;
}

void RestaurantExtras::on_adminButton_clicked()
{
    emit adminButtonClicked();
    this->close();
}

void RestaurantExtras::on_reportsButton_clicked()
{
    emit reportsButtonClicked();
    this->close();
}

void RestaurantExtras::on_openTillButton_clicked()
{
    CounterPrinterHelper *p = new CounterPrinterHelper;
    p->openTillByCode();
    this->close();
}

void RestaurantExtras::on_screenLockButton_clicked()
{
    emit screenLockClicked();
    this->close();
}
