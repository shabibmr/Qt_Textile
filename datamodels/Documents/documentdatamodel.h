#ifndef DOCUMENTDATAMODEL_H
#define DOCUMENTDATAMODEL_H

#include <QString>

class DocumentDataModel
{
public:
    DocumentDataModel();

    QString fileName;
    QString fileDescription;
    QString fileType;
    QString reqId;
    QString quoteNumber;
    QString projectID;
    int id;
    QByteArray file;
};

#endif // DOCUMENTDATAMODEL_H
