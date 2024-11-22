#include "gmitemsortfilterproxymodel.h"
#include <QDebug>
GMItemSortFilterProxyModel::GMItemSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    qDebug()<<Q_FUNC_INFO;
}

bool GMItemSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
//    qDebug()<<Q_FUNC_INFO;
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp())
            || sourceModel()->data(index2).toString().contains(filterRegExp())
            );
//    filterAcceptsRow(sourceRow,sourceParent);
}

bool GMItemSortFilterProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
//    qDebug()<<Q_FUNC_INFO;
    int const left_row  = source_left.row();
    int const right_row = source_right.row();

    int const num_columns = sourceModel()->columnCount();
    for(int compared_column = source_left.column(); compared_column<num_columns; ++compared_column) {
        QModelIndex const left_idx = sourceModel()->index(left_row, compared_column, QModelIndex());
        QModelIndex const right_idx = sourceModel()->index(right_row, compared_column, QModelIndex());

        QVariant leftVal = sourceModel()->data(left_idx);
        QVariant rightVal = sourceModel()->data(right_idx);

        bool ok=false;
        int l = sourceModel()->data(left_idx).toInt(&ok);
        int r = sourceModel()->data(right_idx).toInt(&ok);


        if(leftVal.type() == QVariant::Int || ok ){

//            qDebug()<<"Value type is INT";
            float const leftData = sourceModel()->data(left_idx).toInt();
            float const rightData = sourceModel()->data(right_idx).toInt();
            return (leftData <rightData);
        }

        if(leftVal.type() == QVariant::String){
//            qDebug()<<"Value type is String";
            QString const leftData = sourceModel()->data(left_idx).toString();
            QString const rightData = sourceModel()->data(right_idx).toString();
            int const compare = QString::localeAwareCompare(leftData, rightData);
            if(compare!=0) {
                return compare<0;
            }
        }
        if(leftVal.type() == QVariant::Double){
            float const leftData = sourceModel()->data(left_idx).toFloat();
            float const rightData = sourceModel()->data(right_idx).toFloat();
            return (leftData <rightData);
        }

        if (leftVal.type() == QVariant::DateTime)
                 return leftVal.toDateTime().toTime_t() < rightVal.toDateTime().toTime_t();

        if (leftVal.type() == QVariant::Date)
                 return leftVal.toDate().toJulianDay() < rightVal.toDate().toJulianDay();

    }
    return false;
}


QVariant GMItemSortFilterProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return sourceModel()->headerData(section, orientation,role);
}
