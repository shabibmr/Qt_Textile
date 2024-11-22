#include "contacts_sidebar.h"
#include "ui_contacts_sidebar.h"

Contacts_SideBar::Contacts_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contacts_SideBar)
{
    ui->setupUi(this);

    QLinearGradient buttonGradient(0,0,0,ui->listWidget->width());
    buttonGradient.setColorAt(0,QColor(150,190,150));
    buttonGradient.setColorAt(1,QColor(180,220,180));

    QPalette palette = qApp->palette();
    palette.setBrush( QPalette::Base, buttonGradient );
    palette.setBrush( QPalette::Highlight, buttonGradient );
    palette.setBrush( QPalette::HighlightedText, Qt::white );
    palette.setBrush( QPalette::Text, Qt::white );

    ui->listWidget->setPalette(palette);

    QListWidgetItem *item1 = new QListWidgetItem(ui->listWidget);
    item1->setText("Customers");
    item1->setIcon( QIcon(":/images/customers.png") );

    QListWidgetItem *item2 = new QListWidgetItem(ui->listWidget);
    item2->setText("Vendors");
    item2->setIcon( QIcon(":/images/vendors.png") );

    QListWidgetItem *item3 = new QListWidgetItem(ui->listWidget);
    item3->setText("Team Mates");
    item3->setIcon( QIcon(":/images/teammates.png") );

}

Contacts_SideBar::~Contacts_SideBar()
{
    delete ui;
}
