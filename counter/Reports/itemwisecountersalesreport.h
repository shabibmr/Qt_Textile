#ifndef ITEMWISESALESREPORT_H
#define ITEMWISESALESREPORT_H

#include <QWidget>

namespace Ui {
class ItemWiseCounterSalesReport;
}

class ItemWiseCounterSalesReport : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWiseCounterSalesReport(QWidget *parent = 0);
    ~ItemWiseCounterSalesReport();

private:
    Ui::ItemWiseCounterSalesReport *ui;
};

#endif // ITEMWISESALESREPORT_H
