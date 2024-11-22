#ifndef ZOMATOREPORT_H
#define ZOMATOREPORT_H

#include <QWidget>
#include <QDebug>
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "sharetools/excel/excelreporthelper.h"

namespace Ui {
class ZomatoReport;
}

class ZomatoReport : public QWidget
{
    Q_OBJECT

public:
    explicit ZomatoReport(QWidget *parent = 0);
    ~ZomatoReport();

private slots:
    void on_OKpushButton_clicked();
    void setTable();
    void on_exportToExcelButton_clicked();
    void on_commPercent_textChanged(const QString &arg1);

    void on_calcButton_clicked();



    void on_excel_clicked();

private:
    Ui::ZomatoReport *ui;
};

#endif // ZOMATOREPORT_H
