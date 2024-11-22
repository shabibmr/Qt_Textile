#include "mainworkarea.h"

MainWorkArea::MainWorkArea(QWidget *parent) :
    QWidget (parent)
{
    this->setContentsMargins(0,0,0,0);
    this->setObjectName("workarea");
    this->setStyleSheet("QWidget#workarea{background-color:#ffffff}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);
    this->setWindowFlag(Qt::FramelessWindowHint);

    setWidgets();
}

MainWorkArea::~MainWorkArea()
{
}

void MainWorkArea::openTab(QWidget *widget, QString uiName)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    tabWidget->openTab(widget, uiName);

}

void MainWorkArea::hideTabBar()
{
    tabWidget->mainWidget->tabBar()->hide();

}

void MainWorkArea::showTabBar()
{
    tabWidget->mainWidget->tabBar()->show();
}



void MainWorkArea::closeTab()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    tabWidget->closeTab(tabWidget->mainWidget->currentIndex());

}

void MainWorkArea::loadDrawer(QString moduleSelected)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    drawerWid->setModule(moduleSelected);
    drawer->openDrawer();
    //    drawerWid->openDrawer();
}

void MainWorkArea::setTabTitle(QString title)
{
    tabWidget->setTabTitle(title);
}

void MainWorkArea::setWidgets()
{
    // add maintabwidget

    tabWidget = new MainTabWidget(this);
    QVBoxLayout *WALayout = new QVBoxLayout;
    this->setLayout(WALayout);

    WALayout->addWidget(tabWidget);
    WALayout->setContentsMargins(0,0,0,0);

    // init drawer

    drawer = new QtMaterialDrawer(this);
    drawer->setOverlayMode(true);
    drawer->setClickOutsideToClose(true);
    drawer->setDrawerWidth(300);
    drawer->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    //    drawer->setStyleSheet("");
    //    drawer->setAutoRaise(true);


    drawerLayout = new QVBoxLayout;
    drawerWid = new MainWindowDrawerBuilder();
    drawer->setDrawerLayout(drawerLayout);
    drawerLayout->addWidget(drawerWid);



    connect(drawerWid, &MainWindowDrawerBuilder::uiSelected, this, [=](int id, bool dbConfigured){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<id<< dbConfigured;
        emit uiSelected(id, dbConfigured);
        drawer->closeDrawer();
    });

}
