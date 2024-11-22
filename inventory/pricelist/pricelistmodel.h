#ifndef PRICELISTMODEL_H
#define PRICELISTMODEL_H

#include <QAbstractItemModel>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>
#include <QTableView>
#include "customwidgets/Material/components/qtmaterialscrollbar.h"

class PriceListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PriceListModel(PriceListDataObject *obj,int col, QObject *parent = nullptr);

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

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setHeaderStrings(const QStringList &value);

    void setPriceList(PriceListDataObject *value);

private:
    PriceListDataObject *priceList;
    int colCount;
    QStringList headerStrings;

signals:
    void updateValues();
    void newLineAdded(int row);

};

#endif // PRICELISTMODEL_H
