#ifndef PENDINGDELIVERYDELEGATE_H
#define PENDINGDELIVERYDELEGATE_H

#include <QStyledItemDelegate>

class PendingDeliveryDelegate : public QStyledItemDelegate
{
public:
    PendingDeliveryDelegate();

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // PENDINGDELIVERYDELEGATE_H
