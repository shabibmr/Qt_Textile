#ifndef ACCOUNTGROUPS_H
#define ACCOUNTGROUPS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlRecord>

#include "addaccountgroup.h"
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "datamodels/accounts/accountgroupdatamodel.h"
#include <QKeyEvent>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class accountGroups;
}

class accountGroups : public QWidget
{
    Q_OBJECT

public:

    explicit accountGroups(QWidget *parent = 0);
    ~accountGroups();

    // AccountGroupMasterDatabaseHelper *agHelper;
    addAccountGroup *newAccountGroup;
    AccountGroupMasterDatabaseHelper* dbhelper;
    QList <AccountGroupDataModel> groupObjs;
    QSqlQueryModel* accGroupsModel;

    GMItemSortFilterProxyModel *sort;


private slots:
    void on_createNewPushButton_clicked();

//    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_searchLine_textChanged(const QString &arg1);

public slots:
    void AddAccountGroupClickedSlot();
//    void setTable();
    void setTableView();

private:
    Ui::accountGroups *ui;

signals:
    void AddAccountGroupClicked();
};

#endif // ACCOUNTGROUPS_H
