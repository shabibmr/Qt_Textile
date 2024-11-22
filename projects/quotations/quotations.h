#ifndef QUOTATIONS_H
#define QUOTATIONS_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "datamodels/projects/Quotation/quotationstatus.h"
#include "projects/quotations/addquotation.h"

namespace Ui {
class Quotations;
}

class Quotations : public QWidget
{
    Q_OBJECT

public:
    explicit Quotations(QWidget *parent = 0);
    ~Quotations();
    addQuotation* addQuotationObject;
    QList<GeneralVoucherDataObject> voucherList;
    QuotationDatabaseHelper *dbhelper;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTable();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);
    void enterPressed();

    void on_fromDateDateEdit_editingFinished();

private:
    Ui::Quotations *ui;
};

#endif // QUOTATIONS_H
