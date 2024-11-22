#ifndef GMLEDGERSEARCHDELEGATE_H
#define GMLEDGERSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

class GMLedgerSearchDelegate : public QStyledItemDelegate
{
public:
    GMLedgerSearchDelegate();
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // GMLEDGERSEARCHDELEGATE_H
