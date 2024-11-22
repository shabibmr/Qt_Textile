#include "itembundlelistmodel.h"
#include <QIcon>
#include <QFont>

ItemBundleListModel::ItemBundleListModel(QList<inventoryItemDataModel> items, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->items = items;
}


QModelIndex ItemBundleListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=items.size() && column< colCount){
        if((row == items.size() && column <= 1 )|| row < items.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();

}

QModelIndex ItemBundleListModel::parent(const QModelIndex &child) const
{
return QModelIndex();
}



int ItemBundleListModel::rowCount(const QModelIndex &parent) const
{
    return items.length()+1;
}



int ItemBundleListModel::columnCount(const QModelIndex &parent) const
{
    return colCount;
}


bool ItemBundleListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool ItemBundleListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i =0;i<count;i++)
        items.removeAt(i);
    endRemoveRows();
}

Qt::ItemFlags ItemBundleListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}


QVariant ItemBundleListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && section != 3)
        return QVariant();


    if(orientation == Qt::Horizontal){
        if(section ==0 ){
            return "#";
        }
        else if (section ==1 ){
            return "Item";
        }
        else if (section ==2 ){
            return "Qty";
        }
        else if (section == 3 ){
            if(role == Qt::DecorationRole)
                return  QVariant::fromValue(QIcon(":/icons/trash.ico"));
        }
    }

    return QVariant();

}

QVariant ItemBundleListModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index))
        return QVariant();



    if (index.row() > items.size() || index.row() < 0)
        return QVariant();

    if(index.row() == items.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
    {
        if(index.column()==0){
            return  index.row()+1;
        }
        else if(index.column()==1){
            return "";
        }
        return "";
    }

    if (index.column() >1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignRight|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
        index.row() == items.size()) {
        // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }

    QString displayVal=0;
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return items[index.row()].ItemName;
        }

        else if (index.column() == 2)
        {
            return items[index.row()].quantity>0 ? QString::number(items[index.row()].quantity,'f',2):"";
        }

        else if (index.column() == 3)
        {
            return "";
        }


    }
    //    if(index.column() == 1)
    //        qDebug()<<"failed at check role "<<index.column()<<" role : "<<role << Qt::DisplayRole << Qt::EditRole;
    // FIXME: Implement me!
    return QVariant();
}

bool ItemBundleListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();


        if(row == items.size() && index.column() == 1){
            //Create New
            //            qDebug()<<"Append";
            inventoryItemDataModel item;
            item = value.value<inventoryItemDataModel>();
            beginInsertRows(QModelIndex(),items.size(),items.size());
            items.append(item);
            newLineAddedFlag = true;
            endInsertRows();

        }
        else if (row< items.size() && row >=0 ){

            if (index.column() == 1){
                inventoryItemDataModel item;
                item = value.value<inventoryItemDataModel>();
                beginInsertRows(QModelIndex(),items.size(),items.size());
                items.insert(row,item);
                endInsertRows();
            }
            else if (index.column() == 2)
            {
               items[row].quantity =  value.value<float>();

            }
            else if(index.column() == 3){
                int rowNumber = value.value<int>();
                beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
                items.removeAt(rowNumber);
                endRemoveRows();
            }
            else{
                return false;
            }
        }


        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;

}
