#ifndef PRICELISTDELEGATE_H
#define PRICELISTDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>
#include <QtGui>
#include <QApplication>

class PriceListDelegate : public QStyledItemDelegate
{
public:

    explicit PriceListDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                            QSqlQueryModel *model,
                                            UomDataBaseHelper *uHelper);
    virtual ~PriceListDelegate() override;

    SalesInventoryItemDatabaseHelper *itemHelper;
    UomDataBaseHelper *uomHelper;

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);
    void commitAndCloseUOMEditor(QModelIndex index);


private slots:
    void deleteButtonClicked();
};

#endif // PRICELISTDELEGATE_H
