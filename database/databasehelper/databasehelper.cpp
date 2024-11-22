#include "databasehelper.h"
#include <QFile>

#include <QMessageBox>
#include <QSqlError>

QJsonArray* DatabaseHelper::logJson;
QJsonObject DatabaseHelper::x;
QString DatabaseHelper::clientId="Y";
bool DatabaseHelper::transFlag=true;
QString DatabaseHelper::errMSG="";
QSqlDatabase DatabaseHelper::db;
QString DatabaseHelper::logErr="";
QString DatabaseHelper::selectString;




//DatabaseHelper::DatabaseHelper(QString selectString,QObject *parent)
//    :QObject(parent)

//{
//    db = QSqlDatabase::database(selectString);
//    clientId = DatabaseValues::ClientID;
//    this->selectString=selectString;
//}

DatabaseHelper::DatabaseHelper(QString selectString)
{
    db = QSqlDatabase::database(selectString);
    clientId = DatabaseValues::ClientID;
    this->selectString=selectString;
}

DatabaseHelper::~DatabaseHelper()
{
    db.close();
    //    qDebug()<<" delete Main DataBase Helper";
}

void DatabaseHelper::startTransaction()
{
    // this->createConnection();
    logJson = new QJsonArray;
    transFlag=true;
    errMSG ="";
    //    clearJsonArray(logJson);

    db.transaction();

    //    qDebug()<<"start trans"<<transFlag;
}

void DatabaseHelper::StartTransaction()
{
    // this->createConnection();
    logJson = new QJsonArray;
    transFlag=true;
    errMSG ="";
    QSqlDatabase::database(selectString).transaction();
}

QString DatabaseHelper::getTransactionID(QString vPrefix)
{
    return QString::number(QDateTime::currentDateTime().toTime_t()) + vPrefix + clientId;
}

bool DatabaseHelper::rollBackTransaction()
{
    if(db.rollback())
        return true;
    else
        return false;
}

bool DatabaseHelper::RollBackTransaction()
{
    if(db.rollback())
        return true;
    else
        return false;
}

bool DatabaseHelper::commitTransaction()
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
        box.setWindowModality(Qt::ApplicationModal);
        box.exec();

//        qDebug()<<"Flag is false";
        return !rollBackTransaction();
    }
}

bool DatabaseHelper::CommitAlgoConfigTransaction()
{


    //    transFlag = true;   // remove this for transaction


    if(transFlag)
    {
        //            qDebug()<<"DB COMMIT b4";
        db.commit();
        //            qDebug()<<"DB COMMIT";
        //            if(logJson!=NULL)
        delete logJson;
        return true;

    }
    else{
        QMessageBox  box;
        box.setText("Entry NOT SAVED\n"+errMSG);
        box.exec();

        //        qDebug()<<"Flag is false";
        return !RollBackTransaction();
    }
}

bool DatabaseHelper::CommitTransaction()
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
            return !RollBackTransaction();
    }
    else{
        QMessageBox  box;
        box.setText("Entry NOT SAVED\n"+errMSG);
        box.exec();

//        qDebug()<<"Flag is false";
        return !RollBackTransaction();
    }
}


int DatabaseHelper::insertRecords(QMap<QString,QString> map,QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called"<<tablename;
    }

    QString colString="";
    QString dataString="";
    QMapIterator<QString, QString> i(map);
    QJsonObject x;
    QSqlQuery query(db);

    //    qDebug()<<"Map have "<<map.size() <<" records 42";
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();

        QString value = i.value();

        colString += "`"+colname+"`,";
        QString bindname = ":"+colname+",";
        dataString += bindname;

        //dataString += "'"+value+"',";
        x[colname]=value;
    }
    dataString.chop(1);
    colString.chop(1);

    //QString qStr = "INSERT INTO "+tablename+" (_id) values (:id)";

    QString qStr = "INSERT INTO "+tablename+" ("+colString+") values ("+dataString+")";

    query.prepare(qStr);

    //  query.bindValue(":id",25);

    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QString value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        // qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    QJsonObject rootObject;
    rootObject.insert("Data",DatabaseHelper::x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1001");

    logJson->append(rootObject);
    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));

    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1001','insert')";


    if(!query.exec()){
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();
        qDebug()<<"Error Inserting";
        qDebug()<<query.lastError();
        transFlag=false;
        logErr = " Error code 505 : " + tablename;
    }
    else{
        //        qDebug()<<query.lastQuery();
    }
    int n = query.lastInsertId().toInt();
    //    query.exec(qStr1);
    return n;
}

