#include "purchaseregister.h"
#include "ui_purchaseregister.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

PurchaseRegister::PurchaseRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseRegister)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ledHelper = new LedgerHelper();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&PurchaseRegister::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);

    tableView = new ReportsTableView;
    tableView->setItemDelegate(new ReportCommonDelegate);
    ui->tableLay->addWidget(tableView);

    setTableView();

}

PurchaseRegister::~PurchaseRegister()
{
    delete ui;
}

void PurchaseRegister::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = ledHelper->getLedgerwiseTransactionSummaryByVoucherType(fromDateTime,toDateTime, GMVoucherTypes::PurchaseVoucher);

    tableView->setModel(model);
    tableView->setModelQry(model);
    tableView->setAvlcols(QList<int>{3});
    tableView->setSortingEnabled(true);
    tableView->setColumns();
    tableView->hideColumn(0);
    tableView->hideColumn(1);
    tableView->hideColumn(4);

    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

void PurchaseRegister::on_OKButton_clicked()
{
    setTableView();
}

void PurchaseRegister::on_excelButton_clicked()
{
    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\PendingPurchaseOrders_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<<1<<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ExcelReportHelper *ex = new ExcelReportHelper;

    ex->GenerateReportFromTableView(fileName,"Group Summary",
                                    widthList,tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}

void PurchaseRegister::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    CounterPrinterHelper *printer = new CounterPrinterHelper;
    widthList <<60<<20<<20;
    AlignmentList<< 0<<1<<1;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    printer-> printReportGeneral(tableView
                                 ,widthList,"Purchase Register",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void PurchaseRegister::on_backButton_clicked()
{

}

void PurchaseRegister::showShareOptions()
{

}
