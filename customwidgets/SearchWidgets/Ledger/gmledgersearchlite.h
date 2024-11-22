#ifndef GMLEDGERSEARCHLITE_H
#define GMLEDGERSEARCHLITE_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include "datamodels/accounts/accountgroupdatamodel.h"
#include "datamodels/accounts/ledgermasterdatamodel.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "gmledgersearchlistwidgetlite.h"

class GMLedgerSearchLite : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMLedgerSearchLite(QSqlQueryModel *model,QLineEdit *parent=Q_NULLPTR);

    QStringList list;
    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;

    bool accountsFlag = false;

    QSqlQueryModel* itemsModel;

    GMLedgerSearchListWidgetLite *listWidget;

    void setAccountsFlag(bool value);

    void setItemsModel(QSqlQueryModel *value);

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(QString,QString);
    void noneSelected();

signals:

    void SelectedAccount(AccountGroupDataModel);
    void selectedLedger(LedgerMasterDataModel);
    void clearItem();

};

#endif // GMLEDGERSEARCHLITE_H
