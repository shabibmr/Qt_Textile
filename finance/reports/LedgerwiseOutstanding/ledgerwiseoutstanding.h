#ifndef LEDGERWISEOUTSTANDING_H
#define LEDGERWISEOUTSTANDING_H

#include <QWidget>

#include <database/finance/reports/reportsdatabasehelper.h>
#include <datamodels/transaction/generalvoucherdataobject.h>

namespace Ui {
class LedgerwiseOutstanding;
}

class LedgerwiseOutstanding : public QWidget
{
    Q_OBJECT

public:
    explicit LedgerwiseOutstanding(QWidget *parent = 0);
    ~LedgerwiseOutstanding();

    QList<GeneralVoucherDataObject> voucherList;
    ReportsDatabaseHelper *dbhelper;

private:
    Ui::LedgerwiseOutstanding *ui;

private slots:
    void setTable();
};

#endif // LEDGERWISEOUTSTANDING_H
