#include "bulkloaddatabasehelper.h"


#include <QMessageBox>
#include <QSqlError>
#include <QFile>
#include <QtMath>

QJsonArray* BulkLoadDatabaseHelper::logJson;
QJsonObject BulkLoadDatabaseHelper::x;
QString BulkLoadDatabaseHelper::clientId="Y";
bool BulkLoadDatabaseHelper::transFlag=true;
QString BulkLoadDatabaseHelper::errMSG="";

BulkLoadDatabaseHelper::BulkLoadDatabaseHelper(QString selectString)
{
    db = QSqlDatabase::database(selectString);
    clientId = DatabaseValues::ClientID;
}

BulkLoadDatabaseHelper::~BulkLoadDatabaseHelper()
{
    db.close();
    //    qDebug()<<" delete Main DataBase Helper";
}

void BulkLoadDatabaseHelper::startTransaction()
{
    // this->createConnection();
    logJson = new QJsonArray;
    transFlag=true;
    errMSG ="";
    //    clearJsonArray(logJson);

    db.transaction();

    //    qDebug()<<"start trans"<<transFlag;
}

bool BulkLoadDatabaseHelper::rollBackTransaction()
{
    if(db.rollback())
        return true;
    else
        return false;
}

bool BulkLoadDatabaseHelper::commitTransaction()
{


    //    transFlag = true;   // remove this for transaction

    if( transFlag ){
        //        qDebug()<<"Insert log"<<transFlag;
        QJsonDocument doc(*logJson);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        QSqlQuery query(db);

        QString qStr1 = "INSERT INTO `log` (`data`,`action`,`status`,`client_id`) values ('"+strJson+"','1001','insert','"+clientId+"')";
        //        qDebug()<<qStr1;
        if(!query.exec(qStr1)){
            logErr += "Error Code: 505 log " ;
            qDebug()<<"LOG Insert Error";
            qDebug()<<query.lastError();
            transFlag = false;   // uncomment for trans
        }
        if(transFlag)
        {
            //            qDebug()<<"DB COMMIT b4";
            db.commit();
            //            qDebug()<<"DB COMMIT";
            //            if(logJson!=NULL)
            delete logJson;
            return true;
        }
        else
            return !rollBackTransaction();
    }
    else{
        QMessageBox  box;
        box.setText("Entry NOT SAVED\n"+errMSG);
        box.exec();

        qDebug()<<"Flag is false";
        return !rollBackTransaction();
    }
}

qint64 BulkLoadDatabaseHelper::createInsertQuery(QMap<QString, QVariant> map, QString tablename)
{

    QString fileName = tablename + ".txt";
    QString colString="";
    QString dataString="";
    QMapIterator<QString, QVariant> i(map);
    QJsonObject x;
    QSqlQuery query(db);

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

        colString += "`"+colname+"`,";
        //        QString bindname = ":"+colname+",";
        //        dataString += bindname;
        QString colValue ;

        if(value.userType() == QMetaType::Bool){
            colValue =  i.value().toString() + ", ";
        }
        else if(value.userType() == QMetaType::Int){
            colValue = QString::number(i.value().toInt()) + ", ";
        }
        else if(value.userType() == QMetaType::Float)
            colValue =  QString::number(i.value().toFloat()) + ", ";
        else if(value.userType() == QMetaType::QDate){
            colValue = "'" + i.value().toDate().toString("yyyy-MM-dd") + "', ";
            if(i.value().toDate().toString("yyyy-MM-dd") == ""){
                QMessageBox box; box.setText("Blank found" + i.key() + tablename); box.exec();
            }
        }
        else if(value.userType() == QMetaType::QDateTime){
            colValue = "'" + i.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss") + "', ";
        }
        else if(value.userType() == QMetaType::QTime){
            colValue = "'" + i.value().toTime().toString("hh:mm:ss") + "', ";
        }
        else
        {
             colValue = "'" + escapeJson( i.value().toString() )+ "', ";

        }

        dataString += colValue;
        //dataString += "'"+value+"',";
        //        x[colname]=value.value<QString>();

    }
    dataString.chop(2);
    colString.chop(1);



    QString qStr;

