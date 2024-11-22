#ifndef SALESADJUSTMENTWIDGET_H
#define SALESADJUSTMENTWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QDebug>

#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "finance/transactions/calculator/transactioncalculator.h"
#include "database/Reports/inventoryhelper.h"
#include <QMessageBox>
#include <QFutureWatcher>

namespace Ui {
class SalesAdjustmentWidget;
}

class SalesAdjustmentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SalesAdjustmentWidget(QWidget *parent = nullptr);
    ~SalesAdjustmentWidget();

    salesVoucherDatabaseHelper *dbHelper;


    QSqlQueryModel *itemModel;

    QStringList itemList;
    QDateTime toDateTime;
    QDateTime fromDateTime;
    float totalSelected =0;

    QList<QPair<QString, QString>> voucherList;

    QStringList excludedVouchersList;


public slots:
    void setItemTable();


private slots:
    void on_okButton_clicked();
    void setTableHeader();

    void on_recalculatePushButton_clicked();
    void recalc_finished();
    void backupFinished();



    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_exclVoucherLineEdit_editingFinished();

    void on_backupPushButton_clicked();


private:
    QFutureWatcher<void> FutureWatcher;
    Ui::SalesAdjustmentWidget *ui;

signals:
    void voucherRecalculated();
};

#endif // SALESADJUSTMENTWIDGET_H
