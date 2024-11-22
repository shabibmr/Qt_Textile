#ifndef TABLERESERVATIONEDITOR_H
#define TABLERESERVATIONEDITOR_H

#include <QWidget>
#include "database/Counter/TableManagement/tablereservationdatabasehelper.h"
#include "database/addressbook/addressbookdatabasehelper.h"

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"



#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/Basic/gmlineedit.h"


#include "customwidgets/SearchWidgets/Contact/gmcontactlinematerial.h"
#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Basic/gmtimeedit.h"

class TableReservationEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TableReservationEditor(TableReservationDataModel *reservation = new TableReservationDataModel(),
                                    TableReservationDatabaseHelper *dbHelper =new TableReservationDatabaseHelper(),
                                    QWidget *parent = nullptr);
    ~TableReservationEditor();

    TableReservationDatabaseHelper *dbHelper;
    TableReservationDataModel *reservation;
    AddressBookDatabaseHelper *contactsHelper;

    QtMaterialSnackbar *snackbar;
    QtMaterialFloatingActionButton *saveButton;
    QtMaterialIconButton *deleteButton;
    QtMaterialAppBar *appBar;

    GMMaterialLineEdit *tableName;
    GMPlainTextEdit *narration;

    GMContactLineMaterial *contactSearch;
    GMDateEdit *entryDate;
    GMDateEdit *resDate;
    GMTimeEdit *fromTime;
    GMTimeEdit *toTime;

    void initWidgets();
    void setWidgets();

public slots:

    void saveReservation();

    void deleteReservation();

    void disableSave();
signals:

    void closing();

};

#endif // TABLERESERVATIONEDITOR_H
