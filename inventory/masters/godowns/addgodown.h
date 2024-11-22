#ifndef ADDGODOWN_H
#define ADDGODOWN_H

#include <QWidget>
#include "database/inventory/godown/godowndatabasehelper.h"
#include "datamodels/inventory/godown/godowndataobject.h"

#include "Notifications/notificationledgerslist.h"

namespace Ui {
class addGodown;
}

class addGodown : public QWidget
{
    Q_OBJECT

public:
    explicit addGodown(QWidget *parent = 0);
    ~addGodown();
    QString id="";
    GodownDatabaseHelper *dbHelper;
    GodownDataObject *godown;
    void setGodown(GodownDataObject *value);
    void disableSave();

public slots:
    void edit();
    void setTitle(QString text);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

       void sendNotifications(QStringList);

private:
    Ui::addGodown *ui;
signals:
    void closing();
};

#endif // ADDGODOWN_H
