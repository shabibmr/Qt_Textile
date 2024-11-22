#ifndef GMSHIFTSALESMODEL_H
#define GMSHIFTSALESMODEL_H

#include <QAbstractItemModel>
#include "datamodels/Counter/shiftdatamodel.h"
#include <QDebug>
#include <QFont>
#include "finance/transactions/calculator/transactioncalculator.h"


class GMShiftSalesModel : public QAbstractItemModel
{
     Q_OBJECT

public:
    explicit GMShiftSalesModel(ShiftDataModel *shift, int col, QObject *parent = nullptr);

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

    void setHeaderStrings(const QStringList &value);

    void setModel(ShiftDataModel *value);

private:
    ShiftDataModel *shift;
    int colCount;
    QStringList headerStrings;

signals:
//    void updateValues();
//    void newLineAdded(int row);
};

#endif // GMShiftSalesModel_H
