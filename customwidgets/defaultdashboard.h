#ifndef DEFAULTDASHBOARD_H
#define DEFAULTDASHBOARD_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "database/Reports/ledgerhelper.h"
#include <QtCharts>
#include <customwidgets/Charts/drilldownchart.h>
#include <customwidgets/Charts/drilldownslice.h>
#include <database/finance/Closing/shiftdatabasehelper.h>
#include <database/Counter/countersettingsdatabasehelper.h>


#include "database/Reports/ledgerhelpermt.h"

#include <QtConcurrent>
#include <QFuture>

namespace Ui {
class DefaultDashBoard;
}

class DefaultDashBoard : public QWidget
{
    Q_OBJECT


public:
    explicit DefaultDashBoard(QWidget *parent = 0);
    ~DefaultDashBoard();

    LedgerHelper *ledHelper;

    void setBarchart();
    void setPieChart();

private slots:
    void on_toolButton_clicked();


    void setSalesModel(QSqlQueryModel *model);
    void setPurchaseTable(QSqlQueryModel *model);
    void setRecTable(QSqlQueryModel *model);
    void setPaytable(QSqlQueryModel *model);

private:
    Ui::DefaultDashBoard *ui;
};

#endif // DEFAULTDASHBOARD_H
