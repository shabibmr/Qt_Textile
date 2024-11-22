#ifndef APPUPDATER_H
#define APPUPDATER_H

#include "QSimpleUpdater.h"
#include <QObject>

class AppUpdater : public QObject
{
public:
    AppUpdater(QString verison = "0.1",QString url="http://www.algoray.in/algoERP/sample1.json");

    QString jsonURL;
public slots:
    void checkForUpdates();
private:
    QSimpleUpdater* m_updater;
};

#endif // APPUPDATER_H
