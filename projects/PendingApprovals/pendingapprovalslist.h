#ifndef PENDINGAPPROVALSLIST_H
#define PENDINGAPPROVALSLIST_H

#include <QWidget>
#include "database/inventory/quotation/quotationdatabasehelper.h"
#include "datamodels/projects/Quotation/quotationstatus.h"


namespace Ui {
class PendingApprovalsList;
}

class PendingApprovalsList : public QWidget
{
    Q_OBJECT

public:
    explicit PendingApprovalsList(QWidget *parent = 0);
    ~PendingApprovalsList();

    QuotationDatabaseHelper *dbHelper;
    QList<GeneralVoucherDataObject> voucherList;


    void setTable();
private:
    Ui::PendingApprovalsList *ui;
};

#endif // PENDINGAPPROVALSLIST_H
