#include "deliveryordersmodel.h"

#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

DeliveryOrdersModel::DeliveryOrdersModel(SalesOrderDatabaseHelper *soHelper, QSqlQueryModel *model, QObject *parent )
    :QAbstractItemModel(parent)
{
    this->model = model;
    this->soHelper = soHelper;

    rowCnt = model->rowCount();
    colCnt = 13;

    //    qDebug()<<"Const";
}


QModelIndex DeliveryOrdersModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row <= this->rowCnt && column< this->colCnt){
        return this->createIndex(row,column);
    }
    return QModelIndex();
}

QModelIndex DeliveryOrdersModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int DeliveryOrdersModel::rowCount(const QModelIndex &parent) const
{
    return rowCnt;
}

int DeliveryOrdersModel::columnCount(const QModelIndex &parent) const
{
    return colCnt;
}

QVariant DeliveryOrdersModel::data(const QModelIndex &index, int role) const
{
    if(!checkIndex(index)){
        return QVariant();
    }

    int row = index.row();
    int col = index.column();
    //    qDebug()<<"Row : L "<<row;
    QVariant value = model->record(row).value(col);

    if(role == Qt::TextAlignmentRole){
        if(col == 10){
            return QVariant(Qt::AlignRight | Qt::AlignCenter);
        }
    }
    if(role == Qt::UserRole){
        return model->record(row).value(1).toString();
    }
    if(role == Qt::UserRole+1){
        return model->record(row).value(2).toString();
    }
    if(role == Qt::UserRole+2){
        return model->record(row).value(14).toInt();
    }
    if(role == Qt::UserRole+3){
        return model->record(row).value(15).toInt();
    }
    if(role == Qt::UserRole+4){
        qDebug()<<"Return Time";
        return model->record(row).value(5).toDateTime();
    }

    if(role == Qt::DisplayRole || role == Qt::EditRole){

        if(col == this->colCnt-1){
            return   "";
        }
        if(col == 5){
            return value.toDateTime().time().toString("hh:mm") + " - " + model->record(row).value(13).toTime().toString("hh:mm");
        }
        if(col == 0){
            return value.toDate().toString("dd-MMM");
        }
        if(col == 10){
            return QString::number(value.toDouble(),'f',2);
        }
        else
            return value;
    }
    return QVariant();

}


QVariant DeliveryOrdersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal){
        if(section == colCnt-2){
            return "Bill";
        }
        else  if(section == colCnt-1){
            return "KOT";
        }
        else{
            return model->headerData(section,orientation,role);
        }
    }

    return QVariant();
}


bool DeliveryOrdersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int column = index.column();
        int row = index.row();
        qDebug()<<"Editing"<<column;
        if(column == 9){
            LedgerMasterDataModel ledger;
            ledger = value.value<LedgerMasterDataModel>();
            QString ledID = ledger.LedgerID;
            QString ledName = ledger.LedgerName;
            QString vNo = model->record(row).value(1).toString();
            QString vPrefix = model->record(row).value(2).toString();
            updateLedgerID(ledID,ledName,vNo,vPrefix);
            model->query().exec();
            emit dataChanged(index, index, {role});
            return true;
        }
        else if(column == 3){
            QString vNo = model->record(row).value(1).toString();
            QString vPrefix = model->record(row).value(2).toString();
            UserProfileDataModel emp = value.value<UserProfileDataModel>();
            updateEmployee(emp._id,vNo,vPrefix);
            model->query().exec();
            emit dataChanged(index, index, {role});
            return true;
        }
        else
            return false;
    }
    return false;
}

void DeliveryOrdersModel::updateLedgerID(QString ledID,QString ledName,QString vNo,QString vPrefix)
{
    qDebug()<<"Updating Ledger"<<vNo<<vPrefix;
    soHelper->updateLedger(ledID,ledName,vNo,vPrefix);
}

void DeliveryOrdersModel::updateEmployee(int empID,QString vNo,QString vPrefix)
{
    soHelper->updateSalesMan(empID,vNo,vPrefix);
}


Qt::ItemFlags DeliveryOrdersModel::flags(const QModelIndex &index) const
{
    if(index.column()== 9 || index.column() == 3){
        return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
    }
    else{
        return  QAbstractItemModel::flags(index);
    }
}
