#include "restaurantclosingstock.h"
#include "ui_restaurantclosingstock.h"
#include <QMessageBox>

#include "itemtrasactionlist.h"

RestaurantClosingStock::RestaurantClosingStock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantClosingStock)
{
    ui->setupUi(this);


    appBar = new QtMaterialAppBar(this);
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));


//    printButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "print"));
//    printButton->setParent(appBar);
//    printButton->setColor(QColor(115,45,187));
//    printButton->setIconSize(QSize(40,40));

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&RestaurantClosingStock::showShareOptions);

    QLabel *windowLabel = new QLabel("Closing Stock");
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

    appBar->appBarLayout()->addWidget(itemWise);
    appBar->appBarLayout()->addWidget(grouped);
    appBar->appBarLayout()->addWidget(shareButton);

    QObject::connect(backButton,&QPushButton::clicked,
                     this,&RestaurantClosingStock::backButtonPressed);
//    QObject::connect(printButton,&QPushButton::clicked,
//                     this,&RestaurantClosingStock::askForPrint);


    QObject::connect(itemWise,&QPushButton::clicked,
                     this,&RestaurantClosingStock::setItemwiseReport);

    QObject::connect(grouped,&QPushButton::clicked,
                     this,&RestaurantClosingStock::setGroupedReport);

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget(this);

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

    ui->AppbarLayout->addWidget(Widget);


    dbHelper = new ClosingStockHelper();
    invGrpHelper = new SalesInventoryGroupsDatabaseHelper();
    invHelper = new InventoryHelper();
    sort = new GMItemSortFilterProxyModel();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

    grpModel = invGrpHelper->getSalesInventoryGroupDataAsModel();

    itemLineLite = new GMItemSearchLite(grpModel);
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

    selectedGroups<<"0";

    godownHelper = new GodownDatabaseHelper;
    QSqlQueryModel* godownsModel = godownHelper->getGodownForSearchModel();
    QPointer<GMGodownSearchLite> godownSearch = new GMGodownSearchLite(godownsModel);

    QWidget *SearchWid2 = new QWidget(this);
    QVBoxLayout *lay2 = new QVBoxLayout(SearchWid2);
    lay2->setContentsMargins(0,0,0,0);
    lay2->setSpacing(0);
    lay2->addWidget(godownSearch);
    SearchWid2->setLayout(lay2);
    //    ui->itemTableWidget->setCellWidget(i,1,SearchWid);
    ui->godownLayout->addWidget(SearchWid2);
    QObject::connect(godownSearch,SIGNAL(SelectedGodown(GodownDataObject)),
                     this,SLOT(getGodownDetails(GodownDataObject)));

//        tableView->setItemDelegate(new ClosingStockRetDelegate());

    ui->show0CheckBox->setChecked(false);
    ui->showTransCheckBox->setEnabled(true);
    ui->ShowGodownTransferCheckBox->setChecked(false);

    showVariance = ConfigurationSettingsDatabaseHelper::getValue(showVarianceStock, false).toBool();

    tableView = new ReportsTableView("");
    tableView->setItemDelegate(new ClosingStockRetDelegate(showVariance));
    ui->tableLayout->addWidget(tableView);
    connect(tableView, &ReportsTableView::doubleClicked, this, &RestaurantClosingStock::on_tableView_doubleClicked);

    setTable();

}

RestaurantClosingStock::~RestaurantClosingStock()
{
    delete invHelper;
    delete ui;
}

void RestaurantClosingStock::on_OKButton_clicked()
{
    setTable();
}

void RestaurantClosingStock::setTable()
{
    qDebug()<<"GOdown : "<<godownId;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date();//.addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    if(groupID.length() == 0){
        groupID ="0";
    }



    if(reportType == "Group"){
        qDebug()<<"show godown transfer"<<ui->ShowGodownTransferCheckBox->isChecked();
        model = dbHelper->getGroupwiseClosingStockByGodownAndLPP(fromDate, toDate, godownId, groupID,
                                                                 ui->show0CheckBox->isChecked(),
                                                                 LoginValues::company.useBomForStockCalc,
                                                                 ui->showTransCheckBox->isChecked(),
                                                                 ui->ShowGodownTransferCheckBox->isChecked(),
                                                                 showVariance);
    }
//        model = dbHelper->getGroupwiseClosingStockByGodownAndFifoPrice(fromDate, toDate, godownId, groupID, ui->show0CheckBox->isChecked());
    else
        model = dbHelper->getItemwiseClosingStockOfGrpByGodownAndLPP(fromDate, toDate, godownId, groupID,
                                                                     ui->show0CheckBox->isChecked(),
                                                                     ui->showTransCheckBox->isChecked(),
                                                                     ui->ShowGodownTransferCheckBox->isChecked(),
                                                                     LoginValues::company.useBomForStockCalc,
                                                                     showVariance);
