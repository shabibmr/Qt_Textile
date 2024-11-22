#ifndef DOCUMENTSDATAMODEL_H
#define DOCUMENTSDATAMODEL_H
#include <QString>
#include <QDateTime>


class DocumentsDataModel
{
public:
    DocumentsDataModel();

    int id=0;
    QString filename;
    QString fileType;
    QString fileDescr;
    QByteArray fileObj;
    QString quoteNo;
    QString ReqID;
    QString projectID;
    int addedByID;
    QDateTime timestamp;
    QString TagType;
    QString Searchtag;

    int status;
};

#endif // DOCUMENTSDATAMODEL_H
