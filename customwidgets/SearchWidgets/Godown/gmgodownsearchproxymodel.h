#ifndef GMGODOWNSEARCHPROXYMODEL_H
#define GMGODOWNSEARCHPROXYMODEL_H


#include <QObject>
#include <QSortFilterProxyModel>

class GMGodownSearchProxyModel: public QSortFilterProxyModel
{
public:
    Q_OBJECT
public:
    GMGodownSearchProxyModel(QObject* parent =0);

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

};

#endif // GMGODOWNSEARCHPROXYMODEL_H
