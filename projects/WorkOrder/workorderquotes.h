#ifndef WORKORDERQUOTES_H
#define WORKORDERQUOTES_H

#include <QWidget>
#include "workorderitemdetail.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "sharetools/print/printerhelper.h"
#include <QToolButton>
#include <database/inventory/salesorder/salesorderdatabasehelper.h>
#include <QDialog>
#include <QComboBox>
#include <QDateEdit>

namespace Ui {
class WorkOrderQuotes;
}

class WorkOrderQuotes : public QWidget
{
    Q_OBJECT

public:
    explicit WorkOrderQuotes(QWidget *parent = 0);
    ~WorkOrderQuotes();
    GeneralVoucherDataObject Voucher;

    GeneralVoucherDataObject ReadyMadeVoucher;
    GeneralVoucherDataObject CustomItemsVoucher;




    GeneralVoucherDataObject SalesOrderVoucher;

    WorkOrderItemDetail* WorkOrderItemDetailWidget;

    UserProfileDatabaseHelper *user;

    QList<DocumentsDataModel> docObj;

    SalesOrderDatabaseHelper* dbHelper;

    PrinterHelper* pHelper;

    QToolButton *CompletedButton;

    QToolButton *printButton;

    QLineEdit* SerialNumber;
    QDateEdit* CompletedDate;
    QComboBox* CompletedByCombo;
    QDialog* SerialNumberDialog;
    QMap<int,QString> people;

    int glindex=0;
    void showSerialDialog(int index);

private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_save_clicked();

    void on_cancelButton_clicked();

    void delButtonClicked(bool);

    void downloadClicked(bool);

    void printWorkOrder();

    void checkToggle(bool);

public slots:
    void settable();
    void technicianAssigned(int x);
    void ExpectedDateChenged(QDate date);
    void showVoucher(GeneralVoucherDataObject quote);
    void completedClicked(bool t);
    void setSerialNumber();
    void printSerialButtonClicked();
    void OkButtonClicked();
    void CancelButtonClicked();

signals:
    void closing();
private:
    Ui::WorkOrderQuotes *ui;
};

#endif // WORKORDERQUOTES_H
