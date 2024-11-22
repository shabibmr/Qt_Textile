#ifndef UPDATEPLULISTTEXTFILECLASS_H
#define UPDATEPLULISTTEXTFILECLASS_H

#include <QFile>
#include <QTextStream>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QDir>

class UpdatePLUListTextFileClass
{
public:
    UpdatePLUListTextFileClass();

    SalesInventoryItemDatabaseHelper *sHelper;

    void UpdateFile(QString filename="PLU.txt");

    void printPLU();
};

#endif // UPDATEPLULISTTEXTFILECLASS_H
