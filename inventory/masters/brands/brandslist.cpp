#include "brandslist.h"
#include "ui_brandslist.h"
#include "login/loginvalues.h"
#include <QMessageBox>

BrandsList::BrandsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrandsList)
{
    ui->setupUi(this);
    dbHelper = new BrandsDatabaseHelper();

    ui->tableWidget->setColumnCount(3);

    ui->tableWidget->hideColumn(2);
    qDebug()<<LoginValues::getUserName();
    if(!(LoginValues::getUserName().contains("naseem",Qt::CaseInsensitive) ||
           LoginValues::getUserName().contains("shabib",Qt::CaseInsensitive)) ){
        ui->tableWidget->hideColumn(1);
    }

    settable();

}

BrandsList::~BrandsList()
{
    delete ui;
}

void BrandsList::settable()
{
    qDebug()<<"set brands table";
    ui->tableWidget->setRowCount(0);
    brandsList = dbHelper->getAllBrands();

    QTableWidgetItem* propertyName = new QTableWidgetItem;
    propertyName->setText("Brand");
    ui->tableWidget->setHorizontalHeaderItem(0,propertyName);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(1,deleteItem);



    ui->tableWidget->setColumnWidth(1,25);



    //qDebug()<<groupObjs.size();
    int r=0;
    for(int i=0;i<brandsList.size();i++){
        BrandsDataModel *obj=brandsList[i];
        if(obj->brandName.contains(x,Qt::CaseInsensitive)){
            ui->tableWidget->insertRow(r);
            QTableWidgetItem* GroupName = new QTableWidgetItem;
            GroupName->setText(obj->brandName);
            ui->tableWidget->setItem(r,0,GroupName);

            QToolButton* delButton = new QToolButton;
            delButton->setIcon(QIcon(":/icons/trash.ico"));
            delButton->setProperty("row",i);
            delButton->setAutoRaise(true);
            ui->tableWidget->setCellWidget(r,1,delButton);
            QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

            QTableWidgetItem* hide = new QTableWidgetItem;
            hide->setText(QString::number(i));
            ui->tableWidget->setItem(r,2,hide);

            r++;
        }
    }

}

void BrandsList::searchTextChanged(QString text)
{
    x=text;
    settable();

}

void BrandsList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void BrandsList::delButtonClicked(bool v)
{
    int x= sender()->property("row").toInt();
    dbHelper->deleteBrand(brandsList[x]->brandID);
    settable();
}

void BrandsList::on_createNewPushButton_clicked()
{
    AddBrandsWidget = new AddBrands(this);
    AddBrandsWidget->setWindowFlags(Qt::Window);
    AddBrandsWidget->show();
    QObject::connect(AddBrandsWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void BrandsList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r=index.row();
    int col = ui->tableWidget->item(r,2)->text().toInt();
    AddBrandsWidget = new AddBrands(this);
    AddBrandsWidget->setWindowFlags(Qt::Window);
    AddBrandsWidget->setBrand(brandsList[col]);
    AddBrandsWidget->edit();
    AddBrandsWidget->show();
    QObject::connect(AddBrandsWidget,SIGNAL(closing()),this,SLOT(settable()));

}
