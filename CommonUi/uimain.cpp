#include "uimain.h"
#include "ui_uimain.h"

#include <QSize>
#include <QDebug>

#include <QSystemTrayIcon>
#include "counter/MainScreen/restuarantcountermainwindow.h"
#include "database/hr/usergroupdatabasehelper.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"

//#include "customwidgets/Misc/gmdock.h"

UiMain::UiMain(bool isRestart, int id, QString voucherPrefix, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UiMain)
{
    ui->setupUi(this);
    this->isRestart = isRestart;
    this->userId = id;
    this->voucherPrefix = voucherPrefix;


    setWindowIcon(QIcon(":/images/gmico.ico"));
    setWindowTitle("AlgoERP");

    ui->centralWidget->setObjectName("workarea");
    ui->centralWidget->setStyleSheet("QWidget#workarea{background-color:#ffffff}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);
    initObjects();
    setShortcutsDock();

    QFont new_font = qApp->font();
    int nSize = settings.getValue("FontSize", new_font.pointSize()).toInt();
    new_font.setPointSize( nSize);
    qApp->setFont( new_font );

    //    setTrayIcon();
    setMouseTracking(true);
}


UiMain::~UiMain()
{
    delete ui;
}

void UiMain::setShortcutsDock()
{

    if(DatabaseValues::isPOS)
        return;

    //    GMDock *dl = new GMDock();
    //    dl->setParent(this);
    //    addDockWidget(Qt::NoDockWidgetArea,dl);

    //    dl->show();
    //    dl->setQueryModel(getWidgetModel());



    dock = new QDockWidget("Shortcuts",this,Qt::FramelessWindowHint);
    dock->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    dock->setFeatures(QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::NoDockWidgetArea);
    dock->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    dock->setTitleBarWidget(0);
    QWidget *widget= new QWidget();
    QVBoxLayout *lay = new QVBoxLayout();
    //    lay->setSizeConstraint(QLayout::SetMaximumSize);
    //    lay->setStretch(1,80);
    widget->setLayout(lay);

    QListWidget *list = new QListWidget(this);
    lay->addWidget(list);
    dock->setWidget(widget);

    list->setSpacing(1);

    addDockWidget(Qt::RightDockWidgetArea,dock);
    list->setStyleSheet("background-color:transparent;");

    QtMaterialFloatingActionButton *b = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "add"));
    b->setMini(true);
    b->setXOffset(10);
    b->setYOffset(10);
    b->setParent(this);
    b->hide();

    b->setHidden(true);

    //    list->scroll()->

    list->setStyleSheet("QListView{background-color:#f4f2e3;} QListView::item:selected{background-color: #e0f7fa;,color:#222233}");

    connect(dock,&QDockWidget::visibilityChanged,this,[=](bool visible){
        qDebug()<<"Dock visible "<<visible;
        if(!visible)
            b->show();
        else
            b->setHidden(true);
    });

    connect(b,&QPushButton::clicked,this,[=](bool v){
        dock->show();

    });

    QListWidgetItem* item1 = new QListWidgetItem(list);
    QListWidgetItem* item2 = new QListWidgetItem(list);
    QListWidgetItem* item3 = new QListWidgetItem(list);
    QListWidgetItem* item4 = new QListWidgetItem(list);
    QFont sfont; sfont.setPointSize(12);
    item1->setIcon(QtMaterialTheme::icon("content", "add"));
    item1->setText("Sales Voucher");
    item1->setFont(sfont);
    //    item1->setSizeHint(QSize(400,40));

    item2->setIcon(QtMaterialTheme::icon("content", "add"));
    item2->setText("Purchase Voucher");
    //    item2->setSizeHint(QSize(400,40));
    item2->setFont(sfont);

    item3->setIcon(QtMaterialTheme::icon("content", "add"));
    item3->setText("Payment Voucher");
    //    item3->setSizeHint(QSize(400,40));
    item3->setFont(sfont);

    item4->setIcon(QtMaterialTheme::icon("content", "add"));
    item4->setText("Receipt Voucher");
    item4->setFont(sfont);
    //    list->layout()->setSizeConstraint(QLayout::SetMaximumSize);
    //    item4->setSizeHint(QSize(400,40));
    UserGroupDatabaseHelper *userGroupHelper = new UserGroupDatabaseHelper();
    static UiAccessControlDataModel *permissions = new UiAccessControlDataModel();
    connect(list,&QListWidget::clicked,this,[=](QModelIndex index){
        qDebug()<<"Clicked : "<<index.row();
        switch(index.row()){
        case 0 : {
            qDebug()<<"At 0";
            permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 30);
            VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
            vEditor->createNewVoucher(GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
            break;
        }
        case 1 : {
            permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 45);
            VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
            vEditor->createNewVoucher(GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));
            break;
        }
        case 2 : {
            permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 48);
            VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
            vEditor->createNewVoucher(GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher));
            break;
        }
        case 3 : {
            permissions = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 49);
            VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
            vEditor->createNewVoucher(GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher));
            break;
        }
        }
    } );


    list->addItem(item1);
    list->addItem(item2);
    list->addItem(item3);
    list->addItem(item4);

    //    QWidget *wid = new QWidget(dock);
    //    wid->setWindowFlag(Qt::Window);
    //    wid->setObjectName("dck");
    //    QVBoxLayout *lay2 = new QVBoxLayout(wid);

    ////    setTitleBarWidget(new QWidget());
    //    lay2->setSpacing(5);

    //    lay2->addWidget(new QLabel("Table"),0,Qt::AlignHCenter);

    //    wid->show();



    //    list->addItem("Sales Voucher");
    //    list->addItem("Purchase Voucher");
    //    list->addItem("Payment Voucher");
    //    list->addItem("Receipt Voucher");
    list->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Minimum);
    //    bool showDock = ConfigurationSettingsDatabaseHelper::getValue(showShortcuts, false).toBool();
    //    if(!showDock)
    //        dock->hide();
}

