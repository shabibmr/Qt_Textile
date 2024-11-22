#ifndef ADDPROPERTY_H
#define ADDPROPERTY_H

#include <QWidget>
#include "datamodels/inventory/property/propertydatamodel.h"
#include "database/inventory/property/propertydatabasehelper.h"

namespace Ui {
class AddProperty;
}

class AddProperty : public QWidget
{
    Q_OBJECT

public:
    explicit AddProperty(QWidget *parent = 0);
    ~AddProperty();

    PropertyDataModel *property;

    PropertyDatabaseHelper *dbHelper;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
public slots:
    void edit(PropertyDataModel *obj);
    void setTitle(QString text);
private:
    Ui::AddProperty *ui;
signals:
    void closing();
};

#endif // ADDPROPERTY_H
