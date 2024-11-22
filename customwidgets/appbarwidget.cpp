#include "appbarwidget.h"

AppBarWidget::AppBarWidget(QString reportName, QWidget *parent) : QWidget(parent)
{

    this->reportName = reportName;
    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(this);
    backButton->setColor(QColor(iconColor));
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this,[=](){
        emit backButtonClicked();
    });
    //    backButton->hide();


    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(iconColor));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QtMaterialIconButton::clicked, this, [=](){
            emit shareButtonClicked();
        });


    windowLabel = new QLabel(reportName);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(appBarColor));

    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(titleColor));
    palette.setColor(windowLabel->backgroundRole(), QColor(appBarColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(titleFont);

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setFixedHeight(50);
    appBar->appBarLayout()->addWidget(backButton);
    //    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(shareButton);


    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(canvas);

    this->setLayout(mainLayout);




}

void AppBarWidget::showShareOptions()
{
    shareOptionsWidget = new ShareTools(reportName, shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

//    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
//                     this,&ReportWidget::printReport);
//    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
//                     this,&ReportWidget::printReport);
//    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
    //                     this,&ReportWidget::exportToExcel);
}

void AppBarWidget::addExtraActions(QWidget *widget)
{
    appBar->appBarLayout()->addWidget(widget);
}
