#include "gmreconciliationmodel.h"



GMReconciliationModel::GMReconciliationModel(QSqlQueryModel *model, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->model = model;

}

QVariant GMReconciliationModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    qDebug()<<"headerData";
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMReconciliationModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=model->rowCount()){
        if((row == model->rowCount() && column <= 1 )|| row < model->rowCount())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMReconciliationModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMReconciliationModel::rowCount(const QModelIndex &parent) const
{
    return model->rowCount();
}

int GMReconciliationModel::columnCount(const QModelIndex &parent) const
{
    return  colCount;
}

QVariant GMReconciliationModel::data(const QModelIndex &index, int role) const
{

    if(!index.isValid())
        return QVariant();

    if (index.row() > model->rowCount() || index.row() < 0)
        return QVariant();

    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
            index.row() == model->rowCount()) {
        // First column items are bold.
        QFont font;
        font.setItalic(true);
        return font;
    }

//    QString displayVal=0;
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() <=7)
        {
            return model->record(index.row()).value(index.column()).toString();
        }

//        else if (index.column() == 6)
//        {
//            return voucher->empList[index.row()]->Halfday;
//        }
    }
    //    if(index.column() == 1)
    //        qDebug()<<"failed at check role "<<index.column()<<" role : "<<role << Qt::DisplayRole << Qt::EditRole;
    // FIXME: Implement me!
//    qDebug()<<"data";
    return QVariant();
}

bool GMReconciliationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        qDebug()<<"Setting Data row count : "<< index.row();

//    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



//        if(row == model->rowCount() && index.column() == 1){
//            //Create New
//            //            qDebug()<<"Append";
//            AttendanceDataModel *ledger;
//            ledger = value.value<AttendanceDataModel *>();
//            beginInsertRows(QModelIndex(),model->rowCount(),model->rowCount());
//            voucher->empList.append(ledger);
//            //            qDebug()<<"New Item at "<<voucher.size()<< " is "<<
////            voucher[voucher.size()-1]->EmpId;

//            //            this->insertRow(voucher.size(),QModelIndex());
//            qDebug()<<"New Line Added";
//            newLineAddedFlag = true;
//            endInsertRows();

//        }
         if (row< model->rowCount() && row >=0 ){

           /* if (index.column() == 1){
                AttendanceDataModel *ledger;
                ledger = value.value<AttendanceDataModel *>();
                voucher->empList[row] =  ledger;
            }
            else*/

             // from here

//             if (index.column() == 2)
//            {
//                voucher->empList[row]->StartTime = value.toDateTime();

//                voucher->empList[row]->WorkingHours = voucher->empList[row]->StartTime.secsTo(voucher->empList[row]->EndTime)/3600;

//                voucher->empList[row]->Overtime = voucher->empList[row]->StartTime.secsTo(voucher->empList[row]->EndTime)/3600 - voucher->empList[row]->employee.WorkingHours;
//                if(voucher->empList[row]->Overtime <0)
//                    voucher->empList[row]->Overtime=0;
//            }
//            else if (index.column() == 3)
//            {
//                voucher->empList[row]->EndTime = value.toDateTime();

//                voucher->empList[row]->WorkingHours = voucher->empList[row]->StartTime.secsTo(voucher->empList[row]->EndTime)/3600;

//                voucher->empList[row]->Overtime = voucher->empList[row]->StartTime.secsTo(voucher->empList[row]->EndTime)/3600 - voucher->empList[row]->employee.WorkingHours;
//                if(voucher->empList[row]->Overtime <0)
//                    voucher->empList[row]->Overtime=0;
//            }
//            else if (index.column() == 4)
//            {
//                 voucher->empList[row]->WorkingHours = value.toInt();
//            }
//            else if (index.column() == 5)
//            {
//                voucher->empList[row]->Overtime = value.toInt();
//            }
//            else if (index.column() == 6)
//            {
//                voucher->empList[row]->Halfday = value.toBool();
//            }

//            else
                return false;
        }
//        TransactionCalculator::VoucherCalculateLedgersTotal(voucher);
        emit dataChanged(index, index, {role});
        emit updateValues();

//        if(newLineAddedFlag)
//            emit newLineAdded(index.row());
        return true;
    }

    return false;
}

Qt::ItemFlags GMReconciliationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

//    if(index.column() == 6)
//        return   Qt::ItemIsEditable|Qt::ItemIsUserCheckable| QAbstractItemModel::flags(index) ;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;

    // FIXME: Implement me!
}

bool GMReconciliationModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"insertRows";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMReconciliationModel::removeRows(int row, int count, const QModelIndex &parent)
{

}

void GMReconciliationModel::setHeaderStrings(const QStringList &value)
{
    qDebug()<<"set header strings";
    headerStrings = value;
}

void GMReconciliationModel::setModel(QSqlQueryModel *model)
{
    qDebug()<<"setVoucher";
    beginResetModel();
    this->model = model;
    endResetModel();
    //    qDebug()<<"Model Reset";
}
