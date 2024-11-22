#ifndef USERGROUPPERMISSIONSEDITOR_H
#define USERGROUPPERMISSIONSEDITOR_H

#include <QWidget>
#include <QTableView>
#include <QPointer>
#include <QHeaderView>
#include <QLabel>

#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialcheckbox.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"

#include "customwidgets/TableViews/userpermissionstableview.h"

#include "database/Settings/uisettingsdatabasehelper.h"
#include "database/hr/usergroupdatabasehelper.h"

#include "customwidgets/Models/gmaccesscontrolmodel.h"
#include "customwidgets/Delegates/userpermissionsdelegate.h"
#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "hr/uacsortmodel.h"

//namespace Ui {
//class UserGroupPermissionsEditor;
//}

class UserGroupPermissionsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit UserGroupPermissionsEditor(UserGroupDataModel *userGroup, QWidget *parent = nullptr);
    ~UserGroupPermissionsEditor();

    QtMaterialAppBar *appBar;
    UserGroupDataModel *userGroup;

    UserGroupDatabaseHelper *dbHelper;
    UiSettingsDatabaseHelper *uiHelper;

    UserPermissionsTableView *tableView;

    GMAccessControlModel *model;
    QSqlQueryModel *qryModel;
    QString module = "Finance";

    QtMaterialFloatingActionButton *saveButton;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *editButton;
    QtMaterialSnackbar *snackBar;

    UACSortModel *sort;
    QLineEdit *filterLineEdit;
    QtMaterialTabs *tabs;
    QCheckBox *checkCreateAll;
    QCheckBox *checkUpdateAll;
    QCheckBox *checkDeleteAll;
    QCheckBox *checkReadAll;
    QCheckBox *checkShareAll;

    QList<bool> allowCreateAll;
    QList<bool> allowUpdateAll;
    QList<bool> allowDeleteAll;
    QList<bool> allowReadAll  ;
    QList<bool> allowShareAll ;





    GMMaterialLineEdit *userGroupEdit;
    QLabel *windowLabel;

    void initUserGroup();


    void setUserGroup(UserGroupDataModel *value);
    void disableSave();

private slots:
    void setWidgets();
    void resetWidgets();
    void initObjects();
    void savePermissions();
    void closeWindow();
    void tabSelected(int index);
    void unsetCheckboxes();

signals:
    void closing();

//private:
//    Ui::UserGroupPermissionsEditor *ui;
};

#endif // USERGROUPPERMISSIONSEDITOR_H
