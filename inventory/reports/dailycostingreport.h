#ifndef DAILYCOSTINGREPORT_H
#define DAILYCOSTINGREPORT_H

#include <QWidget>

namespace Ui {
class DailyCostingReport;
}

class DailyCostingReport : public QWidget
{
    Q_OBJECT

public:
    explicit DailyCostingReport(QWidget *parent = nullptr);
    ~DailyCostingReport();

private:
    Ui::DailyCostingReport *ui;
};

#endif // DAILYCOSTINGREPORT_H