void DatabaseHelper::clearJsonArray(QJsonArray array)
{
    for(int i=0; i<array.count(); i++) {
        array.removeAt(0);
    }
}


qint64 DatabaseHelper::insertRecordsPlus(QMap<QString, QVariant> map, QString tablename)
{
//    qDebug()<<Q_FUNC_INFO<<tablename;
    if(!transFlag){
        qDebug()<<"start transaction not called"<<tablename;
        return 0;
    }

    QString colString="";
    QString dataString="";
    QMapIterator<QString, QVariant> i(map);
    QJsonObject x;
    QSqlQuery query(db);

    //    qDebug()<<"Map have "<<map.size() <<" records 106";
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

//        qDebug()<<Q_FUNC_INFO<<__LINE__<<colname<<value.userType();

        colString += "`"+colname+"`,";
        QString bindname = ":"+colname+",";
        dataString += bindname;

        if(value.userType() == QMetaType::Bool){
            x.insert(i.key(),i.value().toBool());
        }
        else if(value.userType() == QMetaType::Int){
            x.insert(i.key(),i.value().toInt());
        }
        else if(value.userType() == QMetaType::Float){
            x.insert(i.key(),i.value().toFloat());
        }
        else if(value.userType() == QMetaType::QDate){
            x.insert(i.key(),i.value().toDate().toString("yyyy-MM-dd"));
            if(i.value().toDate().toString("yyyy-MM-dd") == ""){
                //                QMessageBox box; box.setText("Blank found" + i.key() + tablename); box.exec();
            }
        }
        else if(value.userType() == QMetaType::QDateTime){
            x.insert(i.key(),i.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
        else
        {
            if(tablename == "SendChannel" ){
                qDebug()<<"send channel"<<Q_FUNC_INFO<<__LINE__<<i.value();
                x.insert(i.key(),i.value().toString());
                qDebug()<<x;
            }
            else if( tablename == "Address_Book"){
                qDebug()<<"address book"<<Q_FUNC_INFO<<__LINE__;
                x.insert(i.key(),escapeJson2(i.value().toString()));
            }
            else
                x.insert(i.key(),escapeJson(i.value().toString()));

        }


        //dataString += "'"+value+"',";
        //        x[colname]=value.value<QString>();

    }
    dataString.chop(1);
    colString.chop(1);

    //  QString qStr = "INSERT INTO "+tablename+" (_id) values (:id)";

    QString qStr = "INSERT INTO "+tablename+" ("+colString+") values ("+dataString+")";

    query.prepare(qStr);

    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();
        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        // qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    QJsonObject rootObject;
    rootObject.insert("Data",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1001");

    //    qDebug()<<"At inset 252";
    //    qDebug()<<"JSON>>"<<rootObject;
    logJson->append(rootObject);
    //    qDebug()<<logJson;

    //    qDebug()<<*logJson;

    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));

    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1001','insert')";

    qint64 n=0;

    if(!query.exec()){
        qDebug()<<"Error Inserting";
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text() + "\n" +
                DatabaseValues::connectionString;
        transFlag=false;
    }
    else{
        //        qDebug()<<query.lastQuery();
        //        qDebug()<<query.boundValues();
        n = query.lastInsertId().toLongLong();
    }

    //    query.exec(qStr1);
    return n;
}

