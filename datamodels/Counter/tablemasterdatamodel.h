#ifndef TABLEMASTERDATAMODEL_H
#define TABLEMASTERDATAMODEL_H
#include <QObject>
class TableMasterDataModel
{

public:
    TableMasterDataModel();

    int _id=-1;
    QString tableID;
    QString tableName;
    int capacity=4;

    int maxCapacity;
    int min;
    QString floor;
};

#endif // TABLEMASTERDATAMODEL_H
