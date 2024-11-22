#ifndef GMMATERIALCHECKBOX_H
#define GMMATERIALCHECKBOX_H

#include "customwidgets/Material/components/qtmaterialcheckbox.h"

class GMMaterialCheckBox : public QtMaterialCheckBox
{
    Q_OBJECT
public:
    GMMaterialCheckBox();

    explicit GMMaterialCheckBox(bool *m, QtMaterialCheckBox* parent = Q_NULLPTR);
    virtual ~GMMaterialCheckBox() override {}

    bool *value;

    void setVariable(bool *temp);

public slots:
    void checked();

signals:
    void itemChecked();

};

#endif // GMMATERIALCHECKBOX_H
