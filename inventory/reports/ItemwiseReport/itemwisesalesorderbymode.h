#ifndef ITEMWISESALESORDERBYMODE_H
#define ITEMWISESALESORDERBYMODE_H

#include <QWidget>
#include <QLabel>
#include <QDateEdit>
#include <QVBoxLayout>
#include "database/Reports/ledgerhelpermt.h"
#include "database/Reports/inventoryhelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "folder/lineediticon.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "sharetools/sharetools.h"
#include <QStackedWidget>

#include "customwidgets/Material/components/qtmaterialtabs.h"

#include <QComboBox>

namespace Ui {
class ItemwiseSalesOrderByMode;
}

class ItemwiseSalesOrderByMode : public QWidget
{
    Q_OBJECT

public:
    explicit ItemwiseSalesOrderByMode(QWidget *parent = nullptr);
    ~ItemwiseSalesOrderByMode();

    QString backgroundColor = "#424242";

    LedgerHelperMT *dbHelper;
    InventoryHelper *invHelper;
    QTableView *tableView;
    QTableView *tableView2;

    QSqlQueryModel *model;
    QSqlQueryModel *model2;

    GMItemSortFilterProxyModel *sort;
    QPointer<QVBoxLayout> TableLay;
    QDate fromDate,toDate;
    GMDateEdit *dateEditFrom,*dateEditTo;
    LineEditIcon *searchLine;

    ShareTools *shareOptionsWidget;
    QtMaterialTabs *tabBar;
    QStackedWidget *stack;

    QComboBox *filter;

    void initDBHelpers();
    void initGlobalObjects();
    void initWidgets();

    void setWidgets();
    void refreshMainTable();

    void setSecondTable();
public slots:
//    void resetTableModel(QSqlQueryModel *model);

    void printReport(QPrinter *printer);

    void filterChanged(int index);



    void showShareOptions();
    void on_excelButton_clicked();
private:
    Ui::ItemwiseSalesOrderByMode *ui;
};

#endif // ITEMWISESALESORDERBYMODE_H
