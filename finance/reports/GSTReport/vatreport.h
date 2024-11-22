#ifndef VATREPORT_H
#define VATREPORT_H

#include <QWidget>
#include "database/Reports/vatreportdatabasehelper.h"
#include "database/Reports/ledgerhelper.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include <QSqlRecord>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include "hsnreportdelegate.h"

#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class VatReport;
}

class VatReport : public QWidget
{
    Q_OBJECT

public:
    explicit VatReport(QWidget *parent = nullptr);
    ~VatReport();

    QSqlQueryModel *b2cModel;
    QSqlQueryModel *purchaseModel;

    LedgerHelper *ledHelper;
    GMItemSortFilterProxyModel *sortB2CSales;
    GMItemSortFilterProxyModel *sortPurchase;

    VatReportDatabaseHelper *dbHelper;

    QMap<QString, float> ledMap;

    ReportsTableView *b2csalesTableView;
    ReportsTableView *purchaseTableView;


    bool loadedOutput = false;
    bool loadedInput = false;


public slots:

    void setTableb2cSales();
    void setTablePurchase();
    void setTable();



private slots:

    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::VatReport *ui;
};

#endif // VATREPORT_H
