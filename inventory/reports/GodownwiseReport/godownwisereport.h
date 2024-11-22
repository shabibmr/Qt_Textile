#ifndef GODOWNWISEREPORT_H
#define GODOWNWISEREPORT_H

#include <QWidget>
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "database/Reports/inventoryhelper.h"

namespace Ui {
class GodownwiseReport;
}

class GodownwiseReport : public QWidget
{
    Q_OBJECT

public:
    explicit GodownwiseReport(QWidget *parent = 0);
    ~GodownwiseReport();

    GMGodownLineMaterial *gdwid;
    GodownDatabaseHelper *dbhelper;
    InventoryHelper *invhelper;

    QSqlQueryModel *model;

    QString godownID ;




private slots:
    void setTable();
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::GodownwiseReport *ui;
};

#endif // GODOWNWISEREPORT_H
