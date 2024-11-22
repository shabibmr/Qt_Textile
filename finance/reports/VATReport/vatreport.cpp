#include "vatreport.h"
#include "ui_vatreport.h"

VATReport::VATReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VATReport)
{
    ui->setupUi(this);

    ui->fromDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateEdit->setDate(QDate::currentDate());

    ui->tabWidget->removeTab(3);
    ui->tabWidget->removeTab(3);
    ui->allDebitNoteTableWidget->hide();
    ui->allCreditNoteTableWidget->hide();
    ui->label_5->hide();
    ui->label_4->hide();

    ui->salesTableView->setItemDelegate(new BillwiseReportDelegate());
    ui->purchaseTableView->setItemDelegate(new BillwiseReportDelegate());


    setTable();
}

VATReport::~VATReport()
{
    delete ui;
}

void VATReport::setTable()
{
    setTableAll();
    setTableSales();
    setTablePurchase();
    setTableDebitNote();
    setTableCreditNote();
}

void VATReport::on_backButton_clicked()
{
    this->close();
}

void VATReport::setTableAll()
{
    ui->allSalesTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allSalesTableWidget->setColumnWidth(1,200);
    ui->allSalesTableWidget->setColumnWidth(2,200);

    ui->allPurchaseTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allPurchaseTableWidget->setColumnWidth(1,200);
    ui->allPurchaseTableWidget->setColumnWidth(2,200);

    ui->allCreditNoteTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allCreditNoteTableWidget->setColumnWidth(1,200);
    ui->allCreditNoteTableWidget->setColumnWidth(2,200);

    ui->allDebitNoteTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->allDebitNoteTableWidget->setColumnWidth(1,200);
    ui->allDebitNoteTableWidget->setColumnWidth(2,200);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    QTableWidgetItem *item1 = new QTableWidgetItem;
    QTableWidgetItem *item2 = new QTableWidgetItem;
    QTableWidgetItem *item3 = new QTableWidgetItem;
    QTableWidgetItem *item4 = new QTableWidgetItem;
    QTableWidgetItem *item5 = new QTableWidgetItem;
    QTableWidgetItem *item6 = new QTableWidgetItem;
    QTableWidgetItem *item7 = new QTableWidgetItem;
    QTableWidgetItem *item8 = new QTableWidgetItem;
    QTableWidgetItem *item9 = new QTableWidgetItem;


    item0->setText(QString::number(0,'f',2));
    item1->setText(QString::number(0,'f',2));
    item2->setText(QString::number(0,'f',2));
    item3->setText(QString::number(0,'f',2));
    item4->setText(QString::number(0,'f',2));
    item5->setText(QString::number(0,'f',2));
    item6->setText(QString::number(0,'f',2));
    item7->setText(QString::number(0,'f',2));
    item8->setText(QString::number(0,'f',2));
    item9->setText(QString::number(0,'f',2));

    item0->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item1->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item2->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item3->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item4->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item5->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item6->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item7->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item8->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    item9->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    QTableWidgetItem *pitem0 = new QTableWidgetItem;
    QTableWidgetItem *pitem1 = new QTableWidgetItem;
    QTableWidgetItem *pitem2 = new QTableWidgetItem;
    QTableWidgetItem *pitem3 = new QTableWidgetItem;
    QTableWidgetItem *pitem4 = new QTableWidgetItem;
    QTableWidgetItem *pitem5 = new QTableWidgetItem;
    QTableWidgetItem *pitem6 = new QTableWidgetItem;
    QTableWidgetItem *pitem7 = new QTableWidgetItem;
    QTableWidgetItem *pitem8 = new QTableWidgetItem;
    QTableWidgetItem *pitem9 = new QTableWidgetItem;


    pitem0->setText(QString::number(0,'f',2));
    pitem1->setText(QString::number(0,'f',2));
    pitem2->setText(QString::number(0,'f',2));
    pitem3->setText(QString::number(0,'f',2));
    pitem4->setText(QString::number(0,'f',2));
    pitem5->setText(QString::number(0,'f',2));
    pitem6->setText(QString::number(0,'f',2));
    pitem7->setText(QString::number(0,'f',2));
    pitem8->setText(QString::number(0,'f',2));
    pitem9->setText(QString::number(0,'f',2));

    pitem0->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem1->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem2->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem3->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem4->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem5->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem6->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem7->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem8->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    pitem9->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ui->allSalesTableWidget->setItem(1,0,item0);
    ui->allSalesTableWidget->setItem(1,1,item1);
    ui->allSalesTableWidget->setItem(1,2,item2);

    ui->allSalesTableWidget->setItem(2,0,item3);
    ui->allSalesTableWidget->setItem(2,1,item4);
    ui->allSalesTableWidget->setItem(2,2,item5);

    ui->allSalesTableWidget->setItem(3,0,item6);
    ui->allSalesTableWidget->setItem(3,1,item7);
    ui->allSalesTableWidget->setItem(3,2,item8);

    ui->allPurchaseTableWidget->setItem(1,0,pitem0);
    ui->allPurchaseTableWidget->setItem(1,1,pitem1);
    ui->allPurchaseTableWidget->setItem(1,2,pitem2);
    ui->allPurchaseTableWidget->setItem(2,0,pitem3);
    ui->allPurchaseTableWidget->setItem(2,1,pitem4);
    ui->allPurchaseTableWidget->setItem(2,2,pitem5);
    ui->allPurchaseTableWidget->setItem(3,0,pitem6);
    ui->allPurchaseTableWidget->setItem(3,1,pitem7);
    ui->allPurchaseTableWidget->setItem(3,2,pitem8);


    float salesTotal =0;
    float outputVat = 0;
    float salesTaxable = 0;

    float purchaseTotal =0;
    float inputVat = 0;
    float purchseTaxable = 0;

    ShiftDataModel *shift = new ShiftDataModel;
    ShiftDatabaseHelper shiftHelper;

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;

    QString billsFrom = shiftHelper.getFirstBillByDate(shift);
    QString billsTo = shiftHelper.getLastBillByDate(shift);

    salesTotal = dbHelper->getSalesTotal(billsFrom,billsTo);
    outputVat = dbHelper->outputVatTotal(billsFrom,billsTo);
    salesTaxable = dbHelper->getSales5Total(billsFrom,billsTo);

    purchaseTotal = dbHelper->getPurchaseTotal(fromDate,toDate);
    inputVat = dbHelper->inputVatTotal(fromDate,toDate);
    purchseTaxable = dbHelper->getPurchase5Total(fromDate,toDate);

    QTableWidgetItem *ItemSalesTotal = new QTableWidgetItem;
    QTableWidgetItem *ItemOutputVat = new QTableWidgetItem;
    QTableWidgetItem *ItemSalesTaxable = new QTableWidgetItem;
    QTableWidgetItem *ItemPurchaseTotal = new QTableWidgetItem;
    QTableWidgetItem *ItempInputVat = new QTableWidgetItem;
    QTableWidgetItem *ItempPurchaseTaxable = new QTableWidgetItem;

    ItemSalesTotal->setText(QString::number(salesTotal,'f',2));
    ItemOutputVat->setText(QString::number(outputVat,'f',2));
    ItemSalesTaxable ->setText(QString::number(salesTaxable,'f',2));
    ItemPurchaseTotal->setText(QString::number(purchaseTotal,'f',2));
    ItempInputVat ->setText(QString::number(inputVat,'f',2));
    ItempPurchaseTaxable ->setText(QString::number(purchseTaxable,'f',2));

    ItemSalesTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemOutputVat->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemSalesTaxable ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItemPurchaseTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItempInputVat ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    ItempPurchaseTaxable ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ui->allSalesTableWidget->setItem(0,0,ItemSalesTotal);
    ui->allSalesTableWidget->setItem(0,1,ItemSalesTaxable);
    ui->allSalesTableWidget->setItem(0,2,ItemOutputVat);
    ui->allPurchaseTableWidget->setItem(0,0,ItemPurchaseTotal);
    ui->allPurchaseTableWidget->setItem(0,1,ItempPurchaseTaxable);
    ui->allPurchaseTableWidget->setItem(0,2,ItempInputVat);




    QTableWidgetItem *TItemSalesTotal = new QTableWidgetItem;
    QTableWidgetItem *TItemOutputVat = new QTableWidgetItem;
    QTableWidgetItem *TItemSalesTaxable = new QTableWidgetItem;
    QTableWidgetItem *TItemPurchaseTotal = new QTableWidgetItem;
    QTableWidgetItem *TItempInputVat = new QTableWidgetItem;
    QTableWidgetItem *TItempPurchaseTaxable = new QTableWidgetItem;

    TItemSalesTotal->setText(QString::number(salesTotal,'f',2));
    TItemOutputVat->setText(QString::number(outputVat,'f',2));
    TItemSalesTaxable ->setText(QString::number(salesTaxable,'f',2));
    TItemPurchaseTotal->setText(QString::number(purchaseTotal,'f',2));
    TItempInputVat ->setText(QString::number(inputVat,'f',2));
    TItempPurchaseTaxable ->setText(QString::number(purchseTaxable,'f',2));

    TItemSalesTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemOutputVat->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemSalesTaxable ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItemPurchaseTotal->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItempInputVat ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    TItempPurchaseTaxable ->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);

    ui->allSalesTableWidget->setItem(3,0,TItemSalesTotal);
    ui->allSalesTableWidget->setItem(3,1,TItemSalesTaxable);
    ui->allSalesTableWidget->setItem(3,2,TItemOutputVat);
    ui->allPurchaseTableWidget->setItem(3,0,TItemPurchaseTotal);
    ui->allPurchaseTableWidget->setItem(3,1,TItempPurchaseTaxable);
    ui->allPurchaseTableWidget->setItem(3,2,TItempInputVat);

    float vatdiff = outputVat - inputVat;
    if(vatdiff > 0){
        ui->payableCreditLineEdit->setText(QString::number(vatdiff,'f',2));
    }
    else{
        ui->receivableCreditLineEdit->setText(QString::number(-vatdiff,'f',2));
    }


}

void VATReport::setTableSales()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    salesModel = ledHelper->getSalesReportModelByDate(fromDateTime,toDateTime);

    sortSales = new GMItemSortFilterProxyModel(this);
    sortSales->setDynamicSortFilter(true);
    sortSales->setSourceModel(salesModel);


    ui->salesTableView->setModel(sortSales);
    ui->salesTableView->setSortingEnabled(true);




    ui->salesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<salesModel->rowCount();i++){
        total += salesModel->record(i).value(3).toFloat();
    }
}

void VATReport::setTablePurchase()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    purchaseModel = ledHelper->getPurchaseReportModelByDate(fromDateTime,toDateTime);
    sortPurchase = new GMItemSortFilterProxyModel(this);
    sortPurchase->setDynamicSortFilter(true);
    sortPurchase->setSourceModel(purchaseModel);


    ui->purchaseTableView->setModel(sortPurchase);
    ui->purchaseTableView->setSortingEnabled(true);
    ui->purchaseTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<purchaseModel->rowCount();i++){
        total += purchaseModel->record(i).value(3).toFloat();
    }
}

void VATReport::setTableCreditNote()
{

}

void VATReport::setTableDebitNote()
{

}

void VATReport::on_OKButton_clicked()
{
    setTable();
}
