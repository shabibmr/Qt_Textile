#include "kotprintersettings.h"
#include "ui_kotprintersettings.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
KOTPrinterSettings::KOTPrinterSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KOTPrinterSettings)
{
    ui->setupUi(this);

    ui->progressBar->setValue(0);
    ui->progressBar->hide();
    AvailablePrintersList= QPrinterInfo::availablePrinterNames();
    dbhelper = new SalesInventoryItemDatabaseHelper;

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ui->label->setText("Select Printers");
    }

    ui->tableWidget->setColumnCount(AvailablePrintersList.size()+1);
    ui->grptableWidget->setColumnCount(AvailablePrintersList.size()+1);
    ui->printerTable->setColumnCount(2);

    itemsList = dbhelper->getALLInventoryObjectsAsPtr();
    itemGroupHelper = new SalesInventoryGroupsDatabaseHelper();
    invGroups=itemGroupHelper->getSalesInventoryGroupDataAsMap();

    QTableWidgetItem* PName = new QTableWidgetItem;
    PName->setText("items");
    ui->tableWidget->setHorizontalHeaderItem(0,PName);

    for(int i=1;i<=AvailablePrintersList.size();i++){
        QTableWidgetItem* PName = new QTableWidgetItem;
        PName->setText(AvailablePrintersList[i-1]);
        ui->tableWidget->setHorizontalHeaderItem(i,PName);
        ui->grptableWidget->setHorizontalHeaderItem(i,PName);

        ui->tableWidget->setColumnWidth(i,100);

        ui->grptableWidget->setColumnWidth(i,100);


    }

    QTableWidgetItem* gPName = new QTableWidgetItem;
    gPName->setText("Groups");
    ui->grptableWidget->setHorizontalHeaderItem(0,gPName);


    QTableWidgetItem* iPName = new QTableWidgetItem;
    iPName ->setText("Printers");
    ui->printerTable->setHorizontalHeaderItem(0,iPName );

    QTableWidgetItem* gPcheck = new QTableWidgetItem;
    gPcheck->setText("Check");
    ui->printerTable->setHorizontalHeaderItem(1,gPcheck);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->grptableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->printerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    setTable();

}

KOTPrinterSettings::~KOTPrinterSettings()
{
    delete ui;
}

void KOTPrinterSettings::setTable()
{
    ui->tableWidget->setRowCount(itemsList.size());
    for(int i=0;i<itemsList.size();i++){

        QTableWidgetItem* ItemName = new QTableWidgetItem;
        ItemName->setFlags(ItemName ->flags() ^ Qt::ItemIsEditable);
        ItemName->setText(itemsList[i]->ItemName);
        ui->tableWidget->setItem(i,0,ItemName);

        for(int j=1;j<=AvailablePrintersList.size();j++){
            QCheckBox* box = new QCheckBox;
            box->setChecked(itemsList[i]->KOTPrinter.contains( AvailablePrintersList[j-1],Qt::CaseInsensitive));
            ui->tableWidget->setCellWidget(i,j,box);
        }
    }

    ui->printerTable->setRowCount(AvailablePrintersList.size());

    for(int i=0;i<AvailablePrintersList.size();i++){
        QTableWidgetItem* ItemName = new QTableWidgetItem;
        ItemName->setFlags(ItemName ->flags() ^ Qt::ItemIsEditable);
        ItemName->setText(AvailablePrintersList[i]);
        ui->printerTable->setItem(i,0,ItemName);

        QCheckBox* box = new QCheckBox;
        box->setChecked(false);
        ui->printerTable->setCellWidget(i,1,box);
        box->setProperty("row",i);

        QObject::connect(box,SIGNAL(toggled(bool)),SLOT(setAllItemsTOPrinter(bool)));


    }
    QStringList groups = invGroups.values();

    ui->grptableWidget->setRowCount(groups.size());
    for(int i =0;i<groups.size();i++){
        QTableWidgetItem* ItemName = new QTableWidgetItem;
        ItemName->setFlags(ItemName ->flags() ^ Qt::ItemIsEditable);
        ItemName->setText(groups[i]);
        ui->grptableWidget->setItem(i,0,ItemName);

        for(int j=1;j<=AvailablePrintersList.size();j++){
            QCheckBox* box = new QCheckBox;
            box->setChecked(false);
            box->setProperty("gpname",groups[i]);
            box->setProperty("row",j);
            ui->grptableWidget->setCellWidget(i,j,box);
            QObject::connect(box,SIGNAL(toggled(bool)),SLOT(setAllGroupsTOPrinter(bool)));

        }
    }
}

void KOTPrinterSettings::setAllItemsTOPrinter(bool val){
    int col = sender()->property("row").toInt()+1;
    for(int i=0;i<itemsList.size();i++){
        QCheckBox* chk = qobject_cast <QCheckBox*> (ui->tableWidget->cellWidget(i,col));
        chk->setChecked(val);
    }
}

void KOTPrinterSettings::setAllGroupsTOPrinter(bool val){
    int col = sender()->property("row").toInt();
    QString grpname = sender()->property("gpname").toString();

    for(int i=0;i<itemsList.size();i++){
        if(itemsList[i]->GroupName==grpname){
            QCheckBox* chk = qobject_cast <QCheckBox*> (ui->tableWidget->cellWidget(i,col));
            chk->setChecked(val);
        }
    }

}

void KOTPrinterSettings::on_cancelButton_clicked()
{
        this->close();
}

void KOTPrinterSettings::on_okButton_clicked()
{
    QProgressDialog progress("Task in progress...", "", 0, itemsList.size(), this);
        progress.setWindowModality(Qt::WindowModal);

        dbhelper->startTransaction();
    for(int i=0;i<itemsList.size();i++){
        progress.setValue(i);
        QString kot ="|";
        for(int j=1;j<=AvailablePrintersList.size();++j){
            QCheckBox* chk = qobject_cast <QCheckBox*> (ui->tableWidget->cellWidget(i,j));
            if(chk->isChecked()){
                kot +=AvailablePrintersList[j-1]+ "|";
            }
        }
        dbhelper->updateKOTPrinter(itemsList[i]->ItemID,kot);
    }
    dbhelper->commit();
    progress.setValue(itemsList.size());

    this->close();
}
