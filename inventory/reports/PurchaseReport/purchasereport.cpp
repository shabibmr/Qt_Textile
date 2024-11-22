#include "purchasereport.h"
#include "ui_purchasereport.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
PurchaseReport::PurchaseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseReport)
{
    ui->setupUi(this);


    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));

    QLabel *windowLabel = new QLabel("Purchase Report");
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addStretch(1);

    itemWise = new QtMaterialIconButton(QtMaterialTheme::icon("action", "list"));
    itemWise->setColor(QColor(255,250,0));
    itemWise->setIconSize(QSize(30,30));
    itemWise->show();

    grouped = new QtMaterialIconButton(QtMaterialTheme::icon("social", "group"));
    grouped->setColor(QColor(255,250,0));
    grouped->setIconSize(QSize(30,30));
    grouped->show();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&PurchaseReport::showShareOptions);

//    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();

    selectedGroups<<"0";

    appBar->appBarLayout()->addWidget(itemWise);
    appBar->appBarLayout()->addWidget(grouped);
    appBar->appBarLayout()->addWidget(shareButton);

    QObject::connect(backButton,&QPushButton::clicked,
                     this,&PurchaseReport::backButtonPressed);


    QObject::connect(itemWise,&QPushButton::clicked,
                     this,&PurchaseReport::setItemwiseReport);

    QObject::connect(grouped,&QPushButton::clicked,
                     this,&PurchaseReport::setGroupedReport);

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

    ui->appBarLayout->addWidget(Widget);

    salesHelper = new PurchaseVoucherDatabaseHelper();
    invGrpHelper = new SalesInventoryGroupsDatabaseHelper();
    dbHelper = new InventoryHelper;
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



    //    setTableHeading();
    setTable();
}

PurchaseReport::~PurchaseReport()
{
    delete ui;
}

void PurchaseReport::setTableHeading()
{

}

void PurchaseReport::setTable()
{

    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    if(LoginValues::Privilege<4){
        qDebug()<<"prevl= "<<LoginValues::Privilege;
        if(dfrom < toDateTime){
            qDebug()<<"Changing date to :"<<dfrom;
            toDateTime = dfrom;
        }
    }

    if(groupID.length() == 0)
        groupID ="0";

    if(selectedGroups.last() != groupID)
        selectedGroups<<groupID;

    if(reportType == "Group"){
        if(datewiseFlag){
            model = dbHelper->getDatewisePurchaseByGroup(groupID,fromDateTime,toDateTime);
        }
        else{
            model = dbHelper->getGroupWisePurchaseDetailsByGroup(groupID,fromDateTime,toDateTime);
        }
    }
    else{
        if(datewiseFlag){
            model = dbHelper->getDatewiseDetailedPurchaseByGroup(groupID,fromDateTime,toDateTime);
        }
        else{
            model = dbHelper->getDetailedPurchaseByGroup(groupID,fromDateTime,toDateTime);
        }
    }

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    sort->sort(2);

    qDebug()<<"sort is set";
    ui->tableView->setModel(sort);
    qDebug()<<"model is set";

    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);


    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

}

void PurchaseReport::resetID(){
    groupID = "";
    setTable();
}

void PurchaseReport::on_OKButton_clicked()
{
    setTable();
}

void PurchaseReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Groupwise Purchase Report";



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

void PurchaseReport::on_printButton_clicked()
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

void PurchaseReport::on_backButton_clicked()
{
    this->close();
}

void PurchaseReport::on_tableWidget_clicked(const QModelIndex &index)
{

}

void PurchaseReport::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}

void PurchaseReport::on_datewiseButton_clicked()
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

void PurchaseReport::on_tableView_doubleClicked(const QModelIndex &index)
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


void PurchaseReport::setGroupedReport()
{
    reportType ="Group";
    setTable();
}

void PurchaseReport::setItemwiseReport()
{
    reportType ="Item";
    setTable();
}

void PurchaseReport::backButtonPressed()
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

void PurchaseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&PurchaseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&PurchaseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&PurchaseReport::on_excelButton_clicked);
}

void PurchaseReport::printReport(QPrinter *printer)
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
                                 ,widthList,"Purchase Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);

}


