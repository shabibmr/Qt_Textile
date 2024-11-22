#ifndef QUOTAITONSLIST_H
#define QUOTAITONSLIST_H

#include <QWidget>
#include "quotationeditor.h"
#include "database/Reports/ledgerhelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "voucherOperations/VoucherEditor/vouchereditor.h"

namespace Ui {
class QuotaitonsList;
}

class QuotaitonsList : public QWidget
{
    Q_OBJECT

public:
    explicit QuotaitonsList(QWidget *parent = nullptr);
    ~QuotaitonsList();

    QuotationEditor* editor;
//    VoucherEditor *editor;
    QList<GeneralVoucherDataObject> voucherList;
    ProformaDatabaseHelper *dbhelper;

    LedgerHelper *ledgerHelper;
    GMItemSortFilterProxyModel* sort;

    QSqlQueryModel *model;

private:
    Ui::QuotaitonsList *ui;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();

    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

};

#endif // QUOTAITONSLIST_H
