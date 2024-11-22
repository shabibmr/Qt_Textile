#ifndef RECEIPTSMAINDASH_H
#define RECEIPTSMAINDASH_H

#include <QWidget>
#include "database/Reports/ledgerhelper.h"
namespace Ui {
class ReceiptsMainDash;
}

class ReceiptsMainDash : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptsMainDash( LedgerHelper *ledgerHelper = new LedgerHelper,QWidget *parent = nullptr);
    ~ReceiptsMainDash();

private:
    Ui::ReceiptsMainDash *ui;
    LedgerHelper *dbhelper;
};

#endif // RECEIPTSMAINDASH_H
