#include "ledgercompletertablemodel.h"


LedgerCompleterTableModel::LedgerCompleterTableModel(QObject *parent)
{

}

LedgerCompleterTableModel::LedgerCompleterTableModel(QList<LedgerMasterDataModel> listOfLedger, QObject *parent)
{
    allLedgers =listOfLedger;
}

int LedgerCompleterTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return allLedgers.size();
}

int LedgerCompleterTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant LedgerCompleterTableModel::data(const QModelIndex &index, int role) const
{

}

QVariant LedgerCompleterTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

}

Qt::ItemFlags LedgerCompleterTableModel::flags(const QModelIndex &index) const
{

}

bool LedgerCompleterTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

bool LedgerCompleterTableModel::insertRows(int position, int rows, const QModelIndex &index)
{

}

bool LedgerCompleterTableModel::removeRows(int position, int rows, const QModelIndex &index)
{

}

QList<LedgerMasterDataModel> LedgerCompleterTableModel::getList()
{

}
