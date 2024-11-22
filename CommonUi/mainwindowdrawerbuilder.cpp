#include "mainwindowdrawerbuilder.h"

MainWindowDrawerBuilder::MainWindowDrawerBuilder(QWidget *parent) :
    QWidget(parent)
{
//    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");
    initGlobal();
    //    this->setFixedWidth(680);
}

MainWindowDrawerBuilder::~MainWindowDrawerBuilder()
{
}

void MainWindowDrawerBuilder::setModule(const QString &value)
{
    qDebug()<<Q_FUNC_INFO<<value;
    if(QString::compare(module, value)){
        module = value;
        uiType = "";
        setDrawer();

    }

}

void MainWindowDrawerBuilder::setDrawer()
{
    qDebug()<<Q_FUNC_INFO;
    clearLayout(drawerLayout);
    layoutList.clear();
    lastLayout = new QVBoxLayout;

    QSqlQueryModel *uiTypes = dbHelper->getUiTypesByModule(module);

    for(int i=0; i<uiTypes->rowCount(); i++)
    {
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        QVBoxLayout *layout = new QVBoxLayout;
        QWidget *widget = new QWidget;
        widget->setLayout(layout);

        QtMaterialFlatButton *typeHead = new QtMaterialFlatButton;
        typeHead->setBackgroundColor(bgcolor);
        typeHead->setForegroundColor(textColor);
        QString typeStr = uiTypes->record(i).value(0).toString();
        typeHead->setText(typeStr.remove(0,2));
        typeHead->setTextAlignment(Qt::AlignLeft);
        typeHead->setFixedHeight(30);
        typeHead->setFixedWidth(100);

        layout->addWidget(typeHead);

        QVBoxLayout *lay1 = new QVBoxLayout;
//        QWidget *wid2 = new QWidget;
//        wid2->setLayout(lay1);

        drawerLayout->addWidget(widget);
//        layout->addWidget(wid2);
        layout->addLayout(lay1);

        connect(typeHead, &QtMaterialFlatButton::clicked, this, [=](){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            if(layoutList.contains(uiTypes->record(i).value(0).toString())){
                closeMenu(layoutList[uiTypes->record(i).value(0).toString()]);
                layoutList.remove(uiTypes->record(i).value(0).toString());
                typeHead->setHaloVisible(false);
            }
            else{
                openMenu(uiTypes->record(i).value(0).toString(), lay1);
                layoutList.insert(uiTypes->record(i).value(0).toString(), lay1);

            }

            // change button color on click
        });


    }

    drawerLayout->addStretch(1);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

}

void MainWindowDrawerBuilder::closeMenu(QVBoxLayout *layout)
{
    qDebug()<<Q_FUNC_INFO;
    clearLayout(layout);
}

void MainWindowDrawerBuilder::openMenu(QString ui_Type, QVBoxLayout *layout)
{
//    if(QString::compare(this->uiType, ui_Type)){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        this->uiType = ui_Type;
        // to clear current open menu
//        if(lastLayout){
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<"clear last lay";
//            clearLayout(lastLayout);
//        }
//        clearLayout(layout);
//        lastLayout = layout;
        qDebug()<<Q_FUNC_INFO<<"set last lay"<<module<<ui_Type<<lastLayout;
        QSqlQueryModel *uiList = dbHelper->getAlScreensByModule(module, ui_Type);

        for(int i=0; i<uiList->rowCount(); i++){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<" adding button "<<i<<uiList->record(i).value(1).toString();
            SideBarButton *uiButton = new SideBarButton(uiList->record(i).value(1).toString());
//            QtMaterialFlatButton *uiButton= new QtMaterialFlatButton(uiList->record(i).value(1).toString());
            uiButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:" + textColor +";");
            //            uiButton->setTextAlignment(Qt::AlignLeft);
            //            uiButton->setFixedWidth(180);
            uiButton->setProperty("ID", uiList->record(i).value(0).toString());
            uiButton->setProperty("dbConfigured", uiList->record(i).value(2).toString());
            qDebug()<<"new button"<<uiButton;
            buttonsList.append(uiButton);
            layout->addWidget(uiButton);

            connect(uiButton,&QtMaterialFlatButton::clicked,this, [=](){
                resetWidgets();
                int ui_id = sender()->property("ID").toInt();
                bool dbConfigured = sender()->property("dbConfigured").toBool();
                qDebug()<<Q_FUNC_INFO<<__LINE__<<ui_id<<dbConfigured;
                uiButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:" + highlightedColor +";");
                emit uiSelected(ui_id, dbConfigured);
            } );

        }
//    }

}

//void MainWindowDrawerBuilder::closeDrawer()
//{
//    drawer->closeDrawer();
//}

void MainWindowDrawerBuilder::initGlobal()
{
    dbHelper = new UiSettingsDatabaseHelper;
    lastLayout = new QVBoxLayout;

//    QScrollArea *scrollArea = new QScrollArea(this);
//    scrollArea->
    drawerLayout = new QVBoxLayout();

//    QWidget *scrollWidget = new QWidget;
//    scrollWidget->setLayout(drawerLayout);
//    scrollArea->setWidget(scrollWidget);
//    ui->verticalLayout_2->addWidget(scrollArea);
    QVBoxLayout *verticalLayout_2 = new QVBoxLayout;
    this->setLayout(verticalLayout_2);
    verticalLayout_2->addLayout(drawerLayout);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<"last layout init"<<lastLayout;
}

void MainWindowDrawerBuilder::resetWidgets()
{
    for(QWidget *button:buttonsList){
        if(button){
            qDebug()<<"reset button"<<button;
            button->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:" + textColor +";");
        }
    }

}

void MainWindowDrawerBuilder::clearLayout(QLayout *layout)
{

    qDebug()<<Q_FUNC_INFO<<layout;

    if (layout) {
        while(layout->count() > 0){
            QLayoutItem *item = layout->takeAt(0);
            QWidget* widget = item->widget();
            if(widget){
                qDebug()<<Q_FUNC_INFO<<__LINE__<<"deleting widget"<<widget;
                delete widget;
            }
            qDebug()<<Q_FUNC_INFO<<__LINE__<<"deletng lay item"<<item<<lastLayout;
            delete item;
        }
    }
//    resetWidgets();
    buttonsList.clear();
}
