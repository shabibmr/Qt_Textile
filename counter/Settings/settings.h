#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QProcess>

#include "database/Counter/countersettingsdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"



namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    CounterSettingsDatabaseHelper *dbHelper;
//    CounterSettingsDataModel *settingsData;
    LedgerMasterDatabaseHelper *ledgerHelper;
    QList<LedgerMasterDataModel> CashLedgersList;

    void setValues();

    QString getMACID();

    void savebackup(QString path);
    void savebackup2(QString filepath);

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void on_toolButton_clicked();

    void on_backupButton_clicked();

    void on_pushButton_clicked();

public slots:
    void backupFinished(int exitCode, QProcess::ExitStatus status);

signals:
    void backupCompleted();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
