#include "helpcontents.h"
#include "ui_helpcontents.h"

HelpContents::HelpContents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpContents)
{
    ui->setupUi(this);
}

HelpContents::~HelpContents()
{
    delete ui;
}
