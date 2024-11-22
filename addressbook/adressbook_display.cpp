#include "adressbook_display.h"
#include "ui_adressbook_display.h"

AdressBook_Display::AdressBook_Display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdressBook_Display)
{
    ui->setupUi(this);
    ui->customerTableWidget->setColumnCount(4);
    ui->customerTableWidget->hideColumn(3);

    ui->tabWidget->setStyleSheet("background-color:#ffffff;");
    ui->customerTableWidget->setAlternatingRowColors(true);

    ui->customerTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->customerTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->customerTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);


    ui->customerSearch->setPlaceholderText(tr("Search.."));

    dbHelper = new AddressBookDatabaseHelper();
    settableCustomers();
}

AdressBook_Display::~AdressBook_Display()
{
    delete ui;
}

void AdressBook_Display::settableCustomers()
{
    int i=0;

    if(LoginValues::getModel().MaxDiscountPercent<=3)
        return;

    QTableWidgetItem* Company = new QTableWidgetItem;
    Company->setText("Company");
    ui->customerTableWidget->setHorizontalHeaderItem(i++,Company);

    QTableWidgetItem* Name = new QTableWidgetItem;
    Name->setText("Name");
    ui->customerTableWidget->setHorizontalHeaderItem(i++,Name);

    QTableWidgetItem* Phone = new QTableWidgetItem;
    Phone->setText("Phone");
    ui->customerTableWidget->setHorizontalHeaderItem(i++,Phone);

    ui->customerTableWidget->setRowCount(0);
    ui->customerTableWidget->setSortingEnabled(false);
    addList = dbHelper->getAllContactsForSearchAsPtr();
    QString search = ui->customerSearch->text();
    for(int j=0,cnt=0;j<addList.size();j++){
        if(addList[j]->Name.contains(search,Qt::CaseInsensitive)||
                addList[j]->CompanyName.contains(search,Qt::CaseInsensitive)){
            ui->customerTableWidget->insertRow(cnt);

            QTableWidgetItem* Company = new QTableWidgetItem;
            Company->setText(addList[j]->CompanyName);
            ui->customerTableWidget->setItem(cnt,0,Company);

            QTableWidgetItem* Name = new QTableWidgetItem;
            Name->setText(addList[j]->Name);
            ui->customerTableWidget->setItem(cnt,1,Name);

            QTableWidgetItem* Phone = new QTableWidgetItem;
            Phone->setText(addList[j]->phoneNumber);
            ui->customerTableWidget->setItem(cnt,2,Phone);

            QTableWidgetItem* id = new QTableWidgetItem;
            id->setText(QString::number(addList[j]->_id));
            ui->customerTableWidget->setItem(cnt,3,id);
        }

    }
    ui->customerTableWidget->setSortingEnabled(true);


}



void AdressBook_Display::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void AdressBook_Display::on_createNewPushButton_clicked()
{
    AddressBookWidget = new AddressBook(this);
    AddressBookWidget ->setWindowFlags(Qt::Window);
    AddressBookWidget ->show();
    QObject::connect(AddressBookWidget,SIGNAL(closing()),this,SLOT(settableCustomers()));
}

void AdressBook_Display::on_customerTableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    int id = ui->customerTableWidget->item(row,3)->text().toInt();
    AddressBookWidget = new AddressBook(this);
    AddressBookWidget ->setWindowFlags(Qt::Window);
    AddressBookWidget ->edit(id);
    AddressBookWidget ->show();
    QObject::connect(AddressBookWidget,SIGNAL(closing()),this,SLOT(settableCustomers()));
}

void AdressBook_Display::on_customerSearch_textChanged(const QString &arg1)
{
    settableCustomers();
}
