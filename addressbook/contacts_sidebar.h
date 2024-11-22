#ifndef CONTACTS_SIDEBAR_H
#define CONTACTS_SIDEBAR_H

#include <QWidget>

namespace Ui {
class Contacts_SideBar;
}

class Contacts_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Contacts_SideBar(QWidget *parent = 0);
    ~Contacts_SideBar();

private:
    Ui::Contacts_SideBar *ui;
};

#endif // CONTACTS_SIDEBAR_H
