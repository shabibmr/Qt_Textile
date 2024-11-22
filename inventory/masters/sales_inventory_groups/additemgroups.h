#ifndef ADDITEMGROUPS_H
#define ADDITEMGROUPS_H

#include <QWidget>
#include <QDebug>
#include "datamodels/inventory/sales_inventory_group/inventoryitemgroupdatamodel.h"
#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "Notifications/notificationledgerslist.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"

namespace Ui {
class AddItemGroups;
}

class AddItemGroups : public QWidget
{
    Q_OBJECT

public:
    explicit AddItemGroups(QWidget *parent = 0);
    ~AddItemGroups();
    QString id ="";
    QMap<QString,QString> groupData;
    SalesInventoryGroupsDatabaseHelper *dbHelper;

    InventoryItemGroupDataModel *invGroup;
    QtMaterialSnackbar* snackbar;

    void setInvGroup(InventoryItemGroupDataModel *value);
    void disableSave();



private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
    void sendNotifications(QStringList);

    void openImageSelectDialog();

private:
    Ui::AddItemGroups *ui;
public slots:
    void edit(InventoryItemGroupDataModel *invGroup);
    void setTitle(QString text);
    void setImage();
signals:
    void closing();
    void NotificationAccepted();
    void NotificationRejected(QString reason);
};

#endif // ADDITEMGROUPS_H
