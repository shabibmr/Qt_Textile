#ifndef PAYMENTSPLITWIDGET_H
#define PAYMENTSPLITWIDGET_H

#include <QWidget>
#include "customwidgets/gmledgersearch.h"
#include <QMessageBox>

namespace Ui {
class PaymentSplitWidget;
}

class PaymentSplitWidget : public QWidget
{
    Q_OBJECT

    GeneralVoucherDataObject* voucher;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;
    LedgerMasterDatabaseHelper *ledgerHelper;
    bool openNewWidgetLedger = true;

public:
    explicit PaymentSplitWidget(QWidget *parent = nullptr);
    ~PaymentSplitWidget();

    void setVoucher(GeneralVoucherDataObject *value);



private slots:

    void on_cancelButton_clicked();

    void on_OKButton_clicked();



signals :
    void paymentSplit(QList<LedgerMasterDataModel> ledList);

public slots:

    void setTable();
    void appendLedger(LedgerMasterDataModel item, int row);

    void updateLedgerValue();
    void getLedgerDetails(LedgerMasterDataModel led);

    void setLedList(QList<LedgerMasterDataModel> ledList);


private:
    Ui::PaymentSplitWidget *ui;
};

#endif // PAYMENTSPLITWIDGET_H
