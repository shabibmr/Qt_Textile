#ifndef ITEMDETAILVIEW_H
#define ITEMDETAILVIEW_H

#include <QWidget>
#include "datamodels/projects/Documents/documentsdatamodel.h"
#include <QPointer>
#include <QToolButton>
#include "datamodels/inventory/sales_inventory_item/compounditemdataobject.h"
#include <QFileDialog>
#include <QMessageBox>
#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "datamodels/Documents/documentdatamodel.h"

#include "login/loginvalues.h"
namespace Ui {
class ItemDetailView;
}

class ItemDetailView : public QWidget
{
    Q_OBJECT

public:
    explicit ItemDetailView(QWidget *parent = 0);
    ~ItemDetailView();

    QuotationDatabaseHelper *dbHelper;
    QList<DocumentsDataModel> docObj;

    CompoundItemDataObject invItem;

    QString quoteNo="";
    QString projectID="";

    void setTable();

private slots:
    void on_cancelButton_clicked();

    void on_saveButton_clicked();



    void on_browseButton_clicked();

    void on_uploadButton_clicked();

    void on_filePath_textChanged(const QString &arg1);

    void downloadClicked(bool val);

    void delButtonClicked(bool val);

    void setImagesTable();

    void checkToggle(bool val);
public slots:
    void editItem(CompoundItemDataObject item);
    void setQuoteParams(QString quoteNo,QString projID);

private:
    Ui::ItemDetailView *ui;
};

#endif // ITEMDETAILVIEW_H
