#include "salesgroups.h"
#include "ui_salesgroups.h"
#include <QStringList>
#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>

SalesGroups::SalesGroups(int colCount,int buttonHeight,int buttonWidth,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesGroups)
{
    ui->setupUi(this);
    this->colCount = colCount;
    this->buttonHeight = buttonHeight;
    this->buttonWidth = buttonWidth;
    dbHelper = new SalesInventoryGroupsDatabaseHelper();
    this->setObjectName("POSMain");
    this->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
    // this->setStyleSheet("QWidget#POSMain{background-color:#a9a9a9}");

}

SalesGroups::~SalesGroups()
{
    delete ui;
}

void SalesGroups::setGroupName(const QString &value)
{
    groupName = value;
    qDebug()<<"Group name"<<groupName;
}

void SalesGroups::setTable()
{
    if(ui->layout!=NULL){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= NULL){
            delete item->widget();
            delete item;
        }
    }

    int lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();

    qDebug()<< "setting groups Table";
    //    QMap<QString,QString> groups = dbHelper->getSalesInventoryGroupDataAsMapForSales();

    grpList= dbHelper->getSalesInventoryGroupDataAsMapForSalesAslist();
    InventoryItemGroupDataModel tmp;
    tmp.GroupName="FAVOURITES";
    tmp.GroupID="0";

    //    for(int i=0;i<grpList.size();i++){
    //        for(int j=0;j<grpList.size();j++){
    //            if(grpList[i]._id<grpList[j]._id){
    //                tmp = grpList[i];
    //                grpList[i] = grpList[j];
    //                grpList[j] = tmp;
    //            }
    //        }
    //    }
    tmp.GroupName="FAVOURITES";
    tmp.GroupID="0";
    grpList.insert(0,tmp);
    //    QStringList groupNamesList = groups.values();

    //    qSort(groupNamesList);

    //    groupNamesList.insert(0,"FAVOURITES");

    QSignalMapper* signalMapper = new QSignalMapper(this);

    QWidget *client= new QWidget(this);

    QGridLayout *gLayout = new QGridLayout(client);
    //QLayoutItem item = new QLayoutItem;
    QObject::connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(salesGroupSelected(QString)));
    int cnt =0;
    //    QMapIterator<QString, QString> i(groups);

    //    while (i.hasNext()) {
    for(int i =0;i<grpList.size();i++){

        QString name = grpList[i].GroupName;
        if(lang == 1 && grpList[i].GroupNameArabic.length()>0){
            name = grpList[i].GroupNameArabic;
        }

        QString id = grpList[i].GroupID;//i.key();
        if(i==0){
            id ="x";

        }
        //        qDebug()<<id<<name;

        QtButton* button = new QtButton(client);
        button->setProperty("id",id);
        button->setFixedSize(QSize(buttonWidth,buttonHeight));
        // button->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 201)"));
        QHBoxLayout *lay = new QHBoxLayout();
        lay->setContentsMargins(1,1,1,1);
        button->setLayout(lay);
        QLabel *label = new QLabel;
        label->setText(name);
        label->setAlignment(Qt::AlignCenter);
        label->setWordWrap(true);
        imgLabels.append(label);
        // button->setImage("0x8",name);

        QFont font;
        font.setPointSize(10);
        // label->setStyleSheet(QString::fromUtf8("color: rgb(256, 256, 256)"));
        // label->setStyleSheet("QLabel { background-color : rgb(70, 72, 70); color : white; }");
        label->setFont(font);
        lay->addWidget(label);

        signalMapper->setMapping(button, id);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        gLayout->addWidget(button,cnt/colCount,cnt%colCount);
        cnt++;
    }

    client->setObjectName("POSMain");
    // client->setLayout(gLayout);

    QScrollArea *scroll = new QScrollArea(this);
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
                                        "    height: px;"
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
    qDebug()<<"Set all Groups";

}

void SalesGroups::salesGroupSelected(QString grpID)
{
    emit selectSalesGroupID(grpID);
}