void UiMain::setShortcutsDialog()
{

}


void UiMain::clearLayout(QLayout *layout){

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

void UiMain::openPOSWindow(int id, QString voucherPrefix)
{
    qDebug()<<Q_FUNC_INFO;
    errorData += "\n opening POS " + QString::number(id) + voucherPrefix;
    RestuarantCounterMainWindow *w = new RestuarantCounterMainWindow(id, voucherPrefix);
    w->setParent(this);
    w->setWindowFlag(Qt::FramelessWindowHint);
    connect(w, &RestuarantCounterMainWindow::closeMain, this,[=](){
        this->close();
    });
    connect(w, &RestuarantCounterMainWindow::minimizeMain, this,[=](){
        this->showMinimized();
    });
    connect(w, &RestuarantCounterMainWindow::uiSelected, this, &UiMain::getWidget);
    connect(w, &RestuarantCounterMainWindow::switchCompany, this, [=](){
        emit openCompanySelector();
        //        w->resetItemWidgets();
    }
            );
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setWindowTitle(LoginValues::getCompany().branch);
    //    if(DatabaseValues::isPOS){
    //        w->showFullScreen();
    //    }
    //    else{
    workArea->openTab(w, "POS");
    //    if(DatabaseValues::isPOS){

    //    }
    this->setWindowFlag(Qt::FramelessWindowHint);
    workArea->hideTabBar();
    //    rtoolbar->hide();
    //    mtoolbar->hide();
    this->showFullScreen();


    w->setFocus();
    //    }
}



void UiMain::setTrayIcon()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(Q_NULLPTR, QObject::tr("Systray"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return ;
    }
    QMenu *trayIconMenu = new QMenu(this);

    trayIconMenu->addSeparator();

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/images/gm.ico"));
    trayIcon->show();

    qDebug()<<" TRAY ICON SET";
}

void UiMain::setCompanyName(QString title)
{
    this->setWindowTitle(title);
    setActiveCompanyLabel(LoginValues::company.CompanyName + " - " + LoginValues::company.branch);
}

void UiMain::setUserId(int userId, QString vPrefix)
{
    this->userId = userId;
    this->voucherPrefix = vPrefix;

}



void UiMain::incrementFontSize(int n)
{
    //    qDebug()<<"Updating Size ";
    //    QFont new_font = qApp->font();
    //    int nSize = new_font.pointSize()+n;
    //    qDebug()<<"New Size "<<nSize;
    //    new_font.setPointSize( nSize); //your option
    //    //    new_font.setWeight( int ** ); //your option
    //    qApp->setFont( new_font );

    //    QLabel *label = new QLabel(this);
    //    label->setGeometry(500,400,250,50);
    //    label->setText("Size "+QString::number(nSize));
    //    label->setAttribute(Qt::WA_TranslucentBackground);
    //    //    label->setStyleSheet("QLabel { background-color : transparent; color : blue; }");
    //    label->setWindowFlags(Qt::FramelessWindowHint);
    //    QFont fnt;fnt.setPointSize(40);
    //    label->setFont(fnt);

    //    label->show();
    //    qDebug()<<"Timer start";
    //    QTimer::singleShot(1  * 1500, this, [=](){
    //        qDebug()<<"Timer done";
    //        label->hide();
    //    });

    //    settings.setValue("FontSize", nSize);

    //    qApp->processEvents();
}

void UiMain::setActiveCompanyLabel(QString company)
{
    if(!DatabaseValues::isPOS){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        rtoolbar->setCompanyLabel(company);
    }

}

void UiMain::loadDrawer(QListWidgetItem *item)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(item->text() == "POS"){
        openPOSWindow();
    }
    else {
        workArea->loadDrawer(item->text());
    }


}


