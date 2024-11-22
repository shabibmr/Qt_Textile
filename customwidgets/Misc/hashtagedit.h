#ifndef HASHTAGEDIT_H
#define HASHTAGEDIT_H

#include <QLineEdit>
#include <QSqlQueryModel>
#include <database/finance/ledgermaster/ledgermasterdatabasehelper.h>
#include <QCompleter>

class HashTagEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit HashTagEdit(
            LedgerMasterDatabaseHelper *iHelper,
            QSqlQueryModel *m,
            LedgerMasterDataModel *led,
            QLineEdit *parent=Q_NULLPTR);
    virtual ~HashTagEdit() override;

    QCompleter *completer() const;
};

#endif // HASHTAGEDIT_H
