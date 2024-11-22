#include "databaseupdaterwidget.h"
#include "ui_databaseupdaterwidget.h"
#include <QFileDialog>

DatabaseUpdaterWidget::DatabaseUpdaterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DatabaseUpdaterWidget)
{
    ui->setupUi(this);
    dbUpdater = new DatabaseUpdater();
}

DatabaseUpdaterWidget::~DatabaseUpdaterWidget()
{
    delete ui;
}

void DatabaseUpdaterWidget::setUpdateAlgoConfig(bool newUpdateAlgoConfig)
{
    updateAlgoConfig = newUpdateAlgoConfig;
}

void DatabaseUpdaterWidget::on_buttonBox_accepted()
{
    QString query = ui->plainTextEdit->toPlainText();

    int version = dbUpdater->getLatestDbVersion(0);
    qDebug()<<"latest version is >"<<version;
    int n = query.count(";");
    bool StopOnError = ui->checkBox->isChecked();

    qDebug()<<dbChanges<<n;
    for(int i=0;i<n;i++){

        QJsonObject newEntry;
        newEntry.insert("Query", query.split(";").at(i));
        newEntry.insert(dbverison,++version);
        newEntry.insert("TimeStamp",QDateTime::currentDateTime().toString());
        newEntry.insert("StopOnError", StopOnError);

        qDebug()<<i<<newEntry;
        dbChanges.append(newEntry);

    }

    qDebug()<<"Updated"<<dbChanges;
    dbUpdater->writeDBChangesFile(fileName, dbChanges);

    this->close();

}

void DatabaseUpdaterWidget::on_buttonBox_rejected()
{
    this->close();
}

void DatabaseUpdaterWidget::on_BrowsePushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,
                                                    "Select a project file ",
                                                    QDir::homePath(),
                                                    QObject::tr("Pro Files (*.pro)"));
    ui->fileNameLineEdit->setText(fileName);
    int n = fileName.lastIndexOf("/");

    fileName = fileName.left(n);
    QString jsonFilePath = "/resources/dbchanges.json";
    if(updateAlgoConfig)
        jsonFilePath = "/resources/algo_config.json";
    fileName += jsonFilePath;
    dbChanges = dbUpdater->readDBChangesFile(fileName);

}


