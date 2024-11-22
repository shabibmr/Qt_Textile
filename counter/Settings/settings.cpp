#include "settings.h"
#include "ui_settings.h"
#include <QtNetwork/QNetworkInterface>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>


Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->KOTwidget->hide();
    ui->typeOfTradeWidget->hide();

    ui->typeOfTradeComboBox->addItems(QStringList()<<""<<"Restaurant"<<"Baqala"<<"ERP");

    dbHelper = new CounterSettingsDatabaseHelper();
    ledgerHelper = new LedgerMasterDatabaseHelper();
    dbHelper->getCounterSettings(LoginValues::voucherPrefix);

    ui->startTime->setTime(QTime::fromString("05:00:00"));
    ui->endTime->setTime(QTime::fromString("04:00:00"));
    ui->endDay->setText("1");
    ui->endDay->setValidator(new QDoubleValidator(0,999,0,this));

    CashLedgersList = ledgerHelper->getAllLedgersUnderGroupByName("Cash-in-Hand");

    for(LedgerMasterDataModel ledger:CashLedgersList){
        ui->defaultCashComboBox->addItem(ledger.LedgerName,ledger.LedgerID);
    }
    setValues();

    ui->toolButton->hide();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_saveButton_clicked()
{
    CounterSettingsDataModel::setCashOutPrintCopies(ui->cashOutSpinBox->value());
    CounterSettingsDataModel::setDeliveryPrintCopies(ui->deliverySpinBox->value());
    CounterSettingsDataModel::setAllOtherPrintCopies(ui->allOtherSpinBox->value());
    CounterSettingsDataModel::setKOTPrintCopies(ui->kOTCopySpinBox->value());
    CounterSettingsDataModel::setDefaultCash(ui->defaultCashComboBox->currentData().toString());
    CounterSettingsDataModel::setTypeOfTrade(ui->typeOfTradeComboBox->currentText());
    CounterSettingsDataModel::setPOSNumber(ui->pOSNoLineEdit->text());
    CounterSettingsDataModel::setMAC_ID(getMACID());
    CounterSettingsDataModel::setVoucherPrefix(ui->voucherPrefixLineEdit->text());
    CounterSettingsDataModel::setPortNo(ui->portNoLineEdit->text());
    CounterSettingsDataModel::setStartTime(ui->startTime->time());
    CounterSettingsDataModel::setEndTime(ui->endTime->time());
    CounterSettingsDataModel::setEndDay(ui->endDay->text().toInt());


    //    if(CounterSettingsDataModel::_id == 0)
    //        dbHelper->insertCounterSettings();
    //    else
            dbHelper->updateCounterSettings(CounterSettingsDataModel::VoucherPrefix);

    this->close();
}

void Settings::on_cancelButton_clicked()
{
    this->close();
}

void Settings::setValues()
{
    ui->allOtherSpinBox->setValue(CounterSettingsDataModel::AllOtherPrintCopies);
    ui->cashOutSpinBox->setValue(CounterSettingsDataModel::CashOutPrintCopies);
    ui->deliverySpinBox->setValue(CounterSettingsDataModel::DeliveryPrintCopies);
    ui->kOTCopySpinBox->setValue(CounterSettingsDataModel::KOTPrintCopies);
    ui->defaultCashComboBox->setCurrentText(ledgerHelper->getLedgerNameByID(CounterSettingsDataModel::DefaultCash));
    ui->typeOfTradeComboBox->setCurrentText(CounterSettingsDataModel::TypeOfTrade);
    ui->pOSNoLineEdit->setText(CounterSettingsDataModel::POSNumber);
    ui->voucherPrefixLineEdit->setText(CounterSettingsDataModel::VoucherPrefix);
    ui->portNoLineEdit->setText(CounterSettingsDataModel::PortNo);

    ui->startTime->setTime(CounterSettingsDataModel::StartTime);
    ui->endTime->setTime(CounterSettingsDataModel::EndTime);
    ui->endDay->setText(QString::number(CounterSettingsDataModel::EndDay));

    if (CounterSettingsDataModel::TypeOfTrade == "")
        ui->typeOfTradeWidget->show();

    if(CounterSettingsDataModel::TypeOfTrade == "Restaurant")
        ui->KOTwidget->show();
}

