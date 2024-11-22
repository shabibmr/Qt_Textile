#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include <QListWidgetItem>
#include <QCompleter>
#include <QMessageBox>

#include "toolbar/maintoolbar.h"

#include "toolbar/utilities_toolbar.h"
#include "inventory/masters/sales_inventory_items/additems.h"

#include "addressbook/contacts_sidebar.h"
#include "dashboard/dashboard_sidebar.h"
#include "helpdesk/helpdesk_sidebar.h"
#include "projects/projects_sidebar.h"
#include "services/services_sidebar.h"
#include "database/databasehelper/databasevalues.h"
#include "customwidgets/defaultdashboard.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clearLayout(QLayout *layout);
private:

    Ui::MainWindow *ui;
    MainToolBar *mtoolbar;

    DefaultDashBoard *dashBoard;
    Utilities_ToolBar *rtoolbar;

    Contacts_SideBar *cSideBar;
    Dashboard_SideBar *dSideBar;
    HelpDesk_SideBar *hdSideBar;
    Projects_SideBar *prSideBar;
    Services_SideBar *sSideBar;

    QWidget *sideBarWidget;
    QVBoxLayout *widLay;




protected:

private slots:
    void loadSideToolbar(QListWidgetItem*);



};

#endif // MAINWINDOW_H
