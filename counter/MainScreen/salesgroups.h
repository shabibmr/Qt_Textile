#ifndef SALESGROUPS_H
#define SALESGROUPS_H

#include <QWidget>
#include <QSignalMapper>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>

#include "customwidgets/qtbutton.h"
#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "datamodels/inventory/sales_inventory_group/inventoryitemgroupdatamodel.h"
#include <QStringList>

namespace Ui {
class SalesGroups;
}

class SalesGroups : public QWidget
{
    Q_OBJECT

public:
    explicit SalesGroups(int colCount=6,int buttonHeight=40,int buttonWidth=130,QWidget *parent = nullptr);
    ~SalesGroups();

    SalesInventoryGroupsDatabaseHelper *dbHelper;
    QString groupName;
    void setGroupName(const QString &value);

    int colCount;

    int buttonWidth;
    int buttonHeight;

    void setTable();

    QList<QLabel*> imgLabels;
    QList<InventoryItemGroupDataModel> grpList;

public slots:
    void salesGroupSelected(QString grpID);

signals:
    void selectSalesGroupID(QString);
    void favoLongPressed();


private:
    Ui::SalesGroups *ui;
};

#endif // SALESGROUPS_H
