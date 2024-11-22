#include "gmcardwidget.h"
#include "ui_gmcardwidget.h"

#include "customwidgets/Misc/customshadoweffect.h"

GMCardWidget::GMCardWidget(QString Label,
                           QString desc,
                           QString Data,
                           bool showViewButton,
                           QString bgColor,
                           QString textColor,
                           QString iconColor,
                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GMCardWidget)
{
    ui->setupUi(this);
    this->label = Label;
    this->description=desc;
    this->data = Data;
    if(this->data.length()==0){
        this->data="x";
    }
    this->bgColor = bgColor;
    this->textColor = textColor;
    this->iconColor = iconColor;
//    this->setMinimumSize(200,200);
    this->showViewButton=showViewButton;

    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(10.0);
    bodyShadow->setDistance(6.0);
    bodyShadow->setColor(QColor("#b2ebf2")); //#1a5050

    QSizePolicy sizePolicy;
    sizePolicy.setHorizontalPolicy(QSizePolicy::Minimum);
    sizePolicy.setVerticalPolicy(QSizePolicy::Minimum);
    this->setSizePolicy(sizePolicy);
    createWidgets();
    setWidgets();

}

GMCardWidget::~GMCardWidget()
{
    delete ui;
}

//void GMCardWidget::setData(const QString &value)
//{
//    data = value;
//    dataWid->setText(data);
//}

void GMCardWidget::setWidgets()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    widget->setLayout(layout);
    widget->setStyleSheet("QWidget#editor{background-color: " + bgColor + " }");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgColor));
    setAutoFillBackground(true);
    widget->setPalette(pal);

    layout->addWidget(title, 0, Qt::AlignLeft);
    layout->addWidget(dataWid, 0, Qt::AlignCenter);
    layout->addWidget(descLabel, 1, Qt::AlignRight);
    layout->addWidget(expandButton,1,Qt::AlignTop);

    layout->setAlignment(descLabel,Qt::AlignLeft|Qt::AlignVCenter);

    layout->setContentsMargins(0,0,0,0);

    QWidget *widgetParent = new QWidget;
//    widgetParent->setFixedSize(100,100);
    QHBoxLayout *lay = new QHBoxLayout;
    widgetParent->setLayout(lay);
    lay->addWidget(widget);
    widgetParent->setAttribute(Qt::WA_TranslucentBackground);
    widgetParent->setWindowFlags(Qt::FramelessWindowHint);
    widgetParent->setStyleSheet("background: " + bgColor);

    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    bodyShadow->setBlurRadius(4.0);
    bodyShadow->setDistance(4.0);
    bodyShadow->setColor(QColor("#b2ebf2"));
    widgetParent->setAutoFillBackground(true);
    widgetParent->setGraphicsEffect(bodyShadow);


    ui->horizontalLayout->addWidget(widgetParent);
}

void GMCardWidget::createWidgets()
{
    title = new QLabel(label);
    title->setAlignment(Qt::AlignTop);
    title->setStyleSheet("QLabel{background-color : " + bgColor + " ; color: " + textColor + " ; border: none}");
    title->setFont(QFont("Calibri", 12, QFont::Normal));

    descLabel = new QLabel(description);
    descLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    descLabel->setStyleSheet("QLabel{background-color : " + bgColor + " ; color: " + textColor + " ; border: none}");
    descLabel->setFont(QFont("Calibri", 8, QFont::Normal,true));

    dataWid = new QWidget;
    dataLayout = new QVBoxLayout;
    dataWid->setLayout(dataLayout);
//    dataWid->setAlignment(Qt::AlignBottom);
    dataWid->setStyleSheet("QWidget{background-color : " + bgColor + " ; color: " + textColor + " ; border: none}");

//    dataWid->setFont(QFont("Calibri", 28, QFont::Bold));

    expandButton =
            new QtMaterialIconButton(QtMaterialTheme::icon("image", "remove_red_eye"), this);
    expandButton->setParent(this);
    expandButton->setColor(QColor(iconColor));
    expandButton->setHidden(!showViewButton);
    QObject::connect(expandButton,&QtMaterialIconButton::clicked,[=]{
        emit clicked();
    });


}

void GMCardWidget::setDataWidget(QWidget *wid)
{
    dataLayout->addWidget(wid);
    wid->setStyleSheet("QWidget{background-color : " + bgColor + " ; color: " + textColor + " ; border: none}");
//    wid->setFont(QFont("Calibri", 28, QFont::Bold));

}
