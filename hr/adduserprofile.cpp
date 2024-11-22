#include "adduserprofile.h"
#include "ui_adduserprofile.h"

#include <QMessageBox>

AddUserProfile::AddUserProfile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddUserProfile)
{
    ui->setupUi(this);
    dbHelper = new UserProfileDatabaseHelper;
    gpHelper = new UserGroupDatabaseHelper;
    payslipHelper = new PayslipDatabaseHelper;
    ledHelper = new LedgerMasterDatabaseHelper;

    this->setWindowTitle("Profile Editor");

    QList<LedgerMasterDataModel> ledList = ledHelper->getAllLedgers();

    for(LedgerMasterDataModel led:ledList){
        ui->salaryLedgercomboBox->addItem(led.LedgerName, led.LedgerID);
        ui->advanceSalaryLedgerComboBox->addItem(led.LedgerName, led.LedgerID);
    }
    //    AttendanceDatabaseHelper *at = new AttendanceDatabaseHelper();

    //    at->getMonthlyPayslipReport(QDate::fromString("01-12-2019","dd/MM/yyyy"), QDate::fromString("31-12-2019","dd/MM/yyyy"));

    userGroupMap = gpHelper->getUserGroups();

    for(int k:userGroupMap.keys()){
        ui->userGroupcomboBox->addItem(userGroupMap.value(k),k);
    }

    user = new UserProfileDataModel();
    emp = new EmployeeSalaryDataModel();


    //    ui->usernameLabel->hide();
    //    ui->passwordLabel->hide();
    //    ui->usernameLabel->hide();
    //    ui->usernameLineEdit->hide();
}

AddUserProfile::~AddUserProfile()
{
    delete ui;
}

void AddUserProfile::disableSave()
{
    ui->buttonBox->hide();
}

void AddUserProfile::saveEmployee(UserProfileDataModel *obj)
{
//    if(obj->Employee_ID.length() == 0)
//    {
//        QMessageBox box; box.setText("Please enter Employee ID in Employee Details tab"); box.exec();
//        return;
//    }
    if(obj->Name.length()<1){
        QMessageBox box; box.setText("Please Enter Employee Name"); box.exec();
        return;
    }

    if(vNo == 0|| (obj->fromExternal == true && obj->action == 1001)){
        vNo = dbHelper->insertUserProfile(obj);
        emp->EmployeeID =QString::number(vNo);
        emp->isActive = obj->showEmployee;
        payslipHelper->insertEmployeeSalary(emp);
        obj->action=1001;
    }
    else{
        qDebug()<<"Updating"<<Q_FUNC_INFO<<emp->EmployeeID<<emp->EmployeeID.length()<<obj->_id;
        dbHelper->updateUserProfile(obj,vNo);
        if(emp->EmployeeID.length() ==0)
        {
            emp->EmployeeID = QString::number(vNo);
            emp->isActive = obj->showEmployee;
            payslipHelper->insertEmployeeSalary(emp);
        }
        else {
            emp->isActive = obj->showEmployee;
            payslipHelper->updateEmployeeSalary(emp, QString::number(vNo));
        }

        obj->action=1002;
    }

    //    if(obj->fromExternal == false){
    //        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper();
    //        QStringList ledgerNames = ledHelper->getLedgersListHavingDbName();
    //        qDebug()<<"Ledgers to send"<<ledgerNames.size()<<ledgerNames;
    //        if(ledgerNames.size()> 0){
    //            NotificationLedgersList *ledWidget = new NotificationLedgersList();
    //            ledWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    //            ledWidget->show();
    //            QObject::connect(ledWidget, &NotificationLedgersList::sendNotification, this, &AddUserProfile::sendNotifications);
    //            QObject::connect(ledWidget, &NotificationLedgersList::closing, this, &AddUserProfile::on_buttonBox_rejected);
    //        }

    //        else{

    //            this->close();
    //            emit closing();
    //        }

    //    }

    //    else{

    this->close();
    emit closing();
    //    }


}

