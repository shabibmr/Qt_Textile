#include "visaexpiryreport.h"
#include "ui_visaexpiryreport.h"
#include "hr/adduserprofile.h"
VisaExpiryReport::VisaExpiryReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisaExpiryReport)
{
    ui->setupUi(this);
    this->setWindowTitle("Visa Expiry");

    ui->spinBox->setValue(30);
    dbHelper = new UserProfileDatabaseHelper;
    setTable();
}

VisaExpiryReport::~VisaExpiryReport()
{
    delete ui;
}

void VisaExpiryReport::on_okButton_clicked()
{
    setTable();
}

void VisaExpiryReport::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    AddUserProfile* AddUserProfileWidget = new AddUserProfile(this);
    AddUserProfileWidget->setWindowFlags(Qt::Window);
    UserProfileDataModel user = dbHelper->getEmployeeByID(model->record(r).value(0).toInt());
    AddUserProfileWidget->showVoucher(user);
    AddUserProfileWidget->show();
    QObject::connect(AddUserProfileWidget,SIGNAL(closing()),this,SLOT(setTable()));

}

void VisaExpiryReport::setTable()
{
    qDebug()<<Q_FUNC_INFO;
    int count_days = ui->spinBox->value();
    qDebug()<<"123";
    model = dbHelper->visaExpiryList(count_days);
    qDebug()<<"456";
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
