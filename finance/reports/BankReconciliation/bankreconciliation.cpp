#include "bankreconciliation.h"
#include "ui_bankreconciliation.h"

BankReconciliation::BankReconciliation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BankReconciliation)
{
    ui->setupUi(this);


    this->setWindowTitle(reportName);
    this->setObjectName("report");
    this->setStyleSheet("QWidget#report{background-color:" + bgcolor + "}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    initDbHelpers();
    initGlobalObjects();
    setWidgets();
    setTableView();
}

BankReconciliation::~BankReconciliation()
{
    delete ui;
}

void BankReconciliation::initDbHelpers()
{
    chqHelper = new ChequeDetailsDatabaseHelper;
    dbHelper= new LedgerHelper;
    ledHelper = new LedgerMasterDatabaseHelper;
}

void BankReconciliation::initGlobalObjects()
{
    QString dateEditSS = "GMDateEdit{background-color:" + bgcolor + ";border: none; color: " + textColor + "} "
                 "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
                 "QDateEdit::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }";


    tableView = new ReportsTableView;

//    sort = new GMQuerySortFilterProxyModel();
//    sort->setCheckCol(8);

    fromDateEdit = new GMDateEdit(&fromDate);
    fromDateEdit->setFixedWidth(80);
    fromDateEdit->setFrame(false);
    fromDateEdit->setStyleSheet(dateEditSS);
    fromDateEdit->setFont(normalFont);
    QLabel *fromDateLabel = new QLabel("From");
    fromDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *fromDateLay = new QVBoxLayout;
    fromDateLay->addWidget(fromDateLabel);
    fromDateLay->addWidget(fromDateEdit);

    fromDateWidget = new QWidget;
    fromDateWidget->setLayout(fromDateLay);
//    fromDateWidget->hide();

    toDateEdit = new GMDateEdit(&toDate);
    toDateEdit->setFixedWidth(80);
    toDateEdit->setFrame(false);
    toDateEdit->setStyleSheet(dateEditSS);
    toDateEdit->setFont(normalFont);


    QLabel *toDateLabel = new QLabel("To");
    toDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *toDateLay = new QVBoxLayout;
    toDateLay->addWidget(toDateLabel);
    toDateLay->addWidget(toDateEdit);

    toDateWidget = new QWidget;
    toDateWidget->setLayout(toDateLay);
//    toDateWidget->hide();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(iconColor));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked, this, &BankReconciliation::showShareOptions);

    okButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "refresh"));
    okButton->setParent(this);
    okButton->setColor(QColor(iconColor));
    connect(okButton, &QPushButton::clicked, this, &BankReconciliation::setTableView);

}

