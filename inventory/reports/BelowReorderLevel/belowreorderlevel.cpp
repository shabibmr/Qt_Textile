#include "belowreorderlevel.h"
#include "ui_belowreorderlevel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

ReOrderLevel::ReOrderLevel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BelowReorderLevel)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->DateWidget->hide();
    itemReportHelper = new InventoryHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
    ui->toDateDateEdit->setDate(QDate::currentDate());
    setTable();

    ui->tableView->setItemDelegate(new ReportCommonDelegate());





}

ReOrderLevel::~ReOrderLevel()
{
    delete ui;
}

void ReOrderLevel::on_OKButton_clicked()
{

}

void ReOrderLevel::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Re-Order Level Report";

    qDebug()<<"1";
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    qDebug()<<"2";
    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<1<<2<<3<<4<<5;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
    qDebug()<<"3";
}

void ReOrderLevel::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20<<20<<20;
    AlignmentList<< 0<< 1<<1<<1<<1;

    QPrinter *printer = new QPrinter;
    printer->setPrinterName("printerName");
    CounterPrinterHelper *printerHelper = new CounterPrinterHelper;

    printerHelper->setPrinter(printer);

    QList<int> avlCols; avlCols<<1<<2<<3<<4<<5;
    printerHelper-> printReportGeneral(ui->tableView
                                       ,widthList,"Re-Order Level Report",fromDate,toDate,AlignmentList
                                       ,extraTop,extraBottom,avlCols);
    qDebug()<<"84^^^^^^";
}

void ReOrderLevel::on_backButton_clicked()
{

}

void ReOrderLevel::setTable()
{
    QDate dateTo = ui->toDateDateEdit->date();
    model = itemReportHelper->getReOrderLevelReport(dateTo);
    ui->tableView->setModel(model);
    if(model->columnCount()>2){
        ui->tableView->hideColumn(0);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    }
    else{
        QMessageBox box; box.setText("Could Not Read!!!"); box.exec();
    }

}

void ReOrderLevel::on_exportButton_clicked()
{

    QList<CompoundItemDataObject> items;


    for(int i=0;i<model->rowCount();i++){
        QString itemid = model->record(i).value(0).toString();
        CompoundItemDataObject item;
        item.BaseItem = itemsHelper->getInventoryItemByID(itemid);
        //        item.BaseItem.quantity = item.BaseItem.moq;
        item.BaseItem.quantity = model->record(i).value(4).toFloat() - model->record(i).value(5).toFloat();
        items.append(item);
    }

    PurchaseOrderVoucherEditor *editor = new PurchaseOrderVoucherEditor();
    editor->AddItemstoVoucher(items);
    editor->showMaximized();
}