void Settings::on_toolButton_clicked()
{
    dbHelper->changeCollation();
}


QString Settings::getMACID()
{
    QString text;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        text += interface.hardwareAddress()+"|";
    }
    return text;
}



void Settings::on_backupButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Select folder",QDir::currentPath());
    qDebug()<<Q_FUNC_INFO<<path;
    savebackup(path);
}


void Settings::savebackup2(QString filepath){


    QString dbname = "gmrestaurant";
    //   +QDate::currentDate().toString("dd_MM_yyyy");
    //    QString dbuser = "";
    //    QString dbpass = "";
    //    QString hostname = "";


    QString backup = "mysqldump --databases " + DatabaseValues::dbName
            + " --user=" + DatabaseValues::dbUser
            + " --password=" + DatabaseValues::dbPassword
            + " --host=" + DatabaseValues::dbHost
            + " --log-error backup.log"
            + " --routines"
            + " --verbose --opt"
            + " --result-file " + filepath+"/"+dbname+".sql";
    QString backupFile = filepath+"/"+dbname+".sql";
    //myProcess->start( backup );

    QProcess process;
    QString exe = "md fathima";
    process.start(exe);
    process.waitForFinished();
    QFile file(backupFile);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug()<<__FUNCTION__<<__LINE__<<" mysqldump is error! ";
        return;
    }
    QByteArray ba = process.readAllStandardOutput();

    qDebug()<<"BA SIZE : "<<ba.length();
    file.write(ba);
    file.close();

    qDebug()<<backup;
}

void Settings::savebackup(QString filepath){


    QString dbname = DatabaseValues::connectionString;

    //   +QDate::currentDate().toString("dd_MM_yyyy");
    //    QString dbuser = "";
    //    QString dbpass = "";
    //    QString hostname = "";

    QString backup = "mysqldump --databases " + DatabaseValues::dbName
            + " --user=" + DatabaseValues::dbUser
            + " --password=" + DatabaseValues::dbPassword
            + " --host=" + DatabaseValues::dbHost
            + " --log-error backup.log"
            + " --routines"
            + " --verbose --opt"
            + " --result-file " + filepath+"/"+dbname+ "_" + QString::number(QDateTime::currentSecsSinceEpoch()) + ".sql";
    QString backupFile = filepath+"/"+dbname+".sql";
    //myProcess->start( backup );

    QProcess * myProc = new QProcess();
    connect(myProc, SIGNAL(finished (int, QProcess::ExitStatus)), qApp, SLOT(aboutQt()));
//    connect(myProc, SIGNAL(finished(int , QProcess::ExitStatus )), this,
//            SLOT(backupFinished(int exitCode, QProcess::ExitStatus status)), Qt::QueuedConnection);
    myProc->startDetached(backup);
    myProc->waitForFinished();
    qDebug()<<"*********"<<backup<<myProc->exitStatus();


//    connect(myProc, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this,
//            [=](int exitCode, QProcess::ExitStatus status){
//        qDebug()<<"***************";
//        QMessageBox box;
//        box.setText("Backup done, status:" + QString::number(exitCode) + " " + status);
//        box.exec();
//        emit backupCompleted();
//    }, Qt::QueuedConnection);

    backupFinished(myProc->exitCode(), myProc->exitStatus());

}

void Settings::on_pushButton_clicked()
{

}

void Settings::backupFinished(int exitCode, QProcess::ExitStatus status)
{
    if(status == QProcess::NormalExit){
        QMessageBox box;
        box.setText("Backup completed");
        box.exec();

        emit backupCompleted();
    }

    else {
        QMessageBox box;
        box.setText("Backup failed");
        box.exec();
    }
}
