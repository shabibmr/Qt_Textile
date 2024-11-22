#ifndef TABLESELECT_H
#define TABLESELECT_H

#include <QWidget>
#include <QSignalMapper>
#include <QScrollArea>
#include <QScrollBar>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QKeyEvent>

#include "datamodels/Counter/servicemode.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/qtbutton.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/Counter/TableManagement/tablesmasterhelper.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/Counter/countersettingsdatabasehelper.h"
#include "login/loginvalues.h"

#include "customwidgets/SearchWidgets/Ledger/gmledgerlinematerial.h"
#include "counter/MainScreen/addressdialog.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/Basic/gmdatetimeedit.h"

namespace Ui {
class TableSelect;
}

class TableSelect : public QWidget
{
    Q_OBJECT

public:
    explicit TableSelect(GeneralVoucherDataObject *voucher,
                         SalesOrderDatabaseHelper *salesOrderhelper,
                         LedgerMasterDatabaseHelper *ledHelper,
                         AccountGroupMasterDatabaseHelper *accHelper,
                         PricelistDatabaseHelper *priceHelper,
                         QWidget *parent = Q_NULLPTR);
    ~TableSelect();


    bool switchFlag = false;

    bool setManualTime = true;
    bool showAllDraftsFlag = true;
    void setTable();

    QString gTable ="";

    bool tableFlag;
    AddressDialog *AddressDialogWidget;

    int colCount;
    void setSwitchFlag(bool value);
    SalesOrderDatabaseHelper *soHelper;

    int fsize = 16;
    int buttonWidth=130;
    int buttonHeight=70;

    LedgerMasterDatabaseHelper *ledHelper;

    AccountGroupMasterDatabaseHelper *accHelper;

    TablesMasterHelper *dbHelper;

    void setNewLineText(QString text);
    QString tempRef;

    GMPlainTextEdit *delivery;
    bool focusOnCreateNewFlag = false;


    //    void setPieChart();
    //    void setBarchart();


public slots:
    void SelectedTable(QString reference);

    void setLineFocus();
    void tableSelected();
    void on_createNewButton_clicked();


signals:

    void selectTable();

    void switchTable();

private slots:

    void on_deliveryButton_clicked();

    void on_backButton_clicked();

    void on_createNewLineEdit_returnPressed();

    void on_pushButton_1_clicked();

    void on_pushButton_Existing_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_splitToolButton_toggled(bool checked);

    void on_preOrderButton_clicked();

    void showContactDetails(LedgerMasterDataModel data);
    void showContactDetailsBlank();

    void on_createNewLineEdit_cursorPositionChanged(int arg1, int arg2);

private:
    GeneralVoucherDataObject *voucher;
    PricelistDatabaseHelper *priceHelper;
    Ui::TableSelect *ui;
};

#endif // TABLESELECT_H
