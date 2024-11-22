#ifndef DATABASEUPDATERWIDGET_H
#define DATABASEUPDATERWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "database/databasehelper/databaseupdater.h"

namespace Ui {
class DatabaseUpdaterWidget;
}

class DatabaseUpdaterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DatabaseUpdaterWidget(QWidget *parent = nullptr);
    ~DatabaseUpdaterWidget();

    QString fileName;

    DatabaseUpdater *dbUpdater;
    QJsonArray dbChanges;

    bool updateAlgoConfig = false;


    void setUpdateAlgoConfig(bool newUpdateAlgoConfig);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_BrowsePushButton_clicked();

//    void on_radioButton_clicked();

private:
    Ui::DatabaseUpdaterWidget *ui;
};

#endif // DATABASEUPDATERWIDGET_H
