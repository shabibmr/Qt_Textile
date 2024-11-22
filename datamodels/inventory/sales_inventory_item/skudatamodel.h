#ifndef SKUDATAMODEL_H
#define SKUDATAMODEL_H

#include <QObject>
#include <QVariant>

class SKUDataModel : public QObject
{
    Q_OBJECT
public:
    explicit SKUDataModel(QObject *parent = nullptr);
    ~SKUDataModel();

    int _id;
    QString skuID;
    QString itemID;
    QString skuName;
    QString skuDescription;
    int skuType;
    QString skuProperty_ID;
    QStringList possibleValues;
    QVariant skuValue;
    QString SKUBarcode;
    int skuUom;

signals:

};

#endif // SKUDATAMODEL_H
