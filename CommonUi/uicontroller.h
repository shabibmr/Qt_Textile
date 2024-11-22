#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include "reportwidget.h"
#include "database/Settings/uisettingsdatabasehelper.h"
#include "datamodels/hr/uiaccesscontroldatamodel.h"
#include "database/hr/usergroupdatabasehelper.h"
#include "CommonUi/reportsselector.h"

#include <QObject>

class UiController : public QObject
{
    Q_OBJECT
public:
    explicit UiController(QObject* parent =nullptr);
    ~UiController();
    UiSettingsDataModel *uiModel;
    UiSettingsDatabaseHelper *uiHelper;
    UserGroupDatabaseHelper *userGroupHelper;
    DatabaseHelper *db;
    UiAccessControlDataModel *permissionsModel;
    ReportsSelector *reportSelector;

    QPointer<ReportWidget> reportWid;
    MastersEditor *masterEditor;
    QMap<QString,QPair<QString,QVariant>> mapForRedirect;

    QList<ReportWidget *> uiList;
    bool IsSwitchTab = false;


    bool setUiData(int ui_id, bool configuredInDb = true, bool isRedirected = false);
    void initUI(bool isRedirect =false);

    void setDefaultFilters(QString key, QString value, QString filterType);
    void setDefaultVal(QString val);
    QString dbName;

    QSqlQueryModel *getReportModel(int ui_id, QMap<QString , QPair<QString, QString >> filterMap);
    QTableView *getTableView(QMap<QString, QPair<QString, QString> > filterMap);

public slots:
    void getReorderModel();
private slots:
    void redirectUi(int row);
    void switchTab(int ui_id);
    void openMasterEditor(QString masterType, QString ID);

signals:
    void uiSet(QWidget *, QString )    ;
    void showUi(QString);
    void closing();
    void noPermissions();
    void showReorderStats();
    void setTabTitle(QString);
};

#endif // UICONTROLLER_H
