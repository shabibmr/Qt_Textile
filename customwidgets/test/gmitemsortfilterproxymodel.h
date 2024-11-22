#ifndef GMITEMSORTFILTERPROXYMODEL_H
#define GMITEMSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDateTime>
class GMItemSortFilterProxyModel : public QSortFilterProxyModel
{
     Q_OBJECT
public:
    GMItemSortFilterProxyModel(QObject *parent = 0);



    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

#endif // GMITEMSORTFILTERPROXYMODEL_H
