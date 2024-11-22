#ifndef HELPDESK_SIDEBAR_H
#define HELPDESK_SIDEBAR_H

#include <QWidget>
#include <customwidgets/defaultdashboard.h>
#include "customwidgets/section.h"
#include <QDebug>

namespace Ui {
class HelpDesk_SideBar;
}

class HelpDesk_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit HelpDesk_SideBar(QWidget *parent = 0);
    DefaultDashBoard* dash;
    ~HelpDesk_SideBar();
public slots:
    void clearLayout();
private:
    Ui::HelpDesk_SideBar *ui;
};

#endif // HELPDESK_SIDEBAR_H
