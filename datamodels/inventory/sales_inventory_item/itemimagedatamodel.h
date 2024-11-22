#ifndef ITEMIMAGEDATAMODEL_H
#define ITEMIMAGEDATAMODEL_H

#include <QString>


class ItemImageDatamodel
{
public:
    ItemImageDatamodel();

    QString imageFilename ="";
    QString itemID="";
    int imageWidth=0;
    int imageHeight=0;
    QString imageNarration;

    QByteArray img;
    QString fileType;
    QString qVoucherNo;
    QString requirementID;
    QString projectID;
    QString addedBy;
};

#endif // ITEMIMAGEDATAMODEL_H
