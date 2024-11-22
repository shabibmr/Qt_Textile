#include "termsconditions.h"
#include "ui_termsconditions.h"

TermsConditions::TermsConditions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TermsConditions)
{
    ui->setupUi(this);
    dbHelper = new TermsDatabaseHelper;
}

TermsConditions::~TermsConditions()
{
    delete ui;
}

void TermsConditions::disableSave()
{
    ui->buttonBox->hide();
}

void TermsConditions::on_buttonBox_accepted()
{
    QMap<QString,QVariant> map;
    map.insert(dbHelper->Terms_title, ui->titleLineEdit->text());
    map.insert(dbHelper->Terms_delivery,ui->DeliveryplainTextEdit->toPlainText());
    map.insert(dbHelper->Terms_payment,ui->paymentplainTextEdit->toPlainText());
    map.insert(dbHelper->Terms_warranty, ui->WarrantyplainTextEdit->toPlainText());
    map.insert(dbHelper->Terms_validity,ui->ValidityplainTextEdit->toPlainText());
    map.insert(dbHelper->Terms_exclusion,ui->ExclusionsplainTextEdit->toPlainText());
    map.insert(dbHelper->Terms_extra,ui->ExtraTextEdit->toPlainText());

    if(id ==0)
        dbHelper->insertTerms(map);
    else
        dbHelper->updateTerms(map,id);
    this->close();

}

void TermsConditions::on_buttonBox_rejected()
{
    this->close();
}

void TermsConditions::editTerms(int id)
{
    this->id=id;
    QMap<QString,QString> map;
    map=dbHelper->getTermByid(id);
    ui->titleLineEdit->setText(map.value(dbHelper->Terms_title));
    ui->DeliveryplainTextEdit->setPlainText(map.value(dbHelper->Terms_delivery));
    ui->paymentplainTextEdit->setPlainText(map.value(dbHelper->Terms_payment));
    ui->WarrantyplainTextEdit->setPlainText(map.value(dbHelper->Terms_warranty));
    ui->ValidityplainTextEdit->setPlainText(map.value(dbHelper->Terms_validity));
    ui->ExclusionsplainTextEdit->setPlainText(map.value(dbHelper->Terms_exclusion));
    ui->ExtraTextEdit->setPlainText(map.value(dbHelper->Terms_extra));

}
