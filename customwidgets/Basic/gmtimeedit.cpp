#include "gmtimeedit.h"

GMTimeEdit::GMTimeEdit()
{

}

GMTimeEdit::GMTimeEdit(QTime *m, QTimeEdit *parent):
    QTimeEdit (parent)
{
    QObject::connect(this,&QTimeEdit::editingFinished,
                     this,&GMTimeEdit::FinishedEdit);
    value =m;
    this->setMinimumWidth(110);
    this->setFrame(false);
//    this->setCalendarPopup(true);

    this->setTime(*value);
    this->setDisplayFormat("hh:mm:ss");
//    this->setStyleSheet("GMTimeEdit{background-color:#e0f7fa;}");
}

void GMTimeEdit::FinishedEdit()
{
    *value = this->time();
    qDebug()<<"edit"<<*value<<this->time()<<value;
    emit editFinished();
}

void GMTimeEdit::setVariable(QTime *temp)
{
    value = temp;
    this->setTime(*temp);
}
