#include "hashtaglinewidget.h"
#include "ui_hashtaglinewidget.h"

HashTagLineWidget::HashTagLineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HashTagLineWidget)
{
    ui->setupUi(this);
}

HashTagLineWidget::~HashTagLineWidget()
{
    delete ui;
}
