#ifndef LEDGERWISEVOUCHERVIEW_H
#define LEDGERWISEVOUCHERVIEW_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/TableViews/ledgervouchertableview.h"
#include "customwidgets/Models/gmledgeritemmodel.h"
#include "customwidgets/Delegates/Ledgers/gmledgertransactiondelegate.h"


namespace Ui {
class LedgerwiseVoucherView;
}
class LedgerwiseVoucherView : public QWidget
{
    Q_OBJECT
public:
    explicit LedgerwiseVoucherView(GeneralVoucherDataObject *voucher,
                                    LedgerMasterDatabaseHelper *ledHelper,
                                   QWidget *parent = nullptr);

    ~LedgerwiseVoucherView();



    void initGlobalObjects();



    LedgervoucherTableView *DataTableWidget;
    GMLedgerItemModel *model;
    QStringList lis;

private:
    GeneralVoucherDataObject *voucher;
    LedgerMasterDatabaseHelper *ledgerHelper;
signals:

};

#endif // LEDGERWISEVOUCHERVIEW_H
