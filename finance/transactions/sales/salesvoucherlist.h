#ifndef SALESVOUCHERLIST_H
#define SALESVOUCHERLIST_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "salesvouchereditor.h"
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "voucherOperations/VoucherEditor/vouchereditor.h"

namespace Ui {
class SalesVoucherList;
}

template<typename TableModel>
struct EagerlyLoaded2 : public TableModel {
    static_assert(std::is_base_of<QAbstractTableModel, TableModel>::value,
    "EagerlyLoaded works only with QAbstractTableModel subclasses");
    explicit EagerlyLoaded2(QObject* parent = nullptr):TableModel(parent) {
        QObject::connect(this, &QAbstractTableModel::modelReset,
                         [this]{ while(this->canFetchMore()) this->fetchMore(); });
    }
};

class SalesVoucherList : public QWidget
{
    Q_OBJECT

public:
    explicit SalesVoucherList(QWidget *parent = 0);
    ~SalesVoucherList();

    SalesVoucherEditor* editor;
//    VoucherEditor *editor;
    QList<GeneralVoucherDataObject> voucherList;
    salesVoucherDatabaseHelper *dbhelper;

    LedgerHelper *ledgerHelper;
    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel *model;

//    QPointer<EagerlyLoaded2<QSqlQueryModel>> eagerModel;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();

    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SalesVoucherList *ui;
};

#endif // SALESVOUCHERLIST_H
