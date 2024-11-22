#include "sidebar.h"

SideBar::SideBar(QString module, QWidget *parent) :
    QWidget(parent)
{

    initGlobal();
    showSideBarDrawer();
}

SideBar::~SideBar()
{
}

void SideBar::setModule(const QString &value)
{
    module = value;
}

void SideBar::showSideBarDrawer()
{
    QSqlQueryModel *uiTypes = dbHelper->getUiTypesByModule(module);

    for(int i=0; i<uiTypes->rowCount(); i++)
    {
        QtMaterialRaisedButton *typeHead = new QtMaterialRaisedButton;
        typeHead->setText(uiTypes->record(i).value(0).toString());
        drawerLayout->addWidget(typeHead);

        connect(typeHead, &QPushButton::clicked, this, [=](){
            openSideBarMenu(uiTypes->record(i).value(0).toString());
        });


    }
}

void SideBar::openSideBarMenu(QString uiType)
{
    QSqlQueryModel *uiList = dbHelper->getAlScreensByModule(module, uiType);
    for(int i=0; i<uiList->rowCount(); i++){
        QMenu *menu = new QMenu();
        QAction *action = new QAction(uiList->record(i).value(1).toString(), this);
        action->setProperty("ID",uiList->record(i).value(0).toString());
        menu->addAction(action);

        connect(action,&QAction::triggered,this, &SideBar::addUiTab );
    }
}

void SideBar::addUiTab()
{
//    int ui_id = sender()->property("ID").toInt();
//    uiCtrl->setUiData(ui_id);
//    ui->verticalLayout->addWidget(uiCtrl->reportWid);

}

void SideBar::initGlobal()
{

    dbHelper = new UiSettingsDatabaseHelper;
    drawer = new QtMaterialDrawer;
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);
    drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);

    menuIcon = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "menu"));
    connect(menuIcon, &QtMaterialIconButton::clicked, drawer,&QtMaterialDrawer::openDrawer);


    tabs = new QtMaterialTabs();
    tabs->setBackgroundColor(QColor("white"));
    tabs->setTextColor(QColor("#00bcd4"));
    tabs->setFixedHeight(50);
    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);
    verticalLayout->addWidget(tabs);


}
