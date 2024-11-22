#ifndef STOCKENTRYDELEGATE_H
#define STOCKENTRYDELEGATE_H

#include "QStyledItemDelegate"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>


class StockEntryDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit StockEntryDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                QSqlQueryModel *model,
                                UomDataBaseHelper *uHelper);
    virtual ~StockEntryDelegate() override;

    SalesInventoryItemDatabaseHelper *itemHelper;
    UomDataBaseHelper *uomHelper;

    // QAbstractItemDelegate interface

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)  override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);

private slots:
    void deleteButtonClicked();

    void moreButtonClicked();
signals:
    void showItemDetailClicked(int);
};
#endif // STOCKENTRYDELEGATE_H
