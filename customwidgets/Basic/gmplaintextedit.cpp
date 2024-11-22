#include "gmplaintextedit.h"

GMPlainTextEdit::GMPlainTextEdit()
{

}
GMPlainTextEdit::GMPlainTextEdit(QString *m, QPlainTextEdit *parent) :
    QPlainTextEdit (parent)
{
    QObject::connect(this,&QPlainTextEdit::textChanged,
                     this,&GMPlainTextEdit::FinishedEdit);
    value =m;
    this->setMaximumHeight(50);
    this->setPlainText(*value);

}

void GMPlainTextEdit::FinishedEdit()
{
    *value = this->toPlainText();
    emit editFinished();
}


void GMPlainTextEdit::setVariable(QString *temp)
{
    value = temp;
    this->setPlainText(*temp);
}
