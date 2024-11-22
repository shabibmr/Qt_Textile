#include "registerdelegate.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "database/Settings/configsettingsvariables.h"

RegisterDelegate::RegisterDelegate(int vNoPos)
{
    this->vNoPos = vNoPos;
    qDebug()<<Q_FUNC_INFO<<this->vNoPos;

}

