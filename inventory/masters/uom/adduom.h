#ifndef ADDUOM_H
#define ADDUOM_H

#include <QWidget>
#include <QDoubleValidator>

#include "datamodels/inventory/uom/uomdatamodel.h"
#include "database/inventory/uom/uomdatabasehelper.h"
#include "Notifications/notificationledgerslist.h"
namespace Ui {
class addUOM;
}

class addUOM : public QWidget
{
    Q_OBJECT

public:
    explicit addUOM(QWidget *parent = 0);
    ~addUOM();
    QString id;
    UomDataBaseHelper *dbHelper;
    UomDataModel *uomObj;
    void setUomObj(UomDataModel *value);
    void disableSave();

public slots :
    void editUOM(UomDataModel *item);
    void setTitle(QString text);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
void sendNotifications(QStringList);
private:
    Ui::addUOM *ui;
signals:
    void closing();
    void NotificationAccepted();
};

#endif // ADDUOM_H
