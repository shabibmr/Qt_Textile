#ifndef GMLEDGERSEARCHPROXYMODEL_H
#define GMLEDGERSEARCHPROXYMODEL_H
#include <QObject>
#include <QSortFilterProxyModel>

class GMLedgerSearchProxyModel: public QSortFilterProxyModel
{
public:
    GMLedgerSearchProxyModel();
protected:
   bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
   bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

};

#endif // GMLEDGERSEARCHPROXYMODEL_H
