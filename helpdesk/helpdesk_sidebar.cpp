#include "helpdesk_sidebar.h"
#include "ui_helpdesk_sidebar.h"

HelpDesk_SideBar::HelpDesk_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpDesk_SideBar)
{
    ui->setupUi(this);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    dash = new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);
}

HelpDesk_SideBar::~HelpDesk_SideBar()
{
    delete ui;
}

void HelpDesk_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
         qDebug()<<"clearing Inventory Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }
}