#ifndef UIMAIN_H
#define UIMAIN_H

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

#include "CommonUi/mainwindowdrawerbuilder.h"
#include "CommonUi/uicontroller.h"
#include "CommonUi/mainworkarea.h"
#include "customwidgets/Company/companyselectorwidget.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "utils/SettingsHelper/settingshelper.h"


namespace Ui {
class UiMain;
}

class UiMain : public QMainWindow
{
    Q_OBJECT

public:

    QString errorData;
    bool isRestart = false;
    int userId = 0;
    QString voucherPrefix = "0";
    SettingsHelper settings;

    explicit UiMain(bool isRestart = false, int id = 0, QString voucherPrefix = "0", QWidget *parent = nullptr);
    ~UiMain();

    void clearLayout(QLayout *layout);
    void openPOSWindow(int id = 0, QString voucherPrefix = "0");

    void setTrayIcon();

    void setCompanyName(QString title);
    void setUserId(int userId = 0, QString vPrefix = "0");

    QDockWidget *dock;

    QtMaterialFloatingActionButton *alertButton;
    QListView *listView;

    void incrementFontSize(int n);

    void setShortcutsDock();

    void setShortcutsDialog();
public slots:
    void setActiveCompanyLabel(QString company);

private:

    Ui::UiMain *ui;
    MainToolBar *mtoolbar;

    Utilities_ToolBar *rtoolbar;

    QWidget *sideBarWidget;
    QVBoxLayout *widLay;

    MainWorkArea *workArea;
    QWidget *reorderWidget;


protected:

private slots:
    void loadDrawer(QListWidgetItem*item);
    void initObjects();
    void getWidget(int ui_id, bool configuredInDb);
    void showPopup();
//    QSqlQueryModel *getWidgetModel();

signals:
    void openCompanySelector();
    void UiShow(QString);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

    // QWidget interface
protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // UIMAIN_H