void AddUserProfile::showVoucher(UserProfileDataModel obj)
{
    qDebug()<<Q_FUNC_INFO<<obj._id;
    ui->titleLabel->setText(obj.Name);
    vNo = obj._id;
    emp = payslipHelper->getEmpSalaryDetailsAsPtr(QString::number(obj._id));
    ui->nameLineEdit->setText(obj.Name );
    ui->fatherSNameLineEdit->setText(obj.Fathers_Name );
    ui->dateOfBirthDateEdit->setDate(obj.Date_of_Birth );
    ui->genderComboBox->setCurrentText(obj.Gender );
    ui->nationalityLineEdit->setText(obj.Nationality );
    ui->maritalStatusComboBox->setCurrentText(obj.Marital_Status );
    ui->emailLineEdit->setText(obj.Email );
    ui->phoneLineEdit->setText(obj.Phone );
    ui->addressTextEdit->setPlainText(obj.Address );
    ui->usernameLineEdit->setText(obj.Username );
    ui->passwordLineEdit->setText(obj.Password );
    ui->employeeIDLineEdit->setText(obj.Employee_ID );
    ui->designationLineEdit->setText(obj.Designation );
    ui->departmentLineEdit->setText(obj.Department);
    ui->dateOfJoiningDateEdit->setDate(obj.Date_of_Joining);
    ui->passportNoLineEdit->setText(obj.Passport_No);
    ui->passportExpiryDateEdit->setDate(obj.Passport_Expiry );
    ui->visaNoLineEdit->setText(obj.Visa_No );
    ui->visaExpiryDateEdit->setDate(obj.Visa_Expiry );
    ui->bankAccountNoLineEdit->setText(obj.Bank_Account_No );
    ui->bankNameLineEdit->setText(obj.Bank_Name );
    ui->bankBranchLineEdit->setText(obj.Bank_Branch );
    ui->bankCodeLineEdit->setText(obj.Bank_Code );
    ui->highestQualificationLineEdit->setText(obj.Highest_Qualification );
    ui->dateOfQualificationDateEdit->setDate(obj.Date_of_Qualification );
    ui->nameLineEdit_2->setText(obj.Emergency_Contact_Name );
    ui->contactNoLineEdit->setText(obj.Emergency_Contact_No );
    ui->relationLineEdit->setText(obj.Emergency_Contact_Relation);
    ui->userGroupcomboBox->setCurrentText(userGroupMap.value(obj.userGroup));
    ui->showEmployee->setChecked(obj.showEmployee);

    ui->basicSalaryLineEdit->setText(QString::number(emp->BasicSalary));
    ui->hRALineEdit->setText(QString::number(emp->HRA));
    ui->dALineEdit->setText(QString::number(emp->DA));
    ui->conveyanceLineEdit->setText(QString::number(emp->Conveyance));
    ui->noOfWorkingHoursSpinBox->setValue(emp->WorkingHours);
    ui->AnnualLeaveSpinBox->setValue(emp->AnnualLeave);
    ui->weeklyLeaveSpinBox->setValue(emp->WeeklyLeave);
    ui->MonthlySpinBox->setValue(emp->MonthlyLeave);
    ui->hourlyPayForOverTimeLineEdit->setText(QString::number(emp->overTimePay));
    ui->salaryLedgercomboBox->setCurrentText(ledHelper->getLedgerNameByID(emp->salaryLedgerId));
    ui->advanceSalaryLedgerComboBox->setCurrentText(ledHelper->getLedgerNameByID(emp->advLedgerId));


}

