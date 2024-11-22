#ifndef MODEOFPAY_H
#define MODEOFPAY_H

#include <QWidget>

#include "datamodels/Counter/servicemode.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/MainScreen/paymentsplitwidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/Basic/gmdatetimeedit.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"

namespace Ui {
class ModeOfPay;
}

class ModeOfPay : public QWidget
{
    Q_OBJECT

public:
    explicit ModeOfPay(GeneralVoucherDataObject *voucher,LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper,
                       SalesOrderDatabaseHelper *orderHelper = new SalesOrderDatabaseHelper(),
                        bool update=false,
                        QWidget *parent = 0);
    ~ModeOfPay();

    GeneralVoucherDataObject* voucher;
    QList<LedgerMasterDataModel> BankList;
    GMLedgerLineMaterial *mainLedgerWidget;

    QLineEdit *advancePaidAmount;

    QList<LedgerMasterDataModel> splitLeds;

    GMDateTimeEdit *delivery;

    bool hideLedgerwiseFlag=true;

    void setVoucher(GeneralVoucherDataObject *value);

    void resetWidgets();

    void calcTotals();

    float getRoundOff(float r, int multiple);

    bool carriageFlag = false;
    QFont font;

    bool updateMode=false;



private slots:

    void on_cashToolButton_clicked();
    
    void on_cardToolButton_clicked();

    void on_creditToolButton_clicked();

    void on_splitToolButton_clicked();

    void on_complimentarycashToolButton_clicked();

    void LedgerSelectedFromFav();

    // Start

    void on_zomatoToolButton_clicked();

    void on_splitOKPushButton_clicked();

    void on_ledgerOKPushButton_clicked();

    void on_backButton_clicked();

    void setBillPreview();

    void on_cashPaidLineEdit_textChanged(const QString &arg1);

    void on_discountPercentLineEdit_textChanged(const QString &arg1);

    void on_discountAmountLineEdit_textChanged(const QString &arg1);

    void on_roundOffLineEdit_textChanged(const QString &arg1);

    void on_amountChange4Line_returnPressed();

    void on_createLedger_clicked();

    void setFavButtons();



    void on_ledgersButton_clicked();

    void on_loyalityToolButton_clicked();

    void on_splitItemsButton_clicked();

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

    void restrictDiscounts();

    void openItemsSplit();
signals:
    void modeOfPaySignal();

    void resetPrice();

    void resetTable();

private:
    Ui::ModeOfPay *ui;
    void setPotafoPrice();

     LedgerMasterDatabaseHelper *LedgerHelper;
     SalesOrderDatabaseHelper *orderHelper;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MODEOFPAY_H
