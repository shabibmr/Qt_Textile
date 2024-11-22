#include "uacsortmodel.h"
#include <QDebug>
UACSortModel::UACSortModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    module = "Finance";
}

void UACSortModel::setModule(const QString &value)
{
    module = value;
//    qDebug()<<Q_FUNC_INFO<<module;
}

bool UACSortModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    //    QModelIndex index0 = sourceModel()->index(source_row, 0, source_parent);
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);

    QModelIndex moduleIndex = sourceModel()->index(source_row, 3, source_parent);
    bool glag= sourceModel()->data(index1,Qt::DisplayRole).toString().contains(filterRegExp())
            && sourceModel()->data(moduleIndex,Qt::UserRole +1 ).toString().contains(module);

    return glag;
}


QVariant UACSortModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel()->headerData(section, orientation,role);
}


QVariant UACSortModel::data(const QModelIndex &index, int role) const
{
    return sourceModel()->data(mapToSource(index),role);
}
