#ifndef COUPLEDVOUCHERHANDLER_H
#define COUPLEDVOUCHERHANDLER_H

#include <QObject>
#include <QWidget>
#include <QKeyEvent>
#include <QCheckBox>

#include "finance/transactions/receipts/receiptvouchereditor.h"
#include "finance/transactions/payments/paymentvouchereditor.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "customwidgets/Material/components/qtmaterialcheckbox.h"


class CoupledVoucherHandler : public QWidget
{
    Q_OBJECT

public:
    CoupledVoucherHandler(QWidget *parent = nullptr);
    ~CoupledVoucherHandler();

    GeneralVoucherDataObject *coupledVoucher;
    GeneralVoucherDataObject *parentVoucher;

    GMMaterialNumberEdit *advancePaid;
//    QtMaterialCheckBox *cashPaid;
    QCheckBox *cashPaid;
    GMLedgerLineMaterial *mainLedgerWidget;

    LedgerMasterDataModel modeOfPay;

//    bool addedWidgets = false;


    void initDbHelpers();
    void setParentVoucher(GeneralVoucherDataObject *voucher);
    void setWidgets();
    void resetWidgets();

    void createCoupledPaymentVoucher();
    void createCoupledReceiptVoucher();
    bool retrieveCoupledReceiptVoucher();
    void updateCoupledReceiptVoucher();
    void saveCoupledReceiptVoucher(int editorStatus);
    bool retrieveCoupledPaymentVoucher();
    void updateCoupledPaymentVoucher();
    void saveCoupledPaymentVoucher(int editorStatus);

    bool checkModeOfPayEntered();

    void saveCoupledVoucher();
    bool retrieveCoupledVoucher();

    void createCoupledVoucher();
private:

    LedgerMasterDatabaseHelper *ledHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
//    PaymentVoucherDatabaseHelper *paymentDbHelper;

};

#endif // COUPLEDVOUCHERHANDLER_H
