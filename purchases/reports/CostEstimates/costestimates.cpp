#include "costestimates.h"
#include "ui_costestimates.h"

CostEstimates::CostEstimates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CostEstimates)
{
    ui->setupUi(this);
}

CostEstimates::~CostEstimates()
{
    delete ui;
}

void CostEstimates::on_OKButton_clicked()
{

}

void CostEstimates::on_excelButton_clicked()
{

}

void CostEstimates::on_printButton_clicked()
{

}

void CostEstimates::on_backButton_clicked()
{

}
