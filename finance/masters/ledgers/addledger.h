#ifndef ADDLEDGER_H
#define ADDLEDGER_H

#include <QWidget>
#include <QDate>
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/accounts/ledgermasterdatamodel.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "datamodels/transaction/billwiserecorddatamodel.h"
#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"
#include "database/finance/BillwiseRecords/billwiserecordsdatabasehelper.h"
#include "Notifications/notificationledgerslist.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"


namespace Ui {
class addLedger;
}

class addLedger : public QWidget
{
    Q_OBJECT

public:
    explicit addLedger(QWidget *parent = 0);
    ~addLedger();
    QString id;
    int ContactID=0;

    AccountGroupMasterDatabaseHelper* accHelper;
    LedgerMasterDatabaseHelper* ledHlper;
    PricelistDatabaseHelper* priceHelper;
    BillwiseRecordsDatabaseHelper* billHelper;

    QList<LedgerMasterDataModel> ledgerList;

    QMap<QString,QString> accGroups;
    QMap<QString,QString> priceListNames;
    QList<BillwiseRecordDataModel*> mappingList;

    LedgerMasterDataModel *ledger;

    void setContactID(int value);

    void setLedger(LedgerMasterDataModel value);
    QtMaterialSnackbar *snackBar;
    void disableSave();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_openingBalanceLineEdit_editingFinished();
    void on_billwiseToolButton_clicked();
    void sendNotifications(QStringList);

private:
    Ui::addLedger *ui;
public slots:
    void edit();
    void setTitle(QString text);
    void settable();
    bool validate();
    void setLedger(LedgerMasterDataModel *value);
    void setLedgerName(QString name);
signals:
    void closing();
    void newCreated(LedgerMasterDataModel);
};

#endif // ADDLEDGER_H
