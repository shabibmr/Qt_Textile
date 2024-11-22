#ifndef PAYMENTVOUCHEREDITOR_H
#define PAYMENTVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/ledgervouchertableview.h"
#include "customwidgets/Models/gmpaymententrymodel.h"
#include "CommonUi/Transactions/vouchereditorbase.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include <customwidgets/Delegates/Ledgers/gmledgertransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"



#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"


#include "sharetools/print/printerhelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/finance/paymentvoucher/paymentvoucherdatabasehelper.h"
#include "customwidgets/Bank/chequewidget.h"
//#include "finance/transactions/BillwiseRecords/addbillwiserecord.h"
#include "finance/transactions/BillwiseRecords/billwisemappingeditor.h"

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"


class PaymentVoucherEditor : public VoucherEditorBase
{
    Q_OBJECT

    bool chequeEntryCompleted =false;



    QList<int> avlcols {1,2};



    LedgervoucherTableView *DataTableWidget;                 //X


    ChequeDetail *chqObj;
    ChequeWidget *chequeWidget;


    QPointer<GMPaymentEntryModel> model;

    QStringList lis;


public:
    explicit PaymentVoucherEditor(VoucherDBAbstract *dbhelper, VoucherEditorBase *parent = nullptr);

    void initExtraActions();

    void initVoucher();

    void initGlobalObjects();


    void resetWidgets() override;



public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;

    bool setChequeData();
    void sendEmail();

    void appendLedgersList(LedgerMasterDataModel led);
    void setNarration(QString narration);


signals:
//    void closing();
//    void NotificationAccepted();
    void VoucherSaved(QString, QString, QString);

private slots:
    void setBillwiseMapping(LedgerMasterDataModel *ledger, QDate voucherDate);


};

#endif // PAYMENTVOUCHEREDITOR_H
