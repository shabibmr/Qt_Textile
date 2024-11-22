#ifndef VOUCHEREDITORBASE_H
#define VOUCHEREDITORBASE_H

#include <QWidget>
#include <QScreen>
#include <QLabel>
#include <QGroupBox>
#include <QInputDialog>

#include <login/loginvalues.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include <datamodels/transaction/gmvouchertypes.h>

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"

#include "customwidgets/Basic/gmlineedit.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"

#include "sharetools/print/printerhelper.h"
#include "sharetools/print/printdotmatrix.h"

#include "sharetools/email/email.h"
#include "sharetools/excel/excel.h"
#include "sharetools/sharetools.h"
#include "counter/MainScreen/addressdialog.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeelinematerial.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownlinematerial.h"
#include "customwidgets/SearchWidgets/Pricelist/gmpricelistlinematerial.h"

#include "database/Notifications/receivechanneldatabasehelper.h"
#include "database/voucherdbabstract.h"


class VoucherEditorBase : public QWidget
{
    Q_OBJECT

public:
    explicit VoucherEditorBase(QWidget *parent = nullptr);
    ~VoucherEditorBase();

    GeneralVoucherDataObject *voucher;
    UiAccessControlDataModel *permissions;
    int editorStatus;
    bool saveStatus = false;

    QString windowTitle;
    QLabel *windowLabel;
    QString bgcolor = "#e0f7fa";
    QColor appBarColor;

    int height;
    int width;

    QPointer<QVBoxLayout> mainLayout;
    QPointer<QVBoxLayout> tableLayout;
    QPointer<QHBoxLayout> voucherDataLayout;
    QPointer<QHBoxLayout> extrasLayout;
    QPointer<QHBoxLayout> extrasLayout2;

    QPointer<QVBoxLayout> verticalLayout;

    QtMaterialAppBar *appBar;
    QtMaterialSnackbar *snackBar;
    QtMaterialDrawer *drawer;

    QtMaterialFloatingActionButton *saveButton;
    QtMaterialFloatingActionButton *rejectButton;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *shareButton;
    QtMaterialIconButton *deleteButton;
    QtMaterialIconButton *moreButton;
    QtMaterialIconButton *contactButton;
    QtMaterialIconButton *exportButton;
    QtMaterialIconButton *paymentButton;

    QPointer<GMDateEdit> voucherDateWidget;
    QPointer<GMLineEdit> voucherNumberWidget;
    QPointer<GMPlainTextEdit> voucherNarrationWidget;
    QPointer<GMDateEdit> invoiceDateWidget;
    QPointer<GMLineEdit> invoiceNumberWidget;
    QPointer<GMMaterialComboBox> ReasonWidget;
    QLabel *invDateLabel;
    QLabel *invNumberLabel;

    QWidget *invoiceDataWidget;


    GMLedgerLineMaterial *mainLedgerWidget;
    GMEmployeeLineMaterial *SalesmanSearchBox;
    GMGodownLineMaterial *godownWidget;
    GMPriceListLineMaterial *priceListWidget;

    ShareTools *shareOptionsWidget;
    AddressDialog *AddressDialogWidget;
    PrinterHelper *printHelper;

    LedgerMasterDatabaseHelper *ledgerHelper;
    AccountGroupMasterDatabaseHelper *accHelper;
    VoucherDBAbstract *dbHelper;
    UomDataBaseHelper *uomHelper;
    UserProfileDatabaseHelper *userHelper;
    SalesInventoryItemDatabaseHelper *itemsHelper;
    receiveChannelDatabaseHelper *rHelper;
    PricelistDatabaseHelper *priceHelper;
    GodownDatabaseHelper *godownHelper;


    void setTitle(const QString &value);
    void setTableLayout(QWidget *widget);

    void setVoucher(GeneralVoucherDataObject *value);
    void setPermissions(UiAccessControlDataModel *value);

    virtual void showVoucher();
    virtual void saveVoucher();
    virtual void initVoucher();
    virtual void resetWidgets();

    virtual void closeEditor();
    virtual void deleteVoucher();

    virtual void setDrawerLayout();

    virtual void printVoucher(QPrinter *printer);
    virtual void exportToPdf(QPrinter *printer);

    virtual void importVoucher(GeneralVoucherDataObject *voucher2);
    virtual void importItemsFromVoucher(GeneralVoucherDataObject *voucher2);


    virtual void setMainLedger(LedgerMasterDataModel led);
    virtual void setSalesMan(int user);
    virtual void setGodown(QString godownId);
    virtual void setAdvancePaid(float amount);



    void addToExtrasLayout(QWidget *wid);
    void addToExtrasLayout2(QWidget *wid);
    void addToDrawerLayout(QWidget *wid);

    void setEditorStatus(int value);
    void disableSave();




private slots:

    void initGlobalObjects();
    void initExtraActions();
    void initDbHelpers();
    void setWidgets();

    void setVoucherDataLayout();
    void setAppBarLayout();
    void setExtrasLayout();

    void rejectVoucher();
    void showShareOptions();
    void showContactDetails();

signals:
    void closing();
    void saveCompleted();
    void NotificationAccepted();
    void NotificationRejected(QString);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // VOUCHEREDITORBASE_H
