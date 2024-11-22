#include "groupwiseoutstanding.h"
#include "ui_groupwiseoutstanding.h"

GroupwiseOutstanding::GroupwiseOutstanding(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupwiseOutstanding)
{

    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ui->fromDateLabel->hide();
    ui->fromDateDateEdit->hide();

    accHelper = new AccountGroupMasterDatabaseHelper();
    groupsModel = accHelper->getAllAccountForSearchModel();

    ui->tableView->setItemDelegate(new ReportCommonDelegate());

    GMLedgerSearchLite* groupLineLite = new GMLedgerSearchLite(groupsModel);
    groupLineLite->setAccountsFlag(true);
    QWidget* wid = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0,0,0,0);
    wid->setContentsMargins(0,0,0,0);
    ui->groupLay->setContentsMargins(0,0,0,0);
    lay->addWidget(groupLineLite);
    wid->setLayout(lay);
    wid->setMinimumWidth(250);
    ui->groupLay->addWidget(wid);

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);

    //    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&GroupwiseOutstanding::showShareOptions);
    ui->horizontalLayout->addWidget(shareButton);

    QObject::connect(groupLineLite,SIGNAL(SelectedAccount(AccountGroupDataModel)),this,SLOT(setAccountDetails(AccountGroupDataModel)));

    setTableView();
}

GroupwiseOutstanding::~GroupwiseOutstanding()
{
    delete ui;
}

void GroupwiseOutstanding::on_pushButton_clicked()
{
    setTableView();
}

void GroupwiseOutstanding::setTableView()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();

    model = ledHelper->getOutstandingBalanceByGroup(group.groupID, fromDate, toDate); //ledgers listed
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    float total = 0;
    for(int i=0; i<model->rowCount(); i++){
        total += (model->record(i).value(1).toFloat() - model->record(i).value(2).toFloat());
    }

    if(total > 0)
        ui->totalLineEdit->setText(QString::number(total, 'f', 2) + " Dr ");
    else
        ui->totalLineEdit->setText(QString::number(-1 *total, 'f', 2) + " Cr ");
}

void GroupwiseOutstanding::setAccountDetails(AccountGroupDataModel obj)
{
    group = obj;
    qDebug()<<group.groupID;
    setTableView();
}

void GroupwiseOutstanding::showShareOptions()
{
    ShareTools *shareOptionsWidget = new ShareTools("Ledger Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&GroupwiseOutstanding::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&GroupwiseOutstanding::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&GroupwiseOutstanding::on_excelButton_clicked);

}

void GroupwiseOutstanding::printReport(QPrinter *printer)
{
    qDebug()<<"printing report";
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<20<<15<<15;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols;
    avlCols <<0<<1<<2;

    //    printer-> printReportGeneral(tableView,widthList,"Ledger Report",fromDate,toDate,AlignmentList
    //                                 ,extraTop,extraBottom);

    CounterPrinterHelper *printerHelper= new CounterPrinterHelper();
    printer->setPrinterName("printerName");
    printerHelper->setPrinter(printer);
    printerHelper->printReportGeneral(ui->tableView
                                      ,widthList,"Groupwise Outstanding",fromDate,toDate,AlignmentList
                                      ,extraTop,extraBottom,avlCols
                                     );

}

void GroupwiseOutstanding::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Groupwise Outstanding Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+group.groupName+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20";
    //    reportName = "Time Based Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}
