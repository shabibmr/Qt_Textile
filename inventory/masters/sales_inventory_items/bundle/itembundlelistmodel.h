
#ifndef ITEMBUNDLELISTMODEL_H
#define ITEMBUNDLELISTMODEL_H


#include <QObject>
#include <QAbstractItemModel>
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include <QSqlQueryModel>

class ItemBundleListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ItemBundleListModel(QList<inventoryItemDataModel> items, QObject *parent=Q_NULLPTR);
    ~ItemBundleListModel(){}

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override ;
    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role)  override;

    QSqlQueryModel *model;

    QList<inventoryItemDataModel> items;

    int colCount = 4;

    // QAbstractItemModel interface

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // QAbstractItemModel interface
public:
    bool insertRows(int row, int count, const QModelIndex &parent)override;
    bool removeRows(int row, int count, const QModelIndex &parent)override;
    Qt::ItemFlags flags(const QModelIndex &index) const;
signals:
    void updateValues();
    void newLineAdded(int row);
};








#endif // ITEMBUNDLELISTMODEL_H
