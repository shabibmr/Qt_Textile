#include "addemployeesalarydetails.h"
#include "ui_addemployeesalarydetails.h"

AddEmployeeSalaryDetails::AddEmployeeSalaryDetails(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddEmployeeSalaryDetails)
{
    ui->setupUi(this);

    dbHelper = new PayslipDatabaseHelper;
    emp = new EmployeeSalaryDataModel;

    ledModel = ledgerHelper->getLedgerQueryModel();
    salaryLedger = new GMLedgerSearchLite(ledgerHelper->getLedgerQueryString());
    //    ledLineLite->setAccflag(false);
    salaryLedger->setText("");

    QWidget *SearchWid1 = new QWidget(this);
    QVBoxLayout *lay1 = new QVBoxLayout(SearchWid1);
    lay1->setContentsMargins(0,0,0,0);
    lay1->setSpacing(0);
    lay1->addWidget(salaryLedger);
    SearchWid1->setLayout(lay1);
    ui->salaryLay->addWidget(SearchWid1);
    connect(salaryLedger,&GMLedgerSearchLite::selectedLedger,
            this,[=](LedgerMasterDataModel led){
        emp->salaryLedgerId = led.LedgerID;
        showData();
    });

}

AddEmployeeSalaryDetails::~AddEmployeeSalaryDetails()
{
    delete ui;
}

void AddEmployeeSalaryDetails::on_buttonBox_accepted()
{
    emp->WorkingHours = ui->noOfWorkingHoursSpinBox->text().toInt();
    emp->BasicSalary = ui->basicSalaryLineEdit->text().toFloat();
    emp->HRA = ui->hRALineEdit->text().toFloat();
    emp->Conveyance = ui->conveyanceLineEdit->text().toFloat();
    emp->DA = ui->dALineEdit->text().toFloat();
    emp->WeeklyLeave = ui->weeklyLeaveSpinBox->text().toFloat();
    emp->MonthlyLeave = ui->MonthlySpinBox->text().toFloat();
    emp->AnnualLeave = ui->AnnualLeaveSpinBox->text().toFloat();
    emp->overTimePay = ui->hourlyPayForOverTimeLineEdit->text().toInt();

    dbHelper->deleteEmployeeSalary(emp->EmployeeID);
    dbHelper->insertEmployeeSalary(emp);
}

void AddEmployeeSalaryDetails::on_buttonBox_rejected()
{
    this->close();
    emit closing();
}

void AddEmployeeSalaryDetails::showData()
{
    ui->basicSalaryLineEdit->setText(QString::number(emp->BasicSalary));
    ui->hRALineEdit->setText(QString::number(emp->HRA));
    ui->dALineEdit->setText(QString::number(emp->DA));
    ui->conveyanceLineEdit->setText(QString::number(emp->Conveyance));
    ui->noOfWorkingHoursSpinBox->setValue(emp->WorkingHours);
    ui->AnnualLeaveSpinBox->setValue(emp->AnnualLeave);
    ui->weeklyLeaveSpinBox->setValue(emp->WeeklyLeave);
    ui->MonthlySpinBox->setValue(emp->MonthlyLeave);
    ui->hourlyPayForOverTimeLineEdit->setText(QString::number(emp->overTimePay));

}
