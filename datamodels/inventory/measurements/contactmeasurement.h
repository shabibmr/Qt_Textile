#ifndef CONTACTMEASUREMENT_H
#define CONTACTMEASUREMENT_H

// #include "datamodels/addressbook/addressbookdatamodel.h"
#include "measurementmaster.h"
#include "contactmeasurementlable.h"
// #include <QMap>
#include <QString>
#include <QVariant>
class ContactMeasurement
{
public:
    ContactMeasurement();

    int _id;

    // AddressBookDataModel *contact;
    QString contactID;
    QString contactMeasureID;
    QString reference;
    MeasurementMaster *measurement;
    QList<ContactMeasurementLable> lables;
    QMap<QString,QVariant> valueMap;

};

#endif // CONTACTMEASUREMENT_H
