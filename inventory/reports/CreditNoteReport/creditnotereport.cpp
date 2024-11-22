#include "creditnotereport.h"
#include "ui_creditnotereport.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
CreditNoteReport::CreditNoteReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreditNoteReport)
{
    ui->setupUi(this);


    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));
    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&CreditNoteReport::showShareOptions);

    QLabel *windowLabel = new QLabel("Debit Note Report");
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);
    ui->excelButton->hide();
    ui->printButton->hide();

    itemWise = new QtMaterialIconButton(QtMaterialTheme::icon("action", "list"));
    itemWise->setColor(QColor(255,250,0));
    itemWise->setIconSize(QSize(30,30));
    itemWise->show();

    grouped = new QtMaterialIconButton(QtMaterialTheme::icon("social", "group"));
    grouped->setColor(QColor(255,250,0));
    grouped->setIconSize(QSize(30,30));
    grouped->show();

    selectedGroups<<"0";

    appBar->appBarLayout()->addWidget(itemWise);
    appBar->appBarLayout()->addWidget(grouped);
    appBar->appBarLayout()->addWidget(shareButton);

    QObject::connect(backButton,&QPushButton::clicked,
                     this,&CreditNoteReport::backButtonPressed);


    QObject::connect(itemWise,&QPushButton::clicked,
                     this,&CreditNoteReport::setItemwiseReport);

    QObject::connect(grouped,&QPushButton::clicked,
                     this,&CreditNoteReport::setGroupedReport);

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);


    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);
    layout->addStretch(1);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    mainLayout->addWidget(canvas);

    ui->AppBarLayout->addWidget(Widget);
    
    invGrpHelper = new SalesInventoryGroupsDatabaseHelper();
    dbHelper = new InventoryHelper;
    ledgerHelper = new LedgerMasterDatabaseHelper;
    sort = new GMItemSortFilterProxyModel();
    ui->intervalLine->hide();
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;

    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    model = invGrpHelper->getSalesInventoryGroupDataAsModel();
    itemLineLite = new GMItemSearchLite(model);
    itemLineLite->setGroupFlag(true);
    itemLineLite->setText("");

    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(itemLineLite);
    SearchWid->setLayout(lay);
    ui->groupLay->addWidget(SearchWid);
    QObject::connect(itemLineLite,SIGNAL(SelectedGroup(QString)),
                     this,SLOT(getItemDetails(QString)));
    QObject::connect(itemLineLite,SIGNAL(clearItem()),
                     this,SLOT(resetID()));


    //    mainLedgerWidget = new GMLedgerLineMaterial(ledgerHelper,ledgerHelper->getLedgerQueryString());
    //    mainLedgerWidget->setLabel("Ledger");
    //    mainLedgerWidget->setParent(this);
    //    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");

    //    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
    //            this,[=](LedgerMasterDataModel led){
    //        ledgerID = led.LedgerID;
    //    });

    //    ui->ledLayout->addWidget(mainLedgerWidget);


    //    setTableHeading();

    ledModel = ledgerHelper->getLedgerQueryModel();
    ledLineLite = new GMLedgerSearchLite(ledgerHelper->getLedgerQueryString());
    //    ledLineLite->setAccflag(false);
    ledLineLite->setText("");

    QWidget *SearchWid1 = new QWidget(this);
    QVBoxLayout *lay1 = new QVBoxLayout(SearchWid1);
    lay1->setContentsMargins(0,0,0,0);
    lay1->setSpacing(0);
    lay1->addWidget(ledLineLite);
    SearchWid1->setLayout(lay1);
    ui->ledLayout->addWidget(SearchWid1);
    connect(ledLineLite,&GMLedgerSearchLite::selectedLedger,
            this,[=](LedgerMasterDataModel led){
        ledgerID = led.LedgerID;
        setTable();
    });

    connect(ledLineLite,&GMLedgerSearchLite::clearItem,
            this,[=](){
        ledgerID = "%";
        setTable();
    });
    //    QObject::connect(itemLineLite,SIGNAL(SelectedGroup(QString)),
    //                     this,SLOT(getItemDetails(QString)));
    //    QObject::connect(itemLineLite,SIGNAL(clearItem()),
    //                     this,SLOT(resetID()));
    setTable();
}

CreditNoteReport::~CreditNoteReport()
{
    delete ui;
}

