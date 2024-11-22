#ifndef CONTACTLISTWIDGETITEM_H
#define CONTACTLISTWIDGETITEM_H

#include <QWidget>

namespace Ui {
class ContactListWidgetItem;
}

class ContactListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit ContactListWidgetItem(QWidget *parent = 0);
    ~ContactListWidgetItem();
    int itemID;
    QFont nameFont;
    void setData(QString ContactName,QString CompanyName,QString phoneNumber,int id);
    int getID();
    QString getItemName();
private:
    Ui::ContactListWidgetItem *ui;
};

#endif // CONTACTLISTWIDGETITEM_H
