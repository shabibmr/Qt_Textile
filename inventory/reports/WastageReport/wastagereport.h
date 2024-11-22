#ifndef WASTAGEREPORT_H
#define WASTAGEREPORT_H

#include "customwidgets/section.h"
#include "database/Reports/inventoryhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include <QWidget>
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


#include <QPushButton>

namespace Ui {
class WastageReport;
}

class WastageReport : public QWidget
{
    Q_OBJECT

public:
    explicit WastageReport(QWidget *parent = 0);
    ~WastageReport();

    GMItemSortFilterProxyModel* sort;
    
    QList<GeneralVoucherDataObject> voucherList;
    SalesInventoryGroupsDatabaseHelper* invGrpHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;
    QDateTime dfrom;

    QtMaterialIconButton *backButton;
    QtMaterialIconButton *itemWise;
    QtMaterialIconButton *grouped;
    QtMaterialAppBar *appBar;
    QStringList selectedGroups;

    QString reportType="Group";
    QString voucherType = "WastageEntry";

    InventoryHelper *dbHelper;
    QSqlQueryModel* model;
    QPointer<GMItemSearchLite> itemLineLite;
    QSqlQueryModel* ledModel;
    QPointer<GMLedgerSearchLite> ledLineLite;


    bool datewiseFlag=false;

    void setTableHeading();
    void setTable();

    QString groupID;
    QString ledgerID = "%";
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void getItemDetails(QString item);

    void resetID();
    void on_datewiseButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);


    void setGroupedReport();
    void setItemwiseReport();
    void backButtonPressed();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);

private:
    Ui::WastageReport *ui;
};

#endif // WASTAGEREPORT_H
