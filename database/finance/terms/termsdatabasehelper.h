#ifndef TERMSDATABASEHELPER_H
#define TERMSDATABASEHELPER_H

#include "database/databasehelper/databasehelper.h"

#include <QMap>
#include <QDebug>



class TermsDatabaseHelper
{
public:
    TermsDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());

    QString Terms_TableName ="terms_and_conditions";
    QString Terms_id="_id";
    QString Terms_title="Title";
    QString Terms_validity ="Validity";
    QString Terms_warranty="Warranty";
    QString Terms_delivery="Delivery_Time";
    QString Terms_payment="Payment_Terms";
    QString Terms_exclusion="Exclusion";
    QString Terms_extra="Extra";

    bool flag = true;
    DatabaseHelper *db;


    int startTransaction();


    int insertTerms(QMap<QString,QVariant> obj);

    int updateTerms(QMap<QString,QVariant> obj, int oldID);

    int deleteTerms(int oldID);

    int commit();

    QList<QMap<QString,QString>> getAllTerms();

    QMap<QString,QString> getTermByid(int id);

};

#endif // TERMSDATABASEHELPER_H
