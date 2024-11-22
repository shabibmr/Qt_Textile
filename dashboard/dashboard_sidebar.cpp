#include "dashboard_sidebar.h"
#include "ui_dashboard_sidebar.h"
#include <QDebug>

#include "inventoryitemdashboard.h"
#include "purchasesmaindash.h"
#include "salesmaindash.h"
#include "customerdashboard.h"
#include "receiptsmaindash.h"
#include "accountsmaindashboard.h"

Dashboard_SideBar::Dashboard_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Dashboard_SideBar)
{
    ui->setupUi(this);

    QString bgcolor = "#e0f7fa";

    accButton = new SideBarButton("Accounts");
    itemsButton = new SideBarButton("Items");
    cusButton = new SideBarButton("Customers");
    salesButton = new SideBarButton("Sales");
    purchaseButton = new SideBarButton("Purchases");
    receiptsButton = new SideBarButton("Receipts");


    ledHelper = new LedgerHelper();

    this->setObjectName("dashboard");
    this->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);


    ui->verticalLayout_3->addWidget(accButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(itemsButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(cusButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(salesButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(purchaseButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addWidget(receiptsButton);
    ui->verticalLayout_3->addWidget(getLine());
    ui->verticalLayout_3->addStretch(1);

    QObject::connect(accButton,SIGNAL(clicked(bool)),this,SLOT(AccClicked()));
    QObject::connect(itemsButton,SIGNAL(clicked(bool)),this,SLOT(InvClicked()));
    QObject::connect(cusButton,SIGNAL(clicked(bool)),this,SLOT(CusClicked()));
    QObject::connect(salesButton,SIGNAL(clicked(bool)),this,SLOT(SalClicked()));
    QObject::connect(purchaseButton,SIGNAL(clicked(bool)),this,SLOT(PurClicked()));
    QObject::connect(receiptsButton,SIGNAL(clicked(bool)),this,SLOT(RecClicked()));

    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");
    resetAllColors();
    dash = new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);
}

Dashboard_SideBar::~Dashboard_SideBar()
{
    delete ui;
}

QFrame *Dashboard_SideBar::getLine()
{
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

void Dashboard_SideBar::resetAllColors()
{
    accButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    itemsButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    cusButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    salesButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    purchaseButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    receiptsButton ->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#"+fontColor+";");
    clearLayout();
}

void Dashboard_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
        qDebug()<<"clearing dashboard Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}

void Dashboard_SideBar::InvClicked()
{
    resetAllColors();

    itemsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    InventoryItemDashboard* widget = new InventoryItemDashboard(this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}

void Dashboard_SideBar::CusClicked()
{
    resetAllColors();
    cusButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    CustomerDashboard* widget = new CustomerDashboard(ledHelper,this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}

void Dashboard_SideBar::SalClicked()
{
    resetAllColors();
    salesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    SalesMainDash* widget = new SalesMainDash(this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}

void Dashboard_SideBar::PurClicked()
{
    resetAllColors();
    purchaseButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PurchasesMainDash* widget = new PurchasesMainDash(this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}

void Dashboard_SideBar::RecClicked()
{
    resetAllColors();
    receiptsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ReceiptsMainDash* widget = new ReceiptsMainDash(ledHelper ,this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}

void Dashboard_SideBar::AccClicked()
{
    resetAllColors();
    accButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    AccountsMainDashboard* widget = new AccountsMainDashboard(this);
    ui->widgetLayout->addWidget(widget);
    widget->setFocus();
}
