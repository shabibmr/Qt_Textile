#include "maintoolbar.h"
#include "ui_maintoolbar.h"
#include "login/loginvalues.h"
#include <QToolButton>
#include <QBitmap>

MainToolBar::MainToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainToolBar)
{
    ui->setupUi(this);

//    QLinearGradient buttonGradient(0,0,0,ui->listWidget->height());
//    buttonGradient.setColorAt(0,QColor(50,70,50));
//    buttonGradient.setColorAt(1,QColor(80,100,80));

//    QPalette palette = qApp->palette();
//    palette.setBrush( QPalette::Base, buttonGradient );
//    palette.setBrush( QPalette::Highlight, buttonGradient );
//    palette.setBrush( QPalette::HighlightedText, Qt::white );
//    palette.setBrush( QPalette::Text, Qt::white );

//    ui->listWidget->setPalette(palette);

    QListWidgetItem *dashboard = new QListWidgetItem(ui->listWidget);
    dashboard->setText("Dashboard");
    dashboard->setIcon(QIcon(":/images/dashboard.png") );


    QListWidgetItem *Finance = new QListWidgetItem(ui->listWidget);
    Finance->setText("Finance");
    Finance->setIcon( QIcon(":/images/finance.png") );
//    Finance->setHidden(!LoginValues::model.Finances);

    QListWidgetItem *Inventory = new QListWidgetItem(ui->listWidget);
    Inventory->setText("Inventory");
    Inventory->setIcon( QIcon(":/images/inventory.png") );
//    Inventory->setHidden(!LoginValues::model.Inventory);

    QListWidgetItem *Purchases = new QListWidgetItem(ui->listWidget);
    Purchases->setText("Purchases");
    Purchases->setIcon( QIcon(":/images/purchases.png") );
//    Purchases->setHidden(!LoginValues::model.Purchases);

    QListWidgetItem *Services = new QListWidgetItem(ui->listWidget);
    Services->setText("Services");
    Services->setIcon( QIcon(":/images/services.png") );
    Services->setHidden(!LoginValues::model.Services);
    Services->setHidden(true);

    QListWidgetItem *HR = new QListWidgetItem(ui->listWidget);
    HR->setText("HR");
    HR->setIcon( QIcon(":/images/hr.png") );
//    HR->setHidden(!LoginValues::model.HR);

    QListWidgetItem *HelpDesk = new QListWidgetItem(ui->listWidget);
    HelpDesk->setText("HelpDesk");
    HelpDesk->setIcon( QIcon(":/images/helpdesk.png") );
    HelpDesk->setHidden(!LoginValues::model.HelpDesk);
    HelpDesk->setHidden(true);

    QListWidgetItem *Warehouse = new QListWidgetItem(ui->listWidget);
    Warehouse->setText("Warehouse");
    Warehouse->setIcon( QIcon(":/images/warehouse.png") );
//    Warehouse->setHidden(!LoginValues::model.WareHouse);

    QListWidgetItem *POS = new QListWidgetItem(ui->listWidget);
    POS->setText("POS");
    POS->setIcon( QIcon(":/images/POS.png") );


    QListWidgetItem *Projects = new QListWidgetItem(ui->listWidget);
    Projects->setText("Projects");
    Projects->setIcon( QIcon(":/images/projects.png") );
    qDebug()<<"Should i show Projects sethidden = "<<LoginValues::model.Projects;
//    Projects->setHidden(!LoginValues::model.Projects);
    Projects->setHidden(true);

    Projects->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);

    ui->listWidget->setIconSize(QSize(25,25));


    QObject::connect(ui->listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(itemClicked(QListWidgetItem*)));

}

MainToolBar::~MainToolBar()
{
    delete ui;
}

void MainToolBar::itemClicked(QListWidgetItem *item)
{
    emit mainItemClicked(item);



}
