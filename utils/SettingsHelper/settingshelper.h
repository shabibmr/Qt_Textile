#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H
#include <QSettings>

class SettingsHelper
{
public:
    SettingsHelper(QString organization ="Algoray", QString application = "GM.ERP");
    ~SettingsHelper();
    void setValue(QString key,QVariant value);
    QVariant getValue(QString key,QVariant defaultValue = QVariant());
    bool keyExist(QString key);
//    QSettings* settings;
    void resetKey(QString key);
private:
    QString organization ;
    QString application;
};

#endif // SETTINGSHELPER_H
