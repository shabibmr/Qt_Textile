#include "gmsearchlineedit.h"

GMSearchLineEdit::GMSearchLineEdit(QLineEdit *parent) : QLineEdit(parent)
{
    QObject::connect(this,SIGNAL(editingFinished()),this,SLOT(checkEdit()));
    //list=data;
    this->setText("");
    this->setPlaceholderText("Type...");
}

GMSearchLineEdit::~GMSearchLineEdit()
{

}

void GMSearchLineEdit::checkEdit()
{
    this->setText(this->text().split('\n')[0]);
    if(!list.contains(this->text(),Qt::CaseInsensitive))
        this->setText("");

}

void GMSearchLineEdit::setData(QStringList ptr)
{
    list=ptr;
}
