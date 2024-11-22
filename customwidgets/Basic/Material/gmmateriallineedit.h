#ifndef GMMATERIALLINEEDIT_H
#define GMMATERIALLINEEDIT_H

#include <QLineEdit>
#include <customwidgets/Validator/myvalidator.h>
#include "customwidgets/Material/components/qtmaterialtextfield.h"

class GMMaterialLineEdit : public QtMaterialTextField
{
    Q_OBJECT
public:
    GMMaterialLineEdit();

    explicit GMMaterialLineEdit(QString *m,int dataType=0,int decimal=2,QtMaterialTextField* parent = Q_NULLPTR);
    virtual ~GMMaterialLineEdit() override {}
    QString *value;

    void setVariable(QString *temp);
public slots:
    void FinishedEdit();

signals:
    void editFinished();

};

#endif // GMMATERIALLINEEDIT_H
