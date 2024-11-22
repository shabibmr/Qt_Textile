#include "vatreport.h"
#include "ui_vatreport.h"
#include <sharetools/excel/excelreporthelper.h>

VatReport::VatReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VatReport)
{
    ui->setupUi(this);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    dbHelper = new VatReportDatabaseHelper;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ledHelper = new LedgerHelper;
qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->fromDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->toDateEdit->setDate(QDate::currentDate());
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    ui->tabWidget->setCurrentIndex(0);  //commented due to crash

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    b2csalesTableView = new ReportsTableView();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    b2csalesTableView->setItemDelegate(new ReportCommonDelegate());
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->b2CSalesLayout->addWidget(b2csalesTableView);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<b2csalesTableView;


    purchaseTableView = new ReportsTableView();
    purchaseTableView->setItemDelegate(new ReportCommonDelegate());
    ui->purchaseLayout->addWidget(purchaseTableView);


//    ui->tabWidget->removeTab(4);
//    ui->tabWidget->removeTab(4);
//    ui->tabWidget->removeTab(2);

qDebug()<<Q_FUNC_INFO<<__LINE__;
    setTable();
}

VatReport::~VatReport()
{
    delete ui;
}

void VatReport::setTable()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    switch (ui->tabWidget->currentIndex()) {

    case 0:
    {
        if(!loadedOutput){
            setTableb2cSales();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            loadedOutput = true;
        }
        break;
    }
    case 1:
    {
        if(!loadedInput){
            setTablePurchase();
            loadedInput = true;
        }
        break;
    }


    }
}



void VatReport::setTableb2cSales()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    b2cModel = ledHelper->getDatewiseSales(fromDateTime,toDateTime);

    sortB2CSales = new GMItemSortFilterProxyModel(this);
    sortB2CSales->setDynamicSortFilter(true);
    sortB2CSales->setSourceModel(b2cModel);

    qDebug()<<Q_FUNC_INFO<<__LINE__;


    b2csalesTableView->setAvlcols(QList<int>({1,2,3}));
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    b2csalesTableView->setModel(sortB2CSales);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    b2csalesTableView->setModelQry(b2cModel);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    b2csalesTableView->setSortingEnabled(true);

    qDebug()<<Q_FUNC_INFO<<__LINE__;


    b2csalesTableView->setColumns();
    b2csalesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    qDebug()<<Q_FUNC_INFO<<__LINE__;


}

void VatReport::setTablePurchase()
{
    qDebug()<<"tab clicked"<<ui->tabWidget->currentIndex();

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    purchaseModel = ledHelper->getDatewisePurchase(fromDateTime,toDateTime);
    sortPurchase = new GMItemSortFilterProxyModel(this);
    sortPurchase->setDynamicSortFilter(true);
    sortPurchase->setSourceModel(purchaseModel);

    purchaseTableView->setAvlcols(QList<int>({1,2,3}));
    purchaseTableView->setModel(purchaseModel);
    purchaseTableView->setModelQry(purchaseModel);
    purchaseTableView->setSortingEnabled(true);

    purchaseTableView->setColumns();
    purchaseTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    float total = 0;
//    for(int i=0;i<purchaseModel->rowCount();i++){
//        total += purchaseModel->record(i).value(3).toFloat();
//    }
}

void VatReport::on_OKButton_clicked()
{

    loadedOutput = false;
    loadedInput = false;


    setTable();
}

void VatReport::on_excelButton_clicked()
{
    ExcelReportHelper excel;
    QDate fDate = ui->fromDateEdit->date();
    QDate tDate = ui->toDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString reportName = "Gst Report";
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20";
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    if(!loadedOutput){
        setTableb2cSales();
        loadedOutput = true;
    }

    if(!loadedInput){
        setTablePurchase();
        loadedInput = true;
    }


    cols.clear(); cols<<"25"<<"20"<<"20"<<"40";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20;
    excel.GenericReportFromTableView
            (fileName,"Sales B2C",cols,b2csalesTableView,fromDate,toDate,avlCols,false);

    cols.clear(); cols<<"25"<<"20"<<"20"<<"40";
    avlCols.clear(); avlCols<<0<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<19<<20;
    excel.GenericReportFromTableView
            (fileName,"Purchases",cols,purchaseTableView,fromDate,toDate,avlCols,false);

    qDebug()<<"Excel Completed";
}

void VatReport::on_tabWidget_tabBarClicked(int index)
{
    setTable();
}

void VatReport::on_tabWidget_currentChanged(int index)
{
    setTable();
}
