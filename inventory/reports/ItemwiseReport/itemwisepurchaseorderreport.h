#ifndef ITEMWISEPURCHASEORDERREPORT_H
#define ITEMWISEPURCHASEORDERREPORT_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include <customwidgets/Delegates/reportcommondelegate.h>
#include <customwidgets/test/gmitemsortfilterproxymodel.h>


namespace Ui {
class ItemwisePurchaseOrderReport;
}

class ItemwisePurchaseOrderReport : public QWidget
{
    Q_OBJECT

public:
    explicit ItemwisePurchaseOrderReport(QWidget *parent = nullptr);
    ~ItemwisePurchaseOrderReport();
    QSqlQueryModel *model;
    InventoryHelper *inHelper;
    GMItemSortFilterProxyModel* sort;

    void setTableView();

private slots:
    void on_okButton_clicked();

private:
    Ui::ItemwisePurchaseOrderReport *ui;
};

#endif // ITEMWISEPURCHASEORDERREPORT_H