void UiMain::initObjects()
{
    errorData = "\n init uimain objects " + QString::number(isRestart);
    // moving to login controller
    //    companySelectWid = new CompanySelectorWidget(this);
    //    companySelectWid->setView();
    //    companySelectWid->selectDefaultBranch();
    //    companySelectWid->hide();
    //    connect(companySelectWid, &CompanySelectorWidget::companySwitched, this, [=](QString val){
    //        qDebug()<<"set db"<<DatabaseValues::connectionString;
    //        this->setWindowTitle(val);
    //        if(DatabaseValues::isPOS){
    //            openPOSWindow();
    //        }
    //    });

    workArea = new MainWorkArea(this);
    ui->horizontalLayout->addWidget(workArea);
    ui->horizontalLayout->setMargin(0);
    ui->horizontalLayout->setSpacing(0);
    ui->horizontalLayout->setContentsMargins(0,0,0,0);

    connect(workArea, &MainWorkArea::uiSelected, this, &UiMain::getWidget);

    if(!DatabaseValues::isPOS){
        mtoolbar = new MainToolBar(ui->widget_maintoolbar);
        ui->horizontalLayout_3->addWidget(mtoolbar);
        mtoolbar->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #00897B, stop:1 #009688)");

        connect(mtoolbar,SIGNAL(mainItemClicked(QListWidgetItem*)),this,SLOT(loadDrawer(QListWidgetItem*)));

        rtoolbar = new Utilities_ToolBar(this);
        ui->horizontalLayout_7->addWidget(rtoolbar);
        rtoolbar->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #00897B, stop:1 #009688)");
        rtoolbar->setFocus();

        connect(rtoolbar, &Utilities_ToolBar::uiSelected, this, &UiMain::getWidget);
        connect(rtoolbar, &Utilities_ToolBar::companySwitchClicked, this, &UiMain::openCompanySelector );

        qDebug()<<Q_FUNC_INFO<<__LINE__;

        alertButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("social", "notifications_active"));
        alertButton->setMini(true);
        alertButton->setCorner(Qt::TopRightCorner);
        alertButton->setParent(this);
        alertButton->setXOffset(50);
        alertButton->setYOffset(50);
        alertButton->hide();

        connect(alertButton, &QtMaterialFloatingActionButton::clicked, this, [=](){
            getWidget(9,true);
            alertButton->hide();
        });
    }

    else{
        if(isRestart){
            errorData += " \n show POS ";
            openPOSWindow(userId, voucherPrefix);
        }
        //openPOSWindow();
    }


    //    if(isRestart){
    QFile File("restartlog.txt");
    File.open(QFile::WriteOnly | QFile::Text | QFile::Append);

    QString functionName = Q_FUNC_INFO;
    File.write(errorData.toUtf8());
    File.close();
    //    }

}

