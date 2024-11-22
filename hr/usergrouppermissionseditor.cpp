#include "usergrouppermissionseditor.h"
//#include "ui_usergrouppermissionseditor.h"
#include <QSqlRecord>
#include "login/loginvalues.h"


UserGroupPermissionsEditor::UserGroupPermissionsEditor(UserGroupDataModel *userGroup, QWidget *parent)  :
    QWidget(parent)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    ui->setupUi(this);


    this->setWindowTitle("Edit Permissions");
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:#ffffff}");
    QPalette pal = palette();

    // set black background
    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    dbHelper = new UserGroupDatabaseHelper;
    this->userGroup = userGroup;

    initObjects();
    setWidgets();
}

UserGroupPermissionsEditor::~UserGroupPermissionsEditor()
{
//    delete ui;
}

void UserGroupPermissionsEditor::setUserGroup(UserGroupDataModel *value)
{
    userGroup = value;
}

void UserGroupPermissionsEditor::disableSave()
{
    if(!LoginValues::permLogin)
        saveButton->hide();
}

void UserGroupPermissionsEditor::initUserGroup()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    userGroupEdit->show();
    userGroupEdit->setFocus();
    windowLabel->hide();
    appBar->setBackgroundColor(QColor("#ffffff"));
    //    windowLabel->setText(userGroup->Name);
}

void UserGroupPermissionsEditor::setWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    windowLabel = new QLabel(userGroup->Name);

    appBar->setBackgroundColor(QColor());

    QPalette palette = windowLabel->palette();
    //        palette.setColor(windowLabel->foregroundRole(), QColor(titleColor));
    //        palette.setColor(windowLabel->backgroundRole(), QColor(appBarColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont( QFont("Calibri", 18, QFont::Normal));

    appBar->setFixedHeight(100);
    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignLeft);
    appBar->appBarLayout()->addWidget(userGroupEdit, 0, Qt::AlignLeft);
    appBar->appBarLayout()->addWidget(editButton);

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;


    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    mainLayout->addWidget(appBar);

    QHBoxLayout *extrasLayout = new QHBoxLayout;
    extrasLayout->addStretch(1);
    extrasLayout->addWidget(checkCreateAll);
    extrasLayout->addWidget(checkReadAll);
    extrasLayout->addWidget(checkUpdateAll);
    extrasLayout->addWidget(checkDeleteAll);
    extrasLayout->addWidget(checkShareAll);
    extrasLayout->addWidget(filterLineEdit);
    QVBoxLayout *tableLayout = new QVBoxLayout;
    tableLayout->addWidget(tableView);


    mainLayout->addWidget(tabs);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(tableLayout);

    QWidget *mainWid = new QWidget;
    mainWid->setLayout(mainLayout);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);
    verticalLayout->addWidget(mainWid);

    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setParent(this);
    saveButton->setOffset(10,10);
    connect(saveButton, &QtMaterialFloatingActionButton::clicked, this, &UserGroupPermissionsEditor::savePermissions);


}

void UserGroupPermissionsEditor::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    delete model;


    model = new GMAccessControlModel(qryModel, userGroup );
    model->setHeaderStrings(QStringList {"#", "Screen", "Type", "Create", "Read", "Update", "Delete", "Share", "Select All"});


    sort->setSourceModel(model);
    tableView->setModel(sort);

}

