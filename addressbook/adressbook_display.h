#ifndef ADRESSBOOK_DISPLAY_H
#define ADRESSBOOK_DISPLAY_H

#include <QWidget>
#include <QPointer>
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "database/addressbook/addressbookdatabasehelper.h"
#include "addressbook.h"
#include <QKeyEvent>
#include "login/loginvalues.h"

namespace Ui {
class AdressBook_Display;
}

class AdressBook_Display : public QWidget
{
    Q_OBJECT

public:
    explicit AdressBook_Display(QWidget *parent = 0);
    ~AdressBook_Display();
    AddressBook *AddressBookWidget;
    AddressBookDatabaseHelper *dbHelper;

    QList<QSharedPointer<AddressBookSmall>> addList;


public slots:
    void settableCustomers();




private slots:
    void on_createNewPushButton_clicked();

    void on_customerTableWidget_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void on_customerSearch_textChanged(const QString &arg1);

private:
    Ui::AdressBook_Display *ui;
};

#endif // ADRESSBOOK_DISPLAY_H
