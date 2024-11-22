#include "gmdateedit.h"
GMDateEdit::GMDateEdit()
{

}
GMDateEdit::GMDateEdit(QDate *m, QDateEdit *parent) :
    QDateEdit (parent)
{
    QObject::connect(this,&QDateEdit::dateChanged,
                     this,&GMDateEdit::FinishedEdit);
    value = m;

    this->setMinimumWidth(120);
    this->setCalendarPopup(true);
    this->setDate(*value);
    this->setDisplayFormat("dd-MM-yyyy");
    this->setFont(QFont("Calibri", 12, QFont::Bold));
    this->setObjectName("GMDateEdit");
    this->setStyleSheet("GMDateEdit{background-color:#e0f7fa;}");

}

void GMDateEdit::FinishedEdit(QDate value2)
{
    *value = value2;
    emit editFinished();
}

void GMDateEdit::setVariable(QDate *temp)
{
    value = temp;
    this->setDate(*temp);
}
