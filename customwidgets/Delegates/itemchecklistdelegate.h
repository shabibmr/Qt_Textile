#ifndef ITEMCHECKLISTDELEGATE_H
#define ITEMCHECKLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>


class ItemCheckListDelegate: public QStyledItemDelegate
{
public:
    ItemCheckListDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;


};

#endif // ITEMCHECKLISTDELEGATE_H
