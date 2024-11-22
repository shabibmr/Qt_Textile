#include "gmaccesscontrolmodel.h"

//GMAccessControlModel::GMAccessControlModel()
//{
//}

GMAccessControlModel::GMAccessControlModel(QSqlQueryModel *qModel, UserGroupDataModel *model,
                                           QObject *parent)
    :QAbstractItemModel(parent)
{
    this->userGroup = model;
    this->model = qModel;
    this->colCount = 9;
    this->module = module;
}

QVariant GMAccessControlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        return headerStrings[section];
    }
    return QVariant();
}

QModelIndex GMAccessControlModel::index(int row, int column, const QModelIndex &parent) const
{
    if(row<=model->rowCount() && column< colCount){
        if((row == model->rowCount() && column <= 1 )|| row < model->rowCount())
            return this->createIndex(row,column);
    }

    return QModelIndex();
}

QModelIndex GMAccessControlModel::parent(const QModelIndex &index) const
{
    return QModelIndex();

}

int GMAccessControlModel::rowCount(const QModelIndex &parent) const
{
    return model->rowCount();
}

int GMAccessControlModel::columnCount(const QModelIndex &parent) const
{
    return colCount;
}

QVariant GMAccessControlModel::data(const QModelIndex &index, int role) const
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<model->record(index.row()).value(3).toString()<<module;
    //    if( model->record(index.row()).value(3).toString().contains(module) ){
    //        qDebug()<<Q_FUNC_INFO<<__LINE__<<"unfiltered";
    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::UserRole+1) {
//        qDebug()<<model->record(index.row()).value(3).toString();
        return model->record(index.row()).value(3).toString();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return model->record(index.row()).value(1).toString();
        }
        else if (index.column() == 2)
        {
            return model->record(index.row()).value(2).toString().split('|').at(1);
        }
    }

    if(role == Qt::CheckStateRole && index.column() >2 & index.column() <=8)
    {

        int row = findPermissionsForUi(model->record(index.row()).value(0).toInt(),
                                       model->record(index.row()).value(3).toString());
        if(row >= 0){
            if(index.column() == 3){
                if(userGroup->permissions[row].allowCreate)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
            if(index.column() == 4){
                if(userGroup->permissions[row].allowRead)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
            if(index.column() == 5){
                if(userGroup->permissions[row].allowUpdate)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
            if(index.column() == 6){
                if(userGroup->permissions[row].allowDelete)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
            if(index.column() == 7){
                if(userGroup->permissions[row].allowShare)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
            if(index.column() == 8){
                if(userGroup->permissions[row].allowCreate &&
                        userGroup->permissions[row].allowRead &&
                        userGroup->permissions[row].allowUpdate &&
                        userGroup->permissions[row].allowDelete &&
                        userGroup->permissions[row].allowShare)
                    return Qt::Checked;
                else
                    return  Qt::Unchecked;
            }
        }
        else {
            return Qt::Unchecked;
        }
    }
    //    }

    return QVariant();
}

Qt::ItemFlags GMAccessControlModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    //    if(index.column() == 2)
    //        return   Qt::ItemIsEditable|Qt::ItemIsUserCheckable| QAbstractItemModel::flags(index) ;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}

bool GMAccessControlModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if (index.isValid() && role == Qt::EditRole) {
        int row = findPermissionsForUi(model->record(index.row()).value(0).toInt(),
                                       model->record(index.row()).value(3).toString());
        if(row == -1 ){
            UiAccessControlDataModel perm;
            perm.ui_id = model->record(index.row()).value(0).toInt();
            perm.UiName = model->record(index.row()).value(1).toString();
            userGroup->permissions.append(perm);
            row = userGroup->permissions.size()-1;
        }
        if (index.column() == 3)
        {
            userGroup->permissions[row].allowCreate = value.toBool();
        }
        else if (index.column() == 4)
        {
            userGroup->permissions[row].allowRead = value.toBool();
        }
        else if (index.column() == 5)
        {
            userGroup->permissions[row].allowUpdate = value.toBool();
        }
        else if (index.column() == 6)
        {
            userGroup->permissions[row].allowDelete = value.toBool();
        }
        else if (index.column() == 7)
        {
            userGroup->permissions[row].allowShare = value.toBool();
        }
        else if (index.column() == 8)
        {
            userGroup->permissions[row].allowCreate = value.toBool();
            userGroup->permissions[row].allowRead = value.toBool();
            userGroup->permissions[row].allowUpdate = value.toBool();
            userGroup->permissions[row].allowDelete = value.toBool();
            userGroup->permissions[row].allowShare = value.toBool();
        }


        return true;
    }

    return false;
}

void GMAccessControlModel::setHeaderStrings(const QStringList &value)
{
    headerStrings = value;
}

void GMAccessControlModel::setUserGroup(UserGroupDataModel *value)
{
    userGroup = value;
}

int GMAccessControlModel::findPermissionsForUi(int ui_id, QString module) const
{
    for(int i=0; i< userGroup->permissions.size(); i++){
        if(userGroup->permissions[i].ui_id == ui_id){
            userGroup->permissions[i].module = module;
            return i;
        }
    }

    return -1;
}

void GMAccessControlModel::resetModel()
{
    beginResetModel();
    endResetModel();
}


