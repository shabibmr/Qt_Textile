#ifndef ADDACCOUNTGROUP_H
#define ADDACCOUNTGROUP_H

#include <QWidget>
#include <QToolButton>
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "datamodels/accounts/accountgroupdatamodel.h"
#include "customwidgets/gmsearchlineedit.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchlite.h"

#include "counter/PrinterHelper/counterprinterhelper.h"
#include "sharetools/excel/excelreporthelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "Notifications/notificationledgerslist.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"


namespace Ui {
class addAccountGroup;
}

class addAccountGroup : public QWidget
{
    Q_OBJECT

public:
    explicit addAccountGroup(QWidget *parent = 0);
    ~addAccountGroup();

    QString GroupId="";
    AccountGroupDataModel* AccountGroupDataModelObject;
    AccountGroupMasterDatabaseHelper* dbHelper;
    QMap<QString,QString> groupData;
    QSqlQueryModel* accGroupModel;
    GMLedgerSearchProxyModel *sort;

    QPointer<QtMaterialSnackbar> snackBar;
    void disableSave();


public slots :
    void editAccountGroup(AccountGroupDataModel* obj);
    void setTitle(QString text);

    void setGroup(QModelIndex);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


//    void on_parentGroupComboBox_currentIndexChanged(const QString &arg1);

    void on_grpTypecomboBox_currentTextChanged(const QString &arg1);

    void setAccountGroup(AccountGroupDataModel);

    void on_tcombo_currentTextChanged(const QString &arg1);
    void sendNotifications(QStringList);

private:
    Ui::addAccountGroup *ui;

signals:
    void closing();
};

#endif // ADDACCOUNTGROUP_H
