#ifndef GMACCESSCONTROLMODEL_H
#define GMACCESSCONTROLMODEL_H

#include "datamodels/hr/usergroupdatamodel.h"
#include <QDebug>
#include <QAbstractItemModel>
#include <QSqlQueryModel>
#include <QSqlRecord>

class GMAccessControlModel: public QAbstractItemModel
{
    Q_OBJECT

public:
//    GMAccessControlModel();



    explicit GMAccessControlModel(QSqlQueryModel *qModel, UserGroupDataModel *model, QObject *parent = nullptr);

    QString module;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    void setHeaderStrings(const QStringList &value);

    void setUserGroup(UserGroupDataModel *value);
    int findPermissionsForUi(int ui_id, QString module) const;
    QSqlQueryModel *model;

    void resetModel();

private:
    UserGroupDataModel *userGroup;
    int colCount;
    QStringList headerStrings;

};

#endif // GMACCESSCONTROLMODEL_H
