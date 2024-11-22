#ifndef PRICELISTSORTMODEL_H
#define PRICELISTSORTMODEL_H

//#include <QObject>
#include <QSortFilterProxyModel>

class PriceListSortModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PriceListSortModel(QObject *parent=0);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // QAbstractItemModel interface

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
};


#endif // PRICELISTSORTMODEL_H
