#ifndef MODEOFPAYSTORE_H
#define MODEOFPAYSTORE_H

#include <QWidget>

#include "datamodels/Counter/servicemode.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/MainScreen/paymentsplitwidget.h"
#include "finance/transactions/calculator/transactioncalculator.h"

namespace Ui {
class ModeofPayStore;
}

class ModeofPayStore : public QWidget
{
    Q_OBJECT

public:
    explicit ModeofPayStore(QWidget *parent = 0);
    ~ModeofPayStore();

    GeneralVoucherDataObject* voucher;
    QList<LedgerMasterDataModel> BankList;

    QList<LedgerMasterDataModel> splitLeds;

    void setVoucher(GeneralVoucherDataObject *value);

    void calcTotals();

    float getRoundOff(float r, int multiple);

    bool carriageFlag = false;

private slots:


    void on_cashToolButton_clicked();
    
    void on_cardToolButton_clicked();

    void on_creditToolButton_clicked();

    void on_splitToolButton_clicked();

    void on_complimentarycashToolButton_clicked();

    void on_zomatoToolButton_clicked();

    void on_splitOKPushButton_clicked();

    void on_ledgerOKPushButton_clicked();

    void on_backButton_clicked();

    void setBillPreview();

    void on_cashPaidLineEdit_textChanged(const QString &arg1);

    void on_discountPercentLineEdit_textChanged(const QString &arg1);

    void on_discountAmountLineEdit_textChanged(const QString &arg1);

    void on_roundOffLineEdit_textChanged(const QString &arg1);



    void on_deliveryToolButton_toggled(bool checked);

    void on_storeToolButton_toggled(bool checked);

    void on_deliveryToolButton_clicked();

    void on_storeToolButton_clicked();

    void on_zomatoCashButton_clicked();

    void on_zomatoCardButton_clicked();

    void on_amountChange4Line_returnPressed();

    void on_createLedger_clicked();

    void on_carriageToolButton_clicked();

    void on_talabatToolButton_clicked();

    void on_DeliverooToolButton_clicked();

public slots:

    void hideDiscounts();

    void showZomato(bool v);

    void showPax(bool v);

    void showComplimentary(bool v);

    void showSplitWidget(bool v);

    void showSelectLedgerWidget(bool v);

    void modeOfPayCompleted();

    void hideTotalWidget();

    void set_Splits(QList<LedgerMasterDataModel> ledList);
signals:
    void modeOfPaySignal();

private:
    Ui::ModeofPayStore *ui;
    void setPotafoPrice();
};

#endif // MODEOFPAY_H
