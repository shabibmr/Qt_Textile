
#ifndef ITEMBUNDLELISTDELEGATE_H
#define ITEMBUNDLELISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>


class ItemBundleListDelegate : public QStyledItemDelegate
{
public:
    explicit ItemBundleListDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                    QSqlQueryModel *model, QString priceListId = "",QObject *parent = nullptr);

    SalesInventoryItemDatabaseHelper *itemHelper;
    QSqlQueryModel *model;
    QString priceListId ;


    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void commitAndCloseItemEditor(QModelIndex index);
};



#endif // ITEMBUNDLELISTDELEGATE_H
