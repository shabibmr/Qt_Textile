#ifndef SHIFTWISEREPORT_H
#define SHIFTWISEREPORT_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "sharetools/excel/excelreporthelper.h"
#include <QSqlRecord>
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "database/finance/Closing/restaurantshiftdatabasehelper.h"
#include <QMessageBox>
#include "login/loginvalues.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"


#include <counter/Shift/dailyreportcalculator.h>
#include "customwidgets/TableViews/reportstableview.h"

namespace Ui {
class ShiftWiseReport;
}

class ShiftWiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit ShiftWiseReport(QWidget *parent = 0);
    ~ShiftWiseReport();
    QSqlQueryModel *model;
    LedgerHelper *dbHelper;
    RestaurantShiftDatabaseHelper *shifthelper;
    QLineEdit* lineedit;

    int shiftID=0;
    float cashincounter=0;

    void setup();
    QString reportName;
    DailyReportCalculator Calc;
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;
    ReportsTableView *tableView;


     QDate fromDate = QDate::currentDate();
     QDate toDate = QDate::currentDate();

     bool deleteAllowed = true;



     void setDeleteAllowed(bool value);

private slots:
     void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTable();

    void on_amountWiseTableView_doubleClicked(const QModelIndex &index);

    void on_amountWiseTableView_clicked(const QModelIndex &index);

    void on_editButton_clicked();

    void on_mailabridge_clicked();

    void on_printButtonAbridge_clicked();

    void on_okButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);
    void printTable(QPrinter *printer);

private:
    Ui::ShiftWiseReport *ui;
};

#endif // SHIFTWISEREPORT_H
