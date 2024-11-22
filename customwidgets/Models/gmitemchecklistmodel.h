#ifndef GMITEMCHECKLISTMODEL_H
#define GMITEMCHECKLISTMODEL_H

#include <QAbstractItemModel>
#include "datamodels/transaction/generalvoucherdataobject.h"

class GMItemCheckListModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit GMItemCheckListModel(GeneralVoucherDataObject *voucher, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

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

private:
    GeneralVoucherDataObject *voucher;
//    QList<CompoundItemDataObject> refList;
//    QList<CompoundItemDataObject> checkingList;
    int colCount;
    QStringList headerStrings;
signals:
    void checkQtyUpdated();
};

#endif // GMITEMCHECKLISTMODEL_H