qint64 DatabaseHelper::insertRecordsPlusStatic(QMap<QString, QVariant> map, QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called"<<tablename;
        return 0;
    }
    QString colString="";
    QString dataString="";
    QMapIterator<QString, QVariant> i(map);
    QJsonObject x;
    QSqlQuery query(db);

    //    qDebug()<<"Map have "<<map.size() <<" records 106";
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

        colString += "`"+colname+"`,";
        QString bindname = ":"+colname+",";
        dataString += bindname;

        if(value.userType() == QMetaType::Bool)
            x.insert(i.key(),i.value().toBool());
        else if(value.userType() == QMetaType::Int)
            x.insert(i.key(),i.value().toInt());
        else if(value.userType() == QMetaType::Float)
            x.insert(i.key(),i.value().toFloat());
        else if(value.userType() == QMetaType::QDate){
            x.insert(i.key(),i.value().toDate().toString("yyyy-MM-dd"));
            if(i.value().toDate().toString("yyyy-MM-dd") == ""){
                //                QMessageBox box; box.setText("Blank found" + i.key() + tablename); box.exec();
            }
        }
        else if(value.userType() == QMetaType::QDateTime){
            x.insert(i.key(),i.value().toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
        else
        {
            if(tablename == "SendChannel")
                x.insert(i.key(),i.value().toString());
            else
                x.insert(i.key(), escapeJson(i.value().toString()));
        }


        //dataString += "'"+value+"',";
        //        x[colname]=value.value<QString>();

    }
    dataString.chop(1);
    colString.chop(1);

    //QString qStr = "INSERT INTO "+tablename+" (_id) values (:id)";

    QString qStr = "INSERT INTO "+tablename+" ("+colString+") values ("+dataString+")";

    query.prepare(qStr);

    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        // qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    QJsonObject rootObject;
    rootObject.insert("Data",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1001");

    //    qDebug()<<"At inset 252";
    //    qDebug()<<"JSON>>"<<rootObject;
    logJson->append(rootObject);
    //    qDebug()<<logJson;

    //    qDebug()<<*logJson;

    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));

    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1001','insert')";

    qint64 n=0;

    if(!query.exec()){
        qDebug()<<"Error Inserting";
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
        transFlag=false;
    }
    else{
        //qDebug()<<query.lastQuery();
        n = query.lastInsertId().toLongLong();
    }

    //    query.exec(qStr1);
    return n;
}

void DatabaseHelper::updateRecords(QMap<QString,QString> data,QMap<QString,QString> clause,QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QString dataString="";
    QMapIterator<QString, QString> i(data);
    QJsonObject x;
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QString value = i.value();
        dataString += "`"+colname+"`="+":"+colname+",";
        x[colname] = value;
    }

    dataString.chop(1);
    QMapIterator<QString, QString> j(clause);
    QJsonObject y;
    QSqlQuery query(db);
    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QString value =j.value();
        clauseString += "`"+colname+"`="+":x"+colname+" AND ";
        y[colname] = value;
    }
    clauseString.chop(4);
    QString qStr = "UPDATE "+tablename+" SET "+dataString+ " WHERE "+clauseString ;

    query.prepare(qStr);

    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QString value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        //   qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }

    j.toFront();

    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QString value = j.value();

        QString bindname = ":x"+colname;

        //dataString += "'"+value+"',";
        //    qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));
    //    QJsonDocument doc1(y);
    //    QString strJson1(doc1.toJson(QJsonDocument::Compact));


    QJsonObject rootObject;
    rootObject.insert("Data",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1002");

    logJson->append(rootObject);
    //    QString tablename;

    //   qDebug()<<"############# " <<qStr;
    //    query.prepare("insert into "+tablename+"("+colname+")values("+value+"));
    //    QString qStr1 = "UPDATE `log` SET (`timestamp`=NOW(),`data`='"+strJson1+"',`action`=1001,`status`='insert') WHERE '"+strJson ;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1002','Update')";


    //  qDebug()<<"############# " <<qStr1;

    if(!query.exec()){
        qDebug()<<query.lastError();
        transFlag=false;
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
    }
    else{
        //        qDebug()<<query.lastQuery();
        //        qDebug()<<"####################";
        //        qDebug()<<query.lastQuery();
    }
    //    query.exec(qStr1);
}