void UserGroupPermissionsEditor::initObjects()
{
    uiHelper = new UiSettingsDatabaseHelper;
    appBar = new QtMaterialAppBar();

    //    userGroup->permissions = dbHelper->getPermissionsByUserGroupAndUi()
    qryModel = uiHelper->getAllUIList();
    model = new GMAccessControlModel(qryModel, userGroup);
    sort = new UACSortModel;
    sort->setModule(module);
    model->setHeaderStrings(QStringList {"#", "Screen", "Type", "Create", "Read", "Update", "Delete", "Share", "Select All"});
    tableView = new UserPermissionsTableView;
    tableView->setItemDelegate(new UserPermissionsDelegate(qryModel));
    sort->setSourceModel(model);
    sort->setDynamicSortFilter(true);
    tableView->setModel(sort);
    tableView->setEditTriggers(QTableView::AllEditTriggers);
    tableView->hideColumn(0);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    tabs = new QtMaterialTabs();
    tabs->setBackgroundColor(QColor("white"));
    tabs->setTextColor(QColor("#00bcd4"));
    tabs->setFixedHeight(50);

    tabs->addTab("Finance");
    tabs->addTab("Inventory");
    tabs->addTab("Purchases");
    tabs->addTab("Warehouse");
    tabs->addTab("HR");
    tabs->addTab("POS");
    tabs->addTab("Others");

    connect(tabs, &QtMaterialTabs::currentChanged, this, &UserGroupPermissionsEditor::tabSelected);

    for(int i=0; i<7; i++ ){
        allowCreateAll.append(false);
        allowUpdateAll.append(false);
        allowDeleteAll.append(false);
        allowReadAll  .append(false);
        allowShareAll .append(false);
    }


    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(this);
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this, &UserGroupPermissionsEditor::closeWindow);

    editButton = new QtMaterialIconButton(QtMaterialTheme::icon("image", "edit"));
    editButton->setParent(this);
    editButton->setIconSize(QSize(10,10));
    connect(editButton, &QtMaterialIconButton::clicked, this, &UserGroupPermissionsEditor::initUserGroup);

    snackBar = new QtMaterialSnackbar;
    userGroupEdit = new GMMaterialLineEdit(&userGroup->Name);
    //    userGroupEdit->setStyleSheet("GMMaterialLineEdit{background-color:#00bcd4;}");
    userGroupEdit->setLabelColor(QColor("#e0f7fa"));
    userGroupEdit->setLabel("Enter Group Name");
    userGroupEdit->setShowLabel(true);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    userGroupEdit->setParent(appBar);
    userGroupEdit->setFixedSize(200, 80);
    userGroupEdit->hide();
    //    userGroupEdit->setGeometry(100,50,100,100);
    connect(userGroupEdit, &GMMaterialLineEdit::editFinished, this, [=](){
        windowLabel->setText(userGroup->Name) ;
        userGroupEdit->hide();
        windowLabel->show();
        appBar->setBackgroundColor(QColor("#00bcd4"));

    });

    filterLineEdit = new QLineEdit();
    filterLineEdit->setPlaceholderText("Search.. ");
    filterLineEdit->setFrame(false);
    connect(filterLineEdit, &QLineEdit::textChanged, this, [=](){
        QRegExp regExp(filterLineEdit->text(),Qt::CaseInsensitive);

        sort->setFilterRegExp(regExp);
    });

    checkCreateAll = new QCheckBox;
    checkCreateAll->setText("Create All");
    connect(checkCreateAll, &QCheckBox::toggled, this, [=](bool v){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<v;
        for(int i=0; i<qryModel->rowCount(); i++){
            if(qryModel->record(i).value(3).toString().contains(module)){
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                bool entryExists = false;
                for(int j=0; j <userGroup->permissions.size(); j++){
                    qDebug()<<Q_FUNC_INFO<<__LINE__<<qryModel->record(i).value(0).toString()<<userGroup->permissions[j].ui_id;
                    if(qryModel->record(i).value(0).toInt() == userGroup->permissions[j].ui_id){
                        userGroup->permissions[j].allowCreate = v;
                        qDebug()<<Q_FUNC_INFO<<__LINE__<<v;
                        entryExists = true;
                        break;
                    }
                }
                if(!entryExists){
                    qDebug()<<Q_FUNC_INFO<<__LINE__<<"adding new entry";
                    UiAccessControlDataModel perm;
                    perm.ui_id = qryModel->record(i).value(0).toInt();
                    perm.UiName =qryModel->record(i).value(1).toString();
                    perm.allowCreate = v;
                    userGroup->permissions.append(perm);
                }
            }

        }
        allowCreateAll[tabs->currentIndex()] = v;
        resetWidgets();

    });

    checkReadAll = new QCheckBox;
    checkReadAll->setText("Read All");
    connect(checkReadAll, &QtMaterialCheckBox::toggled, this, [=](bool v){
        for(int i=0; i<qryModel->rowCount(); i++){
            if(qryModel->record(i).value(3).toString().contains(module)){
                bool entryExists = false;
                for(int j=0; j <userGroup->permissions.size(); j++){
                    if(qryModel->record(i).value(0).toInt() == userGroup->permissions[j].ui_id){
                        userGroup->permissions[j].allowRead = v;
                        entryExists = true;
                        break;
                    }
                }
                if(!entryExists){
                    UiAccessControlDataModel perm;
                    perm.ui_id = qryModel->record(i).value(0).toInt();
                    perm.UiName =qryModel->record(i).value(1).toString();
                    perm.allowRead = v;
                    userGroup->permissions.append(perm);
                }
            }

        }

            allowReadAll[tabs->currentIndex()] = v;
        resetWidgets();

    });

    checkUpdateAll = new QCheckBox;
    checkUpdateAll->setText("Update All");
    connect(checkUpdateAll, &QtMaterialCheckBox::toggled, this, [=](bool v){
        for(int i=0; i<qryModel->rowCount(); i++){
            if(qryModel->record(i).value(3).toString().contains(module)){
                bool entryExists = false;
                for(int j=0; j <userGroup->permissions.size(); j++){
                    if(qryModel->record(i).value(0).toInt() == userGroup->permissions[j].ui_id){
                        userGroup->permissions[j].allowUpdate = v;
                        entryExists = true;
                        break;
                    }
                }
                if(!entryExists){
                    UiAccessControlDataModel perm;
                    perm.ui_id = qryModel->record(i).value(0).toInt();
                    perm.UiName =qryModel->record(i).value(1).toString();
                    perm.allowUpdate = v;
                    userGroup->permissions.append(perm);
                }
            }

        }
        allowUpdateAll[tabs->currentIndex()] = v;
        resetWidgets();

    });

    checkDeleteAll = new QCheckBox;
    checkDeleteAll->setText("Delete All");
    connect(checkDeleteAll, &QtMaterialCheckBox::toggled, this, [=](bool v){
        for(int i=0; i<qryModel->rowCount(); i++){
            if(qryModel->record(i).value(3).toString().contains(module)){
                bool entryExists = false;
                for(int j=0; j <userGroup->permissions.size(); j++){
                    if(qryModel->record(i).value(0).toInt() == userGroup->permissions[j].ui_id){
                        userGroup->permissions[j].allowDelete = v;
                        entryExists = true;
                        break;
                    }
                }
                if(!entryExists){
                    UiAccessControlDataModel perm;
                    perm.ui_id = qryModel->record(i).value(0).toInt();
                    perm.UiName =qryModel->record(i).value(1).toString();
                    perm.allowDelete = v;
                    userGroup->permissions.append(perm);
                }
            }

        }
        allowDeleteAll[tabs->currentIndex()] = v;
        resetWidgets();

    });

    checkShareAll = new QCheckBox;
    checkShareAll->setText("Share All");
    connect(checkShareAll, &QtMaterialCheckBox::toggled, this, [=](bool v){
        for(int i=0; i<qryModel->rowCount(); i++){
            if(qryModel->record(i).value(3).toString().contains(module)){
                bool entryExists = false;
                for(int j=0; j <userGroup->permissions.size(); j++){
                    if(qryModel->record(i).value(0).toInt() == userGroup->permissions[j].ui_id){
                        userGroup->permissions[j].allowShare = v;
                        entryExists = true;
                        break;
                    }
                }
                if(!entryExists){
                    UiAccessControlDataModel perm;
                    perm.ui_id = qryModel->record(i).value(0).toInt();
                    perm.UiName =qryModel->record(i).value(1).toString();
                    perm.allowShare = v;
                    userGroup->permissions.append(perm);
                }
            }

        }
        allowShareAll[tabs->currentIndex()] = v;
        resetWidgets();

    });

}

