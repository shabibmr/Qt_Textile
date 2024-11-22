#ifndef DASHBOARD_SIDEBAR_H
#define DASHBOARD_SIDEBAR_H

#include <QWidget>
#include "customwidgets/defaultdashboard.h"
#include "customwidgets/section.h"
#include <QDebug>
#include "customwidgets/Misc/sidebarbutton.h"

#include "database/Reports/ledgerhelper.h"
#include "database/Reports/inventoryhelper.h"

namespace Ui {
class Dashboard_SideBar;
}

class Dashboard_SideBar : public QWidget
{
    Q_OBJECT
public:

    explicit Dashboard_SideBar(QWidget *parent = 0);
    ~Dashboard_SideBar();
    DefaultDashBoard *dash;

    QString fontColor = "feffef";

    SideBarButton *accButton;
    SideBarButton *itemsButton;
    SideBarButton *cusButton;
    SideBarButton *salesButton;
    SideBarButton *purchaseButton;
    SideBarButton *receiptsButton;

    LedgerHelper *ledHelper;

    QFrame *getLine();


public slots:
    void clearLayout();
private slots:
    void InvClicked();
    void CusClicked();
    void SalClicked();
    void PurClicked();
    void RecClicked();
    void AccClicked();

private:
    Ui::Dashboard_SideBar *ui;
    void resetAllColors();
};

#endif // DASHBOARD_SIDEBAR_H
