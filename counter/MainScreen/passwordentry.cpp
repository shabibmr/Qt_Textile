#include "passwordentry.h"
#include "ui_passwordentry.h"

#include "database/hr/userprofiledatabasehelper.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

PasswordEntry::PasswordEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordEntry)
{
    ui->setupUi(this);
    ui->message->hide();
    ui->remarksLineEdit->setPlaceholderText("Reason : ");

}

PasswordEntry::~PasswordEntry()
{
    delete ui;
}

void PasswordEntry::on_OKButton_clicked()
{
    UserGroupDatabaseHelper *uHelper = new UserGroupDatabaseHelper();
    QString qStr;
    QString comment = ui->remarksLineEdit->text();
    qStr = "SELECT `Employee_ID`,`Name`,`privilege`, `UserGroupID` FROM  " +UserProfileDatabaseHelper::Employee_Details_TableName +"  WHERE (`Employee_ID` = '"+ui->passwordLineEdit->text()+"')";
    qStr += " OR `password`='"+ui->passwordLineEdit->text()+"'";
    //    qStr += " AND privilege = 2";

    if(kotDeleted && !ConfigurationSettingsDatabaseHelper::getValue(deleteKOTItemWithoutRemarks,true).toBool() && comment.length()<3)
    {
        ui->message->setText("Enter Remarks");
        ui->message->show();
        return;
    }
    qDebug()<<qStr;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    //    query.prepare(qStr);
    //    query.bindValue(":pass",ui->passwordLineEdit->text());
    if(ui->passwordLineEdit->text() == "perm"){
        LoginValues::setPermLogin(true);
        emit loginSuccess();
        this->close();
    }
    else if(ui->passwordLineEdit->text().length()>1){
        if(query.exec(qStr)){
            if(query.size()>0 ){
                if(query.first()){
                    LoginValues::setPermLogin(false);
                    qDebug()<<query.value(1).toString();
                    QString val = query.value(1).toString();
                    int priv = query.value(2).toInt();
                    LoginValues::setModel(uHelper->getUserGroup(query.value(3).toInt()));
                    qDebug()<<"Priv = "<<priv;
                    emit loginSuccessWithComment(val,comment, priv);
                    emit loginSuccess();
                    emit loginSuccessWithPrivilege(priv);
                    this->close();
                    qDebug()<<"loginsuccess";
                    LoginValues::setPrivelage(priv);
                }
            }
            else{
                ui->message->setText("Wrong Password");
                ui->message->show();
                qDebug()<<"loginfail";
            }
        }
        else{
            qDebug()<<query.lastError();
            qDebug()<<query.lastQuery();
        }
    }
}

void PasswordEntry::on_cancelButton_clicked()
{
    this->close();
}

void PasswordEntry::on_passwordLineEdit_returnPressed()
{
    on_OKButton_clicked();
}


void PasswordEntry::on_itemwiseButton_clicked()
{
    this->close();
    emit showItemwiseSales();
}

void PasswordEntry::on_todaysbillsButton_clicked()
{
    this->close();
    emit showtodaysBills();
}

void PasswordEntry::on_pushButton_clicked()
{
    this->close();
    emit sendEmailReport();
}

void PasswordEntry::hideWidgets()
{
    ui->buttonsWidget->hide();
}

void PasswordEntry::setKotDeleted(bool value)
{
    kotDeleted = value;
}