int DatabaseHelper::updateRecordsPlus(QMap<QString, QVariant> data, QMap<QString, QVariant> clause, QString tablename)
{
    qDebug()<<Q_FUNC_INFO;
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QString dataString="";
    QMapIterator<QString, QVariant> i(data);
    QJsonObject x;
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();
        dataString += "`"+colname+"`="+":"+colname+",";

                qDebug()<<colname<<":"<<value.toString();
//                x["data"] = value.value<QString>();
//                x[colname]=value.value<QString>();

        if(value.userType() == QMetaType::Bool)
            x.insert(i.key(),i.value().toBool());
        else if(value.userType() == QMetaType::Int)
            x.insert(i.key(),i.value().toInt());
        else if(value.userType() == QMetaType::Float)
            x.insert(i.key(),i.value().toFloat());
//        else if(value.userType() == QMetaType::QJsonObject)
//            x.insert(i.key(),jsonToString(i.value()));
        else
            if(tablename == "Address_Book")
                x.insert(i.key(), escapeJson2( i.value().toString()));
        else
            //            x.insert(i.key(),i.value().toString());
                x.insert(i.key(), escapeJson(i.value().toString()));
    }
        qDebug()<<"X======="<<x;
    dataString.chop(1);

    QMapIterator<QString, QVariant> j(clause);
    QJsonObject y;
    QSqlQuery query(db);
    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QVariant value =j.value();
        clauseString += "`"+colname+"`="+":x"+colname+" AND ";

        //        y["clause"] = value.value<QString>();
        //        y[colname]=value.value<QString>();

        //        qDebug()<<" (clause)Values : "<<value.toString();
        if(value.userType() == QMetaType::Bool)
            y.insert(j.key(),j.value().toBool());
        else if(value.userType() == QMetaType::Int)
            y.insert(j.key(),j.value().toInt());
        else if(value.userType() == QMetaType::Float)
            y.insert(j.key(),j.value().toFloat());
        else
            //            y.insert(j.key(),j.value().toString());
            y.insert(j.key(), escapeJson(j.value().toString()));
    }
    clauseString.chop(4);
    QString where = " WHERE ";
    if(clauseString.length()==0)
        where ="";

    QString qStr = "UPDATE "+tablename+" SET "+dataString+  where +clauseString ;
            qDebug()<<qStr;
    if(! query.prepare(qStr)){
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
        qDebug()<<query.lastError();
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
        qDebug()<<qStr;
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    }
    else
    {

    }
    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();
        QString bindname = ":"+colname;
        //dataString += "'"+value+"',";
        //                qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    j.toFront();

    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QVariant value = j.value();

        QString bindname = ":x"+colname;

        //dataString += "'"+value+"',";
        //        qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    /*
    QJsonDocument doc(x);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    //    qDebug()<<"####"<<strJson;
    QJsonDocument doc1(y);
    QString strJson1(doc1.toJson(QJsonDocument::Compact));
    //    qDebug()<<"####"<<strJson1*/;
    QJsonObject rootObject;
    rootObject.insert("Data",x);
    rootObject.insert("Clause",y);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1002");

    logJson->append(rootObject);

    //    QString tablename;

    //        qDebug()<<"############# " <<qStr;
    //    query.prepare("insert into "+tablename+"("+colname+")values("+value+"));
    //    QString qStr1 = "UPDATE `log` SET (`timestamp`=NOW(),`data`='"+strJson1+"',`action`=1001,`status`='insert') WHERE '"+strJson ;
    //    qDebug()<<"############# " <<qStr1;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1002','Update')";


    if(!query.exec()){
        qDebug()<<query.lastError()<<"IN UPDATE";
        qDebug()<<query.lastQuery();
        qDebug()<<db.lastError();
        transFlag=false;
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
    }
    else{
        //        qDebug()<<"####################";
        //        qDebug()<<query.lastError();
        //                qDebug()<<query.lastQuery();
        //                qDebug()<<query.boundValues();
        //        qDebug()<<"####################";

    }
    int n = query.lastInsertId().toInt();

    //    query.exec(qStr1);

    return n;

}

