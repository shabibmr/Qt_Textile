#ifndef TABLESMASTEREDITOR_H
#define TABLESMASTEREDITOR_H

#include <QWidget>
#include "datamodels/Counter/tablemasterdatamodel.h"
#include "database/Counter/TableManagement/tablesmasterhelper.h"

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialappbar.h"



#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/gmplaintextedit.h"
#include "customwidgets/Basic/gmlineedit.h"


class TablesMasterEditor : public QWidget
{
    Q_OBJECT
public:
    explicit TablesMasterEditor(TableMasterDataModel *table= new TableMasterDataModel(),
                                TablesMasterHelper *dbHelper= new TablesMasterHelper(),
        QWidget *parent = nullptr);
    ~TablesMasterEditor();

    TableMasterDataModel *table;
    TablesMasterHelper *dbHelper;

    QString windowTitle = tr("Tables");

    QtMaterialSnackbar *snackbar;
    QtMaterialFloatingActionButton *saveButton;
    QtMaterialIconButton *deleteButton;
    QtMaterialIconButton *backButton;
    QtMaterialAppBar *appBar;


    GMLineEdit *tableName;

    QLineEdit *capacity;
    QLineEdit *min;
    QLineEdit *max;
    QLineEdit *floor;

    void initWidgets();
    void setWidgets();

public slots:

    void saveTable();

    void deleteTable();
      void disableSave();
signals:

    void closing();
};

#endif // TABLESMASTEREDITOR_H
