#ifndef LEDGEROPENINGVALUES_H
#define LEDGEROPENINGVALUES_H

#include <QWidget>
#include <QLineEdit>

#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

namespace Ui {
class LedgerOpeningValues;
}

class LedgerOpeningValues : public QWidget
{
    Q_OBJECT

public:
    explicit LedgerOpeningValues(QWidget *parent = 0);
    ~LedgerOpeningValues();

    LedgerMasterDatabaseHelper* ledHelper;
    QList<QSharedPointer<LedgerMasterDataModel >> ledsList;
    QString text;



private slots:
    void on_lineEdit_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void updateValue();

    void setTable();

private:
    Ui::LedgerOpeningValues *ui;
};

#endif // LEDGEROPENINGVALUES_H
