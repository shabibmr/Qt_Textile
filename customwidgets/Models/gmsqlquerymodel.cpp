#include "gmsqlquerymodel.h"
#include <QDebug>

GMSQLQUeryModel::GMSQLQUeryModel(QMap<int, Qt::CheckState> check_state_map1, int checkCol, QSqlQueryModel *parent)
    : QSqlQueryModel(parent)
{

    this->check_state_map = check_state_map1;
    this->checkCol = checkCol;
}



QVariant GMSQLQUeryModel::data(const QModelIndex &index, int role) const
{

    if(!checkIndex(index))
        return QVariant();

    if(index.column() == checkCol && role == Qt::CheckStateRole)
    {
        if (check_state_map.contains(index.row()))
            return check_state_map[index.row()] == Qt::Checked ? Qt::Checked : Qt::Unchecked;
        return Qt::Unchecked;
    }
    else
        return QSqlQueryModel::data(index, role);

}

bool GMSQLQUeryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!checkIndex(index))
        return false;
    if (role == Qt::CheckStateRole && index.column() == checkCol)
    {
        if(check_state_map.contains(index.row()))
            check_state_map[index.row()] = (value == Qt::Checked ? Qt::Checked : Qt::Unchecked);
        else{
            check_state_map.insert(index.row(),(value == Qt::Checked ? Qt::Checked : Qt::Unchecked));
        }
        emit checked(index.row(), check_state_map[index.row()] );
        return true;
    }
    return QSqlQueryModel::setData(index,value,role);

}

Qt::ItemFlags GMSQLQUeryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == checkCol)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void GMSQLQUeryModel::markChecked(int row, bool checkState)
{
    qDebug()<<"Marked "<<row;
    beginResetModel();
    check_state_map[row]=checkState?Qt::Checked:Qt::Unchecked;
    emit checked(row, check_state_map[row] );

    //    //qDebug()<<"%% setVoucher Price:"<<voucher->InventoryItems[0].BaseItem.price<<value->InventoryItems[0].BaseItem.price;
    endResetModel();

}
