#ifndef MEASUREMENTMASTER_H
#define MEASUREMENTMASTER_H

#include "measurementlabel.h"
#include <QObject>
class MeasurementMaster
{
public:
    MeasurementMaster();
    int _id;
    QString measurementMasterID;
    QString measurementName;
    QList<MeasurementLabel> lables;

};

#endif // MEASUREMENTMASTER_H
