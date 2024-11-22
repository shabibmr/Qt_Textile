#include "getschedule.h"
#include "ui_getschedule.h"

GetSchedule::GetSchedule(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GetSchedule)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());

}

GetSchedule::~GetSchedule()
{
    delete ui;
}
