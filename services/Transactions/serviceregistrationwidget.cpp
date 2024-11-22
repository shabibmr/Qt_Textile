#include "serviceregistrationwidget.h"
#include "ui_serviceregistrationwidget.h"

ServiceRegistrationWidget::ServiceRegistrationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServiceRegistrationWidget)
{
    ui->setupUi(this);
}

ServiceRegistrationWidget::~ServiceRegistrationWidget()
{
    delete ui;
}
