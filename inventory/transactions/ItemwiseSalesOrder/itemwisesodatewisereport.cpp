#include "itemwisesodatewisereport.h"
#include "ui_itemwisesodatewisereport.h"
#include <QLabel>
//#include <QSpacerItem>
#include <QSplitter>
#include <QHeaderView>
#include <QScrollArea>

#include <QGroupBox>

ItemwiseSODatewiseReport::ItemwiseSODatewiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemwiseSODatewiseReport)
{
    ui->setupUi(this);

    initDBHelpers();
    initGlobalObjects();
    initWidgets();
    setWidgets();
    refreshMainTable();
    //    itemLine->setFocus();
}

ItemwiseSODatewiseReport::~ItemwiseSODatewiseReport()
{
    delete ui;
}

void ItemwiseSODatewiseReport::initDBHelpers()
{
    dbHelper = new ItemwiseSalesOrderDatabaseHelper(
                SalesOrderDatabaseHelper::Sales_Order_Main_TableName,
                SalesOrderDatabaseHelper::Sales_Order_Details_TableName
                );
    itemsHelper = new SalesInventoryItemDatabaseHelper;
}

void ItemwiseSODatewiseReport::initGlobalObjects()
{
    salesDate = QDate::currentDate();
}

void ItemwiseSODatewiseReport::initWidgets()
{
    tableView = new ReportsTableView();

    //    tableView = new QTableView;

    dateEdit = new GMDateEdit(&salesDate);
    dateEdit->setVariable(&salesDate);
    dateEdit->setMinimumWidth(140);
    itemLine = new GMInventoryItemSearch(itemsHelper,itemsHelper->getInventoryItemsQueryModelForSearch());
    itemLine->setMinimumWidth(300);
    itemLine->setMinimumHeight(30);
    //    itemLine->setFont(QFont())

    QObject::connect(itemLine,&GMInventoryItemSearch::itemSelectedwithItem,
                     [=](inventoryItemDataModel i){
        this->item = i;
        qDebug()<<"Item is "<<item.ItemName<<item.quantity;
        refreshData();
    }
    );
}

void ItemwiseSODatewiseReport::setWidgets()
{

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QWidget> tablesWidget = new QWidget(this);

    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout();
    Widget->setLayout(mainLayout);
    QPointer<QHBoxLayout> filterLay = new QHBoxLayout;
    TableLay = new QVBoxLayout;

    tablesWidget->setLayout(TableLay);
    mainLayout->addLayout(filterLay);

    mainLayout->addWidget(tablesWidget);

    QToolButton *button = new QToolButton(this);
    button->setText("GO");
    button->setIcon(QtMaterialTheme::icon("av", "play_arrow"));
    button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    button->setIconSize(QSize(30,30));

    connect(button,&QToolButton::clicked,this,[=](){
        if(item.ItemID.length()>0)
            refreshData();

        itemLine->setFocus();
    });

    QString styls= "QToolButton {";
    styls += "color: grey;";

    //    styls += "border-top: 10px transparent;";
    //    styls += "border-bottom: 10px transparent;";
    //    styls += "border-right: 10px transparent;";
    //    styls += "border-left: 10px transparent;}";

    styls += "border-radius: 15px;";
    styls += "}";

    resetButton = new QToolButton(this);
    resetButton->setText("Reset");
    resetButton->setIcon(QtMaterialTheme::icon("navigation", "refresh"));
    resetButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    resetButton->setIconSize(QSize(30,30));
    resetButton->setStyleSheet(styls);

    connect(resetButton,&QToolButton::clicked,this,[=](){
        refreshMainTable();
    });

    filterLay->addWidget(resetButton);
    filterLay->addWidget(new QLabel("Date"));
    filterLay->addWidget(dateEdit);
    filterLay->setSpacing(7);
    filterLay->addSpacerItem(new QSpacerItem(20,20));
    filterLay->addWidget(new QLabel("Item"));
    filterLay->addWidget(itemLine);
    filterLay->addWidget(button);
    filterLay->addSpacerItem(new QSpacerItem(20,20));
    filterLay->addStretch(1);

    TableLay->addWidget(tableView);
    ui->verticalLayout->addWidget(Widget);

}

