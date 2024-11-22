#include "gmledgeritemmodel.h"

#include <QIcon>

GMLedgerItemModel::GMLedgerItemModel(GeneralVoucherDataObject *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col;
}

QVariant GMLedgerItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && section != 4)
        return QVariant();

    if(section == 4) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole)
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));

    }


    if (orientation == Qt::Horizontal && section != 4) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMLedgerItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->ledgersList.size() && column< colCount){
        if((row == voucher->ledgersList.size() && column <= 1 )|| row < voucher->ledgersList.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMLedgerItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMLedgerItemModel::rowCount(const QModelIndex &parent) const
{
    return voucher->ledgersList.size()+1;
}

int GMLedgerItemModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}

QVariant GMLedgerItemModel::data(const QModelIndex &index, int role) const
{
    //    if(index.column() == 1) qDebug()<<"At request data";

    if (!checkIndex(index))
        return QVariant();
    //    if(index.column() == 1) qDebug()<<"failed at check index";


    if (index.row() > voucher->ledgersList.size() || index.row() < 0)
        return QVariant();

    if(index.row() == voucher->ledgersList.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
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
            index.row() == voucher->ledgersList.size()) {
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
            return voucher->ledgersList[index.row()].LedgerName;
        }

        else if (index.column() == 2)
        {

            return voucher->ledgersList[index.row()].crAmount>0? QString::number(voucher->ledgersList[index.row()].crAmount,'f',2):"";
        }
        else if (index.column() == 3)
        {
            return voucher->ledgersList[index.row()].drAmount>0? QString::number(voucher->ledgersList[index.row()].drAmount,'f',2):"";
        }
        else if (index.column() == 4)
        {
            return "";
        }


    }
    //    if(index.column() == 1)
    //        qDebug()<<"failed at check role "<<index.column()<<" role : "<<role << Qt::DisplayRole << Qt::EditRole;
    // FIXME: Implement me!
    return QVariant();
}

bool GMLedgerItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //    qDebug()<<"Setting Data row count : "<< index.row();

    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



        if(row == voucher->ledgersList.size() && index.column() == 1){
            //Create New
            //            qDebug()<<"Append";
            LedgerMasterDataModel ledger;
            ledger = value.value<LedgerMasterDataModel>();
            beginInsertRows(QModelIndex(),voucher->ledgersList.size(),voucher->ledgersList.size());
            ledger.isInvoiceItem = false;
            voucher->ledgersList.append(ledger);
            newLineAddedFlag = true;
            endInsertRows();

        }
        else if (row< voucher->ledgersList.size() && row >=0 ){

            if (index.column() == 1){
                LedgerMasterDataModel ledger;
                ledger = value.value<LedgerMasterDataModel>();
                voucher->ledgersList[row] =  ledger;
            }
            else if (index.column() == 2)
            {
                if(value.toString().length()>0){
                    voucher->ledgersList[row].crAmount = value.toFloat();
                    voucher->ledgersList[row].amount   = value.toFloat();
                    voucher->ledgersList[row].drAmount = 0;
                }
            }
            else if (index.column() == 3)
            {
                if(value.toString().length()>0){
                    voucher->ledgersList[row].amount   = value.toFloat();
                    voucher->ledgersList[row].drAmount = value.toFloat();
                    voucher->ledgersList[row].crAmount = 0;
                }
            }
            else if(index.column()==4){
                int rowNumber = value.value<int>();
                beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
                voucher->ledgersList.removeAt(rowNumber);
                endRemoveRows();
            }
            else
                return false;
        }

        TransactionCalculator::VoucherCalculateLedgersTotal(voucher);
        emit dataChanged(index, index, {role});
        emit updateValues();

        if(newLineAddedFlag)
            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMLedgerItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
    // FIXME: Implement me!
}

bool GMLedgerItemModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMLedgerItemModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i =0;i<count;i++)
        voucher->ledgersList.removeAt(i);
    endRemoveRows();
}

void GMLedgerItemModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void GMLedgerItemModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}
