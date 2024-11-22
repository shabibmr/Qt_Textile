#ifndef GMREPORTSORTFILTERPROXYMODEL_H
#define GMREPORTSORTFILTERPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDateTime>
class GMReportSortFilterProxyModel : public QSortFilterProxyModel
{
     Q_OBJECT
public:
    GMReportSortFilterProxyModel( QObject *parent = nullptr);

    QList<int> avlCols;
    QString str;

    // QSortFilterProxyModel interface
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setAvlCols(const QList<int> &value);
    void setStr(const QString &value);
};

#endif // GMREPORTSORTFILTERPROXYMODEL_H
