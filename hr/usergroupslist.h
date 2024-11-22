#ifndef USERGROUPSLIST_H
#define USERGROUPSLIST_H

#include <QWidget>
#include <QKeyEvent>
#include <QToolButton>
#include "addusergroup.h"

namespace Ui {
class UserGroupsList;
}

class UserGroupsList : public QWidget
{
    Q_OBJECT

public:
    explicit UserGroupsList(QWidget *parent = 0);
    AddUserGroup* AddUserGroupWidget;
    ~UserGroupsList();
    UserGroupDatabaseHelper *dbhelper;
    QList<UserGroupDataModel> voucherList;
    QMap<int,QString> map;

private slots:
    void keyPressEvent(QKeyEvent *e);
    void on_createNewPushButton_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void setTable();
    void delButtonClicked(bool);

private:
    Ui::UserGroupsList *ui;
};

#endif // USERGROUPSLIST_H