int DatabaseHelper::updateRecordsStatic(QMap<QString, QVariant> data, QMap<QString, QVariant> clause, QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QString dataString="";
    QMapIterator<QString, QVariant> i(data);
    QJsonObject x;
    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();
        dataString += "`"+colname+"`="+":"+colname+",";

        //        qDebug()<<colname<<":"<<value.toString();
        //        x["data"] = value.value<QString>();
        //        x[colname]=value.value<QString>();

        if(value.userType() == QMetaType::Bool)
            x.insert(i.key(),i.value().toBool());
        else if(value.userType() == QMetaType::Int)
            x.insert(i.key(),i.value().toInt());
        else if(value.userType() == QMetaType::Float)
            x.insert(i.key(),i.value().toFloat());
        else
            //            x.insert(i.key(),i.value().toString());
            x.insert(i.key(), escapeJson(i.value().toString()));
    }

    //    qDebug()<<"X======="<<x;
    dataString.chop(1);

    QMapIterator<QString, QVariant> j(clause);
    QJsonObject y;
    QSqlQuery query(db);

    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QVariant value =j.value();
        clauseString += "`"+colname+"`="+":x"+colname+" AND ";

        //        y["clause"] = value.value<QString>();
        //        y[colname]=value.value<QString>();

        //        qDebug()<<" (clause)Values : "<<value.toString();
        if(value.userType() == QMetaType::Bool)
            y.insert(j.key(),j.value().toBool());
        else if(value.userType() == QMetaType::Int)
            y.insert(j.key(),j.value().toInt());
        else if(value.userType() == QMetaType::Float)
            y.insert(j.key(),j.value().toFloat());
        else
            //            y.insert(j.key(),j.value().toString());
            y.insert(j.key(), escapeJson(j.value().toString()));
    }
    clauseString.chop(4);
    QString where = " WHERE ";
    if(clauseString.length()==0)
        where ="";

    QString qStr = "UPDATE "+tablename+" SET "+dataString+  where +clauseString ;

    if(! query.prepare(qStr)){

        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
        qDebug()<<query.lastError();
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
        qDebug()<<qStr;
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    }
    else
    {

    }
    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();
        QString bindname = ":"+colname;
        //dataString += "'"+value+"',";
        //                qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    j.toFront();

    while (j.hasNext()) {
        j.next();
        QString colname = j.key();
        QVariant value = j.value();

        QString bindname = ":x"+colname;

        //dataString += "'"+value+"',";
        //        qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }
    /*
    QJsonDocument doc(x);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    //    qDebug()<<"####"<<strJson;
    QJsonDocument doc1(y);
    QString strJson1(doc1.toJson(QJsonDocument::Compact));
    //    qDebug()<<"####"<<strJson1*/;
    QJsonObject rootObject;
    rootObject.insert("Data",x);
    rootObject.insert("Clause",y);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1002");

    logJson->append(rootObject);

    //    QString tablename;

    //        qDebug()<<"############# " <<qStr;
    //    query.prepare("insert into "+tablename+"("+colname+")values("+value+"));
    //    QString qStr1 = "UPDATE `log` SET (`timestamp`=NOW(),`data`='"+strJson1+"',`action`=1001,`status`='insert') WHERE '"+strJson ;
    //    qDebug()<<"############# " <<qStr1;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1002','Update')";


    if(!query.exec()){
        qDebug()<<query.lastError()<<"IN UPDATE";
        qDebug()<<query.lastQuery();
        qDebug()<<db.lastError();
        transFlag=false;
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
    }
    else{
        //        qDebug()<<"####################";
        //        qDebug()<<query.lastError();
        //        qDebug()<<query.lastQuery();
        //        qDebug()<<"####################";

    }
    int n = query.lastInsertId().toInt();

    //    query.exec(qStr1);

    return n;

}

