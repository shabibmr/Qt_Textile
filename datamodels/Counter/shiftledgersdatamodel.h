#ifndef SHIFTLEDGERSDATAMODEL_H
#define SHIFTLEDGERSDATAMODEL_H

#include <QString>

class ShiftLedgersDataModel
{
public:
    ShiftLedgersDataModel();

    QString ledgerID;
    QString ledgerName;
    float systemAmount;
    float actualAmount;
    float difference;

};

#endif // SHIFTLEDGERSDATAMODEL_H
