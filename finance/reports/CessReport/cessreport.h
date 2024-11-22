#ifndef CESSREPORT_H
#define CESSREPORT_H

#include <QWidget>

namespace Ui {
class CessReport;
}

class CessReport : public QWidget
{
    Q_OBJECT

public:
    explicit CessReport(QWidget *parent = nullptr);
    ~CessReport();

private:
    Ui::CessReport *ui;
};

#endif // CESSREPORT_H