void UserGroupPermissionsEditor::savePermissions()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__<<userGroup->Name<<userGroup->_id;
    //    dbHelper->deleteUserGroup(userGroup->_id);
    if(userGroup->Name.length() == 0){
        snackBar->addMessage("Enter Group Name");
        return;
    }

    if(userGroup->_id < 0)
        dbHelper->insertUserGroup(userGroup);
    else
        dbHelper->updateUserGroup(userGroup, userGroup->_id);
    closeWindow();

}

void UserGroupPermissionsEditor::closeWindow()
{
    emit closing();
    this->close();

}

void UserGroupPermissionsEditor::tabSelected(int index)
{

    switch (index) {
    case 0:
        module = "Finance";
        break;
    case 1:
        module = "Inventory";
        break;
    case 2:
        module = "Purchases";
        break;
    case 3:
        module = "Warehouse";
        break;
    case 4:
        module = "HR";
        break;
    case 5:
        module = "POS";
        break;
    case 6:
        module = "Others";
        break;
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<module;

    sort->setModule(module);
    QRegExp regExp(filterLineEdit->text(),Qt::CaseInsensitive);

    sort->setFilterRegExp(regExp);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    unsetCheckboxes();



}

void UserGroupPermissionsEditor::unsetCheckboxes()
{

//    return;
    checkCreateAll->blockSignals(true);
    checkReadAll->blockSignals(true);
    checkUpdateAll->blockSignals(true);
    checkDeleteAll->blockSignals(true);
    checkShareAll->blockSignals(true);

    if(allowCreateAll[tabs->currentIndex()]){
        checkCreateAll->setCheckState(Qt::Checked);
    }
    else {
        checkCreateAll->setCheckState(Qt::Unchecked);
    }
    if(allowReadAll[tabs->currentIndex()]){
        checkReadAll->setCheckState(Qt::Checked);
    }
    else {
        checkReadAll->setCheckState(Qt::Unchecked);
    }
    if(allowUpdateAll[tabs->currentIndex()]){
        checkUpdateAll->setCheckState(Qt::Checked);
    }
    else {
        checkUpdateAll->setCheckState(Qt::Unchecked);
    }
    if(allowDeleteAll[tabs->currentIndex()]){
        checkDeleteAll->setCheckState(Qt::Checked);
    }
    else {
        checkDeleteAll->setCheckState(Qt::Unchecked);
    }
    if(allowShareAll[tabs->currentIndex()]){
        checkShareAll->setCheckState(Qt::Checked);
    }
    else {
        checkShareAll->setCheckState(Qt::Unchecked);
    }

    checkCreateAll->blockSignals(false);
    checkReadAll->blockSignals(false);
    checkUpdateAll->blockSignals(false);
    checkDeleteAll->blockSignals(false);
    checkShareAll->blockSignals(false);

}
