#include "gmpayslipmodel.h"



GMPayslipModel::GMPayslipModel(PayslipDataModel *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col;

    qDebug()<<"model";

}

QVariant GMPayslipModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    qDebug()<<"headerData";
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMPayslipModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->empList.size() && column< colCount){
        if((row == voucher->empList.size() && column <= 1 )|| row < voucher->empList.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMPayslipModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMPayslipModel::rowCount(const QModelIndex &parent) const
{
//    qDebug()<<"rowcount";
    return voucher->empList.size();
}

int GMPayslipModel::columnCount(const QModelIndex &parent) const
{
//    qDebug()<<"columnCount";
    return  colCount;
}

QVariant GMPayslipModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<"data";

//    qDebug()<<"%%%%%%%%%%%%%%%"<<voucher->empList.size();

    if(!index.isValid())
        return QVariant();

    if (index.row() > voucher->empList.size() || index.row() < 0)
        return QVariant();

//    if(index.row() == voucher->empList.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
//    {
//        qDebug()<<"1111";
//        if(index.column()==0){
//            return  index.row()+1;
//        }
//        else if(index.column()==1){
//            return "";
//        }
//        return "";
//    }

//    if(index.column() == 6 && role == Qt::CheckStateRole )
//    {
//        if(voucher->empList[index.row()]->Halfday)
//            return Qt::Checked;
//        else
//            return  Qt::Unchecked;
//    }

    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
            index.row() == voucher->empList.size()) {
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
        else if (index.column() == 1)
        {
            return voucher->empList[index.row()]->EmployeeName;
        }

        else if (index.column() == 2)
        {

            return voucher->empList[index.row()]->NumOfDaysWorked;
        }
        else if (index.column() == 3)
        {
            return voucher->empList[index.row()]->TotalLeave;
        }
        else if (index.column() == 4)
        {
            return voucher->empList[index.row()]->WorkingHours;
        }
        else if (index.column() == 5)
        {
            return voucher->empList[index.row()]->overTimeHours;
        }
        else if (index.column() == 6)
        {
            return voucher->empList[index.row()]->BasicSalary;
        }
        else if (index.column() == 7)
        {
            return voucher->empList[index.row()]->HRA;
        }
        else if (index.column() == 8)
        {
            return voucher->empList[index.row()]->DA;
        }
        else if (index.column() == 9)
        {
            return voucher->empList[index.row()]->Conveyance;
        }
        else if (index.column() == 10)
        {
            return voucher->empList[index.row()]->overTimePay;
        }
        else if (index.column() == 11)
        {
            return voucher->empList[index.row()]->SalaryDeducted;
        }
        else if (index.column() == 12)
        {
            return voucher->empList[index.row()]->totalSalary;
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

bool GMPayslipModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        qDebug()<<"Setting Data row count : "<< index.row();

//    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



//        if(row == voucher->empList.size() && index.column() == 1){
//            //Create New
//            //            qDebug()<<"Append";
//            AttendanceDataModel *ledger;
//            ledger = value.value<AttendanceDataModel *>();
//            beginInsertRows(QModelIndex(),voucher->empList.size(),voucher->empList.size());
//            voucher->empList.append(ledger);
//            //            qDebug()<<"New Item at "<<voucher.size()<< " is "<<
////            voucher[voucher.size()-1]->EmpId;

//            //            this->insertRow(voucher.size(),QModelIndex());
//            qDebug()<<"New Line Added";
//            newLineAddedFlag = true;
//            endInsertRows();

//        }
         if (row< voucher->empList.size() && row >=0 ){

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

Qt::ItemFlags GMPayslipModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

//    if(index.column() == 6)
//        return   Qt::ItemIsEditable|Qt::ItemIsUserCheckable| QAbstractItemModel::flags(index) ;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;

    // FIXME: Implement me!
}

bool GMPayslipModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"insertRows";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMPayslipModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i =0;i<count;i++)
        voucher->empList.removeAt(i);
    endRemoveRows();
}

void GMPayslipModel::setHeaderStrings(const QStringList &value)
{
    qDebug()<<"set header strings";
    headerStrings = value;
}

void GMPayslipModel::setVoucher(PayslipDataModel *value)
{
    qDebug()<<"setVoucher";
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}
