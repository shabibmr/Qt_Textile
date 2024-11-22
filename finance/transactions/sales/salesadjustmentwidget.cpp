#include "salesadjustmentwidget.h"
#include "ui_salesadjustmentwidget.h"

#include "database/Counter/countersettingsdatabasehelper.h"
#include "voucherrecalculater.h"
#include "counter/Settings/settings.h"
#include "counter/Reports/Restaurant/itemtrasactionlist.h"
#include "database/Reports/inventoryhelper.h"


#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFileDialog>

SalesAdjustmentWidget::SalesAdjustmentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesAdjustmentWidget)
{
    ui->setupUi(this);

    ui->fromDate->setDate(QDate::currentDate());
    ui->toDate->setDate(QDate::currentDate());

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(0);
    ui->progressBar->hide();

    ui->recalculatePushButton->hide();

    //    ui->exclVoucherLineEdit->setMaximumHeight(50);
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(&this->FutureWatcher, SIGNAL (finished()), this, SLOT (recalc_finished()));

    dbHelper = new salesVoucherDatabaseHelper();

    setTableHeader();

    setItemTable();
}

SalesAdjustmentWidget::~SalesAdjustmentWidget()
{
    delete ui;
}

void SalesAdjustmentWidget::setItemTable()
{
    totalSelected = 0;
    excludedVouchersList = ui->exclVoucherLineEdit->text().split(",");
    itemList.clear();

    ui->totalSelectedLineEdit->setText(QString::number(totalSelected, 'f',2));

    QDate fromDate = ui->fromDate->date();
    fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDate->date().addDays(CounterSettingsDataModel::EndDay);
    toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    ui->totalLineEdit->setText(QString::number(dbHelper->getTotalSalesByDate(fromDateTime, toDateTime, excludedVouchersList), 'f',2));
    itemModel = dbHelper->getListOfItemsByDate(fromDateTime, toDateTime, excludedVouchersList);

    ui->tableWidget->setRowCount(0);

    for(int row=0;row<itemModel->rowCount();row++){
        ui->tableWidget->insertRow(row);

        int col = 0;

        QCheckBox* selectItem = new QCheckBox;
        selectItem->setProperty("row",row);
        ui->tableWidget->setCellWidget(row,col++,selectItem);
        QObject::connect(selectItem, &QCheckBox::stateChanged, this, [=](int state){
            qDebug()<<"state"<<itemModel->record(row).value(1).toString()<<state;
            if(state ==2){
                itemList.append(itemModel->record(row).value(0).toString());
                totalSelected += itemModel->record(row).value(5).toFloat();
                qDebug()<<itemList;
            }
            else{
                qDebug()<<"pos"<<itemList.indexOf(itemModel->record(row).value(0).toString());
                itemList.removeAt(itemList.indexOf(itemModel->record(row).value(0).toString()));
                totalSelected -= itemModel->record(row).value(5).toFloat();
                qDebug()<<itemList;
            }
            ui->totalSelectedLineEdit->setText(QString::number(totalSelected, 'f',2));
        });

        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(itemModel->record(row).value(1).toString());
        //        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(row,col++,item);

        QTableWidgetItem* vCount = new QTableWidgetItem;
        vCount->setText(itemModel->record(row).value(2).toString());
        //        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(row,col++,vCount);

        QTableWidgetItem* qty = new QTableWidgetItem;
        qty->setText(itemModel->record(row).value(3).toString());
        //        qty->setFlags(qty->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(row,col++,qty);

        QTableWidgetItem* value = new QTableWidgetItem;
        value->setText(itemModel->record(row).value(4).toString());
        //        DueDate->setFlags(DueDate->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(row,col++,value);

        QTableWidgetItem* totalValue = new QTableWidgetItem;
        totalValue->setText(itemModel->record(row).value(5).toString());
        //        DueDate->setFlags(DueDate->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(row,col++,totalValue);
    }

}

void SalesAdjustmentWidget::on_okButton_clicked()
{
    setItemTable();
}

void SalesAdjustmentWidget::setTableHeader()
{
    ui->tableWidget->setColumnCount(6);

    int i=0;

    QTableWidgetItem* check = new QTableWidgetItem;
    check->setText("");
    ui->tableWidget->setHorizontalHeaderItem(i++,check);


    QTableWidgetItem* itemName = new QTableWidgetItem;
    itemName->setText("Item");
    ui->tableWidget->setHorizontalHeaderItem(i++,itemName);

    QTableWidgetItem* billCount = new QTableWidgetItem;
    billCount->setText("Num of Bills");
    ui->tableWidget->setHorizontalHeaderItem(i++,billCount);

    QTableWidgetItem* Quantity = new QTableWidgetItem;
    Quantity->setText("Quantity");
    ui->tableWidget->setHorizontalHeaderItem(i++,Quantity);

    QTableWidgetItem* Value = new QTableWidgetItem;
    Value->setText("Sub Total");
    ui->tableWidget->setHorizontalHeaderItem(i++,Value);

    QTableWidgetItem* TotalValue = new QTableWidgetItem;
    TotalValue->setText("Total Amount");
    ui->tableWidget->setHorizontalHeaderItem(i++,TotalValue);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();
}

