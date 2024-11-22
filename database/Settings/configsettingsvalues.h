#ifndef CONFIGSETTINGSVALUES_H
#define CONFIGSETTINGSVALUES_H

#include <QString>

class ConfigSettingsValues
{
public:
    ConfigSettingsValues();

    static bool useBom;
    static bool askForPrint;
    static bool tokenSystem;
    static QString defaultServiceMode;
};

#endif // CONFIGSETTINGSVALUES_H
