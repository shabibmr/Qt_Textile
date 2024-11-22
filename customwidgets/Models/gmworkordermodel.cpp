#include "gmworkordermodel.h"
#include <QFont>
#include <QIcon>
#include "datamodels/transaction/gmvouchertypes.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QBitmap>
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

GMWorkOrderModel::GMWorkOrderModel(GeneralVoucherDataObject *voucher, int col, bool isData, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->isData = isData;

    if(isData)
        this->colCount = col+2;
    else
        this->colCount = col;

    fifoCalc = new FifoCalculator();
}

QVariant GMWorkOrderModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    // FIXME: Implement me!
    if (role != Qt::DisplayRole && section !=9)
        return QVariant();

    if(section == 7) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));

    }

    else if(section == 8) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole){
            QPixmap pixmap = QPixmap(":/icons/icons/action/svg/production/ic_print_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#ffffff")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }

    }


    else if (orientation == Qt::Horizontal && section !=7 ) {
        return headerStrings[section];
    }
    return QVariant();
}

//bool GMWorkOrderModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
//{
//    if (value != headerData(section, orientation, role)) {
//        // FIXME: Implement me!
//        emit headerDataChanged(orientation, section, section);
//        return true;
//    }
//    return false;
//}


int GMWorkOrderModel::rowCount(const QModelIndex &parent) const
{
    if(isData)
        return voucher->InventoryItems.size()+1;
    else
        return voucher->InventoryItems.size();
}

int GMWorkOrderModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}



QVariant GMWorkOrderModel::data(const QModelIndex &index, int role) const
{

    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > voucher->InventoryItems.size() || index.row() < 0)
        return QVariant();

    if(index.row() == voucher->InventoryItems.size() &&
            ( role == Qt::DisplayRole || role == Qt::EditRole ))
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
            return voucher->InventoryItems[index.row()].BaseItem.quantity ;
        }
        else if (index.column() == 4)
        {
            return voucher->InventoryItems[index.row()].BaseItem.drQty;
//            return  QuotationStatus::getStatusString(voucher->InventoryItems[index.row()].BaseItem.itemProductionStatus);
        }
        else if (index.column() == 5)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ClosingStock ;
        }

        else if (index.column() == 7)
        {
            return "";
        }

        else if (index.column() == 8)
        {
            return "";
        }


        else if (index.column() == 6)
        {
            return voucher->InventoryItems[index.row()].BaseItem.quantity
                    - voucher->InventoryItems[index.row()].BaseItem.drQty;
        }
    }

    return QVariant();
}

bool GMWorkOrderModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    bool newLineAddedFlag =false;


    if (index.isValid() && role == Qt::EditRole && isData) {
        int row = index.row();



        if(row == voucher->InventoryItems.size() && index.column() == 1){
            //Create New
//            qDebug()<<"Append";
            CompoundItemDataObject item;
            //            QObject *object = qvariant_cast <QObject*>(value);
            //            item.BaseItem = qobject_cast<inventoryItemDataModel> (object);


            item.BaseItem = value.value<inventoryItemDataModel>();
            item.BaseItem.quantity = 1;
            item.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
//            item.BaseItem.fromGodownID = voucher->fromGodownID;
//            item.BaseItem.toGodownID = voucher->toGodownID;
            beginInsertRows(QModelIndex(),voucher->InventoryItems.size(),voucher->InventoryItems.size());
            voucher->InventoryItems.append(item);



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
                item.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;

                //                qDebug()<<"======================Set Item name "<<item.BaseItem.ItemName;
                voucher->InventoryItems[row] =  item;

            }

            else if (index.column() == 2)
            {
                voucher->InventoryItems[row].BaseItem.uomObject = value.value<UomDataModel>();
                qDebug()<<"set barcode uom"<<voucher->InventoryItems[row].BaseItem.uomObject.barCode;

//                voucher->InventoryItems[row].BaseItem.crQty = value.toFloat();

            }


            else if (index.column() == 3)
            {
                voucher->InventoryItems[row].BaseItem.quantity =
                        value.toFloat();

//                voucher->InventoryItems[row].BaseItem.crQty = value.toFloat();

            }

            else if (index.column() == 4) {
                qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->InventoryItems[row].BaseItem.ItemName<<voucher->InventoryItems[row].BaseItem.section;
                voucher->InventoryItems[row].BaseItem.drQty = value.toFloat();
                if(voucher->InventoryItems[row].BaseItem.drQty == voucher->InventoryItems[row].BaseItem.quantity)
                    voucher->InventoryItems[row].BaseItem.itemProductionStatus = QuotationStatus::WorkOrderCompleted;

                bool useSectionFlag = ConfigurationSettingsDatabaseHelper::getValue(useDefaultItemSection,false).toBool();
                if(useSectionFlag){
                    voucher->InventoryItems[row].BaseItem.toGodownID = voucher->InventoryItems[row].BaseItem.section;
                }
                qDebug()<<"mark completed for item "<<voucher->InventoryItems[row].BaseItem.ItemName<<voucher->InventoryItems[row].BaseItem.drQty;
//                GodownTransactionDataModel godownEntry;
//                godownEntry.fromGodown = voucher->InventoryItems[row].BaseItem.fromGodownID;
//                godownEntry.toGodown = voucher->InventoryItems[row].BaseItem.toGodownID;
//                voucher->InventoryItems[row].BaseItem.godownList.append(godownEntry);
//                if(voucher->InventoryItems[row].BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted){
                    fifoCalc->setItemQtyByFifo(voucher, row, 0);
//                }
            }
            else if(index.column() == 7){
//                int rowNumber = value.value<int>();
//                beginRemoveRows(QModelIndex(), row, row);
//                voucher->InventoryItems.removeAt(row);
//                endRemoveRows();
            }

            else if(index.column() == 8){
//                int rowNumber = value.value<int>();
//                beginRemoveRows(QModelIndex(), row, row);
//                voucher->InventoryItems.removeAt(row);
//                endRemoveRows();
            }
            else
                return false;
        }

        updateGodownList(row);
        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMWorkOrderModel::flags(const QModelIndex &index) const
{

    if (!index.isValid() && isData)
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ; // FIXME: Implement me!
}

bool GMWorkOrderModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    endInsertRows();
}

bool GMWorkOrderModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"Begin remove Rows";
    beginRemoveRows(parent, row, row + 0);
    voucher->InventoryItems.removeAt(row);
    endRemoveRows();
}

void GMWorkOrderModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void GMWorkOrderModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
}

void GMWorkOrderModel::updateGodownList(int row)
{
    if(voucher->InventoryItems[row].BaseItem.godownList.size() == 0)
        return;
    else if (voucher->InventoryItems[row].BaseItem.godownList.size() == 1) {
        voucher->InventoryItems[row].BaseItem.godownList[0].drQty = voucher->InventoryItems[row].BaseItem.drQty;
    }
}


QModelIndex GMWorkOrderModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();

}

QModelIndex GMWorkOrderModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}


