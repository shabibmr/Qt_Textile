#include "groupwisereport.h"
#include "ui_groupwisereport.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
#include "database/finance/Closing/shiftdatabasehelper.h"
#include "customwidgets/SearchWidgets/Godown/gmgodownsearchlite.h"

GroupwiseReport::GroupwiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupwiseReport)
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
                     this,&GroupwiseReport::showShareOptions);

    ui->excelButton->hide();
    ui->printButton->hide();

    QLabel *windowLabel = new QLabel("Sales Report");
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

    selectedGroups<<"0";

    appBar->appBarLayout()->addWidget(itemWise);
    appBar->appBarLayout()->addWidget(grouped);
    appBar->appBarLayout()->addWidget(shareButton);

    //    ui->tableView->hide();


    QObject::connect(backButton,&QPushButton::clicked,
                     this,&GroupwiseReport::backButtonPressed);


    QObject::connect(itemWise,&QPushButton::clicked,
                     this,&GroupwiseReport::setItemwiseReport);

    QObject::connect(grouped,&QPushButton::clicked,
                     this,&GroupwiseReport::setGroupedReport);

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

    //    voucherData->setContentsMargins(20,0,0,0);
    mainLayout->addWidget(canvas);

    ui->AppBarLayout->addWidget(Widget);


    salesHelper = new salesVoucherDatabaseHelper();
    invGrpHelper = new SalesInventoryGroupsDatabaseHelper();
    dbHelper = new InventoryHelper;
    sort = new GMItemSortFilterProxyModel();
    ui->intervalLine->hide();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ShiftDatabaseHelper shift;
    dfrom = shift.getlastShiftDetailByVoucherPrefix("%").ShiftEndDateTime;
    //    dfrom = QDateTime::currentDateTime().addDays(-1);

    model = invGrpHelper->getSalesInventoryGroupDataAsModel();
    groupSearch = new GMItemSearchLite(model);
    groupSearch->setGroupFlag(true);
    groupSearch->setText("");
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(groupSearch);
    SearchWid->setLayout(lay);
    ui->groupLay->addWidget(SearchWid);
    QObject::connect(groupSearch,SIGNAL(SelectedGroup(QString)),
                     this,SLOT(getItemDetails(QString)));
    QObject::connect(groupSearch,SIGNAL(clearItem()),
                     this,SLOT(resetID()));

    GodownDatabaseHelper *godownHelper = new GodownDatabaseHelper;
    QSqlQueryModel* godownsModel = godownHelper->getGodownForSearchModel();
    QPointer<GMGodownSearchLite> godownSearch = new GMGodownSearchLite(godownsModel);

    QWidget *SearchWid2 = new QWidget(this);
    QVBoxLayout *lay2 = new QVBoxLayout(SearchWid2);
    lay2->setContentsMargins(0,0,0,0);
    lay2->setSpacing(0);
    lay2->addWidget(godownSearch);
    SearchWid2->setLayout(lay2);
    //    ui->itemTableWidget->setCellWidget(i,1,SearchWid);
    ui->godownLay->addWidget(SearchWid2);
    QObject::connect(godownSearch,SIGNAL(SelectedGodown(GodownDataObject)),
                     this,SLOT(getGodownDetails(GodownDataObject)));

    QObject::connect(godownSearch,&GMGodownSearchLite::clearItem,this, [=](){
        godownID = "%";
        setTable();
    });


    initWidgets();
    setTable();
}

GroupwiseReport::~GroupwiseReport()
{
    delete ui;
}

void GroupwiseReport::getGodownDetails(GodownDataObject obj)
{
    qDebug()<<" changing godown ";
    godownID = obj.GodownID;
    setTable();
}

void GroupwiseReport::setTableHeading()
{

}

