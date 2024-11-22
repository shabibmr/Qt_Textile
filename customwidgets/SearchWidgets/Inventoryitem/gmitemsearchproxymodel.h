#ifndef GMITEMSEARCHPROXYMODEL_H
#define GMITEMSEARCHPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>


class GMItemSearchProxyModel : public QSortFilterProxyModel
{
public:

    Q_OBJECT
public:
   GMItemSearchProxyModel(QObject *parent = 0);



   // QSortFilterProxyModel interface
protected:
   bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
   bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};


#endif // GMITEMSEARCHPROXYMODEL_H
