#ifndef GMGODOWNALLOCATIONMODEL_H
#define GMGODOWNALLOCATIONMODEL_H

#include <QWidget>
#include <QAbstractItemModel>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QDebug>
#include <QFont>

class GMGodownAllocationModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit GMGodownAllocationModel(GodownTransactionDataModel *godownEntry,QObject *parent = nullptr);

    ~GMGodownAllocationModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;



    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QStringList headerStrings;
    void setGodownEntry(GodownTransactionDataModel *value);
    int colCount;

signals:
    void updateValues();
private:
    GodownTransactionDataModel *godownEntry;

};

#endif // GMGODOWNALLOCATIONMODEL_H
