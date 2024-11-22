#ifndef GODOWNWISEITEMMOVEMENT_H
#define GODOWNWISEITEMMOVEMENT_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownsearchlite.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"



namespace Ui {
class GodownwiseItemMovement;
}

class GodownwiseItemMovement : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    QSqlQueryModel* model;
    QSqlQueryModel* ledgerModel;

    QString godownID = "%";
    QString groupID = "0";
    QPointer<GMItemSearchLite> groupSearch;
    ReportsTableView *tableView;

    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QString reportType="Group";
    QStringList selectedGroups;


    void setTable();

public:
    explicit GodownwiseItemMovement(QWidget *parent = nullptr);
    ~GodownwiseItemMovement();

public slots:
    void setLedger(LedgerMasterDataModel);
private slots:
    void setGroupedReport();
    void setItemwiseReport();
    void on_OKButton_clicked();

    void on_printButton_clicked();

    void on_excelButton_clicked();

    void getGodownDetails(GodownDataObject obj);
    void getItemDetails(QString item);
    void on_tableView_doubleClicked(const QModelIndex &index);
private:
    Ui::GodownwiseItemMovement *ui;
};

#endif // GODOWNWISEITEMMOVEMENT_H