void SalesAdjustmentWidget::on_recalculatePushButton_clicked()
{

    QTime startTime = QTime::currentTime();
    qDebug()<<"start"<<QTime::currentTime();

    QString fnName = Q_FUNC_INFO;
    QString errorData = " start recalculate " +  fnName + QTime::currentTime().toString();
    QFile File("featadjlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    File.write(errorData.toUtf8());
    File.close();

    // add here - remove this line

    voucherList = dbHelper->getVoucherNosListByItems(itemList, fromDateTime, toDateTime, excludedVouchersList);
    qDebug()<<"^^^^^^^^^^^"<<voucherList<<"items"<<itemList<<QTime::currentTime();
    QMessageBox::StandardButton reply;

    reply= QMessageBox::question(this,"Delete Items","Are you Sure You want to Delete?\n No. of bills: "
                                 + QString::number(voucherList.size()) + " \n No. of items: "
                                 + QString::number(itemList.size()),
                                 QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){

        ui->progressBar->setMaximum(voucherList.size());
        ui->progressBar->show();
        if(dbHelper->deleteItemsFromVouchers(itemList, fromDateTime, toDateTime, voucherList )){

        QThread* thread = new QThread();
        VoucherRecalculater *recalc = new VoucherRecalculater();
        recalc->moveToThread(thread);
        thread->start();
        connect(recalc,&VoucherRecalculater::recalcCompleted, this, [=](){
            ui->progressBar->hide();
            QTime endTime = QTime::currentTime();
            qDebug()<<"time taken"<<startTime.QTime::secsTo(endTime)<<voucherList.size();
            setItemTable();
        }, Qt::QueuedConnection);

        connect(recalc, &VoucherRecalculater::voucherUpdated,this,[=](int step){
            ui->progressBar->setValue(step);
        });

        connect(thread, &QThread::started, this, [=](){
            recalc->deleteItemsinPeriod(itemList, fromDateTime, toDateTime, voucherList);
            QList<QList<QPair<QString, QString>>> mainList;
            int j = voucherList.size()/100 + 1; // no. of sets needed
            for(int i=0; i<j; i++) //
            {
                QList<QPair<QString, QString>> vList;
                mainList.append(vList);
            }

            for(int i=0; i<voucherList.size(); i++){
                mainList[i%j].append(voucherList[i]);
            }
            for(int i=0 ; i<j ; i++)
                recalc->recalculateVouchers(mainList[i]);

            // Start the computation.
                //            QFuture<void> future = QtConcurrent::run(recalc, &VoucherRecalculater::recalculateVouchers,voucherList);
                //            this->FutureWatcher.setFuture(future);
        });
    }
    }
}

void SalesAdjustmentWidget::recalc_finished()
{
    ui->progressBar->hide();

    QString fnName = Q_FUNC_INFO;
    QString errorData = " end recalculate " +  fnName + QTime::currentTime().toString();
    QFile File("featadjlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    File.write(errorData.toUtf8());
    File.close();
}

void SalesAdjustmentWidget::backupFinished()
{
    ui->recalculatePushButton->show();
    ui->backupPushButton->hide();
}

void SalesAdjustmentWidget::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    //    InventoryHelper *invHelper = new InventoryHelper();
    int row =  index.row();
    QString itemid = itemModel->record(row).value(0).toString();
    QString ItemName =itemModel->record(row).value(1).toString();
    ItemTrasactionList *itemsWid = new ItemTrasactionList(itemid,
                                                          new SalesInventoryItemDatabaseHelper(),
                                                          new InventoryHelper(),
                                                          this);
    itemsWid->setDateFilters(ui->fromDate->date(),ui->toDate->date());
    itemsWid->setVoucherTypeFilter(GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
    itemsWid->setValue();
    itemsWid->setAttribute(Qt::WA_DeleteOnClose);
    itemsWid->setMinimumSize(500,700);
    itemsWid->show();


}

void SalesAdjustmentWidget::on_exclVoucherLineEdit_editingFinished()
{
    setItemTable();
}

void SalesAdjustmentWidget::on_backupPushButton_clicked()
{
    Settings *settings = new Settings();
    QString path = QDir::currentPath();
    connect(settings, SIGNAL(backupCompleted()), this, SLOT(backupFinished()));
    settings->savebackup(path);
}


