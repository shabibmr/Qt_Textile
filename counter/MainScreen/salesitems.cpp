#include "salesitems.h"
#include "ui_salesitems.h"

#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>

SalesItems::SalesItems(int colCount,int buttonHeight,int buttonWidth,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesItems)
{
    ui->setupUi(this);
    this->colCount = colCount;
    this->buttonHeight = buttonHeight;
    this->buttonWidth = buttonWidth;
    this->setObjectName("POSMain");

    this->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
    // this->setStyleSheet("QWidget#POSMain{background-color:#a9a9a9}");

    dbHelper = new SalesInventoryItemDatabaseHelper();
    setWidgets();
    setTable();

}

SalesItems::~SalesItems()
{
    delete ui;
}

void SalesItems::setWidgets()
{
//    QPointer<QWidget> client= new QWidget(this);

//    gLayout = new QGridLayout(client);

//    client->setLayout(gLayout);

//    client->setObjectName("POSMain");

//    client->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");

//    QPointer<QScrollArea> scroll = new QScrollArea(this);

//    scroll->setObjectName("POSMain");

//    scroll->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");


//    scroll->verticalScrollBar()->
//            setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
//                                            "    border: 1px solid #999999;"
//                                            "    background:white;"
//                                            "    width:20px;    "
//                                            "    margin: 0px 0px 0px 0px;"
//                                            "}"
//                                            "QScrollBar::handle:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    min-height: 0px;"
//                                            ""
//                                            "}"
//                                            "QScrollBar::add-line:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    height: 0px;"
//                                            "    subcontrol-position: bottom;"
//                                            "    subcontrol-origin: margin;"
//                                            "}"
//                                            "QScrollBar::sub-line:vertical {"
//                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
//                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
//                                            "    height: 0px;"
//                                            "    subcontrol-position: top;"
//                                            "    subcontrol-origin: margin;"
//                                            "}"
//                                            ""));

//    scroll->setWidget(client);
//    scroll->setAlignment(Qt::AlignCenter);
//    QPointer<QVBoxLayout> vLayout = new QVBoxLayout(this);
//    vLayout->addWidget(scroll);
//    ui->layout->addLayout(vLayout);

}

void SalesItems::setTable(int l)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();

    if(gLayout!=nullptr){
        QLayoutItem* item;
        while((item=gLayout->takeAt(0))!= nullptr){
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<item->widget();
            delete item->widget();
            delete item;
        }
    }

    if(ui->layout!=nullptr){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= nullptr){
            delete item->widget();
            delete item;
        }
    }

    QPointer<QWidget> client= new QWidget(this);

    gLayout = new QGridLayout(client);


    if(grpID == "x"){
        items = dbHelper->getALLInventoryObjectsFavourites();
    }
    else{
        items = dbHelper->getALLInventoryObjectsByGroup(grpID);
    }

    QSignalMapper* signalMapper = new QSignalMapper(this);
    QObject::connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(salesItemSelected(QString)));
    int cnt =0;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<items.size();
    for(int i=0;i<items.size();i++){
        QString id = items[i]->ItemID;
        QString name = items[i]->ItemName;
        QString arabicName = items[i]->itemNameArabic;

        QPointer<QtButton> button = ( new QtButton(this));
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<button;
        button->setProperty("id",id);
        button->setFixedSize(QSize(buttonWidth,buttonHeight));
        QPointer<QHBoxLayout> lay = new QHBoxLayout(button.data());
        lay->setContentsMargins(1,1,1,1);
        // button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 201)"));
        button->setObjectName("POSMain");
        QPointer<QLabel> label = new QLabel(this);
        if(lang==0 )
            label->setText(name);
        else if(arabicName.length()>0)
            label->setText(arabicName);

        label->setObjectName("POSMain");
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        // label->setStyleSheet("QLabel { background-color : rgb(70, 72, 70); color : white; }");

        QFont font;
        font.setPointSize(10);

        label->setFont(font);
        lay->addWidget(label);


        signalMapper->setMapping(button, id);

        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        gLayout->addWidget(button,cnt/colCount,cnt%colCount);
        cnt++;
    }

    client->setLayout(gLayout);

    client->setObjectName("POSMain");

    client->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
    // client->setStyleSheet("QWidget#POSMain{background-color:#a9a9a9}");


    QPointer<QScrollArea> scroll = new QScrollArea(this);

    scroll->setObjectName("POSMain");

    scroll->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
    // scroll->setStyleSheet("QWidget#POSMain{background-color:#a9a9a9}");


    scroll->verticalScrollBar()->
            setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                            "    border: 1px solid #999999;"
                                            "    background:white;"
                                            "    width:20px;    "
                                            "    margin: 0px 0px 0px 0px;"
                                            "}"
                                            "QScrollBar::handle:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    min-height: 0px;"
                                            ""
                                            "}"
                                            "QScrollBar::add-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: 0px;"
                                            "    subcontrol-position: bottom;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            "QScrollBar::sub-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: 0px;"
                                            "    subcontrol-position: top;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            ""));

    scroll->setWidget(client);
    scroll->setAlignment(Qt::AlignCenter);
    QPointer<QVBoxLayout> vLayout = new QVBoxLayout(this);
    vLayout->addWidget(scroll);
    ui->layout->addLayout(vLayout);

    qDebug()<<"Set all Groups";

}

