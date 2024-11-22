#ifndef ITEMMOVEMENTANALYSISDELEGATE_H
#define ITEMMOVEMENTANALYSISDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>


class ItemMovementAnalysisDelegate : public QStyledItemDelegate
{
public:
    ItemMovementAnalysisDelegate();

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ITEMMOVEMENTANALYSISDELEGATE_H
