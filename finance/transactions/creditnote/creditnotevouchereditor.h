#ifndef CREDITNOTEVOUCHEREDITOR_H
#define CREDITNOTEVOUCHEREDITOR_H

#include <QWidget>
#include <QPointer>
#include <QKeyEvent>
#include <QLabel>
#include <QSpacerItem>
#include <QMessageBox>
#include <QInputDialog>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/TableViews/inventoryvouchertableview.h"
#include "customwidgets/Models/gminventoryitemmodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include <customwidgets/Delegates/Inventory/gminventorytransactiondelegate.h>
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "counter/MainScreen/itemdetail.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/gmcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"


#include "sharetools/print/printerhelper.h"
#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"

#include "database/finance/creditnote/creditnotevoucherdatabasehelper.h"
#include "CommonUi/Transactions/InventoryLedger/inventoryledgertransactionseditor.h"

class CreditNoteVoucherEditor : public InventoryLedgerTransactionsEditor
{
    Q_OBJECT

public:
    explicit CreditNoteVoucherEditor(CreditNoteVoucherDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent = nullptr);
//    ~CreditNoteVoucherEditor();

};

#endif // CREDITNOTEVOUCHEREDITOR_H
