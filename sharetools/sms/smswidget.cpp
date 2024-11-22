#include "smswidget.h"
#include "ui_smswidget.h"
#include "webservices/test/webservicetestsms.h"
#include "database/databasehelper/databasehelper.h"

SMSWidget::SMSWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SMSWidget)
{
    ui->setupUi(this);
//    ui->msgText->setMaximumBlockCount(160);
    QString qStr = "SELECT PHONE FROM CUSTOMER_DETAILS";
    model = (GMSQLQUeryModel*) DatabaseHelper::getQueryModel(qStr);
//    model = new GMSQLQUeryModel(DatabaseHelper::getQueryModel(qStr));
//    ui->tableView->setModel( DatabaseHelper::getQueryModel(qStr));
    qDebug()<< "Row count : "<<model->rowCount();
    qDebug()<< "Col count : "<<model->columnCount();

    setTable();
}

SMSWidget::~SMSWidget()
{
    delete ui;
}

void SMSWidget::setTable()
{
    ui->tableView->setModel(model);
}

void SMSWidget::on_sendButton_clicked()
{
    QString msg = ui->msgText->toPlainText();
    QStringList phone = ui->phoneLine->text().split(",");
    webservicetestSMS sms(msg,phone);
    sms.sendSMS();
}

void SMSWidget::on_cancelBuuton_clicked()
{
    this->close();
}

void SMSWidget::on_phoneLine_textChanged(const QString &arg1)
{

}
