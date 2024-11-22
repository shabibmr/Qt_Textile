#include "receiptentry.h"
#include "ui_receiptentry.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "sharetools/excel/excelreporthelper.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

ReceiptEntry::ReceiptEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceiptEntry)
{
    ui->setupUi(this);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    ui->totalAmountLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->totalAmountLineEdit->setAlignment(Qt::AlignRight);
    dbhelper = new ReceiptVoucherDatabaseHelper();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();
}

ReceiptEntry::~ReceiptEntry()
{
    delete ui;
}

void ReceiptEntry::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void ReceiptEntry::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNew_clicked();
    }

}

void ReceiptEntry::on_createNew_clicked()
{
    editor = new ReceiptVoucherEditor();
    editor ->setWindowFlags(Qt::Window);
    editor ->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}

\

void ReceiptEntry::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date();//.addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateModel(fromDateTime,toDateTime);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(7).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
}

void ReceiptEntry::on_okPushButton_clicked()
{
    setTableView();
}

void ReceiptEntry::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void ReceiptEntry::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);

    editor = new ReceiptVoucherEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher(voucher);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}

void ReceiptEntry::on_pushButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateEdit->date();
    QDate toDate = ui->toDateEdit->date();

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\ReceiptsReport_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    widthList <<20<<10<<40<<20<<10<<30;
    AlignmentList<<0<<0<<0<<0<<1<<0;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;

    ExcelReportHelper *ex = new ExcelReportHelper;

    QSqlQueryModel *m = dbhelper->getDetailsList(fromDate,toDate);
    float total = 0;
    for(int i =0;i<m->rowCount();i++){
          total += model->record(i).value(2).toFloat();
    }
    extraBottom<<"Total : "+QString ::number(total,'f',2);

    if(m->rowCount()>0){
        //        printer-> printReportGeneralwithModel(m
        //                                              ,widthList,"Payments Report",fromDate,toDate,AlignmentList
        //                                              ,extraTop,extraBottom,avlCols);
        ex->GenericReportFromModel(fileName,"Ledgerwise Receipt",
                                        widthList,m,
                                        fromDate.toString("yyyy-MM-dd"),
                                        toDate.toString("yyyy-MM-dd"),
                                        avlCols);

    }
}

void ReceiptEntry::on_pushButton_2_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateEdit->date();
    QDate toDate = ui->toDateEdit->date();

//    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
//    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\Ledgerwise Payments_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
//    if (fileName.isEmpty())
//    {
//        return;
//    }

    widthList <<20<<10<<40<<20<<10<<30;
    AlignmentList<<0<<0<<0<<0<<1<<0;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;

    CounterPrinterHelper *printer = new CounterPrinterHelper;
    printer->setPrinter(new QPrinter(QPrinter::ScreenResolution));

//    ExcelReportHelper *ex = new ExcelReportHelper;

    QSqlQueryModel *m = dbhelper->getDetailsList(fromDate,toDate);
    float total = 0;
    for(int i =0;i<m->rowCount();i++){
          total += model->record(i).value(2).toFloat();
    }
    extraBottom<<"Total : "+QString ::number(total,'f',2);

    if(m->rowCount()>0){
                printer-> printReportGeneralwithModel(m
                                                      ,widthList,"Receipts Report",fromDate,toDate,AlignmentList
                                                      ,extraTop,extraBottom,avlCols);

    }
}
