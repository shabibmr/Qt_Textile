#include "gmfifomodel.h"

GMFifoModel::GMFifoModel(GodownTransactionDataModel *item1, QStringList list, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->godownAllocObj = item1;

    headers = list;
    colCount = list.size();
    rowCnt = item1->fifoList.size();
}

QVariant GMFifoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole )
        return QVariant();

    else if (orientation == Qt::Horizontal ) {
        return headers[section];
    }
    return QVariant();
}

//bool GMFifoModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}

QModelIndex GMFifoModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<rowCnt && column< colCount){
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMFifoModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMFifoModel::rowCount(const QModelIndex &parent) const
{
    return rowCnt;

    // FIXME: Implement me!
}

int GMFifoModel::columnCount(const QModelIndex &parent) const
{
    return headers.size();
}

QVariant GMFifoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role == Qt::DisplayRole || role == Qt::EditRole) {
//             qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return godownAllocObj->fifoList[index.row()]->VoucherDate.toString("dd-MM-yyyy"); // Purchase Date
        }
        else if (index.column() == 2)
        {
            return godownAllocObj->fifoList[index.row()]->From_Location; // Section
        }
        else if (index.column() == 3)
        {
            // return QString::number(item->fifoList[index.row()]->crQty,'f',
               //     item->uomObject.decimalPoints);
        }
        else if (index.column() == 4)
        {
            return QString::number(godownAllocObj->fifoList[index.row()]->Price,'f',
                    2); //
        }
        else if (index.column() == 5)
        {
            return QString::number(godownAllocObj->fifoList[index.row()]->marginPercent,'f',
                    2); //
        }
        else if (index.column() == 6)
        {
            return QString::number(godownAllocObj->fifoList[index.row()]->marinByValue,'f',
                    2); //
        }
        else if (index.column() == 7)
        {
            return QString::number(godownAllocObj->fifoList[index.row()]->sellingPrice,'f',
                    2); //
        }

    }
     return QVariant();
}

bool GMFifoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {

        if (index.isValid() && role == Qt::EditRole) {

            if (index.column() == 5){ // margin %
                godownAllocObj->fifoList[index.row()]->marginPercent = value.toFloat();
                godownAllocObj->fifoList[index.row()]->marinByValue = godownAllocObj->fifoList[index.row()]->Price * value.toFloat() /100;
                godownAllocObj->fifoList[index.row()]->sellingPrice = godownAllocObj->fifoList[index.row()]->Price+ godownAllocObj->fifoList[index.row()]->marinByValue;

            }
            else if (index.column() == 6){  // margin Amount
                godownAllocObj->fifoList[index.row()]->marinByValue = value.toFloat();
                godownAllocObj->fifoList[index.row()]->marginPercent = value.toFloat() * 100/godownAllocObj->fifoList[index.row()]->Price;
                godownAllocObj->fifoList[index.row()]->sellingPrice = godownAllocObj->fifoList[index.row()]->Price+ godownAllocObj->fifoList[index.row()]->marinByValue;

            }
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags GMFifoModel::flags(const QModelIndex &index) const
{
    if (!index.isValid() || index.row()<2)
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable; // FIXME: Implement me!
}


void GMFifoModel::calulateAvgPrice()
{
    float rate =0;
    for(int i=0;i<godownAllocObj->fifoList.size();i++){
        rate += godownAllocObj->fifoList[i]->sellingPrice;
    }
//    godownAllocObj->price = rate/godownAllocObj->quantity;
}

void GMFifoModel::setGodownEntry(GodownTransactionDataModel *value)
{
    godownAllocObj = value;
    rowCnt = godownAllocObj->fifoList.size();
}
