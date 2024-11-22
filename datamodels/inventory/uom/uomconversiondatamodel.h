#ifndef UOMCONVERSIONDATAMODEL_H
#define UOMCONVERSIONDATAMODEL_H

#include <QString>
#include <QMetaType>
#include <QVariant>
#include <QObject>


class UomConversionDataModel
{

public:
    UomConversionDataModel();


    QString _id;
    QString BaseUnit;
    QString ToUnit;

    float conValue;
    QString itemID;
    QString Narration;
    QString barCode;

};

Q_DECLARE_METATYPE(UomConversionDataModel)

#endif // UOMCONVERSIONDATAMODEL_H