void CreditNoteReport::setTableHeading()
{

}

void CreditNoteReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    //    if(LoginValues::Privilege<4){
    //        qDebug()<<"prevl= "<<LoginValues::Privilege;
    //        if(dfrom < toDateTime){
    //            qDebug()<<"Changing date to :"<<dfrom;
    //            toDateTime = dfrom;
    //        }
    //    }

    if(groupID.length() == 0)
        groupID ="0";

    if(selectedGroups.last() != groupID)
        selectedGroups<<groupID;

    if(reportType == "Group"){
        if(datewiseFlag){
            qDebug()<<"Datewise group";
            model = dbHelper->getDatewiseTransactionByGroupByVoucherType(voucherType, groupID,fromDateTime,toDateTime, ledgerID);
        }
        else{
            if(ui->checkBox->isChecked())
                model = dbHelper->getGroupwiseTransactionByVoucherTypeAndReason(voucherType, groupID, fromDateTime, toDateTime, ledgerID);
            else
                model = dbHelper->getGroupwiseTransactionByVoucherType(voucherType, groupID,fromDateTime,toDateTime, ledgerID);
        }
    }
    else{
        if(datewiseFlag){
            qDebug()<<"Datewise detailed";
            model = dbHelper->getDatewiseDetailedTransactionByVoucherType(voucherType, groupID,fromDateTime,toDateTime, ledgerID);
        }
        else{
            if(ui->checkBox->isChecked())
                model = dbHelper->getDetailedTransactionByVoucherTypeAndReason(voucherType, groupID, fromDateTime, toDateTime, ledgerID);
            else{
                model = dbHelper->getDetailedTransactionByGroupByVoucherType(voucherType, groupID,fromDateTime,toDateTime, ledgerID);
            }
        }
    }

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    sort->sort(2);

    ui->tableView->setModel(sort);

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);


    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void CreditNoteReport::resetID(){
    groupID = "";
    setTable();
}

void CreditNoteReport::on_OKButton_clicked()
{
    setTable();
}

void CreditNoteReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Groupwise Sales Report";



    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols;
    cols<<"30"<<"20"<<"20";
    reportName = "Groupwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<2<<3<<4;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableView,fromDate,toDate,avlCols);
}

void CreditNoteReport::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<2<<3<<4;
    CounterPrinterHelper *printer = new CounterPrinterHelper;

    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Groupwise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void CreditNoteReport::on_backButton_clicked()
{
    this->close();
}

void CreditNoteReport::on_tableWidget_clicked(const QModelIndex &index)
{

}

void CreditNoteReport::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}

void CreditNoteReport::on_datewiseButton_clicked()
{
    if(datewiseFlag == true){
        datewiseFlag = false;
        ui->datewiseButton->setText("Datewise");
    }
    else{
        datewiseFlag = true;
        ui->datewiseButton->setText("Consolidated");
    }

    setTable();
}

void CreditNoteReport::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString ID = model->record(row).value(1).toString();
    QString rowType = model->record(row).value(0).toString();
    qDebug()<<"new Group : "<<ID;

    if(rowType=="G"){
        groupID = ID;
        itemLineLite->setText(invGrpHelper->getGroupNameByID(ID));
        setTable();
    }
}


void CreditNoteReport::setGroupedReport()
{
    reportType ="Group";
    setTable();
}

void CreditNoteReport::setItemwiseReport()
{
    reportType ="Item";
    setTable();
}

void CreditNoteReport::backButtonPressed()
{
    qDebug()<<"Back Pressed size : "<<selectedGroups;
    if(selectedGroups.length() == 0 )
        return;
    groupID = selectedGroups.last();
    qDebug()<<"Group id last = "<<groupID;
    itemLineLite->setText(invGrpHelper->getGroupNameByID(groupID));
    selectedGroups.removeLast();
    setTable();
}

void CreditNoteReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Credit Note Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&CreditNoteReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&CreditNoteReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&CreditNoteReport::on_excelButton_clicked);
}

void CreditNoteReport::printReport(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20;
    AlignmentList<< 0<< 1 <<1;
    QList<int> avlCols; avlCols<<2<<3<<4;

    printerHelper-> printReportGeneral(ui->tableView
                                 ,widthList,"Credit Note Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}


