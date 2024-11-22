#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSqlRecord>
#include <QMenu>

#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"

#include "database/Settings/uisettingsdatabasehelper.h"
#include "CommonUi/uicontroller.h"

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QString module, QWidget *parent = nullptr);
    ~SideBar();

    QString module;
    UiSettingsDatabaseHelper *dbHelper;
    QtMaterialDrawer *drawer;
    QVBoxLayout *drawerLayout;
    QtMaterialIconButton *menuIcon;
    QtMaterialTabs *tabs;


    void setModule(const QString &value);

public slots:
    void showSideBarDrawer();
    void openSideBarMenu(QString uiType);
    void addUiTab();


private slots:
    void initGlobal();

private:

};

#endif // SIDEBAR_H
