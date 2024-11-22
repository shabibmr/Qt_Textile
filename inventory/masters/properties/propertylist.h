#ifndef PROPERTYLIST_H
#define PROPERTYLIST_H

#include <QWidget>
#include <QKeyEvent>
#include "addproperty.h"
#include "database/inventory/property/propertydatabasehelper.h"
#include "datamodels/inventory/property/propertydatamodel.h"

namespace Ui {
class PropertyList;
}

class PropertyList : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyList(QWidget *parent = 0);
    ~PropertyList();
    AddProperty* addpropertyWidget;
    QList<PropertyDataModel> propertyList;
    PropertyDatabaseHelper *dbHelper;

    QSqlQueryModel *model;

private slots:
    void on_createNewPushButton_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void keyPressEvent(QKeyEvent *e);


public slots:
    void settable();
private:
    Ui::PropertyList *ui;
};

#endif // PROPERTYLIST_H
