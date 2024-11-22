#ifndef GMCONTACTSEARCH_H
#define GMCONTACTSEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPointer>
#include <QPoint>
#include <QFocusEvent>
#include "gmcontactsearchwidget.h"
#include "database/addressbook/addressbookdatabasehelper.h"

class GMContactSearch : public QLineEdit
{
     Q_OBJECT
public:
    GMContactSearch();

    explicit GMContactSearch(QList<QSharedPointer<AddressBookSmall>> itemsListPtr,QLineEdit *parent=Q_NULLPTR);
    ~GMContactSearch();

    QStringList list;
    AddressBookDatabaseHelper *itemsHelper;
    GMContactSearchWidget *listWidget;

protected:


public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(int,QString);
    void noneSelected();

signals:
    void SelectedItemID(AddressBookDataModel);
    void clearItem();

};

#endif // GMCONTACTSEARCH_H
