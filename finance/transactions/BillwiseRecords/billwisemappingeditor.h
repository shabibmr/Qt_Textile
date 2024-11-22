#ifndef BILLWISEMAPPINGEDITOR_H
#define BILLWISEMAPPINGEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "customwidgets/TableViews/reportstableview.h"
#include "customwidgets/Models/gmbillwisemappingmodel.h"
#include "database/finance/BillwiseRecords/billwiserecordsdatabasehelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Delegates/billwisemappingdelegate.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"



class BillwiseMappingEditor : public QWidget
{
    Q_OBJECT
public:
    explicit BillwiseMappingEditor(LedgerMasterDataModel *ledger,
                                   GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject,
                                   QWidget *parent = nullptr);

    GeneralVoucherDataObject *voucher;
    LedgerMasterDataModel *ledger ;
    BillwiseRecordsDatabaseHelper *dbHelper;
    ReportsTableView *tableView;
    GMBillwiseMappingModel *model;
    QSqlQueryModel *qModel;

    QLabel *amountEntered  ;
    QLabel *amountSelected ;
    QLabel *amountOnAccount;

    QtMaterialFlatButton *clearDuesButton;
    QtMaterialFloatingActionButton *saveButton;
    QtMaterialFloatingActionButton *cancelButton;

    float onAccount = 0;  //
    float selected = 0;

    QString newRef = " ";
    float newRefAmount = 0;

    GMMaterialLineEdit *newRefEdit;
    GMMaterialNumberEdit *newRefAmtEdit;

    QList<BillwiseRecordDataModel *> mapList;

    bool clearDuesFlag = false;




signals:

public slots:

    void setWidgets();
    void calcValues();
    void initMapList();
    void addNewReferenceMap();
    void clearDues();
    void showClearDues();
    void resetWidgets();

    void saveMapping();
    void clearMapping();
};

#endif // BILLWISEMAPPINGEDITOR_H
