#include "pricelistsortmodel.h"
#include <QDebug>
PriceListSortModel::PriceListSortModel(QObject *parent):
    QSortFilterProxyModel(parent)
{

}

bool PriceListSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);

    //    QModelIndex real0 = mapToSource(index1);
    //    qDebug()<<"Filter L:"<<filterRegExp();
    //    qDebug()<<"Sourse Row: "<<source_row;
    //    qDebug()<<"Found at 0: "<<sourceModel()->data(index0,Qt::DisplayRole).toString();
    //    qDebug()<<"Found at 1 : "<<sourceModel()->data(index1,Qt::DisplayRole).toString()<<filterRegExp();
    //    qDebug()<<"Index 1 is : "<<index1;

    //    qDebug()<<"Found at Real 0: "<<sourceModel()->data(real0,Qt::DisplayRole).toString();
    //    qDebug()<<sourceModel()->data(index1,Qt::DisplayRole).toString().contains(filterRegExp());
    bool glag= sourceModel()->data(index1,Qt::DisplayRole).toString().contains(filterRegExp());

//    if(glag)
//        qDebug()<<"Returning "<<glag<<" for "<<sourceModel()->data(index1,Qt::DisplayRole).toString()  ;
    return glag;
}


QVariant PriceListSortModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel()->headerData(section, orientation,role);
}


QVariant PriceListSortModel::data(const QModelIndex &index, int role) const
{
    return sourceModel()->data(mapToSource(index),role);
}
