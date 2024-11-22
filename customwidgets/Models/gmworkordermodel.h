#ifndef GMWORKORDERMODEL_H
#define GMWORKORDERMODEL_H

#include <QAbstractItemModel>
#include <QDebug>
#include <QMessageBox>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include "finance/transactions/calculator/fifocalculator.h"

class GMWorkOrderModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMWorkOrderModel(GeneralVoucherDataObject *voucher,int col,bool isData=1, QObject *parent = nullptr);

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


    void setHeaderStrings(const QStringList &value);

    void setVoucher(GeneralVoucherDataObject *value);

    void updateGodownList(int row);

    FifoCalculator *fifoCalc;
private:
    GeneralVoucherDataObject *voucher;
    int colCount;
    QStringList headerStrings;
    bool isData=true;

signals:
    void updateValues();
    void newLineAdded(int row);
};

#endif // GMWORKORDERMODEL_H
