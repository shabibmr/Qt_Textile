#ifndef DELIVERYORDERSMODEL_H
#define DELIVERYORDERSMODEL_H

#include <QAbstractItemModel>
#include <database/inventory/salesorder/salesorderdatabasehelper.h>

class DeliveryOrdersModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    DeliveryOrdersModel(SalesOrderDatabaseHelper *soHelper,QSqlQueryModel *model,QObject *parent = Q_NULLPTR) ;

private:
    QSqlQueryModel *model;

    int rowCnt;
    int colCnt;

    SalesOrderDatabaseHelper *soHelper;

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void updateLedgerID(QString ledID,QString ledName,QString vNo,QString vPrefix);
    void updateEmployee(int empID,QString vNo,QString vPrefix);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;


};

#endif // DELIVERYORDERSMODEL_H
