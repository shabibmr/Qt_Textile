#ifndef GMINVENTORYTRANSACTIONDELEGATE_H
#define GMINVENTORYTRANSACTIONDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>

class GMInventoryTransactionDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit GMInventoryTransactionDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                            QSqlQueryModel *model,
                                            UomDataBaseHelper *uHelper,
                                            QString priceListId = "");
    virtual ~GMInventoryTransactionDelegate() override;

    SalesInventoryItemDatabaseHelper *itemHelper;
    UomDataBaseHelper *uomHelper;
    QString priceListId;

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)  override;
    void setPriceListId(const QString &value);

signals:
    void showItemDetailClicked(int);
    void refresh();

private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);
    void commitAndCloseUOMEditor(QModelIndex index);


private slots:
//    void deleteButtonClicked();

    // QAbstractItemDelegate interface

    void moreButtonClicked();


};

#endif // GMINVENTORYTRANSACTIONDELEGATE_H
