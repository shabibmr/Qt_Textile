#ifndef PRICELIST_H
#define PRICELIST_H

#include <QWidget>
#include <QKeyEvent>
#include "inventory/pricelist/addpricelist.h"
#include "database/inventory/pricelist/pricelistdatabasehelper.h"
#include "datamodels/inventory/pricelist/pricelistdataobject.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class priceList;
}

class priceList : public QWidget
{
    Q_OBJECT

public:
    explicit priceList(QWidget *parent = Q_NULLPTR);
    ~priceList();
    addPriceList* addPriceListWidget;
    PricelistDatabaseHelper* dbHelper;
    PriceListDataObject* dataObj;
    QList<PriceListDataObject> priceNames;

    QSqlQueryModel *sqlModel;
    GMItemSortFilterProxyModel* sort;

private slots:
    void on_createNewPushButton_clicked();

    void keyPressEvent(QKeyEvent *e);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::priceList *ui;

public slots:
    void settable();


};

#endif // PRICELIST_H
