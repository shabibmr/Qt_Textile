#ifndef POCWISESALESSUMMARY_H
#define POCWISESALESSUMMARY_H

#include <QWidget>
#include <database/Reports/inventoryhelper.h>
#include <customwidgets/test/gmitemsortfilterproxymodel.h>


namespace Ui {
class POCwiseSalesSummary;
}

class POCwiseSalesSummary : public QWidget
{
    Q_OBJECT

public:
    explicit POCwiseSalesSummary(QString title,QWidget *parent = nullptr);
    ~POCwiseSalesSummary();
    InventoryHelper *inventoryReportsHelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

    QString phoneNumber;

    void setPhoneNumber(const QString &value);

private slots:
    void on_OKButton_2_clicked();
    void setTableView();
private:
    Ui::POCwiseSalesSummary *ui;
};

#endif // POCWISESALESSUMMARY_H
