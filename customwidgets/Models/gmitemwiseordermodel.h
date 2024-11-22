#ifndef GMITEMWISEORDERMODEL_H
#define GMITEMWISEORDERMODEL_H

#include <QAbstractItemModel>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <datamodels/projects/Quotation/quotationstatus.h>
#include "finance/transactions/calculator/transactioncalculator.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"

#include <QDebug>

class GMItemwiseOrderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMItemwiseOrderModel(GeneralVoucherDataObject *voucher,PricelistDatabaseHelper *phelp, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

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

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;



    void setHeaderStrings(const QStringList &value);

    void setVoucher(GeneralVoucherDataObject *value);

    void setCurrentItem(inventoryItemDataModel value);

private:
    GeneralVoucherDataObject *voucher;
    inventoryItemDataModel CurrentItem;
    int colCount;
    QStringList headerStrings;
    PricelistDatabaseHelper *phelper;

signals:
    void updateValues();
    void newLineAdded(int row);

};

#endif // GMITEMWISEORDERMODEL_H
