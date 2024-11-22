#ifndef GMFIFOMODEL_H
#define GMFIFOMODEL_H

#include <QAbstractItemModel>
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"

class GMFifoModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMFifoModel(GodownTransactionDataModel *godownAllocObj,QStringList list,QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

//    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setItems( inventoryItemDataModel *value);

    void calulateAvgPrice();

    void setGodownEntry(GodownTransactionDataModel *value);

private:
    GodownTransactionDataModel *godownAllocObj;
    QStringList headers;
    int colCount;
    int rowCnt;
};

#endif // GMFIFOMODEL_H
