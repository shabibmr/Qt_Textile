#include "gmstockjournalmodel.h"
#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

GMStockJournalModel::GMStockJournalModel(GeneralVoucherDataObject *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col+2;

    fifoCalc = new FifoCalculator();
}

QVariant GMStockJournalModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section < this->colCount - 2
            )
        return QVariant();

    if(section == 5) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));
    }

    else if(section == 6) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QtMaterialTheme::icon("navigation", "more_vert"));
    }

    else if (orientation == Qt::Horizontal && section !=5 ) {
        return headerStrings[section];
    }
    return QVariant();
}

int GMStockJournalModel::rowCount(const QModelIndex &parent) const
{
    return voucher->InventoryItems.size()+1;


}

int GMStockJournalModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMStockJournalModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > voucher->InventoryItems.size() || index.row() < 0)
        return QVariant();

    if(index.row() == voucher->InventoryItems.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
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
            index.row() == voucher->InventoryItems.size()) { // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }

    if(role == Qt::UserRole){
        return voucher->InventoryItems[index.row()].BaseItem.ItemID;
    }

    if(role == Qt::UserRole+1){
        //        return QVariant::fromValue(voucher->InventoryItems[index.row()].BaseItem.uomObject);
        return  voucher->InventoryItems[index.row()].BaseItem.uomObject._id;
    }

    if(role == Qt::UserRole+2){
        //return ledger ID
        return voucher->ledgerObject.LedgerID;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemName;
        }
        else if (index.column() == 2)
        {
            return voucher->InventoryItems[index.row()].BaseItem.uomObject.UomSymbol;
        }
        else if (index.column() == 3)
        {
            return voucher->InventoryItems[index.row()].BaseItem.crQty ;
        }
        else if (index.column() == 4)
        {
            return voucher->InventoryItems[index.row()].BaseItem.drQty ;
        }
        else if (index.column() == 5)
        {
            return "";
        }

        else if (index.column() == 6)
        {
            return "";
        }


    }

    return QVariant();
}

bool GMStockJournalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag =false;


    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



        if(row == voucher->InventoryItems.size() && index.column() == 1){
            //Create New
            qDebug()<<"Append";
            CompoundItemDataObject item;
            //            QObject *object = qvariant_cast <QObject*>(value);
            //            item.BaseItem = qobject_cast<inventoryItemDataModel> (object);


            item.BaseItem = value.value<inventoryItemDataModel>();
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);
            qDebug()<<"New Item at "<<voucher->InventoryItems.size()<< " is "<<
                      voucher->InventoryItems[voucher->InventoryItems.size()-1].BaseItem.ItemName;

            //            this->insertRow(voucher->InventoryItems.size(),QModelIndex());
            newLineAddedFlag = true;

            endInsertRows();

        }
        else if (row< voucher->InventoryItems.size() && row >=0 ){
            //Replace
            //            qDebug()<<"Replace ";
            if (index.column() == 1){
                CompoundItemDataObject item;
                item.BaseItem = value.value<inventoryItemDataModel>();
                //                qDebug()<<"======================Set Item name "<<item.BaseItem.ItemName;
                voucher->InventoryItems[row] =  item;
            }

            else if (index.column() == 3)
            {
                voucher->InventoryItems[row].BaseItem.crQty = value.toFloat();
                voucher->InventoryItems[row].BaseItem.quantity =
                       qFabs( voucher->InventoryItems[row].BaseItem.drQty -
                        voucher->InventoryItems[row].BaseItem.crQty);
                qDebug()<<Q_FUNC_INFO<<__LINE__<<"CR"<<voucher->InventoryItems[row].BaseItem.crQty;
            }
            else if (index.column() == 4)
            {
                voucher->InventoryItems[row].BaseItem.drQty = value.toFloat();

                voucher->InventoryItems[row].BaseItem.quantity =
                        qFabs(voucher->InventoryItems[row].BaseItem.drQty -
                        voucher->InventoryItems[row].BaseItem.crQty);
                qDebug()<<Q_FUNC_INFO<<__LINE__<<"DR"<<voucher->InventoryItems[row].BaseItem.drQty<<voucher->InventoryItems[row].BaseItem.crQty;
            }
            else if(index.column()==5){
                int rowNumber = value.value<int>();
                //                qDebug()<<"At model "<<rowNumber;
            }
            else if(index.column()==6){
                int rowNumber = value.value<int>();
                //                qDebug()<<"At model "<<rowNumber;
            }
            else
                return false;
        }


        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMStockJournalModel::flags(const QModelIndex &index) const
{

    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMStockJournalModel::insertRows(int row, int count, const QModelIndex &parent)
{
    //    qDebug()<<"insert called";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMStockJournalModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i =0;i<count;i++)
        voucher->InventoryItems.removeAt(i);
    endRemoveRows();
}

void GMStockJournalModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
    qDebug()<<"Header Set";
}

void GMStockJournalModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}


QModelIndex GMStockJournalModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();


}

QModelIndex GMStockJournalModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

