#include "gmdatetimeedit.h"

GMDateTimeEdit::GMDateTimeEdit()
{

}
GMDateTimeEdit::GMDateTimeEdit(QDateTime *m,QString displayFormat, QDateTimeEdit *parent) :
    QDateTimeEdit (parent)
{
    QObject::connect(this,&QDateTimeEdit::editingFinished,
                     this,&GMDateTimeEdit::FinishedEdit);
    value =m;
    this->setMinimumWidth(210);
    this->setCalendarPopup(true);

    this->setDateTime(*value);
    this->setDisplayFormat(displayFormat);
}

void GMDateTimeEdit::FinishedEdit()
{
    *value = this->dateTime();
    emit editFinished();
}

void GMDateTimeEdit::setVariable(QDateTime *temp)
{
    value = temp;
    this->setDateTime(*temp);
}
