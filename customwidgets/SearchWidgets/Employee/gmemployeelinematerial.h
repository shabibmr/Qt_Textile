#ifndef GMEmployeeLINEMATERIAL_H
#define GMEmployeeLINEMATERIAL_H

#include "customwidgets/Material/components/qtmaterialtextfield.h"
#include <QCompleter>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QString>
#include "customwidgets/SearchWidgets/Inventoryitem/itemsearchdelegate.h"
#include <QListView>
#include <QSqlRecord>
#include <QKeyEvent>
#include <QDebug>
#include <database/hr/userprofiledatabasehelper.h>


class GMEmployeeLineMaterial : public QtMaterialTextField
{
    Q_OBJECT
public:

    explicit GMEmployeeLineMaterial(UserProfileDatabaseHelper *iHelper,QSqlQueryModel *m,QtMaterialTextField *parent=Q_NULLPTR);
    virtual ~GMEmployeeLineMaterial() override;
    QSqlQueryModel *srcModel;
    QString completionPrefix;
    QSortFilterProxyModel* filterProxyModel;
    QCompleter* gmcompleter;
    bool activated=false;
    UserProfileDatabaseHelper *userHelper;
    QString oldText;
    QString oldID;
    UserProfileDataModel item;
    void setNewText(QString text, QString id);
    void setItem(const UserProfileDataModel &value);

protected:


    void sendPositiveEMIT(QModelIndex index, int itemID);

    virtual bool eventFilter(QObject *obj, QEvent *event) override;


public slots:
    void completerhighlighted(QModelIndex);
    void completerActivated(QModelIndex);
    void finishEdit();
    void finishEditByRet();
signals:

    void currentSelection(QModelIndex index);
    void itemSelected(QModelIndex index);
    void itemSelectedwithItem(UserProfileDataModel item);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // GMEmployeeLINEMATERIAL_H
