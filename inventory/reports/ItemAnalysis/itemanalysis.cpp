#include "itemanalysis.h"
#include "ui_itemanalysis.h"

ItemAnalysis::ItemAnalysis(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemAnalysis)
{
    ui->setupUi(this);
}

ItemAnalysis::~ItemAnalysis()
{
    delete ui;
}

void ItemAnalysis::on_OKButton_clicked()
{

}

void ItemAnalysis::on_excelButton_clicked()
{

}

void ItemAnalysis::on_printButton_clicked()
{

}

void ItemAnalysis::on_backButton_clicked()
{

}
