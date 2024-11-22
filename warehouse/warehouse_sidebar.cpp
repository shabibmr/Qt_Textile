#include "warehouse_sidebar.h"
#include "ui_warehouse_sidebar.h"

Warehouse_SideBar::Warehouse_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Warehouse_SideBar)
{
    ui->setupUi(this);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");
    ui->verticalLayout_3->setAlignment(Qt::AlignTop);

    sectionReports = new Section("Reports", 300,new QIcon(":/icons/gm.ico"), this);
    ui->verticalLayout_3->addWidget(sectionReports);

    PhysicalStockEntry = new QToolButton(this);
    DeliveryNotes = new QToolButton(this);
    PurchaseOrders = new QToolButton(this);
    GodownTransfers = new QToolButton(this);
    Stock = new QToolButton(this);
    Registers = new QToolButton(this);

    ui->verticalLayout_3->addWidget(PhysicalStockEntry);
    ui->verticalLayout_3->addWidget(DeliveryNotes);
    ui->verticalLayout_3->addWidget(PurchaseOrders);
    ui->verticalLayout_3->addWidget(GodownTransfers);
    ui->verticalLayout_3->addWidget(Stock);
    ui->verticalLayout_3->addWidget(Registers);
    ui->verticalLayout_3->addWidget(sectionReports);


    PhysicalStockEntry->setText("Physical Stock Entry");
    DeliveryNotes->setText("Delivery Notes");
    PurchaseOrders->setText("Purchase Orders");
    GodownTransfers->setText("Godown Transfers");
    Stock->setText("Stock");
    Registers->setText("Registers");

    PhysicalStockEntry->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    DeliveryNotes->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PurchaseOrders->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    GodownTransfers->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Stock->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Registers->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QFont font;
    font.setPointSize(10);

    PhysicalStockEntry->setFont(font);
    DeliveryNotes ->setFont(font);
    PurchaseOrders ->setFont(font);
    GodownTransfers ->setFont(font);
    Stock ->setFont(font);
    Registers ->setFont(font);

    auto* reportsLayout = new QVBoxLayout();
    reportsLayout ->addWidget(Stock);
    reportsLayout ->addWidget(Registers);
    sectionReports->setContentLayout(*reportsLayout);

    QObject::connect(PhysicalStockEntry,SIGNAL(clicked(bool)),this,SLOT(PhysicalStockEntryClicked()));
    QObject::connect(DeliveryNotes,SIGNAL(clicked(bool)),this,SLOT(DeliveryNotesClicked()));
    QObject::connect(PurchaseOrders,SIGNAL(clicked(bool)),this,SLOT(PurchaseOrdersClicked()));
    QObject::connect(GodownTransfers,SIGNAL(clicked(bool)),this,SLOT(GodownTransfersClicked()));
    QObject::connect(Stock,SIGNAL(clicked(bool)),this,SLOT(StockClicked()));
    QObject::connect(Registers,SIGNAL(clicked(bool)),this,SLOT(RegistersClicked()));

    resetAllColors();

    dash= new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);
}

Warehouse_SideBar::~Warehouse_SideBar()
{
    delete ui;
}

void Warehouse_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
         qDebug()<<"clearing Inventory Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}


void Warehouse_SideBar::resetAllColors()
{
    PhysicalStockEntry->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    DeliveryNotes ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PurchaseOrders ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    GodownTransfers ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Stock ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Registers ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");

    clearLayout();
}

void Warehouse_SideBar::PhysicalStockEntryClicked()
{
    resetAllColors();
    PhysicalStockEntry->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockEntryWidget = new StockEntry(this);
    ui->widgetLayout->addWidget(StockEntryWidget);
    StockEntryWidget->setFocus();
}

void Warehouse_SideBar::DeliveryNotesClicked()
{
    resetAllColors();
    DeliveryNotes->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    deliveryNotesWidget = new deliveryNotes(this);
    ui->widgetLayout->addWidget(deliveryNotesWidget);
    deliveryNotesWidget->setFocus();
}

void Warehouse_SideBar::PurchaseOrdersClicked()
{
    resetAllColors();
    PurchaseOrders->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    purchaseOrdersWidget = new purchaseOrders(this);
    ui->widgetLayout->addWidget(purchaseOrdersWidget);
    purchaseOrdersWidget->setFocus();
}

void Warehouse_SideBar::GodownTransfersClicked()
{
    resetAllColors();
    GodownTransfers->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    GodownTransferWidget = new GodownTransfer(this);
//    GodownTransfers->setWindowFlags(Qt::WA_DeleteOnClose);
    ui->widgetLayout->addWidget(GodownTransferWidget);
    GodownTransferWidget->setFocus();
}

void Warehouse_SideBar::StockClicked()
{
    resetAllColors();
    Stock->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    StockReportWidget = new StockReport(this);
    ui->widgetLayout->addWidget(StockReportWidget);
    StockReportWidget->setFocus();
}

void Warehouse_SideBar::RegistersClicked()
{
    resetAllColors();
    Registers->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
}
