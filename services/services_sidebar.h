#ifndef SERVICES_SIDEBAR_H
#define SERVICES_SIDEBAR_H

#include <QWidget>
#include <QDebug>
#include "customwidgets/defaultdashboard.h"
#include "customwidgets/section.h"
namespace Ui {
class Services_SideBar;
}

class Services_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Services_SideBar(QWidget *parent = 0);
    ~Services_SideBar();
    DefaultDashBoard *dash;

private:
    Ui::Services_SideBar *ui;
public slots :
    void clearLayout();
};

#endif // SERVICES_SIDEBAR_H
