#ifndef GMSQLQUERYMODEL_H
#define GMSQLQUERYMODEL_H

#include <QSqlQueryModel>

class GMSQLQUeryModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit GMSQLQUeryModel(QMap<int, Qt::CheckState> check_state_map, int checkCol =0 , QSqlQueryModel *parent = nullptr);

    // Header:

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private:
    QMap<int, Qt::CheckState> check_state_map;
    int checkCol =0;
public:
    void markChecked(int row, bool checkState)    ;

signals:
    void checked(int row, bool checkState)    ;
};

#endif // GMSQLQUERYMODEL_H
