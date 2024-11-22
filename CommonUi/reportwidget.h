#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QLabel>

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "datamodels/Settings/uisettingsdatamodel.h"
#include "datamodels/hr/uiaccesscontroldatamodel.h"

#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Delegates/registerdelegate.h"

#include <QSqlRecord>
#include "customwidgets/gmreportsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "sharetools/sharetools.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"

#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "database/inventory/sales_inventory_groups/salesinventorygroupsdatabasehelper.h"
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownsearchlite.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchlite.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"
#include "voucherOperations/vouchereditorselector.h"

#include "customwidgets/test/ledgerreportdelegate.h"


#include "CommonUi/masterseditor.h"
//#include "finance/transactions/BillwiseRecords/mappedbillsbyvoucher.h"
#include "database/Reports/ledgerhelper.h"


class ReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWidget(DatabaseHelper *db, QString reportTitle = "Report", QWidget *parent = nullptr);
    ~ReportWidget();

    QString masterDb = DatabaseValues::dbName;
    QString transDb = DatabaseValues::dbName;

    // UI settings

    QString reportName;
    QLabel *windowLabel;
    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "#00bcd4";
    QString titleColor = "#00bcd4";
    QString appBarColor = "white";
    QSize iconSize = QSize(18,18);
    QFont normalFont = QFont("Calibri", 9, QFont::Normal);
    QFont boldFont = QFont("Calibri", 12, QFont::Bold);
    QFont titleFont = QFont("Calibri", 18, QFont::Normal);

    UiSettingsDataModel *uiModel;
    UiAccessControlDataModel *permissions;
    bool isTabChange = false;

    QString dateEditSS;
    QtMaterialAppBar *appBar;
    QtMaterialIconButton *shareButton;
    QtMaterialIconButton *filterButton;
    QtMaterialIconButton *okButton;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *newButton;
    QtMaterialTabs *tabs;
    QHBoxLayout *filterLayout;
    QHBoxLayout *registerLayout;
    QHBoxLayout *additionalLay1;
    QHBoxLayout *additionalLay2;
    QHBoxLayout *diffLay;
    QVBoxLayout *tableLayout;
    QHBoxLayout *tabLayout;
    QHBoxLayout *extrasLayout;
    QLineEdit *filterLineEdit;

    QVBoxLayout *vLayout;


    QSqlQueryModel* model;

    ReportsTableView *tableView;
    GMReportSortFilterProxyModel* sort;

    QPointer<GMDateEdit> fromDateEdit;
    QPointer<GMDateEdit> toDateEdit;

    QWidget *fromDateWidget;
    QWidget *toDateWidget;


    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();
    QMap<QString, QPair<QString, QString>> filterMap;
    QString voucherType = "Select Voucher Type...";
    QStringList voucherTypes;
    QMap<QString, QString> priceLists;
    QMap<QString, QString> routes;
    QMap<int, int> tabMap;


    CounterPrinterHelper *printerHelper;
    ShareTools *shareOptionsWidget;

    QPointer<GMItemSearchLite> groupSearch;
    QPointer<GMItemSearchLite> itemSearch;
    QPointer<GMLedgerSearchLite> accSearch;
    QPointer<GMLedgerSearchLite> ledSearch;
    QPointer<GMGodownSearchLite> godownSearch;
    QPointer<QComboBox> voucherTypeCombo;
    QPointer<QComboBox> priceListCombo;
    QPointer<QComboBox> routeCombo;
    QPointer<GMEmployeeSearchLite> employeesearch;
    QLineEdit *lineEdit;

    SalesInventoryGroupsDatabaseHelper *invGrpHelper;
    SalesInventoryItemDatabaseHelper *itemHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    GodownDatabaseHelper *godownHelper;
    UserProfileDatabaseHelper *empHelper;
    PricelistDatabaseHelper *priceHelper;
//    RouteDatabaseHelper *routeHelper;
    DatabaseHelper *db;
    QString dbName;

    void addFilter(QString filterType, QString filterVar, QString searchFilter="", QString filterQuery = "");
    void addQueryFilter(QJsonValue filterLineEdit);

    void setDateFilter(QString dateVar, bool useTimeStamp=false);
    void setAdditionalWidgets();

    void setUiModel(UiSettingsDataModel *value, bool switchTab = false);

    QList<QPair<UiSettingsDataModel , QMap<QString, QPair<QString, QString>> >> uiList;

    QLabel *itemLabel;
    QLabel *grpLabel;
    QLabel *ledgerLabel;
    QLabel *accountLabel;
    QLabel *godownLabel;
    QLabel *employeeLabel;
    QLabel *pricelistLabel;
    QLabel *routeLabel;
    QLabel *voucherTypeLabel;
    QLabel *textEditLabel;

    QString ledgerName;
    QString accGroupId;
    float openingBalance;
    float closingBalance;


    void setPermissions(UiAccessControlDataModel *value);

    void setDbName(const QString &value);

    void setFilterMap(const QMap<QString, QPair<QString, QString> > &value);

    QSqlQueryModel *prepareQueryModel(QString qStr);
public slots:
    void setFilterValue(QString varName, QString value, QString filterType);
    void resetWidgets();
    void setTableView();

    void setUiValues();
    void clearUiStates();

    void keyPressEvent(QKeyEvent *e) override;

signals:
    void redirecting(int row);
    void tabChanged(int ui_id);

private slots:

    void initGlobalObjects();
    void initDbHelpers();
    void initTableView();
    void setWidgets();
    void goToPrevious();
    void closeWindow();
    void clearLayout(QLayout *layout);
    void redirectUi(QModelIndex index);
    void setTabs();
    void exportToExcel();
    void showShareOptions();
    void printReport(QPrinter *printer);
    void createNewVoucher();
    void openMasterEditor();
    void setMasterLayout();
    void setRegisterLayout();
    void setExtrasLayout();

    void callExtrasSlot(QString action, QJsonObject columns, QString query);

    QString escapeChar(QString inStr);

    void showMessageBox(QString text);
    QString prepareQuery(QString query);
    void showMappingsOfVoucher(QString vNo, QString vType, QString vPrefix);



signals:
    void editMaster(QString, QString)    ;
    void showWidget();
    void closing();
    void voucherSaved();
    void setTabTitle(QString);
private:


    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *event) override;
};

#endif // REPORTWIDGET_H
