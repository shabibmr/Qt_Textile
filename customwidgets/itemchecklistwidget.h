#ifndef ITEMCHECKLISTWIDGET_H
#define ITEMCHECKLISTWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QLabel>

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h"
#include "customwidgets/TableViews/reportstableview.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

#include "customwidgets/SearchWidgets/Inventoryitem/gmitemsearchlite.h"
#include "customwidgets/Delegates/itemchecklistdelegate.h"
#include "customwidgets/Models/gmitemchecklistmodel.h"
#include "customwidgets/Material/components/qtmaterialfab.h"

namespace Ui {
class ItemCheckListWidget;
}

class ItemCheckListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemCheckListWidget(GeneralVoucherDataObject *voucher, QWidget *parent = nullptr);
    ~ItemCheckListWidget();

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

    QList<CompoundItemDataObject> itemList;
    GeneralVoucherDataObject *voucher;
//    QList<CompoundItemDataObject> checkingList;
    SalesInventoryItemDatabaseHelper *itemHelper;

    GMItemSearchLite *itemSearch;
    QTableView *tableView;
    GMItemCheckListModel *model;

//    QtMaterialFloatingActionButton *okButton;
//    QtMaterialFloatingActionButton *rejectButton;

    QtMaterialFlatButton *saveButton;

    void setVoucher(GeneralVoucherDataObject *value);




    void setWidgets();
    void resetWidgets();

private slots:
//    void initCheckingList();
    int findItemInVoucher(QString itemID, QString uom);
    void removeItems();
    bool checklistOk();

private:
    Ui::ItemCheckListWidget *ui;

signals:
    void checkCompleted(bool)    ;
};

#endif // ITEMCHECKLISTWIDGET_H
