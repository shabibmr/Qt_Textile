#ifndef GMMATERIALNUMBEREDIT_H
#define GMMATERIALNUMBEREDIT_H

#include <QLineEdit>
#include <QFocusEvent>
#include <QDebug>
#include <customwidgets/Validator/myvalidator.h>
#include "customwidgets/Material/components/qtmaterialtextfield.h"

class GMMaterialNumberEdit : public QtMaterialTextField
{
    Q_OBJECT
public:
    GMMaterialNumberEdit();

    explicit GMMaterialNumberEdit(float *m, QString prefix="", QString suffix="", int decimal=2, QtMaterialTextField* parent = Q_NULLPTR);
    virtual ~GMMaterialNumberEdit() override {}
    float *value;
    int dec;
    QString prefix; QString suffix;

    void setVariable(float *temp);
//    void focusInEvent(QFocusEvent *) override;
//    void focusOutEvent(QFocusEvent *) override;
public slots:
    void FinishedEdit();
    void ChangedText(QString val);

signals:
    void editFinished();


};

#endif // GMMATERIALNUMBEREDIT_H