void SalesItems::salesItemSelected(QString itemID)
{
    emit selectSalesItemID(itemID);
}

void SalesItems::groupButtonFavouriteClicked()
{
    if(ui->layout!=NULL){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= NULL){
            delete item->widget();
            delete item;
        }
    }

    QList<QSharedPointer<inventoryItemDataModel>> items = dbHelper->getALLInventoryObjectsByGroup(grpID);

    QSignalMapper* signalMapper = new QSignalMapper(this);

    QWidget *client= new QWidget(this);

    QGridLayout *gLayout = new QGridLayout(client);
    //QLayoutItem item = new QLayoutItem;
    QObject::connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(salesItemSelected(QString)));
    int cnt =0;
    //<QString, QString> i(items);

    for(int i=0;i<items.size();i++){
        QString id = items[i]->ItemID;
        QString name = items[i]->ItemName;
        QString arabicName = items[i]->itemNameArabic;
        qDebug()<<id<<name;

        QtButton* button = new QtButton(this);
        button->setProperty("id",id);
        button->setFixedSize(QSize(130,40));
        QHBoxLayout *lay = new QHBoxLayout(button);
        lay->setContentsMargins(1,1,1,1);
        // button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 120, 201)"));
        button->setObjectName("POSMain");
        QLabel *label = new QLabel;
        label->setText(name);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        // label->setStyleSheet("QLabel { background-color : rgb(70, 72, 70); color : white; }");
        label->setObjectName("POSMain");
        QFont font;
        font.setPointSize(10);

        label->setFont(font);
        lay->addWidget(label);


        signalMapper->setMapping(button, id);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        gLayout->addWidget(button,cnt/4,cnt%4);
        cnt++;
    }

    client->setLayout(gLayout);

    QScrollArea *scroll = new QScrollArea(this);

    scroll->verticalScrollBar()->
            setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                            "    border: 1px solid #999999;"
                                            "    background:white;"
                                            "    width:20px;    "
                                            "    margin: 0px 0px 0px 0px;"
                                            "}"
                                            "QScrollBar::handle:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    min-height: 0px;"
                                            ""
                                            "}"
                                            "QScrollBar::add-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: 0px;"
                                            "    subcontrol-position: bottom;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            "QScrollBar::sub-line:vertical {"
                                            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                            "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                            "    height: 0px;"
                                            "    subcontrol-position: top;"
                                            "    subcontrol-origin: margin;"
                                            "}"
                                            ""));

    scroll->setWidget(client);
    scroll->setAlignment(Qt::AlignCenter);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(scroll);
    ui->layout->addLayout(vLayout);
    qDebug()<<"Set all Favos";

}

void SalesItems::clearItems()
{
    if(ui->layout!=NULL){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= NULL){
            delete item->widget();
            delete item;
        }
    }
}
