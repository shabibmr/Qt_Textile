#ifndef PROFORMAINVOICELIST_H
#define PROFORMAINVOICELIST_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "proformainvoiceeditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"


namespace Ui {
class ProformaInvoiceList;
}

class ProformaInvoiceList : public QWidget
{
    Q_OBJECT

public:
    explicit ProformaInvoiceList(QWidget *parent = 0);
    ~ProformaInvoiceList();


    ProformaInvoiceEditor* editor;
    QList<GeneralVoucherDataObject> voucherList;
    ProformaDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);


private:
    Ui::ProformaInvoiceList *ui;
};

#endif // PROFORMAINVOICELIST_H
