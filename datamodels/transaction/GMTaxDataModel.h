#ifndef GMTAXDATAMODEL_H
#define GMTAXDATAMODEL_H
#include <QString>

class GMTaxClassModel{
public:
    QString ledgerID;
    QString ledgerName;

    float crPercent;
    float crAddendum;

    float crTotalAmount;

    float drPercent;
    float drAddendum;

    float drTotalAmount;

    float amount;

};

#endif // GMTAXDATAMODEL_H
