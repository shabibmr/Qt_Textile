#ifndef ADDITEMADDITIONALREQUIREMENT_H
#define ADDITEMADDITIONALREQUIREMENT_H

#include <QWidget>
#include <QHeaderView>
#include <QDoubleValidator>

#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "customwidgets/gmsearchlineedit.h"
#include <QStandardItemModel>
#include <QCompleter>
#include "customwidgets/listviewdelegate.h"
#include <QTableView>
#include "customwidgets/gmledgersearch.h"

namespace Ui {
class addItemAdditionalRequirement;
}

class addItemAdditionalRequirement : public QWidget
{
    Q_OBJECT

public:
    explicit addItemAdditionalRequirement(QWidget *parent = 0);
    ~addItemAdditionalRequirement();
    LedgerMasterDatabaseHelper *ledHelper;
    QList<LedgerMasterDataModel> ledgersList;
    QStandardItemModel* modelLedger;
    QList<QSharedPointer <LedgerMasterDataModel>> ledsPtr;

    QStringList LedNames;

    LedgerMasterDataModel ledger;

    void setLedsPtr(QList<QSharedPointer <LedgerMasterDataModel>> value);

signals:
    void closing(LedgerMasterDataModel item);
public slots:

    void edit(LedgerMasterDataModel ledgerObj);
    void setTitle(QString text);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addItemAdditionalRequirement *ui;
};

#endif // ADDITEMADDITIONALREQUIREMENT_H
