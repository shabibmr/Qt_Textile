#include "documentslist.h"
#include "ui_documentslist.h"

DocumentsList::DocumentsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DocumentsList)
{
    ui->setupUi(this);
}

DocumentsList::~DocumentsList()
{
    delete ui;
}

void DocumentsList::on_okButton_clicked()
{

}
