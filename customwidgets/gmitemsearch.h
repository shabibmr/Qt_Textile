#ifndef GMITEMSEARCH_H
#define GMITEMSEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPointer>
#include <QPoint>
#include <QFocusEvent>
#include "searchlistitemwidget.h"


#include "gmitemsearchwidget.h"

#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"

class GMItemSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMItemSearch(QList<QSharedPointer<inventoryItemDataModel>> itemsListPtr,QLineEdit *parent=Q_NULLPTR);
    ~GMItemSearch();
   // QList<inventoryItemDataModel> items;
    QStringList list;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    GMItemSearchWidget *listWidget;

protected:
    void textClicked();

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(QString,QString);
    void noneSelected();

signals:
    void SelectedItemID(inventoryItemDataModel);
    void clearItem();
    void plusButton();
    void minusButton();
    void blankenterpressed();
    void astriskpressed();

    void SalesOrderSelected(QString vno);
private slots:
    void SOSelectedSlot(QString Vno);
};

#endif // GMITEMSEARCH_H
