#ifndef GMBILLWISEMAPPINGMODEL_H
#define GMBILLWISEMAPPINGMODEL_H

#include <QAbstractItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "datamodels/transaction/generalvoucherdataobject.h"


class GMBillwiseMappingModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GMBillwiseMappingModel(QSqlQueryModel *qModel,
                                    LedgerMasterDataModel *ledger,
                                    QObject *parent = nullptr);

    QSqlQueryModel *model;
    int colCount;
    QStringList headerStrings;

    LedgerMasterDataModel *ledger;


signals:

public slots:

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // QAbstractItemModel interface
    int getCurrentMapping(QString voucherNo, QString voucherType, QString voucherPrefix) const;

signals:
    void mapChanged()    ;
};

#endif // GMBILLWISEMAPPINGMODEL_H
