#include "gmjournalentrymodel.h"

#include <QIcon>

GMJournalEntryModel::GMJournalEntryModel(GeneralVoucherDataObject *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col+1;
}

QVariant GMJournalEntryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole && section <colCount-1 )
        return QVariant();

    if(section == colCount-1) {
        if( orientation == Qt::Horizontal){
            qDebug()<<"delte header";
            return  QVariant::fromValue(QIcon(":/icons/trash.ico"));
        }

    }

    else if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMJournalEntryModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->ledgersList.size() && column< colCount){
        if((row == voucher->ledgersList.size() && column <= 1 )|| row < voucher->ledgersList.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMJournalEntryModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMJournalEntryModel::rowCount(const QModelIndex &parent) const
{
    return voucher->ledgersList.size()+1;
}

int GMJournalEntryModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}

QVariant GMJournalEntryModel::data(const QModelIndex &index, int role) const
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
            return QString::number(voucher->ledgersList[index.row()].crAmount,'f',2);
        }
        else if (index.column() == 3)
        {
            return QString::number(voucher->ledgersList[index.row()].drAmount,'f',2);
        }
        else if (index.column() == colCount-1)
        {
            //qDebug()<<"delete icon";
            return "";
        }

    }
    //    if(index.column() == 1)
    //        qDebug()<<"failed at check role "<<index.column()<<" role : "<<role << Qt::DisplayRole << Qt::EditRole;
    // FIXME: Implement me!
    return QVariant();
}

bool GMJournalEntryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qDebug()<<"Setting Data row count : "<< index.row();

    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        if(row == voucher->ledgersList.size() && index.column() == 1){
            LedgerMasterDataModel ledger;
            ledger = value.value<LedgerMasterDataModel>();
            beginInsertRows(QModelIndex(),voucher->ledgersList.size(),voucher->ledgersList.size());
            voucher->ledgersList.append(ledger);

            voucher->ledgersList[voucher->ledgersList.size()-1].LedgerName;
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
                voucher->ledgersList[row].crAmount = value.toFloat();
            else if (index.column() == 3)
            {

                voucher->ledgersList[row].drAmount = value.toFloat();
            }
            else if (index.column() == colCount-1)
            {
                int rowNumber = value.value<int>();
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

Qt::ItemFlags GMJournalEntryModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
    // FIXME: Implement me!
}

bool GMJournalEntryModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMJournalEntryModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + 0);
//    for(int i =0;i<count;i++)
        voucher->ledgersList.removeAt(row);
    endRemoveRows();
}

void GMJournalEntryModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void GMJournalEntryModel::setVoucher(GeneralVoucherDataObject *value)
{
    beginResetModel();
    voucher = value;
    endResetModel();
}
