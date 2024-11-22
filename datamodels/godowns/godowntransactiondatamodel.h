#ifndef GODOWNTRANSACTIONDATAMODEL_H
#define GODOWNTRANSACTIONDATAMODEL_H

#include <QString>
#include "datamodels/transaction/itemfifodatamodel.h"

class GodownTransactionDataModel
{
public:
    GodownTransactionDataModel();


    QString fromGodown = "OUT";
    QString toGodown = "OUT";

    float crQty=0;
    float drQty=0;
    float enteredQty =0;

    float crAmount=0;
    float drAmount=0;

    QList<ItemFifoDataModel*> fifoList;


};

#endif // GODOWNTRANSACTIONDATAMODEL_H
