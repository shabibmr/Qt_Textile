#ifndef GMINVENTORYLISTITEMDELEGATE_H
#define GMINVENTORYLISTITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class GMInventoryListItemDelegate : public QStyledItemDelegate
{
public:
    GMInventoryListItemDelegate();
    virtual ~GMInventoryListItemDelegate() override;


    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)  override;


    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // GMINVENTORYLISTITEMDELEGATE_H
