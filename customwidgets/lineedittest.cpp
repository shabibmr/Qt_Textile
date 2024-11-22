#include "lineedittest.h"
#include "ui_lineedittest.h"

LineEditTest::LineEditTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineEditTest)
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("background-color:#ffffff");
}

LineEditTest::~LineEditTest()
{
    delete ui;
}
