#ifndef LEDGERCOMPLETERTABLEMODEL_H
#define LEDGERCOMPLETERTABLEMODEL_H

#include <QAbstractTableModel>
#include "datamodels/accounts/ledgermasterdatamodel.h"

class LedgerCompleterTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    LedgerCompleterTableModel(QObject *parent=0);
    LedgerCompleterTableModel(QList< LedgerMasterDataModel > listOfLedger, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::DisplayRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
    QList< LedgerMasterDataModel > getList();

private :
    QList <LedgerMasterDataModel> allLedgers;
};

#endif // LEDGERCOMPLETERTABLEMODEL_H
