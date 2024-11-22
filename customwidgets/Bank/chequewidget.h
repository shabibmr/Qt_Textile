#ifndef CHEQUEWIDGET_H
#define CHEQUEWIDGET_H

#include <QWidget>
#include "datamodels/accounts/chequedetail.h"
#include "database/finance/banking/chequedetailsdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

#include "customwidgets/appbarwidget.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"


//namespace Ui {
//class ChequeWidget;
//}

class ChequeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChequeWidget(GeneralVoucherDataObject *voucher, QWidget *parent = nullptr);
    ~ChequeWidget();

    ChequeDetailsDatabaseHelper* dbHelper;
    LedgerMasterDatabaseHelper* ledHelper;
    GeneralVoucherDataObject* voucher;

    AppBarWidget *appBar;
    QtMaterialFloatingActionButton *saveButton;

    GMDateEdit *instrumentDateWidget;
    GMMaterialLineEdit *payeeNameWidget;
    GMMaterialLineEdit *instrumentNumberWidget;
    GMMaterialNumberEdit *amountWidget;
    GMMaterialComboBox *paymentTypeWidget;
    GMMaterialLineEdit *bankNameWidget;
    GMMaterialLineEdit *branchWidget;


//    void setVoucher(GeneralVoucherDataObject* obj);
//    void setChanges();

private slots:

//    void on_savePushButton_clicked();
//    void on_cancelPushButton_clicked();

    void setWidgets();
    void resetWidgets();

    void closeWidget();

//private:
//    Ui::ChequeWidget *ui;

signals:
    void closing();
    void save();

};

#endif // CHEQUEWIDGET_H
