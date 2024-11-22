#include "proformainvoicewidget.h"
#include "ui_proformainvoicewidget.h"

ProFormaInvoiceWidget::ProFormaInvoiceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProFormaInvoiceWidget)
{
    ui->setupUi(this);
}

ProFormaInvoiceWidget::~ProFormaInvoiceWidget()
{
    delete ui;
}