void BankReconciliation::setWidgets()
{
    windowLabel = new QLabel(reportName);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(appBarColor));

    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(titleColor));
    palette.setColor(windowLabel->backgroundRole(), QColor(appBarColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(titleFont);

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setFixedHeight(50);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(shareButton);


    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    QStringList grpList ;
    grpList<<"0x5x21";

    ledSearch= new GMLedgerSearchLite(ledHelper->getLedgerQueryString(grpList));
    ledSearch->setParent(this);
    ledSearch->setPlaceholderText("Search Ledger...");
    ledSearch->setStyleSheet("GMLedgerSearchLite {color : " + textColor + "; background-color : " + bgcolor + ";}");
    ledSearch->setFixedWidth(200);
    ledSearch->setFont(boldFont);
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    QLabel *ledgerLabel = new QLabel("Ledger");
    ledgerLabel->setStyleSheet("QLabel{color:" + textColor + "}");
    lay->addWidget(ledgerLabel);
    ledgerLabel->hide();
    lay->addWidget(ledSearch);
    SearchWid->setLayout(lay);
    SearchWid->setStyleSheet("GMLedgerSearchLite {color : " + textColor + "}");
    SearchWid->setWhatsThis("Type Ledger");
    SearchWid->setFixedWidth(200);

//    connect(ledSearch,&GMLedgerSearchLite::selectedLedger,
//            this,[=](LedgerMasterDataModel led){
//        ledgerId = led.LedgerID;
//        setTableView();
//    });



    QLabel *label0 = new QLabel("Show Reconciled");
    showReconciled = new QCheckBox;
    showReconciled->setParent(this);
    showReconciled->setTristate(true);
    connect(showReconciled, &QCheckBox::clicked, this, [=](){
//       sort->setCheckState(showReconciled->checkState()) ;
       setTableView();
    });

    filterLineEdit = new QLineEdit;
    filterLineEdit->setFrame(false);
    filterLineEdit->setPlaceholderText("Search.. ");
//    connect(filterLineEdit, &QLineEdit::textChanged, this, [=](){
//        sort->setStr(filterLineEdit->text());
//        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<filterLineEdit->text();
//        QRegExp regExp( filterLineEdit->text(),Qt::CaseInsensitive );

//        sort->setFilterRegExp(regExp);
//        tableView->setColumns();
//    });

    QHBoxLayout *fLayout = new QHBoxLayout;
    fLayout->addWidget(fromDateWidget);
    fLayout->addWidget(toDateWidget);
    fLayout->addWidget(SearchWid);
    fLayout->addWidget(okButton);
    fLayout->addStretch(1);
    fLayout->addWidget(label0);
    fLayout->addWidget(showReconciled);
    fLayout->addWidget(filterLineEdit);



    QVBoxLayout *tableLayout = new QVBoxLayout;
    tableLayout->addWidget(tableView);

    mainLayout->addWidget(appBar);
    mainLayout->addLayout(fLayout);
    mainLayout->addLayout(tableLayout);

    Widget->setStyleSheet("QWidget{background-color:" + bgcolor + "}");

    ui->verticalLayout->addWidget(Widget);


}

void BankReconciliation::setTableView()
{
//    model = dbHelper->getReconciliationReport( toDate);
//    model = (GMSQLQUeryModel*) dbHelper->getReconciliationModel(ledgerId,toDate, "%" );
    QMap<int,Qt::CheckState> map;
//    qModel = dbHelper->getReconciliationModel(ledgerId,fromDate,toDate, "%" );

    for(int i=0; i<qModel->rowCount(); i++){
        if( qModel->record(i).value(8).toBool())
            map.insert(i,Qt::Checked);
        else
            map.insert(i, Qt::Unchecked);

    }

    qDebug()<<Q_FUNC_INFO<<map;
//    model = new GMSQLQUeryModel(map,8);
//    connect(model, &GMSQLQUeryModel::checked, this, &BankReconciliation::updateStatus);
//    model->setQuery(qModel->query());

//    sort->setSourceModel(model);
//    tableView->setModel(sort);
    tableView->setModelQry(model);

    tableView->setColumns();
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableView->hideColumn(0);
    tableView->hideColumn(6);
    tableView->lastRow->hide();
}

void BankReconciliation::closeWindow()
{
    this->close();
}

void BankReconciliation::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&BankReconciliation::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&BankReconciliation::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&BankReconciliation::exportToExcel);
}

void BankReconciliation::printReport(QPrinter *printer)
{

}

void BankReconciliation::exportToExcel()
{

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

//    ex->GenerateReportFromTableView(fileName,reportName,
//                                   widthList,tableView,
//                                   fromDate.toString("yyyy-MM-dd"),
//                                   toDate.toString("yyyy-MM-dd"),
    //                                   avlCols);
}

void BankReconciliation::updateStatus(int row, bool Stat)
{
    QString vNo = qModel->record(row).value(2).toString();
    QString vType = qModel->record(row).value(3).toString();
    QString vPrefix = qModel->record(row).value(6).toString();
    QDate vDate;
    if(Stat)
         vDate = qModel->record(row).value(1).toDate();

    TransactionHelper *transHelper = new TransactionHelper;
//    transHelper->updateTransactionReconciled(vNo, vType, vPrefix, Stat, vDate);

    setTableView();
}
