#include "termsdatabasehelper.h"

TermsDatabaseHelper::TermsDatabaseHelper(DatabaseHelper *db)
{
    this->db = db;
}

int TermsDatabaseHelper::startTransaction()
{
    db->startTransaction();
    
}

int TermsDatabaseHelper::insertTerms(QMap<QString, QVariant> obj)
{
    this->startTransaction();

    db->insertRecordsPlus(obj,Terms_TableName);
    return commit();
}

int TermsDatabaseHelper::updateTerms(QMap<QString, QVariant> obj, int oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Terms_id,oldID);
    db->updateRecordsPlus(obj,map,Terms_TableName);
    return commit();
}

int TermsDatabaseHelper::deleteTerms(int oldID)
{
    this->startTransaction();
    QMap<QString,QVariant> map;
    map.insert(Terms_id,oldID);
    db->deleteRecordsPlus(map,Terms_TableName);
    return commit();
    
}

int TermsDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else
    {
        db->rollBackTransaction();
        return 0;
    }
}

QList<QMap<QString, QString> > TermsDatabaseHelper::getAllTerms()
{
    QList<QMap<QString, QString> > terms;

    QString qStr ="SELECT "
            +Terms_id +", "
            + Terms_title +", "
            + Terms_validity
            + " FROM "
            + Terms_TableName;

    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            QMap<QString,QString> map;
            map.insert(query.value(0).toString()+"|"+query.value(1).toString(),
                       query.value(2).toString());
            terms.append(map);
        }
    }
    else{

    }



    return terms;
}

QMap<QString, QString> TermsDatabaseHelper::getTermByid(int id)
{
    QMap<QString,QString> map;
    QString qStr = " SELECT "
            +Terms_title +", "
            +Terms_validity +", "
            +Terms_warranty +", "
            +Terms_delivery +", "
            +Terms_payment +", "
            +Terms_exclusion +", "
            +Terms_extra +" "
            + " FROM "
            + Terms_TableName +
            " WHERE "+ Terms_id + "='" + QString::number(id)+"'";


            ;


    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    if(query.exec(qStr)){
        while(query.next()){
            map.insert(Terms_title,query.value(0).toString());
            map.insert(Terms_validity,query.value(1).toString());
            map.insert(Terms_warranty,query.value(2).toString());
            map.insert(Terms_delivery,query.value(3).toString());
            map.insert(Terms_payment,query.value(4).toString());
            map.insert(Terms_exclusion,query.value(5).toString());
            map.insert(Terms_extra,query.value(6).toString());

        }
    }
    else{

    }
    return map;
}
