#include "poitemwisereport.h"
#include "ui_poitemwisereport.h"



POItemwiseReport::POItemwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::POItemwiseReport)
{
    ui->setupUi(this);
    this->setWindowTitle("Purchase Itemwise");
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    ui->fromDate->setDate(QDate::currentDate());
    ui->toDate->setDate(QDate::currentDate());
    inHelper = new PurchaseOrderDatabaseHelper();
    ui->tableView->setSortingEnabled(true);

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    //    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&POItemwiseReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    //    ui->excelButton->hide();
    ui->printButton->hide();
    setTableView();

}

POItemwiseReport::~POItemwiseReport()
{
    delete ui;
}

void POItemwiseReport::setTableView()
{
    QDate fromDate = ui->fromDate->date();
    QDate toDate = ui->toDate->date();
    model = inHelper->getItemwisePendingOrders(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    float total = 0;
    for(int i=0;i<model->rowCount();i++){
        total += model->record(i).value(2).toFloat();
    }

    ui->totalLine->setText(QString::number(total,'f',2));

}

void POItemwiseReport::on_okButton_clicked()
{
    setTableView();
}



void POItemwiseReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDate->date();
    QDate toDate = ui->toDate->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2;

    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Item Purchase Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void POItemwiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Itemwise Purchase Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&POItemwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&POItemwiseReport::printReport);
    //    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
    //                     this,&POItemwiseReport::on_excelButton_clicked);
}

void POItemwiseReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDate->date();
    QDate toDate = ui->toDate->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<0<<1<<2;

    printerHelper-> printReportGeneral(ui->tableView
                                       ,widthList,"Item Purchase Report",fromDate,toDate,AlignmentList
                                       ,extraTop,extraBottom,avlCols);
}


void POItemwiseReport::on_searchLine_textChanged(const QString &arg1)
{
    QRegExp regExp(ui->searchLine->text(),Qt::CaseInsensitive);
     sort->setFilterRegExp(regExp);
}