void AddUserProfile::on_buttonBox_accepted()
{
    user->Name = ui->nameLineEdit->text();
    user->Fathers_Name = ui->fatherSNameLineEdit->text();
    user->Date_of_Birth = ui->dateOfBirthDateEdit->date();
    user->Gender = ui->genderComboBox->currentText();
    user->Nationality = ui->nationalityLineEdit->text();
    user->Marital_Status = ui->maritalStatusComboBox->currentText();
    user->Email = ui->emailLineEdit->text();
    user->Phone = ui->phoneLineEdit->text();
    user->Address = ui->addressTextEdit->toPlainText();
    user->Username = ui->usernameLineEdit->text();
    user->Password = ui->passwordLineEdit->text();
    user->Employee_ID = ui->employeeIDLineEdit->text();
    user->Designation = ui->designationLineEdit->text();
    user->Department = ui->departmentLineEdit->text();
    user->Date_of_Joining = ui->dateOfJoiningDateEdit->date();
    user->Passport_No = ui->passportNoLineEdit->text();
    user->Passport_Expiry = ui->passportExpiryDateEdit->date();
    user->Visa_No = ui->visaNoLineEdit->text();
    user->Visa_Expiry = ui->visaExpiryDateEdit->date();
    user->Bank_Account_No = ui->bankAccountNoLineEdit->text();
    user->Bank_Name = ui->bankNameLineEdit->text();
    user->Bank_Branch = ui->bankBranchLineEdit->text();
    user->Bank_Code = ui->bankCodeLineEdit->text();
    user->Highest_Qualification = ui->highestQualificationLineEdit->text();
    user->Date_of_Qualification = ui->dateOfQualificationDateEdit->date();
    user->Emergency_Contact_Name = ui->nameLineEdit_2->text();
    user->Emergency_Contact_No = ui->contactNoLineEdit->text();
    user->Emergency_Contact_Relation = ui->relationLineEdit->text();
    user->userGroup = ui->userGroupcomboBox->currentData().toInt();
    user->showEmployee = ui->showEmployee->isChecked();

    switch(user->userGroup){
    case 1:
    {
        user->privelage = 5;
        break;
    }
    case 2:
    {
        user->privelage = 2;
        break;
    }
    case 3:
    case 5:
    {
        user->privelage = 1;
        break;
    }
     case 4 :
    {user->privelage=6;
        break;
    }
    default:
        user->privelage = 1;
    }


    emp->WorkingHours = ui->noOfWorkingHoursSpinBox->text().toInt();
    emp->BasicSalary = ui->basicSalaryLineEdit->text().toFloat();
    emp->HRA = ui->hRALineEdit->text().toFloat();
    emp->Conveyance = ui->conveyanceLineEdit->text().toFloat();
    emp->DA = ui->dALineEdit->text().toFloat();
    emp->WeeklyLeave = ui->weeklyLeaveSpinBox->text().toFloat();
    emp->MonthlyLeave = ui->MonthlySpinBox->text().toFloat();
    emp->AnnualLeave = ui->AnnualLeaveSpinBox->text().toFloat();
    emp->overTimePay = ui->hourlyPayForOverTimeLineEdit->text().toInt();
    emp->salaryLedgerId = ui->salaryLedgercomboBox->currentData().toString();
    emp->advLedgerId = ui->advanceSalaryLedgerComboBox->currentData().toString();

    saveEmployee(user);

}

void AddUserProfile::on_buttonBox_rejected()
{
    this->close();
}

void AddUserProfile::sendNotifications(QStringList dbNameList)
{
    for(QString dbName:dbNameList){
        //        qDebug()<<"Insert notification data to send"<<dbName;

        NotificationDataModel *notification = new NotificationDataModel();
        QJsonDocument doc(user->ObjPtrToJson(user));
        QString strJson(doc.toJson(QJsonDocument::Compact));

        notification->NotificationData = strJson;
        notification->ToDBName=dbName;
        notification->TypeOfData="Employee";
        notification->Action = user->action;
        notification->GeneratedTransactionId=QString::number(QDateTime::currentDateTime().toTime_t()) + LoginValues::voucherPrefix;

        sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
        sendHelper->insertNotification(notification);
    }

    this->close();
    emit closing();

}