void ItemwiseSODatewiseReport::printReportALLItems()
{
    // Print model

    printer.printItemwiseReport(model,salesDate,"Itemwise Sales Report");

}

void ItemwiseSODatewiseReport::printReportItem(int index)
{
    if(itemwiseModels.length()>index){
        printer.printItemwiseReportDetailed(itemwiseModels[index],item,salesDate,itemPurchaseModel->record(index).value(0).toDate(),"Itemwise Sales Report");
    }

    if(itemwisePurchaseModels.length()>index){
        printer.printItemwiseReportDetailed(itemwisePurchaseModels[index],item,salesDate,itemPurchaseModel->record(index).value(0).toDate(),"Purchase Report");
    }
}


void ItemwiseSODatewiseReport::printReportItemsSeperate()
{
    for(int i=0;i<itemwiseModels.length();i++){
        printer.printItemwiseReportDetailed(itemwiseModels[i],item,salesDate,itemPurchaseModel->record(i).value(0).toDate(),"Itemwise Report");
    }

    for(int i=0;i<itemwisePurchaseModels.length();i++){
        printer.printItemwiseReportDetailed(itemwisePurchaseModels[i],item,salesDate,itemPurchaseModel->record(i).value(0).toDate(),"Itemwise Report");
    }

}

void ItemwiseSODatewiseReport::refreshData()
{

    if(item.ItemID.length()>0){
        clearTableLayout();
        itemwiseModels.clear();
        QSplitter *splitter = new QSplitter(this);
        splitter->setOrientation(Qt::Vertical);
        itemPurchaseModel = dbHelper->getItemDistinctDatesSO(item.ItemID,salesDate);
        for(int i=0;i<itemPurchaseModel->rowCount();i++){
            ReportsTableView *tableViewPurch = new ReportsTableView();

            ReportsTableView *tableView2 = new ReportsTableView();
            ReportsTableView *tableView3 = new ReportsTableView();
            QGroupBox *box = new QGroupBox(this);
            tableView2->setParent(this);
            auto Pdate = itemPurchaseModel->record(i).value(0).toDate();
            QSqlQueryModel *model = dbHelper->getItemSOPendingReportByitem(item.ItemID,salesDate,Pdate);
            QSqlQueryModel *Pmodel = dbHelper->getPurchaseQuantitiesBYITEMDATE(item.ItemID,Pdate);
            QSqlQueryModel *model3 = dbHelper->getSalesOrderReportByPO(item.ItemID,salesDate);
            qDebug()<<"table2";
            tableView2->setItemDelegate(new ReportCommonDelegate());
            tableView2->setModel(model);
            tableView2->setModelQry(model);
            tableView2->setAvlcols(QList<int>{4,5,6,7,9});
            tableView2->setColumns();
            tableView2->setRowName("Grand Total",2);
            tableView2->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
            tableView2->hideColumn(0);
            tableView2->hideColumn(1);
            tableView2->hideColumn(2);

            tableViewPurch->setModel(Pmodel);
            tableViewPurch->setModelQry(Pmodel);
//            tableViewPurch->setAvlcols(QList<int>());
//            tableViewPurch->setColumns();
//            tableViewPurch->setRowName("Grand Total",2);
            tableViewPurch->setItemDelegate(new ReportCommonDelegate());
            tableViewPurch->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
            tableViewPurch->hideColumn(0);
            tableViewPurch->hideColumn(1);
            tableViewPurch->hideColumn(2);

            qDebug()<<"model3";
            tableView3->setModel(model3);
            tableView3->setModelQry(model3);
//            tableView3->setAvlcols(QList<int>());
//            tableView3->setColumns();
//            tableView3->setRowName("Grand Total",0);
            tableView3->setItemDelegate(new ReportCommonDelegate());
            tableView3->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);




            float qtyTotal = 0;
            float trayTotal = 0;

            float qtyLTotal = 0;
            float trayLTotal = 0;
            float grandT = 0;

            for(int i=0;i<model->rowCount();i++){
                if(model->record(i).value(2).toInt() == item.defaultUOMID.toInt()){
                    qtyLTotal += model->record(i).value(5).toFloat();
                    trayLTotal +=  model->record(i).value(6).toFloat();
                }
                else{
                    qtyTotal += model->record(i).value(5).toFloat();
                    trayTotal +=  model->record(i).value(6).toFloat();
                }
                if(model->record(i).value(0).toString().length()>0)
                    grandT += model->record(i).value(8).toFloat();
            }

            tableView2->setRowName(QString::number(qtyLTotal,'f',2)+"|"+QString::number(qtyTotal,'f',2),5,Qt::AlignRight|Qt::AlignVCenter);
            tableView2->setRowName(QString::number(trayLTotal,'f',2)+"|"+QString::number(trayTotal,'f',2),6,Qt::AlignRight|Qt::AlignVCenter);
//            tableView2->setRowName(QString::number(grandT,'f',2),8,Qt::AlignRight|Qt::AlignVCenter);

            /*
             *                    Purchase | Sales | Balance
             * Quantity(Tray)
             * Tray Count
             * Quantity(kg/loose)
             * Tray (kg/loose)
             *
             */
            qDebug()<<"206";
            itemwiseModels.append(model);
            itemwisePurchaseModels.append(Pmodel);

            QtMaterialFloatingActionButton *shareButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "print"));
            shareButton->setMini(true);
            shareButton->setBackgroundColor(QColor(backgroundColor));
            shareButton->setCorner(Qt::TopRightCorner);
            shareButton->setParent(box);
            shareButton->setXOffset(5);
            shareButton->setYOffset(5);
            //            tableView->setCornerWidget(shareButton);
            box->setTitle("Purchase Date : "+Pdate.toString("dd-MM-yyyy"));
            QLayout *lay = new QVBoxLayout;
            box->setLayout(lay);
            lay->addWidget(shareButton);
            lay->addWidget(tableView2);
            lay->addWidget(new QLabel("Purchase"));
            lay->addWidget(tableViewPurch);
            lay->addWidget(tableView3);
            lay->setAlignment(shareButton,Qt::AlignLeft);
            splitter->addWidget(box);
            connect(shareButton,&QtMaterialFloatingActionButton::clicked,this,[=](bool){
                printReportItem(i);
            });
        }

        QToolButton *button = new QToolButton(this);
        button->setText("PRINT ALL");
        button->setIcon(QIcon(":/images/printer.ico"));
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setIconSize(QSize(30,30));
        connect(button,&QToolButton::clicked,this,[=](){
            printReportItemsSeperate();
        });

        TableLay->addWidget(button);
        TableLay->setAlignment(button,Qt::AlignRight);
        TableLay->addWidget(splitter);
    }
    resetButton->show();
}

