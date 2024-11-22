#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QWidget>
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "datamodels/addressbook/employeeaddressdatamodel.h"
#include <QPointer>
#include "customwidgets/gmledgersearch.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

#include "database/addressbook/addressbookdatabasehelper.h"


namespace Ui {
class AddressBook;
}

class AddressBook : public QWidget
{
    Q_OBJECT

public:
    explicit AddressBook(QWidget *parent = 0);

    AddressBookDatabaseHelper *dbHelper;
    LedgerMasterDatabaseHelper *ledgerHelper;
    QList<QSharedPointer<LedgerMasterDataModel>> allLedgersPtr ;

    int contactId=0;
    LedgerMasterDataModel ledger;

    GMLedgerSearch *vendSearch;
    GMLedgerSearch *custSearch;

    ~AddressBook();
public slots:
    void edit(int id);
    void setLedger(LedgerMasterDataModel);

private:
    Ui::AddressBook *ui;

signals:
    void closing();
private slots:
    void on_customerButtonBox_rejected();
    void on_customerButtonBox_accepted();
    void on_vendorButtonBox_accepted();
    void on_employeeButtonBox_accepted();


};

#endif // ADDRESSBOOK_H
