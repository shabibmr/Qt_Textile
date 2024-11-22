#ifndef BillwiseMappingDelegate_H
#define BillwiseMappingDelegate_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QSqlQueryModel>

class BillwiseMappingDelegate: public QStyledItemDelegate
{
public:
    BillwiseMappingDelegate(QSqlQueryModel *model);
    QSqlQueryModel *model;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;



    // QAbstractItemDelegate interface
public:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
};

#endif // BillwiseMappingDelegate_H
