#ifndef DAILYCLOSINGSALESREPORT_H
#define DAILYCLOSINGSALESREPORT_H

#include <QWidget>

namespace Ui {
class DailyClosingSalesReport;
}

class DailyClosingSalesReport : public QWidget
{
    Q_OBJECT

public:
    explicit DailyClosingSalesReport(QWidget *parent = 0);
    ~DailyClosingSalesReport();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::DailyClosingSalesReport *ui;
};

#endif // DAILYCLOSINGSALESREPORT_H
