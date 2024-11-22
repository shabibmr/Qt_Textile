#include "maintabwidget.h"

MainTabWidget::MainTabWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(0,0,0,0);

    mainWidget = new QTabWidget;


    mainWidget->setTabsClosable(true);
    mainWidget->setTabShape(QTabWidget::Triangular);
//    mainWidget->tabBar()->setTabButton()
    mainWidget->setContentsMargins(0,0,0,0);

    mainWidget->setStyleSheet( " QTabWidget {border 1px solid black; } "
                "QTabBar::close-button { image: url(:/icons/icons/navigation/svg/production/ic_close_24px.svg); "
                                        "subcontrol-position: right;  }"
                                        "QTabBar::tab:!selected:hover { text-decoration: underline; }"
                                        );
    mainWidget->tabBar()->setFixedHeight(50);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);
    verticalLayout->setContentsMargins(0,0,0,0);
   verticalLayout->addWidget(mainWidget);

    connect(mainWidget, &QTabWidget::tabCloseRequested, this, &MainTabWidget::closeTab);



        DefaultDashBoard *dash = new DefaultDashBoard(this);
        openTab(dash, "Dashboard");


//    CustomDashboardWidget *dash1 = new CustomDashboardWidget(this);
//    openTab(dash1, "Customize dashboard");

    mainWidget->tabBar()->tabButton(0, QTabBar::RightSide)->deleteLater();
    mainWidget->tabBar()->setTabButton(0, QTabBar::RightSide, nullptr);
}

MainTabWidget::~MainTabWidget()
{
}

void MainTabWidget::openTab(QWidget *widget, QString uiName)
{
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<widget<<uiName;
    mainWidget->addTab(widget, uiName);
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<mainWidget->count();
    mainWidget->setCurrentIndex(mainWidget->count()-1);
    mainWidget->tabBar()->setTabTextColor(mainWidget->count()-1, "#006064");
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<mainWidget->count();

}

void MainTabWidget::closeTab(int index)
{
    mainWidget->removeTab(index);
}

void MainTabWidget::setTabTitle(QString uiName)
{
    mainWidget->setTabText(mainWidget->currentIndex(), uiName);
}

