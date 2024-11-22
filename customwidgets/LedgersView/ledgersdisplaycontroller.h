#ifndef LEDGERSDISPLAYCONTROLLER_H
#define LEDGERSDISPLAYCONTROLLER_H

#include <QWidget>

#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"

#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include <QToolButton>
#include "login/loginvalues.h"


namespace Ui {
class LedgersDisplayController;
}

class LedgersDisplayController : public QWidget
{
    Q_OBJECT

public:

    explicit LedgersDisplayController(QWidget *parent = nullptr);
    ~LedgersDisplayController();

    int currencyDecimal = 2;

    int columnCount;

    bool selectByClick=false;

    int focusColumn=0;
    int focusRow =0;

    QList<int> ColumnsOrder;
    QStringList defaultHeader;

    QLineEdit *crAmount;
    QLineEdit *drAmount;

    GMLedgerSearchLite *ledgerLine;

    GMLedgerSearchLite *mainLed;


    QSqlQueryModel *ledModel;

    GeneralVoucherDataObject *voucher;

    LedgerMasterDatabaseHelper *ledHelper;

    void calcTotal();

    void setTable();

    void setVoucher(GeneralVoucherDataObject *value);

    void setTableHeaders(QStringList headers);

    void setFocusRowColumn(int row ,int column);

    void setnextFocus();

    void setWidgetFocused();

    void createEditableRow();

    void readStaticWidgets();

    void saveVoucher();

    void uisetups();

private slots:

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void deleteLedgerRow();

    void drAmtEntered();

    void crAmtEntered();

    void setLedgerDetails(LedgerMasterDataModel ledger);

private:

    Ui::LedgersDisplayController *ui;

signals:

    void tableClicked( QModelIndex index);
    void tableDoubleClicked(QModelIndex index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // LEDGERSDISPLAYCONTROLLER_H
