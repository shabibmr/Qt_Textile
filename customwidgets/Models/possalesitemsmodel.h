#ifndef POSSALESITEMSMODEL_H
#define POSSALESITEMSMODEL_H

#include <QAbstractItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>

class POSSalesItemsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    POSSalesItemsModel(QSqlQueryModel *queryModel, int ColumnCnt, QObject *parent=nullptr);
    ~POSSalesItemsModel();
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    int RowCount;
    int ColCount;
    int listSize;
    QSqlQueryModel *model;


    // QAbstractItemModel interface

//public slots:

//    void test(QByteArray a,QModelIndex index);
//    void emitDataChanged(QModelIndex &index);
public:


    // QAbstractItemModel interface
public:
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // POSSALESITEMSMODEL_H
