
#include "licensechecker.h"

#include <QMessageBox>
#include "login/loginvalues.h"

LicenseChecker::LicenseChecker(QString Org, QObject *parent) : QObject (parent)
{

    this->organizationName = Org;

    QFile inputFile2("debugdev.txt");
    if (inputFile2.open(QIODevice::ReadOnly))
    {
        this->organizationName = "TEST DEV " + Org;
    }
    settings = new SettingsHelper("Algoray", "GM.ERP");
      qDebug()<<Q_FUNC_INFO<<__LINE__<<"Created";
    mgr = new QNetworkAccessManager(this);
    // check if licenseKey exists
    // no => generate uuid and save in settings
    // check if companyId exists
    // no => generate uuid and save in DB - config_settings
    // get licenseInfo from cloud
    // save validity info in settings
    // check validity -- till this
    // if validity end - show warning or forceStop

    getCompanyId();
    getLicenseKey();


}

LicenseChecker::~LicenseChecker()
{
      qDebug()<<Q_FUNC_INFO<<__LINE__<<"LicenseChecker deleted";
    delete reply;
    delete mgr;
}

bool LicenseChecker::getLicenseInfo()
{
    if(licenseInfo.licenseKey.length() > 0)
        sendRequest("getLicenseInfo", "key="+licenseInfo.licenseKey);
    // // qDebug()<<Q_FUNC_INFO<<__LINE__
    bool stat = checkValidity();
    return stat;
}

void LicenseChecker::replyFinished(QNetworkReply *reply)
{

    licenseInfo.JsonToObj(reply->readAll());
//    qDebug()<<Q_FUNC_INFO<<licenseInfo.contactName;

}

void LicenseChecker::getLicenseKey()
{
    // check if license key exsts in settings
    // if no, generate uuid
    if(!settings->keyExist("LicenseKey")){
        //        showMessage("License Key not in settings!");
        addLicenseEntry();
    }
    else {
        licenseInfo.licenseKey = settings->getValue("LicenseKey").toString();
    }
}

void LicenseChecker::updateLicenseKey()
{
    return;
    //    bool ok;
    //    QString key = QInputDialog::getText(this, tr("QInputDialog::getText()"),
    //                                        tr("Enter License Key:"), QLineEdit::Normal,
    //                                        "0", &ok);
    //    if(ok && !key.isEmpty()){

    //        settings->setValue("LicenseKey", key);
    //    }


}

bool LicenseChecker::checkValidity()
{
    if (reply->error() == QNetworkReply::NoError){
        QString replyStr = reply->readAll();
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<replyStr;
        QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
        QJsonObject json = doc.object();
        //        if(json["Success"].toString() == "1")
        licenseInfo.JsonToObj(replyStr);
        if(licenseInfo.licenseKey.length() >0)
            saveSettings();
        else {
            // if no entry in cloud
            //            showMessage("licenseInfo read error!");
            addLicenseEntry();
            //            return checkValidity();
        }

    }
    else {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"request error";

        licenseInfo.licenseDurationPeriod = settings->getValue("validityPeriod", 50).toInt();
        licenseInfo.LicenseUpdatedDate = settings->getValue("validFrom", QDate::currentDate()).toDate();
        licenseInfo.forceStop = settings->getValue("forceStop", false).toBool();
        licenseInfo.warning_billCount = settings->getValue("warningBillCount", 0).toInt();
        licenseInfo.fatalMessage = settings->getValue("fatalMsg", "").toString();
        licenseInfo.warningMessage = settings->getValue("warningMsg", "").toString();

    }

    if(licenseInfo.LicenseUpdatedDate.addDays(licenseInfo.licenseDurationPeriod) >= QDate::currentDate()){
//        qDebug()<<"license valid"<<licenseInfo.LicenseUpdatedDate.addDays(licenseInfo.licenseDurationPeriod)<<licenseInfo.LicenseUpdatedDate<<licenseInfo.licenseDurationPeriod;
        return true;
    }
    else {
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<licenseInfo.forceStop<<licenseInfo.warning_billCount;
        if(licenseInfo.forceStop){
            if(licenseInfo.fatalMessage.length() > 0)
                showMessage(licenseInfo.fatalMessage);
            return false;

        }

        else if(licenseInfo.warning_billCount > 0){
            //            showMessage("Warning");
            LoginValues::setWarningBillCount(licenseInfo.warning_billCount);
            LoginValues::setWarningMessage(licenseInfo.warningMessage);
//            qDebug()<<"license warning";
        }
        return true;
    }
}

