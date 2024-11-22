#ifndef QUOTATIONDETAILS_H
#define QUOTATIONDETAILS_H

#include <QWidget>
#include "database/finance/terms/termsdatabasehelper.h"

namespace Ui {
class QuotationDetails;
}

class QuotationDetails : public QWidget
{
    Q_OBJECT

public:
    explicit QuotationDetails(QWidget *parent = 0);
    ~QuotationDetails();
    TermsDatabaseHelper* dbHelper;
    int id =0;
    int numTerms;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
    void on_nextButton_clicked();

    void on_prevButton_clicked();


signals:
    void quoteDetailsSaved(QString validity,QString subject,QString payment,
                           QString warranty,QString extra,QString delivery,QString exclusion,
                           bool printExtras,bool printNow,bool sendMail,bool pdf);
private:
    Ui::QuotationDetails *ui;
};

#endif // QUOTATIONDETAILS_H
