#ifndef WORKORDERITEMDETAIL_H
#define WORKORDERITEMDETAIL_H

#include <QWidget>
#include "datamodels/projects/Documents/documentsdatamodel.h"
#include <QPointer>
#include <QToolButton>
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include <QFileDialog>
#include <QMessageBox>
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "sharetools/print/printerhelper.h"

#include "datamodels/Documents/documentdatamodel.h"

#include "login/loginvalues.h"
namespace Ui {
class WorkOrderItemDetail;
}

class WorkOrderItemDetail : public QWidget
{
    Q_OBJECT

public:
    explicit WorkOrderItemDetail(QWidget *parent = 0);
    ~WorkOrderItemDetail();

    SalesOrderDatabaseHelper *dbHelper;
    QuotationDatabaseHelper *qHelper;
    QList<DocumentsDataModel> docObj;

    CompoundItemDataObject invItem;

    QString quoteNo="";
    QString projectID="";

    void setTable();

private slots:
    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void downloadClicked(bool val);

    void delButtonClicked(bool val);

    void setImagesTable();
public slots:
    void editItem(CompoundItemDataObject item);
    void setQuoteParams(QString quoteNo,QString projID);
signals:
    void closing();
private:
    Ui::WorkOrderItemDetail *ui;
};

#endif // WORKORDERITEMDETAIL_H
