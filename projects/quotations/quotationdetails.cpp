#include "quotationdetails.h"
#include "ui_quotationdetails.h"

QuotationDetails::QuotationDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuotationDetails)
{
    ui->setupUi(this);
    ui->nextButton->setIcon(QIcon(":/images/next.png"));
    ui->prevButton->setIcon(QIcon(":/images/back.png"));
    dbHelper = new TermsDatabaseHelper;

    ui->converNote->setEnabled(false);
    QMap<QString,QString> map;
    map=dbHelper->getTermByid(++id);
    QList<QMap<QString,QString>> all = dbHelper->getAllTerms();
    numTerms=all.size();
    ui->subjectLineEdit->setText(map.value(dbHelper->Terms_title));
    ui->delivery->setPlainText(map.value(dbHelper->Terms_delivery));
    ui->payments->setPlainText(map.value(dbHelper->Terms_payment));
    ui->warranty->setPlainText(map.value(dbHelper->Terms_warranty));
    ui->validityLineEdit->setText(map.value(dbHelper->Terms_validity));
    ui->exclusions->setPlainText(map.value(dbHelper->Terms_exclusion));
    ui->extra->setPlainText(map.value(dbHelper->Terms_extra));

    ui->printNow->setChecked(true);

}

QuotationDetails::~QuotationDetails()
{
    delete ui;
}

void QuotationDetails::on_buttonBox_accepted()
{
    QString validity = ui->validityLineEdit->text() + "";
    QString subject  = ui->subjectLineEdit->text();
    QString payments = ui->payments->toPlainText();
    QString warranty = ui->warranty->toPlainText();
    QString extra = ui->extra->toPlainText();
    QString exclusion = ui->exclusions->toPlainText();
    QString delivery = ui->delivery->toPlainText();
    bool printExtra = ui->printDescription->isChecked();
    bool printNow = ui->printNow->isChecked();
    bool sendEmail = ui->sendMail->isChecked();
    bool saveAsPdf = ui->PDFradioButton->isChecked();
    emit quoteDetailsSaved(validity,subject,payments,
                            warranty,extra,delivery,
                           exclusion,printExtra,printNow,sendEmail,saveAsPdf);
    this->close();
}

void QuotationDetails::on_buttonBox_rejected()
{
    this->close();
}

void QuotationDetails::on_nextButton_clicked()
{
    QMap<QString,QString> map;
    if(id>numTerms-1)
        return;
    map=dbHelper->getTermByid(++id);
    ui->subjectLineEdit->setText(map.value(dbHelper->Terms_title));
    ui->delivery->setPlainText(map.value(dbHelper->Terms_delivery));
    ui->payments->setPlainText(map.value(dbHelper->Terms_payment));
    ui->warranty->setPlainText(map.value(dbHelper->Terms_warranty));
    ui->validityLineEdit->setText(map.value(dbHelper->Terms_validity));
    ui->exclusions->setPlainText(map.value(dbHelper->Terms_exclusion));
    ui->extra->setPlainText(map.value(dbHelper->Terms_extra));
}

void QuotationDetails::on_prevButton_clicked()
{
    QMap<QString,QString> map;
    if(id==1)
        return;
    map=dbHelper->getTermByid(--id);
    ui->subjectLineEdit->setText(map.value(dbHelper->Terms_title));
    ui->delivery->setPlainText(map.value(dbHelper->Terms_delivery));
    ui->payments->setPlainText(map.value(dbHelper->Terms_payment));
    ui->warranty->setPlainText(map.value(dbHelper->Terms_warranty));
    ui->validityLineEdit->setText(map.value(dbHelper->Terms_validity));
    ui->exclusions->setPlainText(map.value(dbHelper->Terms_exclusion));
    ui->extra->setPlainText(map.value(dbHelper->Terms_extra));
}
