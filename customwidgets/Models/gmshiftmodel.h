#ifndef GMSHIFTMODEL_H
#define GMSHIFTMODEL_H

#include <QAbstractItemModel>
#include "datamodels/Counter/shiftdatamodel.h"


class GMShiftModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    GMShiftModel(ShiftDataModel *model, int tablePos, QObject *parent = nullptr);

    ShiftDataModel *shift;
    int colCount;
    int rowCnt;

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    // QAbstractItemModel interface
public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // GMSHIFTMODEL_H
