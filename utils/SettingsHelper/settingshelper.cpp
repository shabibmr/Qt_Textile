#include "settingshelper.h"

SettingsHelper::SettingsHelper(QString grp, QString application)
{
    this->organization = grp;
    this->application = application;

}

SettingsHelper::~SettingsHelper()
{

}

void SettingsHelper::setValue(QString key,QVariant value)
{
    QSettings settings(organization, application);
    settings.setValue(key,value);
    settings.sync();
}

QVariant SettingsHelper::getValue(QString key, QVariant defaultValue)
{
    QSettings settings(organization, application);
    if(settings.contains(key)){
        return settings.value(key);
    }
    else {
        setValue(key,defaultValue);
        return defaultValue;
    }
}

void SettingsHelper::resetKey(QString key)
{
    QSettings settings(organization, application);

    settings.remove(key);
    settings.sync();
}


bool SettingsHelper::keyExist(QString key)
{
    QSettings settings(organization, application);

    if(settings.value(key).isNull() || settings.value(key).toString().length() == 0 )
        return false;
    else
        return true;

}
