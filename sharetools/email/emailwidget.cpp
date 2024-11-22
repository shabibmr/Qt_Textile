#include "emailwidget.h"
#include "ui_emailwidget.h"
#include "sharetools/email/emailthread.h"
EmailWidget::EmailWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmailWidget)
{
    ui->setupUi(this);

    initData();
    setWidgets();
}

EmailWidget::~EmailWidget()
{
    delete ui;
}

void EmailWidget::setWidgets()
{

}

void EmailWidget::initData()
{
    // get Default to address;
    // get email Config or Profile;
}

void EmailWidget::setData()
{

}

void EmailWidget::sendMail()
{
    for (int i=0;i< emailData->toMailList.size() ; i++) {
        EmailThread* mail = new EmailThread();
        mail->setToAddress(emailData->toMailList[i]);
        mail->subject = emailData->subject;
        mail->Message = emailData->body;
        mail->files = emailData->filesList;
        mail->run();
    }
}

