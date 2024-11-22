#ifndef GMEMPLOYEESEARCHPROXYMODEL_H
#define GMEMPLOYEESEARCHPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>


class GMEmployeeSearchProxyModel : public QSortFilterProxyModel
{
public:

    Q_OBJECT
public:
   GMEmployeeSearchProxyModel(QObject *parent = 0);



   // QSortFilterProxyModel interface
protected:
   bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
   bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};


#endif // GMEMPLOYEESEARCHPROXYMODEL_H
