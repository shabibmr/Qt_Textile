#ifndef SALESINVENTORYITEMSTABLEMODEL_H
#define SALESINVENTORYITEMSTABLEMODEL_H

#include <QAbstractTableModel>

class SalesInventoryItemsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SalesInventoryItemsTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // SALESINVENTORYITEMSTABLEMODEL_H