void DatabaseHelper::deleteRecordsPlus(QMap<QString, QVariant> clause, QString tablename)
{
    qDebug()<<Q_FUNC_INFO<<tablename;
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QMapIterator<QString, QVariant> i(clause);
    QJsonObject x;

    QSqlQuery query(db);


    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value =i.value();

        clauseString += "`"+colname+"`="+":"+colname+" AND ";
        //        x["clause"] = value.value<QString>();
        //        x[colname]=value.value<QString>();


        if(value.userType() == QMetaType::Bool)
            x.insert(i.key(),i.value().toBool());
        else if(value.userType() == QMetaType::Int)
            x.insert(i.key(),i.value().toInt());
        else if(value.userType() == QMetaType::Float)
            x.insert(i.key(),i.value().toFloat());
        else
            x.insert(i.key(),i.value().toString());
    }
    clauseString.chop(4);
    QString qStr = "DELETE FROM "+tablename+" WHERE "+clauseString ;
    query.prepare(qStr);


    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        //        qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }


    if(!query.exec()){
        qDebug()<<query.lastError();
        transFlag=false;
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
        //        qDebug()<<query.lastQuery();
    }
    else{
        //        qDebug()<<query.lastQuery();
    }

    QJsonObject rootObject;
    //    rootObject.insert("Data",x);
    rootObject.insert("Clause",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1003");

    logJson->append(rootObject);
    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));
    //  qDebug()<<"####"<<strJson;


    //    qDebug()<<"############# " <<qStr;

    //    QString qStr1 = "DELETE FROM `log` WHERE '"+strJson ;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1003','Delete')";

    //    query.exec(qStr1);

}

void DatabaseHelper::deleteRecordsStatic(QMap<QString, QVariant> clause, QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QMapIterator<QString, QVariant> i(clause);
    QJsonObject x;

    QSqlQuery query(db);


    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value =i.value();

        clauseString += "`"+colname+"`="+":"+colname+" AND ";
        //        x["clause"] = value.value<QString>();
        //        x[colname]=value.value<QString>();


        if(value.userType() == QMetaType::Bool)
            x.insert(i.key(),i.value().toBool());
        else if(value.userType() == QMetaType::Int)
            x.insert(i.key(),i.value().toInt());
        else if(value.userType() == QMetaType::Float)
            x.insert(i.key(),i.value().toFloat());
        else
            x.insert(i.key(),i.value().toString());
    }
    clauseString.chop(4);
    QString qStr = "DELETE FROM "+tablename+" WHERE "+clauseString ;
    query.prepare(qStr);


    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QVariant value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        //        qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }


    if(!query.exec()){
        qDebug()<<query.lastError();
        transFlag=false;
        errMSG += "Table : "+tablename+"\n"+query.lastQuery()+"\n"+query.lastError().text();;
        //        qDebug()<<query.lastQuery();
    }
    else{
        //        qDebug()<<query.lastQuery();
    }

    QJsonObject rootObject;
    //    rootObject.insert("Data",x);
    rootObject.insert("Clause",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1003");

    logJson->append(rootObject);
    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));
    //  qDebug()<<"####"<<strJson;


    //    qDebug()<<"############# " <<qStr;

    //    QString qStr1 = "DELETE FROM `log` WHERE '"+strJson ;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1003','Delete')";

    //    query.exec(qStr1);
}

QSqlQueryModel *DatabaseHelper::getQueryModel(QString qStr, QString ls)
{
    qDebug()<<qStr<<ls;
    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery(qStr,QSqlDatabase::database(ls));
    if (model->lastError().isValid()){
        qDebug()<<"&&&&&&&&&&"<<qStr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&";
        qDebug() << model->lastError();

        QFile inputFile2("debugdev.txt");
        if (inputFile2.open(QIODevice::ReadOnly))
        {
            QFile File("errorlog.txt");
            File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

            QString functionName = Q_FUNC_INFO;
            QString errorData = "error in " + functionName + ". Query: " + qStr + " Error is " + model->lastError().text();
            File.write(errorData.toUtf8());
            File.close();
        }
    }
    //    qDebug()<<"Model is "<<model;
    return model;
}

QSqlQueryModel *DatabaseHelper::getQueryModelL(QString qStr, QString ls)
{
    EagerlyLoaded<QSqlQueryModel> *model = new EagerlyLoaded<QSqlQueryModel>() ;
    model->setQuery(qStr,QSqlDatabase::database(ls));
    if (model->lastError().isValid()){
        qDebug()<<"&&&&&&&&&&"<<qStr<<"&&&&&&&&&&&&&&&&&&&&&&&&&&&";
        qDebug() << model->lastError();
    }

    return model;
}

