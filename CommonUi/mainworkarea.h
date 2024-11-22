#ifndef MAINWORKAREA_H
#define MAINWORKAREA_H

#include <QWidget>
#include <QTabBar>
#include <QTabWidget>

#include "CommonUi/mainwindowdrawerbuilder.h"
#include "CommonUi/maintabwidget.h"
#include "customwidgets/defaultdashboard.h"

class MainWorkArea : public QWidget
{
    Q_OBJECT

public:
    explicit MainWorkArea(QWidget *parent = nullptr);
    ~MainWorkArea();

//    QTabWidget *mainWidget;
    MainTabWidget *tabWidget;
    MainWindowDrawerBuilder *drawerWid;
    QtMaterialDrawer *drawer;
    QVBoxLayout *drawerLayout;


    void openTab(QWidget *widget, QString uiName);
    void hideTabBar();
    void showTabBar();
    void loadDrawer(QString moduleSelected);
    void setTabTitle(QString title);

    void closeTab();
signals:
    void uiSelected(int, bool)    ;

private slots:
    void setWidgets();
//    void closeTab(int index);

private:

    void closeDrawer();
};

#endif // MAINWORKAREA_H
