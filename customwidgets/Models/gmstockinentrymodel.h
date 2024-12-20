#ifndef GMSTOCKINENTRYMODEL_H
#define GMSTOCKINENTRYMODEL_H

#include <QAbstractItemModel>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QDebug>
#include <QFont>

class GMStockInEntryModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMStockInEntryModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

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


    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


private:
};

#endif // GMSTOCKINENTRYMODEL_H
