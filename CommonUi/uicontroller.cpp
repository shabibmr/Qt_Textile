#include "uicontroller.h"
#include <QMessageBox>

UiController::UiController(QObject* parent) : QObject (parent)

{
    dbName = DatabaseValues::connectionString;
    db = new DatabaseHelper(dbName);
    uiHelper = new UiSettingsDatabaseHelper(db);
    userGroupHelper = new UserGroupDatabaseHelper(db);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<dbName;


}

UiController::~UiController()
{

}

bool UiController::setUiData(int ui_id, bool configuredInDb, bool isRedirected)
{

    if(ui_id == 63 ){
        int id = ConfigurationSettingsDatabaseHelper::getValue(itemMasterQuery, 63).toInt();
        if(id != ui_id){
            ui_id = id;
        }
    }
    qDebug()<<configuredInDb<<ui_id<<Q_FUNC_INFO<<__LINE__;
    permissionsModel = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, ui_id);

    qDebug()<<LoginValues::permLogin<<ui_id<<Q_FUNC_INFO<<__LINE__;

    if(ui_id!=1005){
        if(LoginValues::permLogin ){
            if(ui_id != 75 && ui_id != 70 ){
                emit noPermissions();
                return false;
            }
        }

        else{
            if(!permissionsModel->allowRead){
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                emit noPermissions();
                return false;
            }
        }
    }
    qDebug()<<__LINE__;
    if(configuredInDb){
        uiModel = uiHelper->getUiConfigById(ui_id);
        qDebug()<<uiModel->ui_id<<uiModel->ui_name;
        if(uiModel->ui_name.length() == 0){
            QMessageBox box; box.setText("UI not found"); box.exec();
            return false;
        }
        else {
            initUI(isRedirected);
        }
    }
    else{
        reportSelector = new ReportsSelector(ui_id);
        connect(reportSelector, &ReportsSelector::reportOpened, this, [=](QWidget *wid, QString title){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<title;
            emit uiSet(wid, title);
        });
//        connect(reportSelector, &ReportsSelector::closin)
        reportSelector->selectReport();
    }
}

void UiController::initUI(bool isRedirect)
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    if(uiModel->ui_type == "Report")
    //    {
    if(isRedirect){
        qDebug()<<"redirecting report"<<uiModel->ui_name<<mapForRedirect;
        reportWid->setUiModel(uiModel, IsSwitchTab);
        QMapIterator<QString, QPair<QString, QVariant>> i(mapForRedirect);
        while(i.hasNext()){
            i.next();
            reportWid->setFilterValue(i.key(), i.value().second.toString(), i.value().first);
        }
    }
    else {
        if(reportWid != nullptr)
            delete reportWid;
        reportWid = new ReportWidget(db);
        reportWid->setPermissions(permissionsModel);
        reportWid->setUiModel(uiModel);
        connect(reportWid, &ReportWidget::redirecting, this, [=](int row){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            redirectUi(row);
        });
        connect(reportWid, &ReportWidget::setTabTitle, this, [=](QString title){
            emit setTabTitle(title);
        });
    }
    reportWid->setTableView();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
    connect(reportWid, &ReportWidget::tabChanged, this, &UiController::switchTab);
    connect(reportWid, &ReportWidget::editMaster, this, &UiController::openMasterEditor);
    connect(reportWid, &ReportWidget::voucherSaved, this, &UiController::getReorderModel);
    connect(reportWid, &ReportWidget::showWidget, this, [=](){
        emit showUi(dbName);
    });
    connect(reportWid, &ReportWidget::closing, this, [=](){
        emit closing();

    });

    //    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<reportWid<<uiModel->ui_name;
    emit uiSet(reportWid, uiModel->ui_name);

}

void UiController::redirectUi(int row)
{
    qDebug()<<"trying to redirect"<<row;
    qDebug()<<reportWid->uiModel->redirect_to;
    QJsonArray redirectArray = reportWid->uiModel->redirect_to["redirect-to"].toArray();
    qDebug()<<redirectArray;
    QJsonObject defaultWid;
    for(QJsonValue val:redirectArray){
        defaultWid = val["default"].toObject();
        qDebug()<<defaultWid;
        QJsonArray keyList = defaultWid["keyList"].toArray();
        for(QJsonValue key:keyList){
            QPair<QString, QVariant> filterPair;
            filterPair.first = key["filter-type"].toString();
            filterPair.second = "";
            if(key["col-pos"].toInt() > 0)
                filterPair.second = reportWid->model->record(row).value(key["col-pos"].toInt());
            else
                filterPair.second = reportWid->filterMap[key["var-value"].toString()].second;

            mapForRedirect.insert(key["var-name"].toString(), filterPair);
        }
    }

    qDebug()<<mapForRedirect<<Q_FUNC_INFO;

    if(defaultWid["id"].toInt() > 0)
        setUiData(defaultWid["id"].toInt(), true, true);

}

void UiController::switchTab(int ui_id)
{
    IsSwitchTab = true;
    setUiData(ui_id, true);
    qDebug()<<"switched";
}

void UiController::openMasterEditor(QString masterType, QString ID)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<masterType<<ID;

    masterEditor = new MastersEditor(db);
    masterEditor->setPermissions(permissionsModel);
    connect(masterEditor, &MastersEditor::editorOpen, this, [=](QWidget *wid, QString title){
        qDebug()<<Q_FUNC_INFO<<"open master "<<uiModel->ui_name;
//        wid->setWindowFlag(Qt::WindowStaysOnTopHint);
        wid->show();
        wid->setWindowTitle(title);
    });
    connect(masterEditor, &MastersEditor::refresh, reportWid, &ReportWidget::setTableView);
    masterEditor->openEditor(masterType, ID);
}

void UiController::getReorderModel()
{
    bool showFlag = ConfigurationSettingsDatabaseHelper::getValue(showReorderPopup, false).toBool();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(showFlag){
        InventoryHelper *invHelper = new InventoryHelper;
        QSqlQueryModel *model = invHelper->getReOrderLevelReport(QDate::currentDate());

        if(model->rowCount() > 0)
            emit showReorderStats();
    }

}


void UiController::setDefaultFilters(QString key, QString value, QString filterType)
{
    reportWid->clearUiStates();
    reportWid->setFilterValue(key,value, filterType);
    //        reportWid->setTableView();
}

void UiController::setDefaultVal(QString val)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    reportSelector->setDefaults(val);

}

QSqlQueryModel *UiController::getReportModel(int ui_id,QMap<QString, QPair<QString, QString> > filterMap)
{

    uiModel = uiHelper->getUiConfigById(ui_id);
    reportWid = new ReportWidget(db);
    reportWid->setFilterMap(filterMap);
    return reportWid->prepareQueryModel(uiModel->query);

}

QTableView *UiController::getTableView(QMap<QString, QPair<QString, QString> > filterMap)
{
    reportWid->setFilterMap(filterMap);
    reportWid->setTableView();
    return reportWid->tableView;
}

