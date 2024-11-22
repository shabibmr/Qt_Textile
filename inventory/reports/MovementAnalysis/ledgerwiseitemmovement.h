#ifndef LEDGERWISEITEMMOVEMENT_H
#define LEDGERWISEITEMMOVEMENT_H

#include <QWidget>
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"


namespace Ui {
class LedgerwiseItemMovement;
}

class LedgerwiseItemMovement : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    QSqlQueryModel* model;
    QSqlQueryModel* ledgerModel;

    QString ledgerID = "%";
    QString groupID = "0";
    QPointer<GMItemSearchLite> groupSearch;
    ReportsTableView *tableView;

    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QString reportType="Group";
    QStringList selectedGroups;

    void setTable();

public:
    explicit LedgerwiseItemMovement(QWidget *parent = nullptr);
    ~LedgerwiseItemMovement();

public slots:
    void setLedger(LedgerMasterDataModel);
private slots:
    void setGroupedReport();
    void setItemwiseReport();
    void on_OKButton_clicked();

    void on_printButton_clicked();

    void on_excelButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void getItemDetails(QString item);
private:
    Ui::LedgerwiseItemMovement *ui;
};

#endif // LEDGERWISEITEMMOVEMENT_H
