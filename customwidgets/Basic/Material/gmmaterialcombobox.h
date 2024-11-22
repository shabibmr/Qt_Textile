#ifndef GMMATERIALCOMBOBOX_H
#define GMMATERIALCOMBOBOX_H

#include <QComboBox>
#include <QDebug>

#include "customwidgets/Material/components/qtmaterialautocomplete.h"

class GMMaterialComboBox : public QtMaterialAutoComplete
{
    Q_OBJECT

public:
    GMMaterialComboBox();

    explicit GMMaterialComboBox(QString *m, QStringList items = QStringList(), QtMaterialAutoComplete *parent = Q_NULLPTR);

    virtual ~GMMaterialComboBox() override {}
    QString *value;

    void setVariable(QString *temp);
public slots:
    void FinishedEdit();
    void ItemSelected(QString temp);

signals:
    void editFinished();
    void selectedItem();
};

#endif // GMMATERIALCOMBOBOX_H
