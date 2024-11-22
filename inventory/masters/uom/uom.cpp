#include "uom.h"
#include "ui_uom.h"

UOM::UOM(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UOM)
{
    ui->setupUi(this);
    dbHelper = new UomDataBaseHelper();
    model = new QSqlQueryModel();
    ui->tableWidget->insertColumn(0);
//    ui->conversionWidget->hide();
    settable();
    qDebug()<<"End of uom contructor";
}

UOM::~UOM()
{
    delete dbHelper;

    delete ui;
}

void UOM::settable()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    uomList = dbHelper ->getAllUOMasPtr();
    ui->tableWidget->setRowCount(0);
    QTableWidgetItem* UOMName = new QTableWidgetItem;
    UOMName->setText("UOM NAME");
    ui->tableWidget->setHorizontalHeaderItem(0,UOMName);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    int r=0;
    for(UomDataModel* data:uomList){
        ui->tableWidget->insertRow(r);
        QTableWidgetItem* UOMName = new QTableWidgetItem;
        UOMName->setText(data->UomName);
        ui->tableWidget->setItem(r,0,UOMName);
        r++;
   }


    model = dbHelper->getDefaultUOMConversions();
    ui->UomConversionTableView->setModel(model);
    ui->UomConversionTableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->UomConversionTableView->hideColumn(0);
}

void UOM::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewMeasurementPushButton_clicked();
    }
   if ((e->key() == Qt::Key_M)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewConversionPushButton_clicked();
    }
}

void UOM::on_createNewMeasurementPushButton_clicked()
{
    addUOMWidget = new addUOM(this);
    addUOMWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMWidget->show();
    QObject::connect(addUOMWidget,SIGNAL(closing()),this,SLOT(settable()));

}

void UOM::on_createNewConversionPushButton_clicked()
{
    addUOMCompoundWidget = new addConversion(this);
    addUOMCompoundWidget ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMCompoundWidget->show();
    QObject::connect(addUOMCompoundWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void UOM::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    addUOMWidget = new addUOM(this);
    addUOMWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMWidget->setTitle("Update UOM");
    addUOMWidget->editUOM(uomList[r]);
    addUOMWidget->show();
    QObject::connect(addUOMWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void UOM::on_UomConversionTableView_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    addUOMCompoundWidget = new addConversion(this);
    addUOMCompoundWidget ->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addUOMCompoundWidget->setTitle("Update UOM Conversion");
    addUOMCompoundWidget->setData(dbHelper->getUOMConversionById(model->record(r).value(0).toString()));
    addUOMCompoundWidget->show();
    QObject::connect(addUOMCompoundWidget,SIGNAL(closing()),this,SLOT(settable()));
}
