#ifndef PURCHASEORDERSPENDING_H
#define PURCHASEORDERSPENDING_H

#include <QWidget>
#include "database/inventory/purchaseorder/purchaseorderdatabasehelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

namespace Ui {
class PurchaseOrdersPending;
}

class PurchaseOrdersPending : public QWidget
{
    Q_OBJECT

public:
    explicit PurchaseOrdersPending(QWidget *parent = 0);
    ~PurchaseOrdersPending();

    PurchaseOrderDatabaseHelper *dbhelper;
    QtMaterialIconButton *shareButton;
    QSqlQueryModel *model;
    ShareTools *shareOptionsWidget;


private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTableView();

    void showShareOptions();
private:
    Ui::PurchaseOrdersPending *ui;
};

#endif // PURCHASEORDERSPENDING_H
