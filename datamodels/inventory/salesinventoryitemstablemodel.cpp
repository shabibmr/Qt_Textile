#include "salesinventoryitemstablemodel.h"

SalesInventoryItemsTableModel::SalesInventoryItemsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant SalesInventoryItemsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int SalesInventoryItemsTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int SalesInventoryItemsTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant SalesInventoryItemsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
