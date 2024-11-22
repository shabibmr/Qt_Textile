#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSize>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/gm.ico"));
    setWindowTitle("algoERP");

    dashBoard = new DefaultDashBoard();

    ui->verticalLayout_4->addWidget(dashBoard);

    mtoolbar = new MainToolBar(ui->widget_maintoolbar);
    ui->horizontalLayout_3->addWidget(mtoolbar);
    mtoolbar->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #00897B, stop:1 #009688)");

    //    mtoolbar->setStyleSheet(", y1:0, x2:0.5, y2:0.4, stop:0 rgba(50, 70, 50, 255), stop:1 rgba(80, 100, 80, 255))");

//    if(hrSideBar){
//        qDebug()<<"Exist";

//    }
//    else{
//        qDebug()<<"Downt exist";
//    }


    connect(mtoolbar,SIGNAL(mainItemClicked(QListWidgetItem*)),this,SLOT(loadSideToolbar(QListWidgetItem*)));

    rtoolbar = new Utilities_ToolBar(this);
    ui->horizontalLayout_7->addWidget(rtoolbar);
    rtoolbar->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #00897B, stop:1 #009688)");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::clearLayout(QLayout *layout){

    if (layout) {
           if(layout->count() > 0){
               QLayoutItem *item = layout->takeAt(0);
               QWidget* widget = item->widget();
               if(widget){
                   delete widget;
               }
               delete item;
           }
       }
}

void MainWindow::loadSideToolbar(QListWidgetItem *item)
{



    clearLayout(ui->verticalLayout_4);


    if(item->text()=="Finance"){
        qDebug()<<"adding finance";
//        fSideBar = new Finance_SideBar(this);
//        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),fSideBar,SLOT(shareClicked()));
//        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),fSideBar,SLOT(searchText(QString)));

//        ui->verticalLayout_4->addWidget(fSideBar);
        return;

    }
    if(item->text()=="Dashboard"){
        dSideBar = new Dashboard_SideBar(this);
        ui->verticalLayout_4->addWidget(dSideBar);
        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),dSideBar,SLOT(shareClicked()));
        return;
    }
    if(item->text()=="Inventory"){
//        iSideBar = new Inventory_SideBar(this);
//        sideBarWidget = new QWidget(this);
//        widLay = new QVBoxLayout(sideBarWidget);
//        widLay->addWidget(iSideBar);
//        widLay->setContentsMargins(0,0,0,0);
//        sideBarWidget->setContentsMargins(0,0,0,0);

//        ui->verticalLayout_4->addWidget(sideBarWidget);
//        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),iSideBar,SLOT(shareClicked()));
//        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),iSideBar,SLOT(searchText(QString)));

        return;
    }
    if(item->text()=="Purchases"){
//        puSideBar = new Purchases_SideBar(this);
//        ui->verticalLayout_4->addWidget(puSideBar);
//        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),puSideBar,SLOT(shareClicked()));
//        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),puSideBar,SLOT(searchText(QString)));

        return;
    }
    if(item->text()=="Projects"){
        prSideBar = new Projects_SideBar(this);

        sideBarWidget = new QWidget(this);
        widLay = new QVBoxLayout(sideBarWidget);
        widLay->addWidget(prSideBar);
        widLay->setContentsMargins(0,0,0,0);
        sideBarWidget->setContentsMargins(0,0,0,0);

        ui->verticalLayout_4->addWidget(sideBarWidget);
        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),prSideBar,SLOT(shareClicked()));
        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),prSideBar,SLOT(searchText(QString)));

        return;
    }
    if(item->text()=="HR"){
//        hrSideBar = new HR_SideBar(this);
//        ui->verticalLayout_4->addWidget(hrSideBar);
//        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),hrSideBar,SLOT(shareClicked()));
//        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),hrSideBar,SLOT(searchText(QString)));

        return;
    }
    if(item->text()=="HelpDesk"){
        hdSideBar = new HelpDesk_SideBar(this);
        ui->verticalLayout_4->addWidget(hdSideBar);
        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),hdSideBar,SLOT(shareClicked()));
        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),hdSideBar,SLOT(searchText(QString)));
        return;
    }
    if(item->text()=="Warehouse"){
//        wSideBar = new Warehouse_SideBar(this);
//        ui->verticalLayout_4->addWidget(wSideBar);
//        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),wSideBar,SLOT(shareClicked()));
//        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),wSideBar,SLOT(searchText(QString)));

        return;
    }
    if(item->text()=="Services"){
        sSideBar = new Services_SideBar(this);
        ui->verticalLayout_4->addWidget(sSideBar);
        QObject::connect(rtoolbar,SIGNAL(ShareClicked()),sSideBar,SLOT(shareClicked()));
        QObject::connect(rtoolbar,SIGNAL(searchText(QString)),sSideBar,SLOT(searchText(QString)));
        return;
    }
}



