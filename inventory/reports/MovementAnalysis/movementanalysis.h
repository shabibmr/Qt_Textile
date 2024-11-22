#ifndef MOVEMENTANALYSIS_H
#define MOVEMENTANALYSIS_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"


namespace Ui {
class MovementAnalysis;
}

class MovementAnalysis : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    QSqlQueryModel* model;

    QSqlQueryModel* itemsModel;
    SalesInventoryItemDatabaseHelper *itemHelper;

    QString itemID;

public:
    explicit MovementAnalysis(QWidget *parent = 0);
    ~MovementAnalysis();

    void setTable();

public slots:
    void getItemDetails(inventoryItemDataModel item);

private slots:
    void on_OKButton_2_clicked();

    void on_excelButton_2_clicked();

    void on_printButton_2_clicked();

    void on_backButton_2_clicked();

private:
    Ui::MovementAnalysis *ui;
};

#endif // MOVEMENTANALYSIS_H
