#ifndef GMINVENTORYITEMMODEL_H
#define GMINVENTORYITEMMODEL_H

#include <QAbstractItemModel>
#include "finance/transactions/calculator/transactioncalculator.h"
#include "finance/transactions/calculator/fifocalculator.h"
#include <QDebug>
#include <QMessageBox>
#include "database/inventory/sales_inventory_items/salesinventoryitemdatabasehelper.h"
#include "customwidgets/Fifo/fifopopupdialog.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"


class GMInventoryItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMInventoryItemModel(GeneralVoucherDataObject *voucher,int transactionType=1, QObject *parent = nullptr);

    bool useSectionFlag;
    bool allowNegativeStock;
    bool allowItemMultipleEntryFlag;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

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

    PricelistDatabaseHelper *pricehelper;
    SalesInventoryItemDatabaseHelper itemHelper;
    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
//  bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

//  Remove data:

//  bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;


    // Fetch data dynamically:
//    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

//    bool canFetchMore(const QModelIndex &parent) const override;
//    void fetchMore(const QModelIndex &parent) override;


//    void setHeaderStrings(const QStringList &value);

    void setVoucher(GeneralVoucherDataObject *value);

    void setAllowNegativeStock(bool newAllowNegativeStock);

private:
    GeneralVoucherDataObject *voucher;
    int colCount;
    QStringList headerStrings;
    int transactionType;

    void setItemQtyByFifo(int row);
    void setItemRateforPurchase(int row);
    void setItemRateforSales(int row);

    SalesInventoryItemDatabaseHelper *shelper;
    FifoCalculator* fifoCalc;

signals:
    void updateValues();
    void newLineAdded(int row);
    void qtyEdited(int row);
    void noStock(int row);
    void itemRepeated(int row);


private slots:
//    void setItemBatchEntry(int row);
};

#endif // GMINVENTORYITEMMODEL_H
