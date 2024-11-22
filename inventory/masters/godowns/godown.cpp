#include "godown.h"
#include "ui_godown.h"

godown::godown(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::godown)
{
    ui->setupUi(this);
    dbHelper = new GodownDatabaseHelper();
    ui->tableWidget->setColumnCount(2);
    settable();
}

godown::~godown()
{
    delete ui;
}

void godown::settable()
{
    godownList = dbHelper->getAllGodowns();
    int r =0;
    ui->tableWidget->setRowCount(0);
    godownList = dbHelper->getAllGodowns();

    QTableWidgetItem* propertyName = new QTableWidgetItem;
    propertyName->setText("Godown");
    ui->tableWidget->setHorizontalHeaderItem(0,propertyName);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    QTableWidgetItem* propertyUnit = new QTableWidgetItem;
    propertyUnit->setText("Location");
    ui->tableWidget->setHorizontalHeaderItem(1,propertyUnit);
    for(GodownDataObject* obj:godownList){
        ui->tableWidget->insertRow(r);
        QTableWidgetItem* GroupName = new QTableWidgetItem;
        GroupName->setText(obj->GodownName);
        ui->tableWidget->setItem(r,0,GroupName);
        QTableWidgetItem* propertyUnit = new QTableWidgetItem;
        propertyUnit->setText(obj->GodownLocation);
        ui->tableWidget->setItem(r,1,propertyUnit);
        r++;
    }
}

void godown::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_pushButton_clicked();
    }

}

void godown::on_pushButton_clicked()
{
    addGodownObject = new addGodown(this);
    addGodownObject ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addGodownObject ->show();
    QObject::connect(addGodownObject,SIGNAL(closing()),this,SLOT(settable()));
}



void godown::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    addGodownObject = new addGodown(this);
    addGodownObject ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addGodownObject ->setTitle("Update Godown");
    addGodownObject ->setGodown(godownList[r]);
    addGodownObject->edit();
    addGodownObject ->show();
    QObject::connect(addGodownObject,SIGNAL(closing()),this,SLOT(settable()));

}
