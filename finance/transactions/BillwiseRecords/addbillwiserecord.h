#ifndef ADDBILLWISERECORD_H
#define ADDBILLWISERECORD_H

#include <QWidget>
#include <QPointer>
#include <QDateEdit>
#include <QToolButton>
#include <QDebug>
#include <QCheckBox>
#include <QtMath>

#include "datamodels/transaction/billwiserecorddatamodel.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "finance/transactions/payments/paymentvouchereditor.h"
#include "finance/transactions/receipts/receiptvouchereditor.h"

namespace Ui {
class AddBillwiseRecord;
}

class AddBillwiseRecord : public QWidget
{
    Q_OBJECT

public:
    explicit AddBillwiseRecord(QWidget *parent = nullptr);
    ~AddBillwiseRecord();

    bool transFlag;
    float enteredAmount=0;

    QList<BillwiseRecordDataModel*> allBills;

    BillwiseRecordDataModel* refObj;
    GeneralVoucherDataObject *voucher;
    LedgerMasterDataModel *ledger;



    BillwiseRecordsDatabaseHelper *billHelper;

    void setBillwiseMapping(QDate date, bool flag);
    void setDefaultMappings(QDate date, bool flag);
    GMSQLQUeryModel *setPendingBillsView(QDate date, bool flag);
    void showOpeningBalanceList(bool flag);

    void setLedger(LedgerMasterDataModel *value);
    void setOnAccountData(float balance);
    void setVoucherData(LedgerMasterDataModel *ledger, QDate voucherDate);

public slots:
    void showPendingBills();
private slots:
    void on_CancelPushButton_clicked();
    void setTable();
    void setTableHeader();

    void on_OkPushButton_clicked();
    void delButtonClicked(bool);

    void on_AddPushButton_clicked();
    void setBillAmount(int);
    void setSelectedAmount();
    void on_clearDuesPushButton_clicked();
    void setMapData(int pos);
    void removeObjFromMapList(int pos);
    void refreshLedgerReport();
private:
    Ui::AddBillwiseRecord *ui;

signals:
  void closing();
  void addReference(QList<BillwiseRecordDataModel*>);
  void refresh(LedgerMasterDataModel);
};

#endif // ADDBILLWISERECORD_H
