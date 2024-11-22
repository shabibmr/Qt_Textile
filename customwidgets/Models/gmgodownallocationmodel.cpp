#include "gmgodownallocationmodel.h"
#include <QIcon>

GMGodownAllocationModel::GMGodownAllocationModel(GodownTransactionDataModel *item,QObject *parent)
  : QAbstractItemModel(parent)
{
    this->godownEntry = item;

    headerStrings << "";
    colCount = headerStrings.length();
}

GMGodownAllocationModel::~GMGodownAllocationModel()
{

}

QVariant GMGodownAllocationModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section != 5)
        return QVariant();

    else if (orientation == Qt::Horizontal && section !=5 ) {
        return headerStrings[section];
    }
    return QVariant();
}

int GMGodownAllocationModel::rowCount(const QModelIndex &parent) const
{
    return godownEntry->fifoList.count();


}

int GMGodownAllocationModel::columnCount(const QModelIndex &parent) const
{
    return  headerStrings.length();
}

QVariant GMGodownAllocationModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > godownEntry->fifoList.size() || index.row() < 0)
        return QVariant();

    if(index.row() == godownEntry->fifoList.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if(index.column()==0){
            return  index.row()+1;
        }
        else if(index.column()==1){
            return "";
        }
        return "";
    }

    if (index.column() >2  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignRight|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
            index.row() == godownEntry->fifoList.size()) { // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }

    if(role == Qt::UserRole){
        return "Gd ID";
    }


    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }

        else if (index.column() == 5)
        {
            return "";
        }

    }

    return QVariant();
}

bool GMGodownAllocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag =false;


    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        emit dataChanged(index, index, {role});
        emit updateValues();


        return true;
    }

    return false;
}

Qt::ItemFlags GMGodownAllocationModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

//bool GMGodownAllocationModel::insertRows(int row, int count, const QModelIndex &parent)
//{
//    //    qDebug()<<"insert called";
//    beginInsertRows(parent, row, row + count - 1);
//    // FIXME: Implement me!

//    endInsertRows();
//}

//bool GMGodownAllocationModel::removeRows(int row, int count, const QModelIndex &parent)
//{
//    beginRemoveRows(parent, row, row + count - 1);
//    for(int i =0;i<count;i++)
//        voucher->InventoryItems.removeAt(i);
//    endRemoveRows();
//}



void GMGodownAllocationModel::setGodownEntry(GodownTransactionDataModel *value)
{
    beginResetModel();
    godownEntry = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}


QModelIndex GMGodownAllocationModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row < godownEntry->fifoList.size() &&  column< colCount){
        if((row == godownEntry->fifoList.size() && column <= 1 )|| row < godownEntry->fifoList.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();


}

QModelIndex GMGodownAllocationModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}
