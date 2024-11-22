#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTabBar>
#include <QTabWidget>
#include "customwidgets/defaultdashboard.h"
//#include "dashboard/customdashboardwidget.h"
//#include "dashboard/salesmandashboard.h"

class MainTabWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainTabWidget(QWidget *parent = nullptr);
    ~MainTabWidget();

    QTabWidget *mainWidget;

    void openTab(QWidget *widget, QString uiName);
    void closeTab(int index);
    void setTabTitle(QString uiName);

private slots:


    // QWidget interface

};

#endif // MAINTABWIDGET_H
