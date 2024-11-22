#ifndef LEDGERWISEVOUCHERTABLEVIEW_H
#define LEDGERWISEVOUCHERTABLEVIEW_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/TableViews/ledgerwisetableview.h"
#include "customwidgets/Models/gmledgeritemmodel.h"
#include "customwidgets/Delegates/Ledgers/gmledgertransactiondelegate.h"

namespace Ui {
class LedgerwiseVoucherTableView;
}

class LedgerwiseVoucherTableView : public QWidget
{
    Q_OBJECT

public:
    explicit LedgerwiseVoucherTableView(GeneralVoucherDataObject *voucher,
                                        LedgerMasterDatabaseHelper *ledHelper,
                                        QWidget *parent = nullptr);
    ~LedgerwiseVoucherTableView();
     void initGlobalObjects();
    LedgerwiseTableView *DataTableWidget;
    GMLedgerItemModel *model;
    QStringList lis;
    void setVoucher(GeneralVoucherDataObject *value);

    void resetWidgets();

public slots:
    void updateData();
signals:
    void ledUpdated();
private:

    GeneralVoucherDataObject *voucher;
    LedgerMasterDatabaseHelper *ledgerHelper;
    Ui::LedgerwiseVoucherTableView *ui;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // LEDGERWISEVOUCHERTABLEVIEW_H
