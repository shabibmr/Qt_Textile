#include "cessreport.h"
#include "ui_cessreport.h"

CessReport::CessReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CessReport)
{
    ui->setupUi(this);
}

CessReport::~CessReport()
{
    delete ui;
}
