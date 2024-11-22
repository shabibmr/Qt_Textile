#include "addproject.h"
#include "ui_addproject.h"

AddProject::AddProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddProject)
{
    ui->setupUi(this);
    dbHelper = new ProjectsDatabaseHelper();

}

AddProject::~AddProject()
{
    delete ui;
}

void AddProject::editProject(ProjectsDataObject prj)
{
    id = prj.ProjectID;
    ui->projectNameLineEdit->setText(prj.ProjectName );
    ui->projectDescriptionLineEdit->setText(prj.ProjectDescription );
    ui->projectDetail->setPlainText(prj.Detail);
    ui->addressLineTextEdit->setPlainText(prj.address );
    ui->contactPersonLineEdit->setText(prj.ContactPerson);
    ui->telephoneLineEdit->setText(prj.ContactPhone);
    ui->projectOwnerComboBox->setCurrentText(prj.ownerID );
    ui->customerNameComboBox->setCurrentText(prj.ProjectLedgerId );

    ui->startDateDateEdit->setDate(prj.ProjectStartDate);
    ui->endDateDateEdit->setDate(prj.ProjectCompletionDate );

}

void AddProject::on_buttonBox_accepted()
{
    projectsDataObject = new ProjectsDataObject();
    projectsDataObject->ProjectName = ui->projectNameLineEdit->text();
    projectsDataObject->ProjectDescription = ui->projectDescriptionLineEdit->text();
    projectsDataObject->Detail = ui->projectDetail->toPlainText();
    projectsDataObject->address = ui->addressLineTextEdit->toPlainText();
    projectsDataObject->ContactPerson =ui->contactPersonLineEdit->text();
    projectsDataObject->ContactPhone = ui->telephoneLineEdit->text();
    projectsDataObject->ownerID = ui->projectOwnerComboBox->currentText();
    projectsDataObject->ProjectLedgerId = ui->customerNameComboBox->currentText();
    projectsDataObject->ProjectStatusID = ui->projectStatusComboBox->currentText();
    projectsDataObject->emailAddress = ui->emailLineEdit->text();
    projectsDataObject->ProjectStartDate = ui->startDateDateEdit->date();
    projectsDataObject->ProjectCompletionDate = ui->endDateDateEdit->date();

    if(id==""){
        dbHelper->insertProject(projectsDataObject);
    }
    else{
        dbHelper->updateProject(projectsDataObject,id);
    }
    this->close();
    emit AddProjectClosed();

}

void AddProject::on_buttonBox_rejected()
{
    emit AddProjectClosed();
    this->close();
}