void UiMain::getWidget(int ui_id, bool configuredInDb)
{

    qDebug()<<Q_FUNC_INFO<<__LINE__<<ui_id<< configuredInDb;
    UiController *uiCtrl = new UiController(this);
    connect(uiCtrl, &UiController::uiSet, this, [=](QWidget *wid, QString val ){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<val;
        workArea->openTab(wid, val);
    });
    connect(uiCtrl, &UiController::closing, this, [=](){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        workArea->closeTab();
        delete uiCtrl;
    });
    connect(uiCtrl, &UiController::setTabTitle, workArea, &MainWorkArea::setTabTitle);
    connect(uiCtrl, &UiController::showUi, this, [=](QString dbName){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<dbName;
        QString str = LoginValues::company.CompanyName;
        this->setWindowTitle(str) ;
        emit UiShow(dbName);
        //        rtoolbar->hide();
        //        mtoolbar->hide();
    });
    connect(uiCtrl, &UiController::noPermissions, this, [=](){
        QMessageBox box;
        box.setText("User " + LoginValues::userName + " does not have permissions;\n Contact Admin");
        box.exec();
    });
    connect(uiCtrl, &UiController::showReorderStats, this, &UiMain::showPopup );
    uiCtrl->setUiData(ui_id, configuredInDb);
    //    uiCtrl->initUI();



    //    connect(uiCtrl, &UiController::uiSet, workArea, &MainWorkArea::openTab);

}

void UiMain::showPopup()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    alertButton->show();

}

//QSqlQueryModel *UiMain::getWidgetModel()
//{
//    QMap<QString, QPair<QString, QString>> filterMap;
//    filterMap.insert("fromDateTime",{"fromDateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")});
//    filterMap.insert("toDateTime",{"fromDateTime", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")});


//    UiController *uiCtrl = new UiController(this);
//    return uiCtrl->getReportModel(30,filterMap);
//}

//void UiMain::openCompanySelector()
//{
//    companySelectWid->resize(500,500);
//    companySelectWid->setWindowFlags(Qt::Window);
//    companySelectWid->show();

//}

void UiMain::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_M)  && (e->modifiers().testFlag(Qt::ControlModifier)))
    {
        workArea->showTabBar();
        rtoolbar->show();
        mtoolbar->show();
        //        this->showNormal();
    }

    if ((e->key() == Qt::Key_H)  && (e->modifiers().testFlag(Qt::ControlModifier)))
    {
        workArea->hideTabBar();
        rtoolbar->hide();
        mtoolbar->hide();
    }

    if(e->key() == Qt::Key_F8){
        QMessageBox box ; box.setText("F8"); box.exec();
    }

    if ((e->key() == Qt::Key_F1)  && (e->modifiers().testFlag(Qt::AltModifier)))
    {
        qDebug()<<"Decrese Font";
        incrementFontSize(-1);
        this->update();
        qApp->processEvents();
    }
    if ((e->key() == Qt::Key_F2)  && (e->modifiers().testFlag(Qt::AltModifier)))
    {
        qDebug()<<"Increase Font";
        incrementFontSize(1);
        this->update();
        qApp->processEvents();

    }



}


void UiMain::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);


    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QMainWindow::paintEvent(event);
}


void UiMain::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<event->pos();
    QMainWindow::mouseMoveEvent(event);
}
