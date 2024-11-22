#include "companyprofilewidget.h"
#include "ui_companyprofilewidget.h"
#include <QMessageBox>

CompanyProfileWidget::CompanyProfileWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompanyProfileWidget)
{
    ui->setupUi(this);
    dbHelper = new CompanyProfileDataBaseHelper();
    profile = new CompanyProfileDataModel();
    ui->installationDateDateEdit->setDate(QDate::currentDate());
    ui->logoLineEdit->setReadOnly(true);
    //    QFile
    //    ui->companyLogoWidget
}

CompanyProfileWidget::~CompanyProfileWidget()
{
    delete ui;
}

void CompanyProfileWidget::on_OKButton_clicked()
{
    profile->CompanyName = ui->companyNameLineEdit->text();
    profile->branch = ui->branchLineEdit->text();
    profile->addressLine = ui->companyAddressPlainTextEdit->toPlainText();
    profile->email = ui->emailLineEdit->text();
    profile->PhoneNumber = ui->phoneNoLineEdit->text();
    profile->webSite = ui->webSiteLineEdit->text();
    profile->trn = ui->tRNLineEdit->text();
    if(ui->logoLineEdit->text().length()>0){
        QFile f(ui->logoLineEdit->text());
        if(f.open(QIODevice::ReadOnly)){
             QFileInfo fileInfo(f.fileName());
             QString filename(fileInfo.fileName());
                profile->logo = f.readAll();
                f.close();
        }
        else{
            QMessageBox box; box.setText("Cannot Open Logo File");
        }
    }
    profile->ManagerName = ui->managerNameLineEdit->text();
    profile->Currency = ui->currencyLineEdit->text();
    profile->billTagLine = ui->billTagLineLineEdit->text();
    profile->city = ui->cityLineEdit->text();
    profile->country = ui->countryLineEdit->text();
    profile->ledgerId = "LedgerId";
    profile->location = "Location";
    profile->Building = "Building";
    profile->POBox = "POBox";
    profile->Street = "Street";
    profile->Fax = "Fax";
    //    profile->DateOfEstablishment = "Date_Of_Establishment";
    profile->mobileNumber = ui->mobileNoLineEdit->text();
    profile->notes = "Notes";
    profile->LicenseNumber = ui->softwareLicenseLineEdit->text();
    profile->InstallationDate = ui->installationDateDateEdit->date();

    dbHelper->updateCompany(profile,1);

}

void CompanyProfileWidget::on_cancelButton_clicked()
{
    this->close();
}

void CompanyProfileWidget::on_uploadButton_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         "Select a File for ",
                                         QDir::homePath(),
                                         tr("Image Files (*.png *.jpg)"));

    ui->logoLineEdit->setText(fileName);

}
