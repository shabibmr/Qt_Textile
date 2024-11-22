#include "GMShiftSalesModel.h"



GMShiftSalesModel::GMShiftSalesModel(ShiftDataModel *model, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->shift = model;
    this->colCount = col + 2;

    headerStrings<<"Ledger"<<"Amount"<<"Actual"<<"Difference";

    qDebug()<<"model";

}

QVariant GMShiftSalesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    qDebug()<<Q_FUNC_INFO;

//    qDebug()<<"headerData";
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMShiftSalesModel::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex GMShiftSalesModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMShiftSalesModel::rowCount(const QModelIndex &parent) const
{
//    qDebug()<<"rowcount";
    return shift->ledgers.size();
}

int GMShiftSalesModel::columnCount(const QModelIndex &parent) const
{
//    qDebug()<<"columnCount";
    return  colCount;
}

QVariant GMShiftSalesModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<Q_FUNC_INFO;
    if(!index.isValid())
        return QVariant();

    if (index.row() > shift->ledgers.size() || index.row() < 0)
        return QVariant();

    int row = index.row();


    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 0 &&
            index.row() == shift->ledgers.size()) {
        // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }

//    QString displayVal=0;
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();
        qDebug()<<"display";
        if (index.column() == 0)
        {
            qDebug()<<shift->ledgers[row]->ledgerName;
            return  shift->ledgers[row]->ledgerName;
        }
        else if (index.column() == 1)
        {
            return shift->ledgers[row]->systemAmount;
        }

        else if (index.column() == 2)
        {
            return shift->ledgers[row]->actualAmount;
        }
        else if (index.column() == 3)
        {
            return shift->ledgers[row]->difference;
        }

    }

    return QVariant();
}

bool GMShiftSalesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        qDebug()<<"Setting Data row count : "<< index.row();

//    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

         if (row< shift->ledgers.size() && row >=0 ){

            if (index.column() == 3){
                shift->ledgers[row]->actualAmount = value.toFloat();
                shift->ledgers[row]->difference = shift->ledgers[row]->actualAmount - shift->ledgers[row]->systemAmount;
            }




        }
        emit dataChanged(index, index, {role});
        return true;
    }

    return false;
}

Qt::ItemFlags GMShiftSalesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

//    if(index.column() == 6)
//        return   Qt::ItemIsEditable|Qt::ItemIsUserCheckable| QAbstractItemModel::flags(index) ;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;

    // FIXME: Implement me!
}

void GMShiftSalesModel::setHeaderStrings(const QStringList &value)
{
    qDebug()<<"set header strings";
    headerStrings = value;
}

void GMShiftSalesModel::setModel(ShiftDataModel *value)
{
    shift = value;
}
