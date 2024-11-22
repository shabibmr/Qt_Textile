#ifndef JOURNALVOUCHEREDITOR_H
#define JOURNALVOUCHEREDITOR_H

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
#include "customwidgets/Models/gmjournalentrymodel.h"

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
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"


#include "database/finance/journalvoucher/journalvoucherdatabasehelper.h"
#include "customwidgets/Bank/chequewidget.h"

#include "CommonUi/Transactions/vouchereditorbase.h"


class JournalVoucherEditor : public VoucherEditorBase
{
    Q_OBJECT

public:
    explicit JournalVoucherEditor(VoucherDBAbstract *db, VoucherEditorBase *parent = nullptr);
    ~JournalVoucherEditor();


    ChequeDetail *chqObj;
    ChequeWidget *chequeWidget;

    LedgervoucherTableView *DataTableWidget;                 //X
    QPointer<GMJournalEntryModel> model;

    QStringList lis;

    void initExtraActions();

    void initVoucher();

    void initGlobalObjects();

    void resetWidgets();

public slots :
    void showVoucher() override;
    void saveVoucher() override;
    void printVoucher(QPrinter *printer) override;
    void setChequeData();
    void sendEmail();



};

#endif // JOURNALVOUCHEREDITOR_H
