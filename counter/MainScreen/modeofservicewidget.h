#ifndef MODEOFSERVICEWIDGET_H
#define MODEOFSERVICEWIDGET_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QToolButton>
#include <QString>
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include <QSqlRecord>
#include "counter/MainScreen/addressdialog.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"

#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include <QMessageBox>
#include "utils/SettingsHelper/settingshelper.h"
#include "customwidgets/modeofservicebutton.h"

#include "customwidgets/test/flowlayout.h"
namespace Ui {
class ModeOfServiceWidget;
}

class ModeOfServiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModeOfServiceWidget(GeneralVoucherDataObject *voucher,
                                 PricelistDatabaseHelper *priceHelper,
                                 LedgerMasterDatabaseHelper *ledHelper,
                                 QWidget *parent = nullptr);
    ~ModeOfServiceWidget();

     QFont font;

    GeneralVoucherDataObject *voucher;

    SettingsHelper *settings;

    void setVoucher(GeneralVoucherDataObject*);

    PricelistDatabaseHelper *priceHelper;

    LedgerMasterDatabaseHelper *ledgerHelper;

    AddressDialog *addressDialog;

    GMLedgerLineMaterial *ledgerWidget;

    QtMaterialDrawer *drawer;

    FlowLayout *mlay;

public slots:

    void setView();

    void recalculatePrice();

    void ServiceSelected();

    void resetIcons();

    void setDefault();

    void openAddressDialog(QString number="");

    void setService(int i);

    void setAddress();

signals:
    void itemPricesChanged();

private:
    Ui::ModeOfServiceWidget *ui;

    QList<ModeOfServiceButton*> buttonsList;

    bool askAddressFlag;
    bool neverAskAddressFlag;
};

#endif // MODEOFSERVICEWIDGET_H
