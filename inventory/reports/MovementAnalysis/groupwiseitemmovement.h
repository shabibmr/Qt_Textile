#ifndef GROUPWISEITEMMOVEMENT_H
#define GROUPWISEITEMMOVEMENT_H

#include <QWidget>


#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/sharetools.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"

namespace Ui {
class GroupwiseItemMovement;
}

class GroupwiseItemMovement : public QWidget
{
    Q_OBJECT
    InventoryHelper* inHelper;
    QSqlQueryModel* model;
    AccountGroupMasterDatabaseHelper *accHelper;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    QSqlQueryModel* groupsModel;

    QString accGroupID = "0";
    QString groupID = "0";
    QPointer<GMItemSearchLite> groupSearch;

    ReportsTableView *tableView;
    QtMaterialIconButton *shareButton;

    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QString reportType="Group";
    QStringList selectedGroups;

public:
    explicit GroupwiseItemMovement(QWidget *parent = nullptr);
    ~GroupwiseItemMovement();

    void setTable();

public slots:
    void setAccountDetails(AccountGroupDataModel acc);
private slots:
    void setGroupedReport();
    void setItemwiseReport();

    void showShareOptions();
    void on_OKButton_clicked();

    void on_excelButton_clicked();

//    void on_printButton_clicked();

    void getItemDetails(QString item);
    void printReport(QPrinter *printer);
    void on_tableView_doubleClicked(const QModelIndex &index);
private:
    Ui::GroupwiseItemMovement *ui;
};

#endif // GROUPWISEITEMMOVEMENT_H
