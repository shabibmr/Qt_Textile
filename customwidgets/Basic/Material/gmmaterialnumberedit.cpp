#include "gmmaterialnumberedit.h"

GMMaterialNumberEdit::GMMaterialNumberEdit(float *m, QString prefix1, QString suffix1, int dec, QtMaterialTextField *parent) :
    QtMaterialTextField (parent)
{
    QObject::connect(this,&QLineEdit::editingFinished,
                     this,&GMMaterialNumberEdit::FinishedEdit);
//    QObject::connect(this, &QLineEdit::textChanged, this, &GMMaterialNumberEdit::ChangedText);
    float temp = *m;
    value =m;
    this->prefix = prefix1;
    this->suffix = suffix1;
    this->dec = dec;
    this->setAlignment(Qt::AlignRight);

//    this->setLabelColor(QColor("#cd9d7d"));
//    this->setTextColor(QColor("#cd9d7d"));
//    this->setMaximumWidth(80);

    this->setText(prefix + QString::number(*value,'f',dec) + suffix);


}

void GMMaterialNumberEdit::FinishedEdit()
{
    QString currText = this->text();
    if(prefix.size()>0)
        currText.remove(0,prefix.size());
    if(suffix.size()>0)
        currText = currText.left(currText.lastIndexOf(suffix));
    *value = currText.toFloat();
    emit editFinished();
}

void GMMaterialNumberEdit::ChangedText(QString val)
{
    QString currText = val;
    if(prefix.size()>0 && currText.contains(prefix))
        currText.remove(0,prefix.size());
    if(suffix.size()>0 && currText.contains(suffix))
        currText = currText.left(currText.lastIndexOf(suffix));
    *value = currText.toFloat();
    emit editFinished();
}

void GMMaterialNumberEdit::setVariable(float *temp)
{
    value = temp;
    this->blockSignals(true);
    this->setText(prefix + QString::number(*value,'f',dec) + suffix);
    this->blockSignals(false);
}

//void GMMaterialNumberEdit::focusInEvent(QFocusEvent *e)
//{
//    if (e->reason() == Qt::MouseFocusReason)
//    {
//        this->setText( QString::number(*value,'f',dec) );
//    }
//}

//void GMMaterialNumberEdit::focusOutEvent(QFocusEvent *e)
//{
//    if (e->reason() == Qt::MouseFocusReason)
//    {
//        this->setText(prefix + QString::number(*value,'f',dec) + suffix);
//    }
//}