void ItemwiseSODatewiseReport::refreshMainTable()
{
    clearTableLayout();
    tableView = new ReportsTableView();
    itemLine->setText("");
    item = inventoryItemDataModel();


    model = dbHelper->getSOitemsBySDate(salesDate);
    tableView->setItemDelegate(new ReportCommonDelegate());
    tableView->setAvlcols(QList<int>({3,4,5,6}));
    qDebug()<<"1";
    tableView->setModel(model);
    tableView->setModelQry(model);
    qDebug()<<"2";
    tableView->setColumns();
    qDebug()<<"3";
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    qDebug()<<"4";
    tableView->hideColumn(0);
    qDebug()<<"5";
    connect(tableView,&ReportsTableView::doubleClicked,this,[=](const QModelIndex index){
        int row = index.row();
        QString itemID = model->record(row).value(0).toString();

        item = itemsHelper->getInventoryItemByID(itemID);
        itemLine->setText(item.ItemName);
        refreshData();
    });

    QtMaterialFloatingActionButton *shareButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "print"));
    shareButton->setMini(true);
    shareButton->setBackgroundColor(QColor(Qt::transparent));
    shareButton->setCorner(Qt::TopRightCorner);
    shareButton->setParent(tableView);
    shareButton->setXOffset(0);
    shareButton->setYOffset(0);

    connect(shareButton,&QtMaterialFloatingActionButton::clicked,this,[=](bool){
        printReportALLItems();
    });
    TableLay->addWidget(tableView);


}

void ItemwiseSODatewiseReport::clearTableLayout()
{
    if (TableLay!= nullptr )
    {
        QLayoutItem* item;
        while (( item = TableLay->takeAt(0)) != nullptr )
        {
            item->widget()->deleteLater();
            delete item;
        }
    }
}
