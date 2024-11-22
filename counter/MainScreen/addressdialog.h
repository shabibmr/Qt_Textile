#ifndef ADDRESSDIALOG_H
#define ADDRESSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QSqlQueryModel>
#include <QCompleter>
#include "database/addressbook/addressbookdatabasehelper.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include <QListWidget>
#include "addressbook/measure/contactmeasurementpage.h"
namespace Ui {
class AddressDialog;
}

class AddressDialog : public QDialog
{
    Q_OBJECT

public:
    QString oldNumber;
    explicit AddressDialog(/*GeneralVoucherDataObject *voucher,*/
                           LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper(),
                           QWidget *parent = Q_NULLPTR);
    QSqlDatabase db;
    ~AddressDialog();

    int callCount =0;
    bool allowEmptyAddress = false;

    bool editMode = false;

    void setFocustoPhone();

    void setVoucher(GeneralVoucherDataObject *value);
    GeneralVoucherDataObject *voucher;
    QSqlQueryModel *smodel;
    QSqlQueryModel *smodel2;
    QCompleter *nameCompleter;
    QCompleter *Phonecompleter;


    GMLedgerLineWidget *ledger;

    AddressBookDatabaseHelper *dbHelper;

    LedgerMasterDatabaseHelper *ledHelper;

    AddressBookDataModel *addr;

    ContactMeasurementPage *measurePage;

//    QWidget *LocationHistory;
    QListWidget* locationsList;

    void setAddr();
    void disableCancel();
    int voucherType;

    void setVoucherType(int value);

private slots:

    void on_buttonBox_accepted();

    void on_phoneEdit_returnPressed();

//    void on_NameEdit_returnPressed();
    void setAddressFromList(QListWidgetItem *item);
    void removeAddressFromList(QListWidgetItem *item);

    void keyPressEvent(QKeyEvent *e);

    void on_phoneEdit_editingFinished();

    void on_selectButton_clicked();

    void on_okButton_clicked();

    void setTable();

    void on_close_clicked();

    void on_addOk_clicked();

    void on_NextButton_clicked();

    void on_prevButton_clicked();

    void on_AddCancel_clicked();

    void on_codeLineEdit_returnPressed();

    void setLedger(LedgerMasterDataModel item);

    void setupMeasurements();

    void on_showMeasurements_clicked();

public slots :

    void setAddressText();

    void setNumberAndOpen(QString number);

    void reset();

    void completerActivated(QModelIndex index);


signals :
    void setAddress(QString,QString,QString, QString);

    void addressSelected();
    void addrSelectedForDraft();
    void addrSelectedForInvoice();

protected slots:
    void completerActivatedByName(QModelIndex index);
private:
    Ui::AddressDialog *ui;
};

#endif // ADDRESSDIALOG_H