void GroupwiseReport::setTable()
{
    qDebug()<<Q_FUNC_INFO;
    qDebug()<<" godown is "<<godownID;
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
//            ui->toDateDateEdit->setDateTime(toDateTime);
//            QMessageBox box; box.setText("No privilege to change date"); box.exec();
//        }
//    }

    if(groupID.length() == 0)
        groupID ="0";

    if(selectedGroups.last() != groupID)
        selectedGroups<<groupID;
    QList<int> avlCols;
    if(reportType == "Group")
    {
        if(datewiseFlag){
            if(ui->ModCheckBox->isChecked()){
                model = dbHelper->getDatewiseSalesByGroupAndModeOfService(groupID,fromDateTime,toDateTime);
                for(int col=4;col<=model->columnCount();col++)
                    avlCols.append(col);
            }
            else{
                model = dbHelper->getDatewiseSalesByGroup(groupID,fromDateTime,toDateTime);
            }
        }
        else{
            if(ui->ModCheckBox->isChecked()){
                model = dbHelper->getGroupwiseSalesByModeOfService(groupID,fromDateTime,toDateTime);
            }
            else
            {
                model = dbHelper->getGroupWiseSalesDetailsByGroup(groupID,fromDateTime,toDateTime, godownID);
            }
        }
    }
    else{
        if(datewiseFlag){

            model = dbHelper->getDatewiseDetailedSalesByGroup(groupID,fromDateTime,toDateTime);

        }
        else{
            if(ui->ModCheckBox->isChecked()){

                model = dbHelper->getDetailedGroupwiseSalesByModeOfService(groupID,fromDateTime,toDateTime);

            }
            else{
                model = dbHelper->getDetailedSalesByGroup(groupID,fromDateTime,toDateTime, godownID);

            }

        }
    }

    qDebug()<<"line 226";
    sort->setDynamicSortFilter(true);
    qDebug()<<"line 228";
    sort->setSourceModel(model);
    qDebug()<<"line 230";
    sort->sort(2);

    if(avlCols.size() == 0)
        for(int col=3;col<model->columnCount();col++)
            avlCols.append(col);
    qDebug()<<avlCols;

    tableView->setAvlcols(avlCols);
    qDebug()<<"line 239";
    tableView->setModel(model);
    qDebug()<<"line 241";
    tableView->setModelQry(model);
    qDebug()<<"line 243";
    if(selectedGroups.size()>0){
        if(selectedGroups[selectedGroups.size()-1] != groupID)
            selectedGroups<<groupID;
    }
    else{
        selectedGroups<<groupID;
    }

    tableView->setColumns();
    tableView->hideColumn(0);
    tableView->hideColumn(1);

    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    //    tableView->resizeColumnsToContents();

}

void GroupwiseReport::resetID(){
    groupID = "";
    setTable();
}

void GroupwiseReport::on_OKButton_clicked()
{
    setTable();
}

void GroupwiseReport::on_excelButton_clicked()
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
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);
}

void GroupwiseReport::on_printButton_clicked()
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

    printer-> printReportGeneral(tableView
                                 ,widthList,"Groupwise Report",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}


void GroupwiseReport::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";

    groupID=item;

    qDebug()<<"Group Set to "<< groupID;
    setTable();
}

void GroupwiseReport::on_datewiseButton_clicked()
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

void GroupwiseReport::on_tableView_doubleClicked(const QModelIndex &index)
{
    qDebug()<<Q_FUNC_INFO;
    //    int row = sort->mapToSource(index).row();
    int row = index.row();
    QString ID = model->record(row).value(1).toString();
    QString rowType = model->record(row).value(0).toString();
    qDebug()<<"new Group : "<<ID;

    if(rowType=="G"){
        groupID = ID;
        groupSearch->setText(invGrpHelper->getGroupNameByID(ID));
        setTable();
    }
}

void GroupwiseReport::on_ModCheckBox_clicked()
{
    setTable();
}

void GroupwiseReport::initWidgets()
{
    tableView = new ReportsTableView();
    ui->tableLayout->addWidget(tableView);

    tableView->setItemDelegate(new ReportCommonDelegate());
    connect(tableView, &QTableView::doubleClicked, this, &GroupwiseReport::on_tableView_doubleClicked);

}

void GroupwiseReport::calculateTotals()
{

}

void GroupwiseReport::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Groupwise Sales Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&GroupwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&GroupwiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&GroupwiseReport::on_excelButton_clicked);
}

void GroupwiseReport::printReport(QPrinter *printer)
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

    printerHelper-> printReportGeneral(tableView
                                       ,widthList,"Groupwise Report",fromDate,toDate,AlignmentList
                                       ,extraTop,extraBottom,avlCols);
}

void GroupwiseReport::setGroupedReport()
{
    reportType ="Group";
    setTable();

}

void GroupwiseReport::setItemwiseReport()
{
    reportType ="Item";
    setTable();
}

void GroupwiseReport::backButtonPressed()
{
    qDebug()<<"Back Pressed size : "<<selectedGroups;
    if(selectedGroups.length() == 0 )
        return;
    groupID = selectedGroups.last();
    qDebug()<<"Group id last = "<<groupID;
    groupSearch->setText(invGrpHelper->getGroupNameByID(groupID));
    selectedGroups.removeLast();
    setTable();
}

void GroupwiseReport::on_ModCheckBox_stateChanged(int arg1)
{
    setTable();
}