void LicenseChecker::addCompany()
{
    sendRequest("addCompany", "CompanyName="+organizationName  + "&CompanyId=" + licenseInfo.companyId);

    return;

    if(settings->keyExist("validityPeriod") && settings->keyExist("validFrom") ){
        licenseInfo.licenseDurationPeriod = settings->getValue("validityPeriod").toInt();
        licenseInfo.LicenseUpdatedDate = settings->getValue("validFrom").toDate();


        sendRequest("addCompany", "CompanyName="+organizationName + "&updatedDate=" + licenseInfo.LicenseUpdatedDate.toString("yyyy-MM-dd")
                    + "&period=" + QString::number(licenseInfo.licenseDurationPeriod));
    }
    else {
        sendRequest("addCompany", "CompanyName="+organizationName );
    }

    if (reply->error() == QNetworkReply::NoError){
        // // qDebug()<<Q_FUNC_INFO<<__LINE__
        licenseInfo.JsonToObj(reply->readAll());

        //        QMessageBox box; box.setText("Generated key : " + licenseInfo.licenseKey); box.exec();
        //        if(licenseInfo.licenseKey.length() == 19){
        //            saveSettings();
        //        }

    }

    else {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"request error";
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"adding expiry to settings";
        if(settings->keyExist("validityPeriod") && settings->keyExist("validFrom")){
            licenseInfo.licenseDurationPeriod = settings->getValue("validityPeriod").toInt();
            licenseInfo.LicenseUpdatedDate = settings->getValue("validFrom").toDate();
            licenseInfo.forceStop = settings->getValue("forceStop", false).toBool();
            licenseInfo.warning_billCount = settings->getValue("warningBillCount", 0).toInt();
        }
        else{
            licenseInfo.licenseDurationPeriod = 50;
            licenseInfo.LicenseUpdatedDate = QDate::currentDate();
            saveSettings();
        }
    }
    //            QDateTime validTill =QDateTime( licenseInfo.LicenseUpdatedDate.addDays(licenseInfo.licenseDurationPeriod));
    //            qint64 validTillTime = validTill.toMSecsSinceEpoch();
    //            // offline values
    //            qDebug()<<Q_FUNC_INFO<<__LINE__<<validTillTime;
    //            settings.setValue("validTill", validTillTime);
}

void LicenseChecker::addLicenseEntry()
{
    if(!settings->keyExist("LicenseKey"))
        licenseInfo.licenseKey = DatabaseHelper::getUUID();
    else
        licenseInfo.licenseKey = settings->getValue("LicenseKey").toString();
    settings->setValue("LicenseKey", licenseInfo.licenseKey);
    //    showMessage("Add License to Cloud");
    sendRequest("addLicenseEntry", "License_Key="+licenseInfo.licenseKey + "&CompanyId=" + licenseInfo.companyId );

    if (reply->error() == QNetworkReply::NoError){
        // // qDebug()<<Q_FUNC_INFO<<__LINE__
        QString replyStr = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(replyStr.toUtf8());
        QJsonObject json = doc.object();
        if(json["Success"].toString() == "1"){
            licenseInfo.JsonToObj(replyStr);
        }
    }

    if(licenseInfo.licenseKey.length() == settings->getValue("LicenseKey")){
        saveSettings();
    }
}

void LicenseChecker::sendRequest(QString action, QString args)
{
    QTimer timer;
    timer.setSingleShot(true);

    QString request = url + action + "&" + args;
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<request;
    // url + "getLicenseInfo&key=" +licenseInfo.licenseKey
    reply = mgr->get(QNetworkRequest(QUrl(request)));
    QEventLoop eventloop;
    connect(&timer, &QTimer::timeout, &eventloop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &eventloop, &QEventLoop::quit);
    timer.start(5000);
    eventloop.exec();
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
    emit requestStarted();


    if(timer.isActive()){
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
        emit requestFinished();
        timer.stop();
        // // qDebug()<<Q_FUNC_INFO<<__LINE__
    }
    else {
        // // qDebug()<<Q_FUNC_INFO<<__LINE__
        disconnect(reply, &QNetworkReply::finished, &eventloop, &QEventLoop::quit);
        // // qDebug()<<Q_FUNC_INFO<<__LINE__
        reply->abort();
    }
    // // qDebug()<<Q_FUNC_INFO<<__LINE__

}

void LicenseChecker::saveSettings()
{

//    qDebug()<<Q_FUNC_INFO<<__LINE__<<licenseInfo.licenseDurationPeriod<<licenseInfo.LicenseUpdatedDate;
    settings->setValue("validityPeriod", licenseInfo.licenseDurationPeriod);
    settings->setValue("validFrom", licenseInfo.LicenseUpdatedDate);
    settings->setValue("forceStop", licenseInfo.forceStop);
    settings->setValue("warningBillCount", licenseInfo.warning_billCount);
    settings->setValue("fatalMsg", licenseInfo.fatalMessage);
    settings->setValue("warningMsg", licenseInfo.warningMessage);

    LoginValues::setAutoUpdate(licenseInfo.autoUpdate);

}

void LicenseChecker::getCompanyId()
{
// if no entry in regedit, create new companyId, else use existing id

    if(!settings->keyExist("LicenseKey"))
        licenseInfo.companyId = DatabaseHelper::getUUID();
    else
        licenseInfo.companyId = ConfigurationSettingsDatabaseHelper::getValue(companyId,
                                                                          DatabaseHelper::getUUID()).toString();

    if(licenseInfo.companyId.length() <5){
//        // entry in regedit exists, but companyId is empty
//        settings->resetKey("LicenseKey");
        licenseInfo.companyId = DatabaseHelper::getUUID();
    }

    addCompany();

}

void LicenseChecker::showMessage(QString text)
{
    QMessageBox box; box.setText(text); box.exec();

}
