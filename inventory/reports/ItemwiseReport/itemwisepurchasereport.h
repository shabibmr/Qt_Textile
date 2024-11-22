#ifndef ITEMWISEPURCHASEREPORT_H
#define ITEMWISEPURCHASEREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include <customwidgets/test/gmitemsortfilterproxymodel.h>
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "sharetools/sharetools.h"
#include "counter/PrinterHelper/counterprinterhelper.h"


namespace Ui {
class ItemwisePurchaseReport;
}

class ItemwisePurchaseReport : public QWidget
{
    Q_OBJECT

public:
    explicit ItemwisePurchaseReport(QWidget *parent = nullptr);
    ~ItemwisePurchaseReport();

    QSqlQueryModel *model;
    InventoryHelper *inHelper;
    GMItemSortFilterProxyModel* sort;

    void setTableView();
    CounterPrinterHelper *printerHelper;

    QtMaterialIconButton *shareButton;
    ShareTools *shareOptionsWidget;

private slots:
    void on_okButton_clicked();

    void on_printButton_clicked();
    void showShareOptions();

    void printReport(QPrinter *printer);


    void on_searchLine_textChanged(const QString &arg1);

private:
    Ui::ItemwisePurchaseReport *ui;
};

#endif // ITEMWISEPURCHASEREPORT_H
