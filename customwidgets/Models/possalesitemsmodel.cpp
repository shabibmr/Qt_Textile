#include "possalesitemsmodel.h"
#include <QtMath>
#include <QtDebug>

POSSalesItemsModel::POSSalesItemsModel(QSqlQueryModel *queryModel,
                                       int ColumnCnt,
                                       QObject *parent)
    : QAbstractItemModel(parent)
{
    listSize= queryModel->rowCount();
    ColCount = ColumnCnt;
    model = queryModel;
    RowCount = qCeil( model->rowCount() / ColCount) + 1 ;

    qDebug()<<"SIze : " <<listSize;
}

POSSalesItemsModel::~POSSalesItemsModel()
{

}

QModelIndex POSSalesItemsModel::index(int row, int column, const QModelIndex &parent) const
{
    if( row*ColCount+column < listSize ){
        return  this->createIndex( row , column );
    }
    return QModelIndex();
}

QModelIndex POSSalesItemsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int POSSalesItemsModel::rowCount(const QModelIndex &parent) const
{
    return RowCount;
}

int POSSalesItemsModel::columnCount(const QModelIndex &parent) const
{
    return ColCount;
}

QVariant POSSalesItemsModel::data(const QModelIndex &index, int role) const
{
    if(!checkIndex(index))
    {
        return QVariant();
    }
    int  row = index.row();
    int column = index.column();

    if (!index.isValid())
        return QVariant();



    int modelindex = row * ColCount + column;

    if(role ==Qt::UserRole){
        return model->record(modelindex).value(0).toString();
    }
    if(role == Qt::DisplayRole || role == Qt::EditRole){
        //        qDebug()<<"model index : "<<modelindex;
        if( modelindex < listSize)
            return model->record(modelindex).value(1).toString();
    }
    //    if( modelindex < listSize){
    //        return  this->createIndex(row,column);
    //    }
    return QVariant();
}

bool POSSalesItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}


Qt::ItemFlags POSSalesItemsModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsEnabled ;
}
