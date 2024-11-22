#ifndef DRAFTSMANVIEW_H
#define DRAFTSMANVIEW_H

#include <QWidget>

#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "quoteview.h"

namespace Ui {
class draftsManview;
}

class draftsManview : public QWidget
{
    Q_OBJECT

public:
    explicit draftsManview(QWidget *parent = 0);
    ~draftsManview();

    QuotationDatabaseHelper *dbHelper;

     QList<GeneralVoucherDataObject> voucherList;

     QuoteView* QuoteViewWidget;
private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void setTable();
    void on_okButton_clicked();

private:
    Ui::draftsManview *ui;
};

#endif // DRAFTSMANVIEW_H
