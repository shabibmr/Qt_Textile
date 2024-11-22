#ifndef GMITEMWISEORDERSDELEGATE_H
#define GMITEMWISEORDERSDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include "QVBoxLayout"
#include <QMessageBox>


class GMItemwiseOrdersDelegate : public QStyledItemDelegate
{
public:
    virtual ~GMItemwiseOrdersDelegate() override;
    LedgerMasterDatabaseHelper *ledHelper;
    UomDataBaseHelper *uomHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    explicit GMItemwiseOrdersDelegate(LedgerMasterDatabaseHelper *iHelper,QSqlQueryModel *model,UomDataBaseHelper *uHelper,
                                      SalesInventoryItemDatabaseHelper *ledHelper,QSqlQueryModel *itemmodel, QString priceListId);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void deleteButtonClicked();
private:
    QSqlQueryModel *model;
    QSqlQueryModel *imodel;
    QString priceID;


public slots :
    void commitAndCloseLedgerEditor(QModelIndex index);
    void commitAndCloseUOMEditor(QModelIndex index);

    // QAbstractItemDelegate interface
    void commitAndCloseItemEditor(QModelIndex index);
    void commitAndCloseDateEditor(QModelIndex index);
public:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

#endif // GMITEMWISEORDERSDELEGATE_H
