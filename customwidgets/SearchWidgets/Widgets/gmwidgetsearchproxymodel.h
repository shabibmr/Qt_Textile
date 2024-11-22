#ifndef GMWIDGETSEARCHPROXYMODEL_H
#define GMWIDGETSEARCHPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>


class GMWidgetSearchProxyModel : public QSortFilterProxyModel
{
public:

    Q_OBJECT
public:
   GMWidgetSearchProxyModel(QObject *parent = 0);



   // QSortFilterProxyModel interface
protected:
   bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
   bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};


#endif // GMWIDGETSEARCHPROXYMODEL_H