QString DatabaseHelper::jsonToString(QJsonObject json)
{
    QJsonDocument doc(json);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    return strJson;
}

QJsonObject DatabaseHelper::strToJson(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());

    return  doc.object();
}

bool DatabaseHelper::checkDbExists(QString dbName)
{
    QString qStr = " SELECT 1 FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = '" + dbName + "'";

    if(getSingletonQueryResult(qStr).toInt() == 1){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"db exists";
        return true;
    }
    else {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"db doesnt exist";
        return false;

    }

}

QSqlQueryModel *DatabaseHelper::getGMQueryModel(QString qStr, QString ls)
{

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(qStr,QSqlDatabase::database(ls));
    if (model->lastError().isValid())
        qDebug() << model->lastError();
    return model;
}


QString DatabaseHelper::getDateString(QDate date)
{
    //Usage - getDateString(fromDate)

    return "str_to_date('" + date.toString("yyyy-MM-dd") + "','%Y-%m-%d')";
}

QString DatabaseHelper::getDateTimeString(QDateTime dateTime)
{
    return "str_to_date('" + dateTime.toString("yyyy-MM-dd hh:mm:ss") + "','%Y-%m-%d %H:%i:%s')";
}

QString DatabaseHelper::getDateStringStatic(QDate date)
{
    return "str_to_date('" + date.toString("yyyy-MM-dd") + "','%Y-%m-%d')";
}

QString DatabaseHelper::getDateTimeStringStatic(QDateTime dateTime)
{
    return "str_to_date('" + dateTime.toString("yyyy-MM-dd hh:mm:ss") + "','%Y-%m-%d %H:%i:%s')";
}

QVariant DatabaseHelper::getSingletonQueryResult(QString qStr, QString ls)
{
//    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(ls));
    QVariant Value=NULL;

    // Make changes to add clause
    if(query.exec(qStr)){
        if(query.first())
            Value = query.value(0);
//        qDebug()<<"exec"<<Value;
    }
    else{
        qDebug()<<"Query Error";
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
    }

//    qDebug()<<Q_FUNC_INFO<<__LINE__<<Value<<DatabaseValues::connectionString<<ls;
    return Value;
}

QMap<QString, QVariant> DatabaseHelper::getSingletonQueryResultandError(QString qStr, QString ls)
{
    QMap<QString, QVariant> map;
    QSqlQuery query(QSqlDatabase::database(ls));

    // Make changes to add clause
    if(query.exec(qStr)){
        query.first();
        map.insert("Status", "SUCCESS");
        map.insert("Result", query.value(0));
    }
    else{
        qDebug()<<"Query Error";
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();

        map.insert("Status", "Failed");
        map.insert("Result",query.lastError().text());
    }

    return map;
}

QMap<QString, QVariant> DatabaseHelper::getSingletonQueryResultandErrorStatic(QString qStr, QString ls)
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr<<ls;
    QMap<QString, QVariant> map;
    QSqlQuery query(QSqlDatabase::database(ls));

    // Make changes to add clause
    if(query.exec(qStr)){
        if(query.first()){
            map.insert("Status", "SUCCESS");
            map.insert("Result", query.value(0));
        }
    }
    else{
        qDebug()<<"Query Error";
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();

        map.insert("Status", "Failed");
        map.insert("Result",query.lastError().text());
    }

    return map;
}

QMap<QString, QVariant> DatabaseHelper::executeDDL(QString qStr, QString ls)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;
    QSqlQuery query(QSqlDatabase::database(ls));
    QMap<QString, QVariant> map;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<ls;

    // Make changes to add clause
    if(query.exec(qStr)){
        map.insert("SUCCESS", true);
    }
    else{
        qDebug()<<"<<<<<<<XXXXXXXXX>>>>>>>   Query Error";
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
        map.insert("SUCCESS", false);
        map.insert("Error", query.lastError().text());
    }

    return map;
}

