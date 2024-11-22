#ifndef GMLEDGERSEARCHLISTWIDGETLITE_H
#define GMLEDGERSEARCHLISTWIDGETLITE_H

#include <QWidget>
#include <QSqlQueryModel>
#include "database/finance/accountmaster/accountgroupmasterdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchproxymodel.h"
#include "customwidgets/SearchWidgets/Ledger/gmledgersearchdelegate.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QSqlRecord>

namespace Ui {
class GMLedgerSearchListWidgetLite;
}

class GMLedgerSearchListWidgetLite : public QWidget
{
    Q_OBJECT

public:
    explicit GMLedgerSearchListWidgetLite(QSqlQueryModel *model,QWidget *parent = 0);
    ~GMLedgerSearchListWidgetLite();
    LedgerMasterDatabaseHelper* ledHelper;
    AccountGroupMasterDatabaseHelper* accHelper;
    GMLedgerSearchProxyModel *sort;


    QSqlQueryModel *model;

    QStringList itemNames;
    int lLoc =0;

    void setFocusExpl(QString text);

private:
    Ui::GMLedgerSearchListWidgetLite *ui;

protected:
    bool eventFilter(QObject* obj, QEvent *event);

public slots:
    void filterData(QString);

    void setData(QSqlQueryModel* model);

    void checkData();

private slots:

    void onTablewidgetEnterpressed(QModelIndex i);

    void on_tableWidget_clicked(const QModelIndex &index);

signals:

    void selectedID(QString,QString);


};

#endif // GMLEDGERSEARCHLISTWIDGETLITE_H
