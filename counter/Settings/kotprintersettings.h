#ifndef KOTPRINTERSETTINGS_H
#define KOTPRINTERSETTINGS_H

#include <QWidget>

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

#include <QCheckBox>
#include <QProgressDialog>
#include <QPrinterInfo>



namespace Ui {
class KOTPrinterSettings;
}

class KOTPrinterSettings : public QWidget
{
    Q_OBJECT

public:
    explicit KOTPrinterSettings(QWidget *parent = 0);
    ~KOTPrinterSettings();

    QStringList AvailablePrintersList;
    QList<QSharedPointer<inventoryItemDataModel>> itemsList;
    QMap<QString,QString> invGroups;


    SalesInventoryItemDatabaseHelper *dbhelper;
    SalesInventoryGroupsDatabaseHelper *itemGroupHelper ;
    void setTable();
public slots:
    void setAllItemsTOPrinter(bool val);
    void setAllGroupsTOPrinter(bool val);
private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::KOTPrinterSettings *ui;
};

#endif // KOTPRINTERSETTINGS_H