QMap<QString, QVariant> DatabaseHelper::runDML(QString qStr, QString ls)
{
    QSqlQuery query(QSqlDatabase::database(ls));
    QMap<QString, QVariant> map;

    // Make changes to add clause
    if(query.exec(qStr)){
        map.insert("SUCCESS", true);
    }
    else{
        qDebug()<<"Query Error";
        qDebug()<<query.lastQuery();
        qDebug()<<query.lastError();
        map.insert("SUCCESS", false);
        map.insert("Error", query.lastError().text());
    }

    return map;
}

void DatabaseHelper::deleteRecords(QMap<QString,QString> clause,QString tablename)
{
    if(!transFlag){
        qDebug()<<"start transaction not called";
    }

    QString clauseString="";
    QMapIterator<QString, QString> i(clause);
    QJsonObject x;

    QSqlQuery query(db);


    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QString value =i.value();

        clauseString += "`"+colname+"`="+":"+colname+" AND ";
        //        x["clause"] = value;
        x[colname]=value;
    }
    clauseString.chop(4);
    QString qStr = "DELETE FROM "+tablename+" WHERE "+clauseString ;
    query.prepare(qStr);


    i.toFront();

    while (i.hasNext()) {
        i.next();
        QString colname = i.key();
        QString value = i.value();

        QString bindname = ":"+colname;

        //dataString += "'"+value+"',";
        //        qDebug()<<"binding "<< bindname << " with "<<value;
        query.bindValue(bindname,value);
    }


    if(!query.exec()){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        transFlag=false;
    }
    else{
        //        qDebug()<<query.lastQuery();
    }

    //    QJsonDocument doc(x);
    //    QString strJson(doc.toJson(QJsonDocument::Compact));
    //  qDebug()<<"####"<<strJson;


    QJsonObject rootObject;
    rootObject.insert("Clause",x);
    rootObject.insert("TableName",tablename);
    rootObject.insert("Action","1003");

    logJson->append(rootObject);
    //    qDebug()<<"############# " <<qStr;

    //    QString qStr1 = "DELETE FROM `log` WHERE '"+strJson ;
    //    QString qStr1 = "INSERT INTO `log` (`timestamp`,`data`,`action`,`status`) values (NOW(),'"+strJson+"','1003','Delete')";

    //    query.exec(qStr1);

}


QString DatabaseHelper::getUUID()
{
    QSqlQuery query(db);
    QString uuid;

    query.prepare("SELECT uuid()");

    if(query.exec()){
        if(query.first()){
            uuid = query.value(0).toString();
        }
    }
    else{
        qDebug()<<"UUID NOT GENERATED";
    }

    return uuid;
}


QString DatabaseHelper::escapeJson(QString jsonString)
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
QString DatabaseHelper::escapeJson2(QString jsonString)
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
    jsonString.replace("\"", "\\\"");
    //qDebug()<<"666666666"<<jsonString;

    return jsonString;
}

int DatabaseHelper::createVoucherNoProcedure()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = " DELIMITER $$ " ;
    qStr += " CREATE DEFINER=`user2grey`@`%` PROCEDURE `UpdateVoucherNo`(IN `VoucherType` VARCHAR(100), IN `VoucherPrefix` VARCHAR(100), OUT `VoucherNo` VARCHAR(100))" ;
    qStr += " BEGIN" ;
    qStr += " " ;
    qStr += " DECLARE v_no VARCHAR(100)\;" ;
    qStr += "" ;
    qStr += " UPDATE voucher_no_seq" ;
    qStr += " SET voucher_no =  cast(voucher_no as int) + 1" ;
    qStr += " WHERE voucher_type = VoucherType" ;
    qStr += " AND voucher_prefix = VoucherPrefix\;" ;
    qStr += " " ;
    qStr += " SELECT voucher_no into VoucherNo" ;
    qStr += " FROM voucher_no_seq" ;
    qStr += " WHERE voucher_type = VoucherType" ;
    qStr += " AND voucher_prefix = VoucherPrefix\;" ;
    qStr += " " ;
    qStr += " COMMIT\;" ;
    qStr += " " ;
    qStr += " END$$" ;
    qStr += " DELIMITER " ;

    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
        qDebug()<<query.lastQuery();
        return 0;
    }
    else
        return 1;

}
