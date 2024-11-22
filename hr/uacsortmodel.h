#ifndef UACSORTMODEL_H
#define UACSORTMODEL_H

//#include <QObject>
#include <QSortFilterProxyModel>

class UACSortModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    UACSortModel(QObject *parent=0);
    QString module;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // QAbstractItemModel interface

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    void setModule(const QString &value);
};


#endif // UACSORTMODEL_H
