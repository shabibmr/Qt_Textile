#ifndef TODAYSBILLS_H
#define TODAYSBILLS_H

#include <QWidget>

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "database/finance/salesvoucher/salesvoucherdeleteddatabasehelper.h"

#include "database/hr/userprofiledatabasehelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/assignsalesman.h"
#include "counter/MainScreen/modeofpay.h"
#include "database/finance/Closing/dailyclosingdatabasehelper.h"
#include "database/finance/Closing/shiftdatabasehelper.h"

#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "customwidgets/Delegates/todaysbillsdelegate.h"
#include <QSqlRecord>
#include "finance/transactions/calculator/transactioncalculator.h"
#include "customwidgets/TableViews/reportstableview.h"

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"

namespace Ui {
class TodaysBills;
}

class TodaysBills : public QWidget
{
    Q_OBJECT

public:
    explicit TodaysBills(QWidget *parent = 0);
    ~TodaysBills();

    TodaysBills* TodaysBillsWidget;
    GeneralVoucherDataObject *voucher;
    QList<GeneralVoucherDataObject> voucherList;
    ReportsDatabaseHelper *dbhelper;
    salesVoucherDatabaseHelper *salesHelper;
    salesVoucherDeletedDatabaseHelper *deletedHelper;

    UserProfileDatabaseHelper *userHelper;
    LedgerMasterDatabaseHelper *ledHelper;
    QStringList voucherNos;
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;
    QTableView *tableView;

    void MailDeletedList(QString name);
    void disableEdit();
    void disableDelete();
    void disablePrint();

public slots:
    void showDateWidget(bool v);

    void setTable();

    void setTitle(QString title);

    void switchModeOfPay();

    void showShareOptions();
    void printReport(QPrinter *printer);
    void on_excelButton_clicked();
private slots:

    void setWidgets();

    void on_backButton_clicked();

    void on_copyButton_clicked();

    void on_editButton_clicked();

    void on_cancelButton_clicked();

    void setBillPreview(GeneralVoucherDataObject *voucher);

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_printButton_clicked();

    void on_switchButton_clicked();

    void printVoucher(GeneralVoucherDataObject *voucher);

    void on_voucherNoLineEdit_returnPressed();

    void on_OKButton_clicked();

    void SwitchSalesMan(int id);

    void on_switchModeofPay_clicked();

    void calcTotals();

    void on_tableView_clicked(const QModelIndex &index);



signals:

    void selectedVoucher(GeneralVoucherDataObject *obj);


private:
    Ui::TodaysBills *ui;
};

#endif // TODAYSBILLS_H
