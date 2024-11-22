#ifndef GMLEDGERTRANSACTIONDELEGATE_H
#define GMLEDGERTRANSACTIONDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "QVBoxLayout"
#include <QMessageBox>


class GMLedgerTransactionDelegate : public QStyledItemDelegate
{
public:
    GMLedgerTransactionDelegate();
    virtual ~GMLedgerTransactionDelegate() override;
    LedgerMasterDatabaseHelper *itemHelper;
    explicit GMLedgerTransactionDelegate(LedgerMasterDatabaseHelper *iHelper,QSqlQueryModel *model);

public:
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

    void deleteButtonClicked();
};

#endif // GMLEDGERTRANSACTIONDELEGATE_H