//    QStringList cols;
//    cols.append("INSERT INTO "+tablename+" ("+colString+") values ");

    QString cols = "INSERT INTO "+tablename+" ("+colString+") values ";

    if(dataList.contains(tablename)){
        if(dataList[tablename].size() % qFloor(65535/map.size()) > 0)   // current pos % max allowed > 0
//        if(dataList[tablename].size() % 5 > 0)      //trial, to be removed - FR
            dataList[tablename].append("("+dataString+"),");
        else{
            dataList[tablename].append("("+dataString+");");
            dataList[tablename].append(cols);
        }
    }
    else {

        dataList.insert(tablename, cols);
        dataList[tablename].append("("+dataString+"),");
    }

    //    dataList[tablename][dataList[tablename].size()-1].chop(1);

}

QMap<QString, QVariant> BulkLoadDatabaseHelper::runDML(QString qStr, QString ls)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QSqlQuery query(QSqlDatabase::database(ls));
    QMap<QString, QVariant> map;

    // Make changes to add clause
    if(query.exec(qStr)){
        map.insert("SUCCESS", true);
    }
    else{
        transFlag = false;
        QMessageBox box; box.setText("Error  \n" + query.lastError().text());
        box.setTextInteractionFlags(Qt::TextSelectableByMouse);
        box.exec();
        qDebug()<<"Query Error";
        qDebug()<<"error query size"<<query.lastQuery().size();
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
        map.insert("SUCCESS", false);
        map.insert("Error", query.lastError().text());
    }

    return map;
}

void BulkLoadDatabaseHelper::writeQueryFiles()
{

    QMapIterator<QString, QString> i(dataList);
    while (i.hasNext()) {
        i.next();
        QString fileName = i.key() + ".txt";
//        QStringListdata = i.value();

        QString qStr = i.value();
//        for(QString entry:data)
//            qStr += entry;
        qStr.chop(1);
        QFile File(fileName);
        File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        File.write(qStr.toUtf8());
        File.close();


    }
}

bool BulkLoadDatabaseHelper::runStoredQueries()
{
//    startTransaction();

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QMapIterator<QString, QString> i(dataList);
    int cnt = 0;
    while (i.hasNext()) {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"in loop"<<cnt++;
        i.next();
        QString tablename = i.key();
//        QStringList data = i.value();

        QString qStr = i.value();
//        for(QString entry:data)
//            qStr += entry;
        qStr.chop(1);

         QMap<QString, QVariant> status = runDML(qStr);

        if( ! status["SUCCESS"].toBool()){
            qDebug()<<"insert query failed for "<<i.key();
            transFlag = false;
            break;
        }

    }

    return commitTransaction();
}

bool BulkLoadDatabaseHelper::readQueryFiles()
{
//    startTransaction();

    QMapIterator<QString, QString> i(dataList);
    while (i.hasNext()) {
        i.next();
        QString fileName = i.key() + ".txt";

        QFile file(fileName);
        file.open(QIODevice::ReadOnly);

        QString data = file.readAll();

        QString queries = data;

        QMap<QString, QVariant> status = runDML(queries);

        if( ! status["SUCCESS"].toBool()){
            qDebug()<<"insert query failed for "<<i.key();
            transFlag = false;
            break;
        }

    }

    return commitTransaction();


}

QString BulkLoadDatabaseHelper::escapeJson(QString jsonString)
{
    //    jsonString.replace("\"", "\\\"");

    //    qDebug()<<"666666666"<<jsonString;
    jsonString.replace("\\", "\\\\");
    jsonString.replace("'", "&apos;");
    jsonString.replace("/", "\\/");
    jsonString.replace("\b", "\\b");
    jsonString.replace("\f", "\\f");
    jsonString.replace("\n", "\\n");
    jsonString.replace("\r", "\\r");
    jsonString.replace("\t", "\\t");
    //qDebug()<<"666666666"<<jsonString;

    return jsonString;
}
