#ifndef WORKORDERDELEGATE_H
#define WORKORDERDELEGATE_H

#include "QStyledItemDelegate"
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>


class WorkOrderDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit WorkOrderDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                QSqlQueryModel *model,
                                UomDataBaseHelper *uHelper);
    virtual ~WorkOrderDelegate() override;

    SalesInventoryItemDatabaseHelper *itemHelper;
    UomDataBaseHelper *uomHelper;

    // QAbstractItemDelegate interface

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)  override;
private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);

    void commitAndCloseUOMEditor(QModelIndex index);

private slots:
    void deleteButtonClicked();
    void printButtonClicked();

signals:
    void printBarcode(int)    ;

};

#endif // WORKORDERDELEGATE_H
