#include "godownselectorwidget.h"
#include "ui_godownselectorwidget.h"

#include <QScreen>

GodownSelectorWidget::GodownSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownSelectorWidget)
{
    ui->setupUi(this);
    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");
    this->setFixedSize(300, 130);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);

    initGlobalObjects();
    setWidgets();


}

GodownSelectorWidget::~GodownSelectorWidget()
{
    delete ui;
}


void GodownSelectorWidget::initGlobalObjects()
{
    toGodownWidget = new GMComboBox(&godown);
    toGodownWidget->getGodownList();
    toGodownWidget->removeItem(toGodownWidget->findData("Godown"));
    toGodownWidget->setCurrentIndex(0);

    selectedButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "check_circle"));
    selectedButton->setMini(true);
    selectedButton->setXOffset(10);
    selectedButton->setYOffset(10);
    selectedButton->setBackgroundColor(QColor::fromRgb(46,125,50));
    selectedButton->setParent(this);
    selectedButton->setToolTip("selected items");

    QObject::connect(selectedButton,&QPushButton::clicked, this, [=](){
        qDebug()<<Q_FUNC_INFO<<godown;
        emit sectionSelected(godown);

        this->close();
    });

    //    QObject::connect(toGodownWidget,&GMComboBox::selected, this, [=](){
    //        emit sectionSelected(godown);
    //    });
}

void GodownSelectorWidget::setWidgets()
{
    qDebug()<<Q_FUNC_INFO;
    ui->verticalLayout_3->addWidget(new QLabel("Section"));
    ui->verticalLayout_3->addWidget(toGodownWidget);
    ui->verticalLayout_3->addStretch(1);
    qDebug()<<Q_FUNC_INFO;
    //  ui->verticalLayout_2->addWidget(new QPushButton);



}


