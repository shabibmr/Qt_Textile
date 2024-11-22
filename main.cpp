#include "mainwindow.h"
#include <QApplication>
#include "database/databasehelper/databasevalues.h"
#include "login/loginpage.h"
#include "QStringList"
#include <QMessageBox>
#include "SysServices/GMSync/myservice.h"
#include "utils/AppUpdater/appupdater.h"
#include "utils/SettingsHelper/settingshelper.h"
#include "utils/License/licensechecker.h"
#include "utils/logincontroller.h"
#include <QTranslator>
//#include "webservices/test/gmwebsocketclient.h"


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    QString txt;
    QByteArray localMsg = msg.toLocal8Bit();

    fprintf(stdout, "log: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
    switch (type) {
        //    case QtDebugMsg:
        //        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //        break;
    case QtInfoMsg:
        txt = QString("Info: %1").arg(msg);
        //        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
        //    case QtWarningMsg:
        //        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //        break;
        //    case QtCriticalMsg:
        //        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //        break;
        //    case QtFatalMsg:
        //        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //        abort();
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        abort();
    }
    QFile outFile("logQdebug.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[])
{

    QFile inputFile2("debugdev.txt");
    if (!inputFile2.open(QIODevice::ReadOnly))
    {
        //        debugMode = true;
        qInstallMessageHandler(myMessageOutput);
        //        return;
    }



    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    int x=250;
    bool restart = false;
    QStringList args = a.arguments();
    qDebug()<<QDateTime::currentDateTime()<<"Args count = "<<args.count();
    qDebug()<<"At main"<<args.at(0);

    int userid=0;
    QString voucherPrefix="0";
    QString dbName;
    if (args.count() > 3){
        qDebug()<<"At main"<<args.at(1);
        userid = args[2].toInt();
        voucherPrefix = args[3];
        //        x=251;
        restart = true;
        dbName = args[4];

    }

    if(restart){
        QFile File("restartlog.txt");
        File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        QString functionName = Q_FUNC_INFO;
        QString errorData = " restarting . dbname= " + dbName ;
        for(QString str:args)
            errorData += str + " ";
        File.write(errorData.toUtf8());
        File.close();
    }

    // QTranslator l;
    // bool loadf= l.load(":/fonts/fonts/lang_ar.qm");
    // bool aaw= a.installTranslator(&l);

    // qDebug()<<"Language : "<<aaw<<loadf;

    //    GMWebsocketClient *client = new GMWebsocketClient();

    // QFile file(":/style/material.qss");

    DatabaseValues* dv = new DatabaseValues();
    //    dv->openDB();



    QPointer<LoginController> loginCtrl = new LoginController(restart, userid, voucherPrefix);

    if(!restart)
        loginCtrl->selectDefaultCompany();
    else {
        loginCtrl->selectDB(dbName);
    }

    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();
    // QTranslator l;
    // bool loadf = l.load(":/fonts/fonts/lang_ar.qm");
    // if(lang==1){
    //     qDebug()<<"Language  to arabic";

    //     bool aaw = a.installTranslator(&l);
    //     qDebug()<<loadf<<aaw;
    // }

    bool showDarkthemeFlag =ConfigurationSettingsDatabaseHelper::getValue(showDarktheme,false).toBool();
    if(showDarkthemeFlag){
        QFile styleFile(":/style/dark_style.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString stylesheet = QLatin1String(styleFile.readAll());
            a.setStyleSheet(stylesheet);
            styleFile.close();
        }
        else{
            qDebug()<<"Cannot Find style file!!!!";
        }
    }
    x= a.exec();


    return 101;
}


/*
 *
 * Service Main
 *
 *
 *
 */
int main2(int argc, char *argv[])
{
    MyService service(argc, argv);
    return service.exec();
}