//        model = dbHelper->getItemwiseClosingStockOfGrpByGodownAndFifoPrice(fromDate, toDate, godownId, groupID, ui->show0CheckBox->isChecked());

    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);
    QList<int> avlCols;
    for(int i=3; i<model->columnCount(); i++)
        avlCols.append(i);


    tableView->setAvlcols(avlCols);
    tableView->setModel(sort);
    tableView->setSortingEnabled(true);
    tableView->setModelQry(model);
    tableView->setColumns();


    if(selectedGroups.size()>0){
        if(selectedGroups[selectedGroups.size()-1] != groupID)
            selectedGroups<<groupID;
    }
    else{
        selectedGroups<<groupID;
    }
    //    model = dbHelper->getClosingStockListModelWithLastPurchasePrice(fromDate, toDate);

    tableView->hideColumn(0);
    tableView->hideColumn(1);
    tableView->hideColumn(9);

    tableView->setSortingEnabled(true);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}

void RestaurantClosingStock::searchList()
{
    QString text=ui->searchLine->text();
    QRegExp regExp(text,Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);
}

void RestaurantClosingStock::on_lineEdit_textChanged(const QString &arg1)
{
    searchList();
}

void RestaurantClosingStock::on_searchLine_textChanged(const QString &arg1)
{
    searchList();
}


void RestaurantClosingStock::getItemDetails(QString item)
{
    qDebug()<<"Group Setting";
    groupID=item;
    qDebug()<<"Group Set to "<< groupID;
    setTable();
}


void RestaurantClosingStock::resetID(){
    groupID = "";
    setTable();
}

void RestaurantClosingStock::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();

    QString ID = model->record(row).value(1).toString();
    QString rowType = model->record(row).value(0).toString();;

    if(rowType== "G"){
        itemLineLite->setText(invGrpHelper->getGroupNameByID(ID));
        groupID = ID;
    }
    if(rowType== "I"){

        int row =  sort->mapToSource(index).row();

        QString itemID = model->record(row).value(1).toString();
        ItemTrasactionList *itemsWid = new ItemTrasactionList(itemID,
                                                              new SalesInventoryItemDatabaseHelper(),
                                                              invHelper,
                                                              this);
        itemsWid->setDateFilters(ui->fromDateDateEdit->date(),ui->toDateDateEdit->date());
        itemsWid->setGodownFilter(godownId);
        itemsWid->setValue();
        itemsWid->setAttribute(Qt::WA_DeleteOnClose);
        itemsWid->setMinimumSize(500,700);
        itemsWid->show();
    }
//    setTable();
}

void RestaurantClosingStock::setGroupedReport()
{
    reportType ="Group";
    setTable();
}

void RestaurantClosingStock::setItemwiseReport()
{
    reportType ="Item";
    setTable();
}

void RestaurantClosingStock::backButtonPressed()
{
    qDebug()<<"Back Pressed size : "<<selectedGroups;
    if(selectedGroups.size() == 1 ){
//        return;
        this->close();
    }
    selectedGroups.removeAt(selectedGroups.size()-1);

    groupID = selectedGroups[selectedGroups.size()-1];
    qDebug()<<"Group id last = "<<groupID;
    itemLineLite->setText(invGrpHelper->getGroupNameByID(groupID));
    setTable();
}


void RestaurantClosingStock::getGodownDetails(GodownDataObject obj)
{
    qDebug()<<" changing godown ";
    godownId = obj.GodownID;
    if(godownId.length() > 0){
        ui->ShowGodownTransferCheckBox->setChecked(true);
        ui->ShowGodownTransferCheckBox->setEnabled(false);
    }
    else{
        ui->ShowGodownTransferCheckBox->setEnabled(true);
    }
    setTable();
//    ui->ShowGodownTransferCheckBox->setEnabled(true);
}

void RestaurantClosingStock::on_show0RadioButton_clicked(bool checked)
{
    show0stock=checked;
    setTable();
}

void RestaurantClosingStock::on_show0CheckBox_clicked(bool checked)
{
    qDebug()<<"Show 0"<<ui->show0CheckBox->isChecked();
//    setTable();
}

void RestaurantClosingStock::on_show0CheckBox_stateChanged(int arg1)
{
    qDebug()<<"Show 0"<<ui->show0CheckBox->isChecked();
    if(arg1 == 2){
        ui->showTransCheckBox->setEnabled(false);
        ui->showTransCheckBox->setChecked(true);
    }
    else {
        ui->showTransCheckBox->setEnabled(true);
        ui->showTransCheckBox->setChecked(false);
    }
//    setTable();
}

void RestaurantClosingStock::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printReport(new QPrinter);
    }
}

void RestaurantClosingStock::printReport(QPrinter *printer)
{
    CounterPrinterHelper *printerHelper = new CounterPrinterHelper;
    printer->setPrinterName("printerName");
    printerHelper->setPrinter(printer);

    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


   // widthList <<20<<20<<20<<20<<20<<20;
    QStringList cols;
    cols<<"30"<<"30"<<"30"<<"30"<<"30"<<"30";
    AlignmentList<< 0<< 1<<1<<1<<1<<1 ;
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6<<7;

    printerHelper-> printReportGeneral(tableView
                                       ,widthList,"Stock Report",fromDate,toDate,AlignmentList
                                       ,extraTop,extraBottom,avlCols);
}

void RestaurantClosingStock::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Stock Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
//    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&RestaurantClosingStock::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&RestaurantClosingStock::printReport);
         QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                          this,&RestaurantClosingStock::exportToExcel);

}

void RestaurantClosingStock::exportToExcel()
{

    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = " Stock Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20"<<"20";
    //    reportName = "Time Based Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6<<7<<8;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);


}
