#ifndef GMINVENTORYITEMCOMPLETER_H
#define GMINVENTORYITEMCOMPLETER_H

#include <QCompleter>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QString>
#include <QLineEdit>
#include "customwidgets/SearchWidgets/Inventoryitem/itemsearchdelegate.h"
#include <QListView>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDebug>
#include <database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h>
#include "inventory/masters/sales_inventory_items/additems.h"


class GMInventoryItemSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMInventoryItemSearch(SalesInventoryItemDatabaseHelper *itemHelper,
                                   QSqlQueryModel *m,
                                   QString priceListId = "",
                                   QLineEdit *parent=Q_NULLPTR);
    virtual ~GMInventoryItemSearch() override;
    void setModel(QSqlQueryModel *model);
    void updateModel() ;

    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    SalesInventoryItemDatabaseHelper *itemHelper;
    QString oldText;
    QString oldID;
    inventoryItemDataModel item;
    QString priceListId;


    void setNewText(QString text, QString id);

    void setItem(const inventoryItemDataModel &value);

    void createItem();


public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();
    void openCompleter();

    void finishEditByRet();

    void emitSelectedVals(QModelIndex index,inventoryItemDataModel *item);
signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(inventoryItemDataModel item);

    // QWidget interface
protected:
    void sendPositiveEMIT(QModelIndex index,QString itemID);
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

#endif // GMINVENTORYITEMCOMPLETER_H


//class InnerProxyModel : QSortFilterProxyModel{
//    protected:
//    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
//    {
//        QModelIndex index0 = sourceModel()->index(sourceRow, 4, sourceParent);
//        QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
//        QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);


//        return (sourceModel()->data(index0).toString().contains(filterRegExp())
//                || sourceModel()->data(index1).toString().contains(filterRegExp())
//                || sourceModel()->data(index2).toString().contains(filterRegExp())
//                );

//    }
//};
