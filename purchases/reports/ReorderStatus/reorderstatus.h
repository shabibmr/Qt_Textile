#ifndef REORDERSTATUS_H
#define REORDERSTATUS_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>

namespace Ui {
class ReorderStatus;
}

class ReorderStatus : public QWidget
{
    Q_OBJECT

public:
    explicit ReorderStatus(QWidget *parent = 0);
    ~ReorderStatus();
    QSqlQueryModel *model;
    InventoryHelper *itemReportHelper;

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

    void setTable();

private:
    Ui::ReorderStatus *ui;
};

#endif // REORDERSTATUS_H
