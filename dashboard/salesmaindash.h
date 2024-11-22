#ifndef SALESMAINDASH_H
#define SALESMAINDASH_H

#include <QWidget>
#include "customwidgets/Basic/gmcardwidget.h"
#include "customwidgets/Basic/gmdateedit.h"
#include <QTableView>
#include <QHeaderView>

#include "finance/transactions/sales/salesvouchereditor.h"

namespace Ui {
class SalesMainDash;
}

class SalesMainDash : public QWidget
{
    Q_OBJECT

public:
    explicit SalesMainDash(QWidget *parent = nullptr);
    ~SalesMainDash();

    salesVoucherDatabaseHelper *dbHelper;

    QHBoxLayout *filterLayout;

    QTableView *salesStatsTable;
    QTableView *salesmanwiseTable;
    QTableView *godownwiseTable;
    QTableView *groupwiseSalesTable;
    QTableView *salesRegister;

    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString iconColor = "#00bcd4";
    QString tableViewSS = "QTableView::item{"
                          "gridline-color : #ffffff;"
                          "border: 0px solid #fffff8;"
                          "}"
                          "QTableView::item:selected{"
                          "color:#00acc1;"
                          "background-color:" + bgcolor + ";"
                          "}"
                          "QTableView{"
                          "background-color: #ffffff;"
                          "color: #006064;"
                          "border : none;"
                          "/*selection-background-color: #999*/}"
                          "QHeaderView::section:horizontal"
                          "{"
                          "background-color: #006064;"
                          "color: white;"
                          "border: 1px solid #ffffff;"
                           "}";

    void setWidgets();
    void setFilterLayout();

private slots:
    void setValues();


private:
    Ui::SalesMainDash *ui;
};

#endif // SALESMAINDASH_H
