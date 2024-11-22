#include "gmemployeemodel.h"



GMEmployeeModel::GMEmployeeModel(AttendanceListDataModel *voucher, int col, QObject *parent)
    : QAbstractItemModel(parent)
{
    this->voucher = voucher;
    this->colCount = col;

    qDebug()<<"model";

}

QVariant GMEmployeeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    qDebug()<<"headerData";
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();

}

QModelIndex GMEmployeeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=voucher->attendanceList.size() && column< colCount){
        if((row == voucher->attendanceList.size() && column <= 1 )|| row < voucher->attendanceList.size())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMEmployeeModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int GMEmployeeModel::rowCount(const QModelIndex &parent) const
{
//    qDebug()<<"rowcount";
    return voucher->attendanceList.size();
}

int GMEmployeeModel::columnCount(const QModelIndex &parent) const
{
//    qDebug()<<"columnCount";
    return  colCount;
}

QVariant GMEmployeeModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<"data";

//    qDebug()<<"%%%%%%%%%%%%%%%"<<voucher->attendanceList.size();

    if(!index.isValid())
        return QVariant();

    if (index.row() > voucher->attendanceList.size() || index.row() < 0)
        return QVariant();

//    if(index.row() == voucher->attendanceList.size() && (role == Qt::DisplayRole || role == Qt::EditRole))
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

    if(index.column() == 6 && role == Qt::CheckStateRole )
    {
        if(voucher->attendanceList[index.row()]->Halfday)
            return Qt::Checked;
        else
            return  Qt::Unchecked;
    }

    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::FontRole && index.column() == 1 &&
            index.row() == voucher->attendanceList.size()) {
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
            qDebug()<<voucher->attendanceList[index.row()]->employee.EmployeeName<<voucher->attendanceList[index.row()]->employee.EmployeeID;
            return voucher->attendanceList[index.row()]->employee.EmployeeName;
        }

        else if (index.column() == 2)
        {

            return voucher->attendanceList[index.row()]->StartTime.toString("yyyy-MM-dd hh:mm:ss");
        }
        else if (index.column() == 3)
        {
            return voucher->attendanceList[index.row()]->EndTime.toString("yyyy-MM-dd hh:mm:ss");
        }
        else if (index.column() == 4)
        {
            return voucher->attendanceList[index.row()]->WorkingHours;
        }
        else if (index.column() == 5)
        {
            return voucher->attendanceList[index.row()]->Overtime;
        }
//        else if (index.column() == 6)
//        {
//            return voucher->attendanceList[index.row()]->Halfday;
//        }
    }
    //    if(index.column() == 1)
    //        qDebug()<<"failed at check role "<<index.column()<<" role : "<<role << Qt::DisplayRole << Qt::EditRole;
    // FIXME: Implement me!
//    qDebug()<<"data";
    return QVariant();
}

bool GMEmployeeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
        qDebug()<<"Setting Data row count : "<< index.row();

//    bool newLineAddedFlag =false;
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();



//        if(row == voucher->attendanceList.size() && index.column() == 1){
//            //Create New
//            //            qDebug()<<"Append";
//            AttendanceDataModel *ledger;
//            ledger = value.value<AttendanceDataModel *>();
//            beginInsertRows(QModelIndex(),voucher->attendanceList.size(),voucher->attendanceList.size());
//            voucher->attendanceList.append(ledger);
//            //            qDebug()<<"New Item at "<<voucher.size()<< " is "<<
////            voucher[voucher.size()-1]->EmpId;

//            //            this->insertRow(voucher.size(),QModelIndex());
//            qDebug()<<"New Line Added";
//            newLineAddedFlag = true;
//            endInsertRows();

//        }
         if (row< voucher->attendanceList.size() && row >=0 ){

           /* if (index.column() == 1){
                AttendanceDataModel *ledger;
                ledger = value.value<AttendanceDataModel *>();
                voucher->attendanceList[row] =  ledger;
            }
            else*/
             if (index.column() == 2)
            {
                voucher->attendanceList[row]->StartTime = value.toDateTime();

//                voucher->attendanceList[row]->WorkingHours = voucher->attendanceList[row]->StartTime.secsTo(voucher->attendanceList[row]->EndTime)/3600;

                voucher->attendanceList[row]->Overtime = voucher->attendanceList[row]->StartTime.secsTo(voucher->attendanceList[row]->EndTime)/3600 - voucher->attendanceList[row]->employee.WorkingHours;
                if(voucher->attendanceList[row]->Overtime <0)
                    voucher->attendanceList[row]->Overtime=0;
            }
            else if (index.column() == 3)
            {
                voucher->attendanceList[row]->EndTime = value.toDateTime();
                qDebug()<<voucher->attendanceList[row]->WorkingHours;
//                voucher->attendanceList[row]->WorkingHours = voucher->attendanceList[row]->StartTime.secsTo(voucher->attendanceList[row]->EndTime)/3600;
                qDebug()<<"working hours changed";
                voucher->attendanceList[row]->Overtime = voucher->attendanceList[row]->StartTime.secsTo(voucher->attendanceList[row]->EndTime)/3600 - voucher->attendanceList[row]->employee.WorkingHours;
                if(voucher->attendanceList[row]->Overtime <0)
                    voucher->attendanceList[row]->Overtime=0;
            }
            else if (index.column() == 4)
            {
                 voucher->attendanceList[row]->WorkingHours = value.toInt();
                 voucher->attendanceList[row]->EndTime = voucher->attendanceList[row]->StartTime.addSecs(voucher->attendanceList[row]->WorkingHours * 3600);
            }
            else if (index.column() == 5)
            {
                voucher->attendanceList[row]->Overtime = value.toInt();
//                voucher->attendanceList[row]->WorkingHours += value.toInt();
                qDebug()<<"Overtime changed"<<voucher->attendanceList[row]->Overtime;
            }
            else if (index.column() == 6)
            {
                voucher->attendanceList[row]->Halfday = value.toBool();
            }

            else
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

Qt::ItemFlags GMEmployeeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

//    if(index.column() == 6)
//        return   Qt::ItemIsEditable|Qt::ItemIsUserCheckable| QAbstractItemModel::flags(index) ;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;

    // FIXME: Implement me!
}

bool GMEmployeeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug()<<"insertRows";
    beginInsertRows(parent, row, row + count - 1);
    // FIXME: Implement me!

    endInsertRows();
}

bool GMEmployeeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i =0;i<count;i++)
        voucher->attendanceList.removeAt(i);
    endRemoveRows();
}

void GMEmployeeModel::setHeaderStrings(const QStringList &value)
{
    qDebug()<<"set header strings";
    headerStrings = value;
}

void GMEmployeeModel::setVoucher(AttendanceListDataModel *value)
{
    qDebug()<<"setVoucher";
    beginResetModel();
    voucher = value;
    endResetModel();
    //    qDebug()<<"Model Reset";
}
