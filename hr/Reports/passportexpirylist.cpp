#include "passportexpirylist.h"
#include "ui_passportexpirylist.h"
#include "hr/adduserprofile.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

PassportExpiryList::PassportExpiryList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassportExpiryList)
{
    ui->setupUi(this);

    dbHelper= new UserProfileDatabaseHelper();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
}

PassportExpiryList::~PassportExpiryList()
{
    delete ui;
}

void PassportExpiryList::on_pushButton_clicked()
{
    setTable();
}

void PassportExpiryList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    AddUserProfile* AddUserProfileWidget = new AddUserProfile(this);
    AddUserProfileWidget->setWindowFlags(Qt::Window);
    UserProfileDataModel user = dbHelper->getEmployeeByID(model->record(r).value(0).toInt());
    AddUserProfileWidget->showVoucher(user);
    AddUserProfileWidget->show();
    QObject::connect(AddUserProfileWidget,SIGNAL(closing()),this,SLOT(setTable()));

}

void PassportExpiryList::setTable()
{
    int count_days = ui->spinBox->value();
    model = dbHelper->passportExpiryList(count_days);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
