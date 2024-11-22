#ifndef DBBACKUPHELPER_H
#define DBBACKUPHELPER_H

#include <QObject>

class DBBackupHelper
{
public:
    DBBackupHelper();

    static void baseBackup();

    static void Incrementalbackup();

    void prepForRestore();

    void copyDB();
};

#endif // DBBACKUPHELPER_H
