#include "restuarantcountermainwindow.h"
#include "ui_restuarantcountermainwindow.h"
#include <math.h>
#include <QFontDatabase>
#include "sharetools/email/emailthread.h"
#include "login/loginpage.h"
#include <QScreen>
#include <QHeaderView>

RestuarantCounterMainWindow::RestuarantCounterMainWindow(int id,QString voucherPrefix, QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::RestuarantCounterMainWindow)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    ui->setupUi(this);
    this->setObjectName("POSMain");
    // this->setStyleSheet("QWidget#POSMain{background-color:#a9a9a9}");
    this->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
    CounterSettingsDatabaseHelper* settingsDB = new CounterSettingsDatabaseHelper();
    settingsDB->getCounterSettings(LoginValues::voucherPrefix);
    CompanyProfileDataBaseHelper* companyHelper = new CompanyProfileDataBaseHelper();
    LoginValues::setCompany(companyHelper->getCompanyDetails());

    allowEmptyAddressFlag =  ConfigurationSettingsDatabaseHelper::getValue(allowEmptyAddress,true).toBool();

    QDesktopWidget *desktop = QApplication::desktop();
    QRect screenGeometry = desktop->availableGeometry();

    // ui->verticalLayout_8->addStretch(0);
    //    this->setMaximumHeight(screenGeometry.height());

    settings = new SettingsHelper();

    QFont new_font = qApp->font();
    int fnSize = settings->getValue("FontSize", new_font.pointSize()).toInt();

    new_font.setPointSize( fnSize); //your option
    //    new_font.setWeight( int ** ); //your option
    qApp->setFont( new_font );

    showModFlag =  ConfigurationSettingsDatabaseHelper::getValue(showPriceListForSaveVoucher,false).toBool();
    printCancelledKOTFlag = ConfigurationSettingsDatabaseHelper::getValue(printCancelledKOT,true).toBool();
    deleteKOTItemWithoutRemarksFlag = ConfigurationSettingsDatabaseHelper::getValue(deleteKOTItemWithoutRemarks,true).toBool();
    serveOnlyPrefix = ConfigurationSettingsDatabaseHelper::getValue(ServeOnlyPrefix,true).toBool();
    autoAllotRefTableFlag = ConfigurationSettingsDatabaseHelper::getValue(autoAllotRefTable,false).toBool();
    bool hidetillButton = ConfigurationSettingsDatabaseHelper::getValue(hideOpenTillButton,false).toBool();
    ui->printOnOff->setCheckable(true);
    ui->dineInToolButton->hide();
    ui->takeAwayToolButton->hide();
    ui->deliveryToolButton->hide();
    //    ui->actionDaily_Closing->setVisible(false);

    ui->openTillButton->setIcon(QtMaterialTheme::icon("action", "lock_open"));
    ui->openTillButton->setHidden(hidetillButton);
    connect(ui->openTillButton,&QToolButton::clicked,this,&RestuarantCounterMainWindow::openTill);

    ui->airportButton->hide();
    ui->switchButton->hide();

    uiCtrl = new UiController;
    connect(uiCtrl, &UiController::noPermissions, this,
            [=](){
                showPermissionDenied();
            });

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    workOrderHelper = new WorkOrderDatabaseHelper();
    deletedSalesHelper = new salesVoucherDeletedDatabaseHelper();
    salesOrderHelper = new SalesOrderDatabaseHelper();
    priceHelper = new PricelistDatabaseHelper();
    uomHelper = new UomDataBaseHelper();
    voucher = new GeneralVoucherDataObject();
    deletedvoucher = new GeneralVoucherDataObject();

    ledHelper = new LedgerMasterDatabaseHelper();
    accHelper = new AccountGroupMasterDatabaseHelper();
    userHelper = new UserProfileDatabaseHelper();
    userGroupHelper = new UserGroupDatabaseHelper();
    printer = new CounterPrinterHelper();


    AddressDialogWidget = new AddressDialog(ledHelper,this);
    ModeOfService = new ModeOfServiceWidget(voucher,priceHelper,ledHelper);
    QObject::connect(ModeOfService,&ModeOfServiceWidget::itemPricesChanged,[=]{
        setTable();
    });

    if(id>0)
    {
        UserGroupDatabaseHelper urHelper ;
        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
        QString qStrx = "SELECT ";
        qStrx += userHelper->Employee_Details_Username + ", ";
        qStrx += userHelper->Employee_Details_Password + ", ";
        qStrx += userHelper->Employee_Details_Email    + ", ";
        qStrx += userHelper->Employee_Details_Name +", ";
        qStrx += userHelper->Employee_Details_id + ", ";
        qStrx += userHelper->Employee_Details_Phone + ", ";
        qStrx += userHelper->Employee_Details_User_Group + " ";
        qStrx += " FROM ";
        qStrx += userHelper->Employee_Details_TableName + " WHERE ";
        qStrx += userHelper->Employee_Details_id +"='"+QString::number(id)+"'";
        if(query.exec(qStrx)){
            while(query.next()){
                LoginValues::setUserName(query.value(0).toString());
                LoginValues::setUserEmail(query.value(2).toString());
                LoginValues::setEmpName(query.value(3).toString());
                LoginValues::setUserID(query.value(4).toInt());
                LoginValues::setUserPhone(query.value(5).toString());
                LoginValues::setModel(urHelper.getUserGroup(query.value(6).toInt()));
            }
        }
    }

    if(voucherPrefix != "0")
        LoginValues::setVoucherPrefix(voucherPrefix);



    colCount = ConfigurationSettingsDatabaseHelper::getValue(posColCount,6).toInt();
    buttWidth = ConfigurationSettingsDatabaseHelper::getValue(posButtonWidth,130).toInt();
    buttHeight = ConfigurationSettingsDatabaseHelper::getValue(posButtonHeight,40).toInt();

    disableItemEdit = ConfigurationSettingsDatabaseHelper::getValue(disableItemEditPOS,false).toBool();
    allowNegativeStock = ConfigurationSettingsDatabaseHelper::getValue(allowNegativeBilling,false).toBool();
    allowOrderCopyPrint = ConfigurationSettingsDatabaseHelper::getValue(POSallowSOCopy,true).toBool();

    SalesGroupsWidget = new SalesGroups(colCount,buttHeight,buttWidth,this);
    SalesItemsWidget = new SalesItems(colCount,buttHeight,buttWidth,this);

    // ui->label_3->setText(tr("Table"));

    ui->groupLayout->addWidget(SalesGroupsWidget);
    ui->itemLayout->addWidget(SalesItemsWidget);
    ui->modeServiceLay->addWidget(ModeOfService);
    QString taxNameStr = ConfigurationSettingsDatabaseHelper::getValue(taxName,"GST").toString();
    ui->vATAt5Label->setText(taxNameStr);


    customerLine = new QLineEdit(this);
    customerLine->setPlaceholderText(tr("Customer Name"));
    customerLine->setMinimumHeight(28);
    ui->customerLayour->addWidget(customerLine);

    kotGenerator = new WorkOrderGenerator(this);
    tabKotGenerator = new WorkOrderGenerator(this);

    bool showCustomerNameinPOSflag = ConfigurationSettingsDatabaseHelper::getValue(showCustomerNameinPOS,false).toBool();
    if(!showCustomerNameinPOSflag)
        customerLine->hide();
    askToPrintKOT = ConfigurationSettingsDatabaseHelper::getValue(askForPrint,false).toBool();
    saveOnRefreshFlag = ConfigurationSettingsDatabaseHelper::getValue(saveOnRefresh,false).toBool();
    lang = ConfigurationSettingsDatabaseHelper::getValue(POSLang,0).toInt();


    bool showPrintOnOffFlag = ConfigurationSettingsDatabaseHelper::getValue(showPrintOnOff,false).toBool();
    if(!showPrintOnOffFlag)
        ui->printOnOff->hide();


    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ui->label_2->hide();
        ui->label_3->hide();
        ui->selectTableToolButton->hide();
        ui->selectWaiterToolButton->hide();
        ui->switchButton->hide();
        ui->actionWaiterwise_Sales->setVisible(false);
        //        ui->actionKotPrinters->setVisible(false);
        ui->actionZomato_Report->setVisible(false);
        ui->actionKotPrinters->setText("Gate Pass");
    }

    ui->pickCallToolButton->hide();

    // ui->label_3->setText("Cabin");
    // ui->label_2->setText("Stylist");

    //    ui->switchButton->hide();

    ui->actionVAT_Report->setVisible(false);
    bool showAdjFlag = ConfigurationSettingsDatabaseHelper::getValue(showAdj,false).toBool();

    if(!showAdjFlag)
    {
        ui->actionSalesAdjustment->setVisible(false);
    }

    notificationWidget = new NotificationsList();

    //        ui->actionSalesAdjustment->setVisible(false);
    //                ui->modeOfPayToolButton->hide();
    //                ui->extrasToolButton->hide();
    //                ui->cashOutToolButton->hide();


    //    ui->plus50ToolButton->hide();

    ui->actionPayable->setVisible(false);
    ui->actionReceivable->setVisible(false);

    //    ui->actionProfit_and_Loss->setVisible(false);

    //    ui->actionTrial_Balance->setVisible(false);
    //    ui->actionBalance_Sheet->setVisible(false);




    SalesGroupsWidget->setTable();

    QObject::connect(SalesGroupsWidget,SIGNAL(selectSalesGroupID(QString)),this,
                     SLOT(groupButtonClicked(QString)));



    //    QObject::connect(SalesGroupsWidget,SIGNAL(selectFavouritesGroupID(QString)),SalesItemsWidget,
    //                     SLOT(groupButtonFavouriteClicked(QString)));

    QObject::connect(SalesItemsWidget,SIGNAL(selectSalesItemID(QString)),this,
                     SLOT(itemButtonClicked(QString)));


    itemsQuery = new QSqlQueryModel;
    QString qStr = "SELECT ";
    qStr += itemsHelper->Sales_Inventory_ItemId;
    qStr +=+", ";
    qStr += itemsHelper->Sales_Inventory_ItemName;
    qStr += " from "+ itemsHelper->Sales_Inventory_Table_Name;
    //    qStr += " where " + itemsHelper->Sales_Inventory_isSalesItem + "=1";

    itemsQuery->setQuery(qStr,QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<itemsQuery->rowCount();

    Inclusive = CounterSettingsDataModel::inclusiveVat;
    serial = new QSerialPort(this);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &RestuarantCounterMainWindow::handleError);

    connect(serial, &QSerialPort::readyRead, this, &RestuarantCounterMainWindow::readData);

    openSerialPort();


    QSqlQueryModel* itemsModel = itemsHelper->getInventoryItemsQueryModelForSearch(1);
    itemLine = new GMItemSearchLite(itemsModel);

    itemLine->setMinimumWidth(ui->tableWidget->width());
    QToolButton* SearchWid = new QToolButton;
    SearchWid->setAutoRaise(true);
    SearchWid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);

    QVBoxLayout *lay = new QVBoxLayout();
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    itemLine->setMinimumHeight(30);
    lay->addWidget(itemLine);
    SearchWid->setLayout(lay);
    SearchWid->setContentsMargins(0,0,0,0);

    bar = new QtMaterialSnackbar(this);

    notification = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("social", "notifications_active"));
    notification->setMini(true);
    //    notification->setXOffset(10);
    //    notification->setYOffset(10);
    notification->setCorner(Qt::BottomLeftCorner);
    notification->setParent(this);
    notification->setToolTip("New Notification");
    notification->hide();

    animation = new QPropertyAnimation(notification,"geometry");

    animation->setDuration(10000/2);

    QSize nSize = notification->size();

    animation->setStartValue(notification->geometry());
    QEasingCurve curve;
    curve.setType(QEasingCurve::InOutElastic);
    animation->setEndValue(notification->geometry());
    animation->setEasingCurve(curve);

    badge = new QtMaterialBadge(this);
    badge->setParent(notification);
    badge->setRelativePosition(18, 18);
    badge->setText("3");

    animation->start();

    connect(notification,&QPushButton::clicked,
            this,&RestuarantCounterMainWindow::openNotifications);

    QObject::connect(itemLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                     this,SLOT(addNewItemBySearch(inventoryItemDataModel)));

    QObject::connect(SearchWid,SIGNAL(clicked(bool)),itemLine,SLOT(setFocus()));

    QObject::connect(itemLine,SIGNAL(plusButton()),
                     this,SLOT(on_plusToolButton_clicked()));

    QObject::connect(itemLine,SIGNAL(minusButton()),
                     this,SLOT(on_minusToolButton_clicked()));

    QObject::connect(itemLine,SIGNAL(astriskpressed()),
                     this,SLOT(on_itemDeleteToolButton_clicked()));

    QObject::connect(itemLine,SIGNAL(astriskpressed()),
                     this,SLOT(on_itemDeleteToolButton_clicked()));

    QObject::connect(itemLine,SIGNAL(SalesOrderSelectedSignal(QString)),
                     this,SLOT(SalesOrderSelected(QString)));


    ui->searchLay->addWidget(SearchWid);

    ui->deliveryToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->deliveryToolButton->setAutoRaise(true);
    ui->dineInToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->dineInToolButton->setAutoRaise(true);
    ui->takeAwayToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->takeAwayToolButton->setAutoRaise(true);

    initDraftedBillsWidget();

    setMainScreen();

    connectMenuActions();

    customerScreenWidget = new CustomerScreen(this);
    customerScreenWidget->setWindowFlags(Qt::Window);
    customerScreenWidget->setWindowTitle("Orders");

    bool hideCustomerScreenF = settings->getValue("hideCustomerScreen", true).toBool();

    //    if(!hideCustomerScreenF)
    hideCustomerScreen(hideCustomerScreenF);

    this->setFocus();
    refreshPage();

    loadLanguage();

    // ui->verticalWidget1->updateGeometry();

    // QRect g= ui->tableWidget->geometry();
    // g.setHeight(100);
    // ui->tableWidget->setGeometry(g);
    // qDebug()<<" Resto Cons";
}

RestuarantCounterMainWindow::~RestuarantCounterMainWindow()
{
    delete ui;
}

void RestuarantCounterMainWindow::initDraftedBillsWidget()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 46);
    if(!perm->allowRead && draftsCount >0){
        // checking drafts count so that message doesnt show durng start up
        showPermissionDenied();
        return ;
    }

    SavedDraftsWidget = new RestaurantSavedDrafts(deletedSalesHelper,
                                                  salesOrderHelper,
                                                  accHelper,ledHelper,salesHelper, this);

    SavedDraftsWidget->setWindowFlags(Qt::Window);
    //    SavedDraftsWidget->setAttribute(Qt::WA_DeleteOnClose);
    SavedDraftsWidget->setPermissions(perm);
    SavedDraftsWidget->setTabWidget();


    QObject::connect(SavedDraftsWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(setVoucher(GeneralVoucherDataObject*)));
    QObject::connect(SavedDraftsWidget,SIGNAL(directCheckOutSignal()),
                     this,SLOT(on_cashOutToolButton_clicked()));

    QObject::connect(SavedDraftsWidget,&RestaurantSavedDrafts::directCheckOutSignal,
                     this,[=]{
                         AddressDialogWidget->hide();
                     });
    QObject::connect(SavedDraftsWidget,&RestaurantSavedDrafts::VoucherModeSelected,this,[=](GeneralVoucherDataObject *v){
        voucher = v;
        ModeOfPaySlot();
    }
                     );
}

bool RestuarantCounterMainWindow::checkDraftsPermission()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 46);
    if(!perm->allowRead && draftsCount >0){
        // checking drafts count so that message doesnt show durng start up
        showPermissionDenied();
        return false;
    }
    return true;
}

void RestuarantCounterMainWindow::on_plusToolButton_clicked()
{

    int row = ui->tableWidget->currentRow();

    if(row>-1 && voucher->InventoryItems.length()>row ){
        if(voucher->InventoryItems[row].BaseItem.listId>0 && disableItemEdit)
            return;

        if(!allowNegativeStock)
            if(voucher->InventoryItems[row].BaseItem.isStockItem == 1 &&
                voucher->InventoryItems[row].BaseItem.quantity+1 >
                    voucher->InventoryItems[row].BaseItem.ClosingStock ){
                return;
            }

        if(voucher->InventoryItems[row].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
            freeCount +=0;
        }
        else{
            if(voucher->InventoryItems[row].BaseItem.price == 0 ){
                if(freeUsed < freeCount){
                    freeUsed++;
                }
                else{
                    return;
                }

            }
        }

        voucher->InventoryItems[row].BaseItem.quantity++;

        voucher->InventoryItems[row].BaseItem.listId = 0;
        setTable();
        selectRow(row);
    }
}

void RestuarantCounterMainWindow::on_minusToolButton_clicked()
{
    int row = ui->tableWidget->currentRow();

    if(row >- 1 && voucher->InventoryItems.length() > row ){
        if(voucher->InventoryItems[row].BaseItem.listId>0 && disableItemEdit)
            return;
        if( voucher->InventoryItems[row].BaseItem.quantity == 0 )
            return;
        if( voucher->InventoryItems[row].BaseItem.quantity - 1 >0 ){

            if(voucher->InventoryItems[row].BaseItem.listId>0){
                deletedvoucher->InventoryItems.append(voucher->InventoryItems[row]);
                deletedvoucher->InventoryItems[deletedvoucher->InventoryItems.size()-1].BaseItem.quantity = 1;
            }
            voucher->InventoryItems[row].BaseItem.quantity--;
            if(voucher->InventoryItems[row].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
                freeCount -= 0;
                if(freeUsed>freeCount){
                    for(int i=2;i>0;i--){
                        if(row+i< voucher->InventoryItems.size()){
                            if(voucher->InventoryItems[row+i].BaseItem.price == 0 ){
                                if(voucher->InventoryItems[row+i].BaseItem.quantity - 1 == 0 ){
                                    if(voucher->InventoryItems[row+i].BaseItem.listId>0)
                                        deletedvoucher->InventoryItems.append(voucher->InventoryItems[row]);
                                    voucher->InventoryItems.removeAt(row+i);
                                }
                                else{
                                    voucher->InventoryItems[row+i].BaseItem.quantity--;
                                    voucher->InventoryItems[row+i].BaseItem.listId = 0;
                                }
                                freeUsed--;
                            }
                        }
                    }

                }
            }
            else{
                // if item is not combo
                if(freeCount>0){
                    freeUsed--;
                }

            }
            voucher->InventoryItems[row].BaseItem.listId = 0;
        }
    }

    setTable();
    selectRow(row);
}

void RestuarantCounterMainWindow::on_itemDeleteToolButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row>-1){
        if(voucher->InventoryItems[row].BaseItem.listId>0 && disableItemEdit)
            return;
        if(voucher->InventoryItems[row].BaseItem.listId>0)
            deletedvoucher->InventoryItems.append(
                voucher->InventoryItems[row]);

        if(voucher->InventoryItems[row].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
            int delFreeCount = voucher->InventoryItems[row].BaseItem.quantity*2;
            freeCount -= 0;
            int mrow = row;
            while(delFreeCount>0 && mrow+1 < voucher->InventoryItems.size()){
                qDebug()<<"IN While"<<mrow+1
                    ;
                if(voucher->InventoryItems[mrow+1].BaseItem.price == 0 ){
                    delFreeCount -= voucher->InventoryItems[mrow+1].BaseItem.quantity;
                    deletedvoucher->InventoryItems.append(
                        voucher->InventoryItems[mrow+1]);
                    voucher->InventoryItems.removeAt(mrow+1);
                    freeUsed -= voucher->InventoryItems[mrow+1].BaseItem.quantity;
                }
                else{
                    mrow++;
                }
                qDebug()<<"Next  Check";
            }


        }
        else{
            if(voucher->InventoryItems[row].BaseItem.price == 0 ){
                freeUsed -= voucher->InventoryItems[row].BaseItem.quantity;
            }
        }
        voucher->InventoryItems.removeAt(row);
        setTable();
        selectRow(row>0?row-1:0);
    }
}

void RestuarantCounterMainWindow::SalesOrderSelected(QString vno)
{
    GeneralVoucherDataObject* vo= salesOrderHelper->getVoucherByVoucherNoPtr(vno,LoginValues::voucherPrefix);
    itemLine->clear();
    if(vo->voucherNumber.length()>0 && vo->status!= QuotationStatus::SalesInvoiceRaised){
        setVoucher(vo);
    }
}

void RestuarantCounterMainWindow::refreshPage()
{
    //    saveVoucherStarted = false;
    cashOutToolButton->setDisabled(false);

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    previousTable = "";

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(voucher!=nullptr){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher<<"delte";
        delete voucher;
        //        qDebug()<<Q_FUNC_INFO<<__LINE__;
    }

    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(deletedvoucher!=nullptr){
        delete deletedvoucher;
    }
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher = new GeneralVoucherDataObject();
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    deletedvoucher = new GeneralVoucherDataObject();

    deletedvoucher->VoucherPrefix = LoginValues::voucherPrefix;
    deletedvoucher->AddedBy = QString::number(LoginValues::getUserID());
    deletedvoucher->ledgerObject = ledHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);

    //    qDebug()<<"refresh complete 174";
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->AddedBy = QString::number(LoginValues::getUserID());
    voucher->ledgerObject = ledHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    qDebug()<<"refresh complete 177";

    //    voucher->AddedBy = LoginValues::getUserName();

    ui->selectTableToolButton->setText(tr("Select Table"));
    ui->selectWaiterToolButton->setText(tr("Select Waiter"));
    voucher->VoucherDate = QDate::currentDate();
    voucher->DeliveryDate = QDateTime::currentDateTime();
    voucher->CustomerExpectingDate = voucher->DeliveryDate.addSecs(60*30);
    voucher->timestamp = QDateTime::currentDateTime();
    voucher->fromGodownID = GodownDatabaseHelper::getDefaultGodown();
    kotGenerator->setVoucher(voucher);

    freeCount = 0;
    freeUsed  = 0;

    //    qDebug()<<"refresh complete 182";
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher->status = QuotationStatus::Started;
    ui->BillNumber->setText(LoginValues::voucherPrefix+" - "+salesHelper->getNextVoucherNo(LoginValues::voucherPrefix));
    voucher->kotNumber = "";

    ui->subTotalLineEdit->clear();
    ui->vATAt5LineEdit->clear();
    ui->grandTotalLineEdit->clear();

    ui->contactLabel->clear();

    itemLine->clear();
    customerLine->clear();

    connect(customerLine,&QLineEdit::textChanged,this,[=](QString v){
        //        qDebug()<<"Text Changed to "<<v;
        voucher->Contact.ContactName = v;
    });


    //    on_takeAwayToolButton_clicked();

    //    on_dineInToolButton_clicked();

    ModeOfService->setVoucher(voucher);
    ModeOfService->setDefault();

    //    qDebug()<<"Default Cash : "  <<CounterSettingsDataModel::DefaultCash;

    //    on_takeAwayToolButton_clicked();

    setTable();

    //    QByteArray str = "WELCOME TO PTH      HAVE A NICE DAY !!! ";
    //    str.prepend(0x0C);
    //    writeData(str);


    //    voucher->SalesmanID=1;
    //    ui->selectWaiterToolButton->setText(userHelper->getEmployeeByID(voucher->SalesmanID).Name);
    //    qDebug()<<"refresh Completed";
}

void RestuarantCounterMainWindow::on_plus50ToolButton_clicked()
{
    int row = ui->tableWidget->currentRow();

    if(row>-1 && voucher->InventoryItems.length()>row ){
        voucher->InventoryItems[row].BaseItem.price += 0.5;
        setTable();
        selectRow(row);
    }
}

void RestuarantCounterMainWindow::on_extrasToolButton_clicked()
{
    //    QFont font;
    //    font.setFamily("Comic Sans MS");
    //    QApplication::setFont(font);

    PasswordEntry *PasswordEntryWidget = new PasswordEntry();
    PasswordEntryWidget->setWindowTitle("Enter Password");
    PasswordEntryWidget->setWindowFlags(Qt::Window);
    PasswordEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    PasswordEntryWidget->hideWidgets();
    PasswordEntryWidget->show();
    QObject::connect(PasswordEntryWidget,SIGNAL(loginSuccess()),this,SLOT(showExtrasWidget()));
    QObject::connect(PasswordEntryWidget,SIGNAL(loginFail()),this,SLOT(on_extrasToolButton_clicked()));

}

void RestuarantCounterMainWindow::on_draftedBillsToolButton_clicked()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 46);
    if(!perm->allowRead){
        showPermissionDenied();
        return;
    }

    // qDebug()<<"Vals";
    // initDraftedBillsWidget();
    // qDebug()<<"Its NUll";
    bool showTakeShortcutFlag = ConfigurationSettingsDatabaseHelper::getValue(showDraftedShortcuts,true).toBool();
    if(!showTakeShortcutFlag){
        SavedDraftsWidget->setTab(0);
    }
    else{
        SavedDraftsWidget->setTab(0,3);

    }
    SavedDraftsWidget->showFullScreen();

}

void RestuarantCounterMainWindow::changeTableAndDraft(){
    //    qDebug()<<Q_FUNC_INFO;
    changeTableToDraft();
    on_draftToolButton_clicked();
}

void RestuarantCounterMainWindow::on_draftToolButton_clicked()
{
    if(voucher->status == QuotationStatus::SalesInvoiceRaised)
        return;

    qDebug()<<"Chk dft";
    if(deletedvoucher->InventoryItems.size()>0){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<"inv size"<<deletedvoucher->InventoryItems.size();
        PasswordEntry *pwdEntry = new PasswordEntry();
        pwdEntry->setKotDeleted(true);
        pwdEntry->show();
        connect(pwdEntry, &PasswordEntry::loginSuccessWithComment, this, [=](QString val, QString comment, int priv){
                if(priv > -1){
                    qDebug()<<"saveDel"<<deletedvoucher->InventoryItems.size();
                    //                if(!deleteKOTItemWithoutRemarksFlag && comment.length() <3)
                    //                    return;
                    saveDeleted(true, comment);
                    qDebug()<<"Call de 603";
                    on_draftToolButton_clicked();
                }
            },Qt::QueuedConnection);
        return;
    }

    if(voucher->InventoryItems.size()>0){
        qDebug()<<"Ref : "<<voucher->reference;
        if(voucher->reference.length() < 1){

            TableSelect *TSWidget = new TableSelect(voucher, salesOrderHelper,ledHelper,accHelper, priceHelper,this);
            TSWidget->setWindowFlags(Qt::Window);
            TSWidget->setAttribute(Qt::WA_DeleteOnClose);

            int next = ConfigurationSettingsDatabaseHelper::getVariableValue(TakeawayCount).toInt();
            next++;
            ConfigurationSettingsDatabaseHelper::upsertSettings(TakeawayCount,QString::number(next));
            TSWidget->setNewLineText(QString::number(next));
            voucher->reference = QString::number(next);
            QObject::connect(TSWidget,SIGNAL(selectTable()),
                             this,SLOT(changeTableAndDraft()));

            if(autoAllotRefTableFlag==true){
                TSWidget->on_createNewButton_clicked();
            }else{
                TSWidget->showFullScreen();
                TSWidget->setLineFocus();
            }
            if(voucher->ModeOfService == ServiceMode::Delivery){
                TSWidget->on_createNewButton_clicked();
            }



            //            if(!priceHelper->getPriceListNameByID(QString::number( voucher->ModeOfService)).contains("Dine",Qt::CaseInsensitive))
            //            {  TSWidget->close();}
            //            else
            return;
        }



        if(voucher->SalesmanID == 0 && voucher->ModeOfService != ServiceMode::Delivery){
            AssignSalesman *SelectWaiterWidget = new AssignSalesman(voucher,this);
            SelectWaiterWidget->setWindowFlags(Qt::Window);
            SelectWaiterWidget->setGroupName("");
            SelectWaiterWidget->setTable();
            SelectWaiterWidget->setAttribute(Qt::WA_DeleteOnClose);
            QObject::connect(SelectWaiterWidget,SIGNAL(selectEmployeeID(int)),
                             this,SLOT(changeWaiterNameAndDraft(int)));

            SelectWaiterWidget->showFullScreen();
            return;
        }

        saveDraft();
    }
}

void RestuarantCounterMainWindow::OKclickedOnDraft()
{
    QString password = passwordShiftLine->text();
    QString userpass;
    QString qStr = "SELECT password from  " +UserProfileDatabaseHelper::Employee_Details_TableName +"  where privilege > -1";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    bool flag = false;
    if(query.exec(qStr)){
        while(query.next()){
            if(query.value(0).toString() == password && password.length() > 0 ){
                flag = true;
            }
        }
    }
    if(flag == true ){
        passWordShiftDialog->close();
        saveDeleted();
        on_draftToolButton_clicked();
    }
}

void RestuarantCounterMainWindow::MailDeletedList(QString name)
{
    QString text = "";
    text+=LoginValues::company.CompanyName+"\n";
    text+="Branch:\t"+LoginValues::company.branch+"\n";
    text+="Time:\t"+QDateTime::currentDateTime().toString()+"<br>";
    if(deletedvoucher->narration.length()>0){
        text += " Message : "+deletedvoucher->narration+"<br>";
    }
    text+= "Item Name - Quantity<br>";

    int i;
    qDebug()<<Q_FUNC_INFO<<deletedvoucher->InventoryItems.size();

    //    for(i=0;i<deletedvoucher->InventoryItems.size();i++){
    //        text+= deletedvoucher->InventoryItems[i].BaseItem.ItemName + " - " + QString::number(deletedvoucher->InventoryItems[i].BaseItem.quantity)+"\n";
    //    }

    text += printer->getHtml(deletedvoucher);

    text+= "Cashier:\t"+LoginValues::empName;
    EmailThread* mail = new EmailThread();
    mail->subject =LoginValues::company.branch+ " - ORDER CANCELLED";
    mail->Message=text;
    mail->setToAddress(LoginValues::getCompany().toMailAddresses.split("|")[0]);
    mail->run();
}

void RestuarantCounterMainWindow::OKclickedOnSaveVoucher()
{
    QString password = passwordShiftLine->text();
    QString userpass;
    QString qStr = "SELECT password,privilege from  " + UserProfileDatabaseHelper::Employee_Details_TableName +"  where password=:pass";
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    bool flag = false;
    query.prepare(qStr);
    query.bindValue(":pass",password);

    if(query.exec()){
        while(query.next()){
            if(query.value(1).toInt() > -1 ){
                flag = true;
            }
        }
    }
    if(flag == true ){
        passWordShiftDialog->close();
        saveDeleted();
        saveVoucher();
    }
}

void RestuarantCounterMainWindow::saveDeleted(bool sendMail, QString narration){

    deletedvoucher->reference = voucher->reference;
    deletedvoucher->SalesmanID = voucher->SalesmanID;
    deletedvoucher->narration = narration;
    deletedvoucher->voucherNumber = voucher->voucherNumber;

    qDebug()<<Q_FUNC_INFO<<deletedvoucher->InventoryItems.size();
    if(sendMail)
        MailDeletedList();
    TransactionCalculator::VoucherCalculateSales(deletedvoucher);
    deletedSalesHelper->insertVoucher(deletedvoucher);
    if(printCancelledKOTFlag){
        printer->printCancelledKOT(deletedvoucher);
    }
    deletedvoucher->InventoryItems.clear();
}

void RestuarantCounterMainWindow::on_modeOfPayToolButton_clicked()
{

    qDebug()<<" Qty flag is set to "<<qtyFlag;
    if(qtyFlag)
        return;
    //    bool ok;
    //    QString text = QInputDialog::getText(this, tr("Customer Details"),
    //                                         tr("Customer Name:"), QLineEdit::Normal,
    //                                         voucher->Contact.ContactName, &ok);
    //    if (ok && !text.isEmpty())
    //    {
    //        voucher->Contact.ContactName = text;
    if(voucher->InventoryItems.size()>0){
        ModeOfPayWidget = new ModeOfPay(voucher,ledHelper,salesOrderHelper, false,this);
        ModeOfPayWidget->setWindowFlags(Qt::Window);
        ModeOfPayWidget->showFullScreen();
        ModeOfPayWidget->setAttribute(Qt::WA_DeleteOnClose);

        QObject::connect(ModeOfPayWidget,SIGNAL(modeOfPaySignal()),this,SLOT(ModeOfPaySlot()));
        QObject::connect(ModeOfPayWidget,SIGNAL(resetPrice()),ModeOfService,SLOT(recalculatePrice()));
        QObject::connect(ModeOfPayWidget,SIGNAL(resetTable()),this,SLOT(setTable()));
    }


}

void RestuarantCounterMainWindow::ModeOfPaySlot(){
    calcTotals();
    saveVoucher();
}

void RestuarantCounterMainWindow::on_cashOutToolButton_clicked()
{
    if(voucher->InventoryItems.size()>0){
        LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
        voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
        qDebug()<<"Select Cash ID : "<<voucher->ledgerObject.LedgerID;
        voucher->NoOfCopies = CounterSettingsDataModel::CashOutPrintCopies;
        printKotCopy = false;
        if(ui->printOnOff->isChecked()){
            voucher->NoOfCopies=0;
        }
        saveVoucher();

        //        bool ok;
        //        QString text = QInputDialog::getText(this, tr("Customer Details"),
        //                                             tr("Customer Name:"), QLineEdit::Normal,
        //                                             voucher->Contact.ContactName, &ok);
        //        if (ok && !text.isEmpty())
        //        {
        //            voucher->Contact.ContactName = text;
        //            saveVoucher();
        //        }

    }
}

void RestuarantCounterMainWindow::on_pickCallToolButton_clicked()
{
    QString number;

    number = dStr.split('=')[3];
    //    qDebug()<<number.left(number.length() - 2);
    number=number.left(number.length() - 2);
    //    number = number.right(number.length()-1);
    //    QString number;

    //    number = dStr.simplified().replace(" ","");

    ui->pickCallToolButton->setHidden(true);

    ModeOfService->setService(1);
    ModeOfService->openAddressDialog(number.right(number.length()));


    return;

}

//void RestuarantCounterMainWindow::getCustomerDetails(QString Address, QString Phone, QString Name, QString route)
//{
//    //    delete AddressDialogWidget;
//    voucher->Contact.address= Address;
//    voucher->Contact.PhoneNumber= Phone;
//    voucher->Contact.ContactName= Name;
//    voucher->ledgerObject.ContactPersonNumber = Phone;
//    voucher->ledgerObject.emailAddress = Address;
//    voucher->ledgerObject.ContactPersonName = Name;
//    voucher->Contact.route = route;

//    ui->contactLabel->setText(Name);

//    ui->takeAwayToolButton->blockSignals(true);
//    ui->dineInToolButton->blockSignals(true);

//    voucher->ModeOfService = ServiceMode::Delivery;

//    ui->takeAwayToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->dineInToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->deliveryToolButton->setChecked(true);
//    ui->deliveryToolButton->setIcon(QIcon(":/icons/check.ico"));

//    ui->takeAwayToolButton->blockSignals(false);
//    ui->dineInToolButton->blockSignals(false);
//}


//void RestuarantCounterMainWindow::justGetCustomerDetails(QString Address, QString Phone, QString Name, QString route)
//{
//    //    delete AddressDialogWidget;

//    voucher->Contact.address= Address;
//    voucher->Contact.PhoneNumber= Phone;
//    voucher->Contact.ContactName= Name;
//    voucher->Contact.route = route;
//    voucher->ledgerObject.ContactPersonNumber = Phone;
//    voucher->ledgerObject.emailAddress = Address;
//    voucher->ledgerObject.ContactPersonName = Name;

//    ui->contactLabel->setText(Name);

//}

//void RestuarantCounterMainWindow::on_deliveryToolButton_clicked()
//{
//    qDebug()<<Q_FUNC_INFO<<voucher->Contact.address<<voucher->Contact.PhoneNumber<<voucher->Contact.ContactName<<voucher->Contact.route;
//    ui->takeAwayToolButton->blockSignals(true);
//    ui->dineInToolButton->blockSignals(true);

//    voucher->ModeOfService = ServiceMode::Delivery;

//    ui->takeAwayToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->dineInToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->deliveryToolButton->setChecked(true);
//    ui->deliveryToolButton->setIcon(QIcon(":/icons/check.ico"));

//    ui->takeAwayToolButton->blockSignals(false);
//    ui->dineInToolButton->blockSignals(false);

//    voucher->ledgerObject.defaultPriceListID ="";

//    AddressDialogWidget->hide();
//    AddressDialogWidget->setWindowFlags(Qt::Window);
//            AddressDialogWidget->setAddr(&voucher->Contact);
////    AddressDialogWidget->setAddressText();
//    AddressDialogWidget->setFocus();
//    AddressDialogWidget->show();
//    AddressDialogWidget->setFocustoPhone();
//    QObject::connect(AddressDialogWidget,SIGNAL(setAddress(QString,QString,QString, QString)),
//                     this,SLOT(getCustomerDetails(QString,QString,QString, QString)));
//}

//void RestuarantCounterMainWindow::on_takeAwayToolButton_clicked()
//{
//    ui->deliveryToolButton->blockSignals(true);
//    ui->dineInToolButton->blockSignals(true);

//    voucher->ModeOfService = ServiceMode::TakeAway;

//    ui->deliveryToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->dineInToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->takeAwayToolButton->setChecked(true);
//    ui->takeAwayToolButton->setIcon(QIcon(":/icons/check.ico"));

//    ui->deliveryToolButton->blockSignals(false);
//    ui->dineInToolButton->blockSignals(false);

//    voucher->ledgerObject.defaultPriceListID ="";

//    if(CounterSettingsDataModel::tokenSystem){
//        AddressDialogWidget->hide();
//        AddressDialogWidget->setWindowFlags(Qt::Window);
//        AddressDialogWidget->setAddr(&voucher->Contact);
////        AddressDialogWidget->setAddressText();
//        AddressDialogWidget->show();
//    }


//    QObject::connect(AddressDialogWidget,SIGNAL(setAddress(QString,QString,QString, QString)),
//                     this,SLOT(justGetCustomerDetails(QString,QString,QString, QString)));

//}

//void RestuarantCounterMainWindow::on_dineInToolButton_clicked()
//{
//    ui->takeAwayToolButton->blockSignals(true);
//    ui->deliveryToolButton->blockSignals(true);

//    voucher->ModeOfService = ServiceMode::DineIn;

//    ui->takeAwayToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->deliveryToolButton->setIcon(QIcon(":/ions/check.ico"));
//    ui->dineInToolButton->setChecked(true);
//    ui->dineInToolButton->setIcon(QIcon(":/icons/check.ico"));

//    ui->takeAwayToolButton->blockSignals(false);
//    ui->deliveryToolButton->blockSignals(false);

//    voucher->ledgerObject.defaultPriceListID ="";
//    if(CounterSettingsDataModel::tokenSystem){
//        AddressDialogWidget->hide();
//        AddressDialogWidget->setWindowFlags(Qt::Window);
//        AddressDialogWidget->setAddr(&voucher->Contact);
////        AddressDialogWidget->setAddressText();
//        AddressDialogWidget->show();
//    }
//    QObject::connect(AddressDialogWidget,SIGNAL(setAddress(QString,QString,QString, QString)),
//                     this,SLOT(justGetCustomerDetails(QString,QString,QString, QString)));

//}



void RestuarantCounterMainWindow::on_selectTableToolButton_clicked()
{
    TableSelect *TSWidget = new TableSelect(voucher,salesOrderHelper,ledHelper,accHelper, priceHelper,this);
    TSWidget->setWindowFlags(Qt::Window);
    TSWidget->setWindowTitle("Select Table");

    TSWidget->setAttribute(Qt::WA_DeleteOnClose);
    TSWidget->showFullScreen();
    TSWidget->setLineFocus();
    QObject::connect(TSWidget,SIGNAL(selectTable()),
                     this,SLOT(changeTable()));
}

void RestuarantCounterMainWindow::on_selectWaiterToolButton_clicked()
{
    AssignSalesman *SelectWaiterWidget = new AssignSalesman(voucher,this);
    SelectWaiterWidget->setWindowFlags(Qt::Window);
    SelectWaiterWidget->setWindowTitle("Select Waiter");
    SelectWaiterWidget->setGroupName("");
    SelectWaiterWidget->setAttribute(Qt::WA_DeleteOnClose);
    SelectWaiterWidget->setTable();
    QObject::connect(SelectWaiterWidget,SIGNAL(selectEmployeeID(int)),
                     this,SLOT(SwitchSalesMan(int)));
    SelectWaiterWidget->showFullScreen();

}


void RestuarantCounterMainWindow::setMainScreen()
{

    QPushButton *printOrderButton = new QPushButton(this);
    printOrderButton->setText("Print Order");
    printOrderButton->setMinimumHeight(40);
    ui->extrasLayout->insertWidget(0,printOrderButton);
    bool hideSalesOrderButton =  ConfigurationSettingsDatabaseHelper::getValue(hideSalesOrderPrintButton,false).toBool();
    printOrderButton ->setHidden(hideSalesOrderButton);
    connect(printOrderButton,&QPushButton::clicked,this,[=](){
        if(voucher->InventoryItems.length()>0)
            printer->printSalesOrder(voucher);
        else{
            bar->addMessage("No Items to print");
        }
    });

    // ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section:horizontal{"
    //                                                    "background-color: "
    //                                                    "QLinearGradient(x1:0, y1:0, x2:0, y2:1, "
    //                                                    "stop:0 #616161, stop: 0.5 #505050, stop: 0.6 #434343, stop:1 #656565); "
    //                                                    "color: white; padding-left: 4px; "
    //                                                    "border: none; border-radius: 10px;"
    //                                                    "border-top-right-radius: 3px;} ");
    // ui->tableWidget->setObjectName("POSMain");
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QFile inputFile3("debugdev.txt");
    if (!inputFile3.open(QIODevice::ReadOnly))
    {
        ui->menubar->hide(); //change for branch
    }
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    //    QTimer *timerBack = new QTimer(this);
    //    connect(timerBack, SIGNAL(timeout()), this, SLOT(showDayNight()));
    //    timerBack->start(1000);


    ui->dateLabel->setStyleSheet("color:#000000;");

    QDateTime t = QDateTime::currentDateTime();
    QString tText = t.toString("hh:mm:ss");
    QString tpli = tText.split(":").at(0);
    int hh = tpli.toInt();

    if(hh>12){
        hh=hh-12;
        if(hh == 0) {
            hh=12;
        }

        tText = QString::number(hh)+":"+tText.split(":").at(1)+":"+tText.split(":").at(2)+" PM";
    }
    else{
        if(hh == 0) { hh=12;}
        tText = QString::number(hh)+":"+tText.split(":").at(1)+":"+tText.split(":").at(2)+ " AM";
    }

    tText = t.toString("dd-MM-yyyy ")+tText;
    tText = t.toString("dd-MM-yyyy hh:mm:ss AP");
    ui->dateLabel->setText(tText);

    bool thisIsServer=false;
    QTimer *tabsync = new QTimer(this);
    connect(tabsync, SIGNAL(timeout()), this, SLOT(tabSync()));

    QFile inputFile2("kot.txt");
    if (inputFile2.open(QIODevice::ReadOnly))
    {
        isKot = true;
        modeOfPayToolButton->hide();
        extrasToolButton->hide();
        cashOutToolButton->hide();
        inputFile2.close();
    }


    QFile inputFile("server.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        thisIsServer = true;
        QString n = inputFile.readLine();
        if(n.contains("1")){
            qDebug()<<"THIS IS SERVER FOR SALES VOUCHER"<<n;
            serverSalesOrder = false;
        }
        inputFile.close();
    }
    else{
        //     QMessageBox box; box.setText("NO SERVER");box.exec();
    }
    if(thisIsServer)
    {
        //     QMessageBox box; box.setText("Server Started");box.exec();
        tabsync->start(3000);
    }
    else{
        qDebug()<<"NO SERVER FOUND";
    }

    //    ui->pickCallToolButton->setCheckable(true);

    ui->vATAt5LineEdit->setAlignment(Qt::AlignRight);
    ui->subTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->grandTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->tableWidget->setColumnCount(4);

    // ui->plusToolButton->setText(tr("Plus"));
    // ui->plusToolButton->setIcon(QIcon(":/images/plus.png"));
    // ui->plusToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->plusToolButton->setAutoRaise(true);

    // ui->minusToolButton->setText(tr("Minus"));
    // ui->minusToolButton->setIcon(QIcon(":/images/minus.png"));
    // ui->minusToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->minusToolButton->setAutoRaise(true);

    // ui->itemDeleteToolButton->setText(tr("Delete"));
    // ui->itemDeleteToolButton->setIcon(QIcon(":/images/delete.png"));
    // ui->itemDeleteToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->itemDeleteToolButton->setAutoRaise(true);

    // ui->refreshToolButton->setText(tr("Refresh"));
    // ui->refreshToolButton->setIcon(QIcon(":/images/refresh.png"));
    // ui->refreshToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->refreshToolButton->setAutoRaise(true);

    // ui->plus50ToolButton->setText(tr("Plus .50"));
    // ui->plus50ToolButton->setIcon(QIcon(":/images/plus50.png"));
    // ui->plus50ToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->plus50ToolButton->setAutoRaise(true);

    // ui->extrasToolButton->setText(tr("Extras"));
    // ui->extrasToolButton->setIcon(QIcon(":/images/admin.png"));
    // ui->extrasToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->extrasToolButton->setAutoRaise(true);

    // ui->draftedBillsToolButton->setText("Drafted Bills");
    // ui->draftedBillsToolButton->setIcon(QIcon(":/images/drafted.png"));
    // ui->draftedBillsToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->draftedBillsToolButton->setAutoRaise(true);

    // ui->draftToolButton->setText("Draft");
    // ui->draftToolButton->setIcon(QIcon(":/images/save.png"));
    // ui->draftToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->draftToolButton->setAutoRaise(true);

    // ui->modeOfPayToolButton->setText("Mode of Pay");
    // ui->modeOfPayToolButton->setIcon(QIcon(":/images/paymentmode.png"));
    // ui->modeOfPayToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->modeOfPayToolButton->setAutoRaise(true);

    // ui->cashOutToolButton->setText("Print Bill");
    // ui->cashOutToolButton->setIcon(QIcon(":/images/print.png"));
    // ui->cashOutToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ui->cashOutToolButton->setAutoRaise(true);

    // ui->plusToolButton->setIconSize(QSize(50,50));
    // ui->minusToolButton->setIconSize(QSize(50,50));
    // ui->itemDeleteToolButton->setIconSize(QSize(50,50));
    // ui->cashOutToolButton->setIconSize(QSize(50,50));
    // ui->modeOfPayToolButton->setIconSize(QSize(50,50));
    // ui->draftToolButton->setIconSize(QSize(50,50));
    // ui->extrasToolButton->setIconSize(QSize(50,50));
    // ui->plus50ToolButton->setIconSize(QSize(50,50));
    // ui->refreshToolButton->setIconSize(QSize(50,50));
    // ui->draftedBillsToolButton->setIconSize(QSize(50,50));

    // ui->plusToolButton->hide();
    // ui->minusToolButton->hide();
    // ui->itemDeleteToolButton->hide();
    // ui->cashOutToolButton->hide();
    // ui->modeOfPayToolButton->hide();
    // ui->draftToolButton->hide();
    // ui->extrasToolButton->hide();
    // ui->plus50ToolButton->hide();
    // ui->refreshToolButton->hide();
    // ui->draftedBillsToolButton->hide();


    plusToolButton= new QToolButton(this);
    minusToolButton= new QToolButton(this);
    itemDeleteToolButton= new QToolButton(this);
    cashOutToolButton= new QToolButton(this);
    modeOfPayToolButton= new QToolButton(this);
    draftToolButton= new QToolButton(this);
    extrasToolButton= new QToolButton(this);
    plus50ToolButton= new QToolButton(this);
    refreshToolButton= new QToolButton(this);
    draftedBillsToolButton= new QToolButton(this);


    QToolButton *tk = new QToolButton(this);
    QToolButton *del = new QToolButton(this);


    plusToolButton->setIcon(QIcon(":/images/plus.png"));
    minusToolButton->setIcon(QIcon(":/images/minus.png"));
    itemDeleteToolButton->setIcon(QIcon(":/images/delete.png"));
    cashOutToolButton->setIcon(QIcon(":/images/print.png"));
    modeOfPayToolButton->setIcon(QIcon(":/images/paymentmode.png"));
    draftToolButton->setIcon(QIcon(":/images/save.png"));
    extrasToolButton->setIcon(QIcon(":/images/admin.png"));
    plus50ToolButton->setIcon(QIcon(":/images/plus50.png"));
    refreshToolButton->setIcon(QIcon(":/images/refresh.png"));
    draftedBillsToolButton->setIcon(QIcon(":/images/drafted.png"));

    del->setIcon(QIcon(":/images/delivery.png"));
    tk->setIcon(QIcon(":/images/documentExport.ico"));



    int mwidth= ConfigurationSettingsDatabaseHelper::getValue(pOSIconWidth,50).toInt();
    int mheight= ConfigurationSettingsDatabaseHelper::getValue(pOSIconHeight,50).toInt();

    plusToolButton->setIconSize(QSize(mwidth,mheight));
    minusToolButton->setIconSize(QSize(mwidth,mheight));
    itemDeleteToolButton->setIconSize(QSize(mwidth,mheight));
    cashOutToolButton->setIconSize(QSize(mwidth,mheight));
    modeOfPayToolButton->setIconSize(QSize(mwidth,mheight));
    draftToolButton->setIconSize(QSize(mwidth,mheight));
    extrasToolButton->setIconSize(QSize(mwidth,mheight));
    plus50ToolButton->setIconSize(QSize(mwidth,mheight));
    refreshToolButton->setIconSize(QSize(mwidth,mheight));
    draftedBillsToolButton->setIconSize(QSize(mwidth,mheight));

    tk->setIconSize(QSize(mwidth,mheight));
    del->setIconSize(QSize(mwidth,mheight));


    // plusToolButton->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    // minusToolButton->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    // itemDeleteToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // cashOutToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // modeOfPayToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // draftToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // extrasToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // plus50ToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // refreshToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    // draftedBillsToolButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);



    plusToolButton->setText("Plus");
    minusToolButton->setText("Minus");
    itemDeleteToolButton->setText("Delete");
    cashOutToolButton->setText("Print");
    modeOfPayToolButton->setText("Mode OF Pay");
    draftToolButton->setText("Draft");
    extrasToolButton->setText("Extras");
    plus50ToolButton->setText("Plus .50");
    refreshToolButton->setText("Refresh");
    draftedBillsToolButton->setText("Drafted Bills");

    tk->setText("Take Away");
    del->setText("Delivery");



    int n =ui->tableWidget->height();



    plusToolButton->setAutoRaise(true);
    minusToolButton->setAutoRaise(true);
    itemDeleteToolButton->setAutoRaise(true);
    cashOutToolButton->setAutoRaise(true);
    modeOfPayToolButton->setAutoRaise(true);
    draftToolButton->setAutoRaise(true);
    extrasToolButton->setAutoRaise(true);
    plus50ToolButton->setAutoRaise(true);
    refreshToolButton->setAutoRaise(true);
    draftedBillsToolButton->setAutoRaise(true);

    tk->setAutoRaise(true);
    del->setAutoRaise(true);


    plusToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    minusToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    itemDeleteToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    cashOutToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    modeOfPayToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    draftToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    extrasToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    plus50ToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    refreshToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    draftedBillsToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    tk->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    del->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->verticalLayout_8->addWidget(plusToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(minusToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(itemDeleteToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(refreshToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(plus50ToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(extrasToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(draftedBillsToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(tk,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(del,1,Qt::AlignTop|Qt::AlignHCenter);

    ui->verticalLayout_8->addWidget(draftToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(modeOfPayToolButton,1,Qt::AlignTop|Qt::AlignHCenter);
    ui->verticalLayout_8->addWidget(cashOutToolButton,1,Qt::AlignTop|Qt::AlignHCenter);


    connect(plusToolButton,QToolButton::clicked,this,[=](bool x){on_plusToolButton_clicked();});
    connect(minusToolButton,QToolButton::clicked,this,[=](bool x){on_minusToolButton_clicked();});
    connect(itemDeleteToolButton,QToolButton::clicked,this,[=](bool x){on_itemDeleteToolButton_clicked();});
    connect(cashOutToolButton,QToolButton::clicked,this,[=](bool x){on_cashOutToolButton_clicked();});
    connect(modeOfPayToolButton,QToolButton::clicked,this,[=](bool x){on_modeOfPayToolButton_clicked();});
    connect(draftToolButton,QToolButton::clicked,this,[=](bool x){on_draftToolButton_clicked();});
    connect(extrasToolButton,QToolButton::clicked,this,[=](bool x){on_extrasToolButton_clicked();});
    connect(plus50ToolButton,QToolButton::clicked,this,[=](bool x){on_plus50ToolButton_clicked();});
    connect(refreshToolButton,QToolButton::clicked,this,[=](bool x){on_refreshToolButton_clicked();});
    connect(draftedBillsToolButton,QToolButton::clicked,this,[=](bool x){on_draftedBillsToolButton_clicked();});

    connect(tk,QToolButton::clicked,this,[=](bool x){
        on_draftedBillsToolButton_clicked();
        SavedDraftsWidget->setTab(0,2);
    });
    connect(del,QToolButton::clicked,this,[=](bool x){
        on_draftedBillsToolButton_clicked();
        SavedDraftsWidget->setTab(1);
    });

    bool showPlusButton = ConfigurationSettingsDatabaseHelper::getValue(showPlusFive,true).toBool();

    if(!showPlusButton){
        plus50ToolButton->hide();
    }

    bool showTakeShortcutFlag = ConfigurationSettingsDatabaseHelper::getValue(showDraftedShortcuts,true).toBool();
    if(!showTakeShortcutFlag){
        tk->hide();
        del->hide();
    }
    else{
        draftedBillsToolButton->setText("Dine In");
        draftedBillsToolButton->setIcon(QIcon(":/images/chair.png"));
    }
    // tk->hide();
    // del->hide();

    int i = 0;

    QTableWidgetItem* Item = new QTableWidgetItem;
    Item->setText(tr("Item Name"));
    ui->tableWidget->setHorizontalHeaderItem(i++,Item);

    QTableWidgetItem* qty = new QTableWidgetItem;
    qty->setText(tr("Qty"));
    ui->tableWidget->setHorizontalHeaderItem(i++,qty);

    QTableWidgetItem* price = new QTableWidgetItem;
    price->setText(tr("Price"));
    ui->tableWidget->setHorizontalHeaderItem(i++,price);

    QTableWidgetItem* total = new QTableWidgetItem;
    total->setText(tr("Total"));
    ui->tableWidget->setHorizontalHeaderItem(i++,total);

    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,50);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    // ui->tableWidget->setGeometry(ui->tableWidget->x(),ui->tableWidget->y(),ui->tableWidget->width(),n);
}
void RestuarantCounterMainWindow::setTable()
{
    if(voucher->InventoryItems.size()>0)
        calcTotals();

    int rowCount =0;
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(voucher->InventoryItems.length());

    if(freeCount!=freeUsed){
        itemLine->setPlaceholderText("Free Count :"+QString::number(freeCount)+"|"+QString::number(freeUsed  ));
    }
    else{
        itemLine->setPlaceholderText(tr("Search..."));
    }

    for(int i =0;i<voucher->InventoryItems.length();i++){
        int rsize = 30;
        if(voucher->InventoryItems[i].BaseItem.ItemName.length()>18)
            rsize = 60;
        ui->tableWidget->setRowHeight(rowCount,rsize);
        QTableWidgetItem* itemName = new QTableWidgetItem();
        QTableWidgetItem* Qty = new QTableWidgetItem();
        QTableWidgetItem* Price = new QTableWidgetItem();
        QTableWidgetItem* total = new QTableWidgetItem();

        //        QToolButton* edit = new QToolButton;

        //        edit->setProperty("row",i);
        //        QObject::connect(edit,SIGNAL(clicked(bool)),this,SLOT(openRestaurantItemDetail()));

        QFont font = itemName->font();
        font.setItalic(true);
        font.setWeight(QFont::Bold); // or
        if(voucher->InventoryItems[i].BaseItem.price==0)
            itemName->setFont(font);

        if(lang==0)
            itemName->setText(voucher->InventoryItems[i].BaseItem.ItemName);
        else if(voucher->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
            itemName->setText(voucher->InventoryItems[i].BaseItem.itemNameArabic);


        Qty->setText(QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',voucher->InventoryItems[i].BaseItem.uomObject.decimalPoints));

        //        qDebug()<<"Qty = "<<QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',voucher->InventoryItems[i].BaseItem.uomObject.decimalPoints);
        Price->setText(QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2));

        total->setText(QString::number(voucher->InventoryItems[i].BaseItem.subTotal,'f',2));

        Qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        Price->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        total->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);
        Qty->setFlags(Qty->flags() ^ Qt::ItemIsEditable);
        Price->setFlags(Price->flags() ^ Qt::ItemIsEditable);
        total->setFlags(total->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget->setItem(i,0,itemName);
        ui->tableWidget->setItem(i,1,Qty);
        ui->tableWidget->setItem(i,2,Price);
        ui->tableWidget->setItem(i,3,total);

        rowCount++;

        //VFD code
        QString word = voucher->InventoryItems[i].BaseItem.ItemName + "            ";
        QString subString = word.mid(0,13);
        //        QStringRef sub(word,0, 13);
        if(voucher->InventoryItems[i].BaseItem.subTotal+
                    voucher->InventoryItems[i].BaseItem.taxAmount>=10 && voucher->InventoryItems[i].BaseItem.subTotal+
                       voucher->InventoryItems[i].BaseItem.taxAmount<100)
            subString += " ";
        else if(voucher->InventoryItems[i].BaseItem.subTotal+
                         voucher->InventoryItems[i].BaseItem.taxAmount>=100 && voucher->InventoryItems[i].BaseItem.subTotal+
                            voucher->InventoryItems[i].BaseItem.taxAmount<1000)
            subString += "";
        else if(voucher->InventoryItems[i].BaseItem.subTotal+
                     voucher->InventoryItems[i].BaseItem.taxAmount<10)
            subString += "  ";
        subString += " " + QString::number(voucher->InventoryItems[i].BaseItem.subTotal+
                                               voucher->InventoryItems[i].BaseItem.taxAmount,'f',2);
        //        qDebug()<<"substring = "<<subString;
        QByteArray str = subString.toLocal8Bit();
        str.prepend(0x0C);
        writeData(str);
        word = "    TOTAL    ";
        if(voucher->grandTotal>=10 && voucher->grandTotal<100)
            word += "  ";
        else if(voucher->grandTotal>=100 && voucher->grandTotal<1000)
            word += " ";
        else if(voucher->grandTotal<10)
            word += "   ";
        else if(voucher->grandTotal>1000)
            word += "";

        word +=  QString::number(voucher->grandTotal,'f',2);
        //        qDebug()<<"WORD22 = "<<word;
        str= word.toLocal8Bit();
        //        str.prepend(0x0D);
        writeData(str);
        //        writeData(1B 46 42 [DATA X 40] 0D");
    }

    customerScreenWidget->setVoucher(voucher);
    customerScreenWidget->setTable();

    itemLine->setFocus();
}

void RestuarantCounterMainWindow::calcTotals()
{
    TransactionCalculator::VoucherCalculateSales(voucher);
    ui->subTotalLineEdit   -> setText(QString::number(voucher->subTotal,'f',2));
    ui->vATAt5LineEdit     -> setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->grandTotalLineEdit ->setText(QString::number(voucher->grandTotal,'f',2));

    if(voucher->taxTotalAmount<=0){
        ui->vATAt5LineEdit->hide();
        ui->vATAt5Label->hide();
        ui->subTotalLabel->hide();
        ui->subTotalLineEdit->hide();
    }
    else{
        ui->vATAt5LineEdit->show();
        ui->vATAt5Label->show();
        ui->subTotalLabel->show();
        ui->subTotalLineEdit->show();
    }

}

float RestuarantCounterMainWindow::getRoundOff(float r,int multiple){

    //    return r;
    int no = (int)r;

    float fraction = r-no;
    if(fraction <0.125){
        fraction = 0;
    }
    else if(fraction>=0.125&& fraction <0.375)
        fraction = .25;
    else if(fraction>=0.375&& fraction<0.625){
        fraction = .5;
    }
    else if(fraction>=0.625&& fraction<0.875){
        fraction = .75;
    }

    else if(fraction>=0.875){
        fraction = 1;
    }
    return fraction+no;
}

void RestuarantCounterMainWindow::settleVoucherAsCash(GeneralVoucherDataObject *voucher)
{
    this->voucher = voucher;

}

void RestuarantCounterMainWindow::switchPaymentMode(GeneralVoucherDataObject *voucher)
{
    this->voucher = voucher;

}

void RestuarantCounterMainWindow::openInventoryItems()
{


    //    inventoryItem* inventoryItemWidget = new inventoryItem(this);

    //    inventoryItemWidget->setWindowFlags(Qt::Window);
    //    //    inventoryItemWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    inventoryItemWidget->setWindowTitle("Inventory");
    //    inventoryItemWidget->showMaximized();
    //    inventoryItemWidget->setLineFocus();
    if(!uiCtrl->setUiData(63, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openUOMList()
{

    qDebug()<<"Uom clicked";
    //    UOM* UOMWidget = new UOM(this);
    //    UOMWidget->setWindowFlags(Qt::Window);
    //    UOMWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    UOMWidget->setWindowTitle("UOM List");
    //    UOMWidget->showMaximized();

    if(!uiCtrl->setUiData(64, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openAccountGroups()
{


    //    accountGroups* accountGroupsWidget = new accountGroups(this);
    //    accountGroupsWidget->setWindowFlags(Qt::Window);
    //    accountGroupsWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    accountGroupsWidget->setWindowTitle("Account Groups");
    //    accountGroupsWidget->showMaximized();
    if(!uiCtrl->setUiData(68, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openSettings()
{
    //    if(LoginValues::Privilege<6)
    //        return;
    Settings* SettingsWidget = new Settings(this);
    SettingsWidget->setWindowFlags(Qt::Window);
    SettingsWidget->setAttribute(Qt::WA_DeleteOnClose);
    SettingsWidget->setWindowTitle("Settings");
    SettingsWidget->show();
}

void RestuarantCounterMainWindow::openUserGroups()
{
    if(!uiCtrl->setUiData(75, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openMeasurements()
{
    if(!uiCtrl->setUiData(6000, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openTablesMaster()
{
    if(!uiCtrl->setUiData(5019, true))
        return;
    uiCtrl->reportWid->showMaximized();
}


void RestuarantCounterMainWindow::openHelpContents()
{
    HelpContents* HelpContentsWidget = new HelpContents(this);
    HelpContentsWidget->setWindowFlags(Qt::Window);
    HelpContentsWidget->setAttribute(Qt::WA_DeleteOnClose);
    HelpContentsWidget->setWindowTitle("Help Contents");
    HelpContentsWidget->show();
}

void RestuarantCounterMainWindow::openAbout()
{
    About* AboutWidget = new About(this);
    AboutWidget->setWindowFlags(Qt::Window);
    AboutWidget->setAttribute(Qt::WA_DeleteOnClose);
    AboutWidget->setWindowTitle("About");
    AboutWidget->show();
}

void RestuarantCounterMainWindow::openShiftWidget(ShiftDataModel *obj)
{
    ShiftEntryWidgett = new RestaurantShiftEntry(this);
    ShiftEntryWidgett->setShift(obj);
    ShiftEntryWidgett->setAttribute(Qt::WA_DeleteOnClose);
    ShiftEntryWidgett->setWindowFlags(Qt::Window);
    ShiftEntryWidgett ->showFullScreen();
    QObject::connect(ShiftEntryWidgett,SIGNAL(shiftClosed()),this,SLOT(refreshPage()));

    //    ShiftValuesEditor * Widget = new ShiftValuesEditor();
    //    Widget->setAttribute(Qt::WA_DeleteOnClose);
    //    Widget->setWindowFlags(Qt::Window);
    //    Widget ->showFullScreen();
}

void RestuarantCounterMainWindow::openPurchaseEntry()
{
    //    PurchaseEntry* PurchaseEntryWidget = new PurchaseEntry(this);
    //    PurchaseEntryWidget->setWindowTitle("Purchases");
    //    PurchaseEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    PurchaseEntryWidget->setWindowFlags(Qt::Window);
    //    PurchaseEntryWidget->showMaximized();

    if(!uiCtrl->setUiData(45, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openSalesEntry()
{
    //    SalesVoucherList *SalesVoucherWidget = new SalesVoucherList();
    //    SalesVoucherWidget->setWindowTitle("Sales");
    //    SalesVoucherWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    SalesVoucherWidget->setWindowFlags(Qt::Window);
    //    SalesVoucherWidget->showMaximized();

    if(!uiCtrl->setUiData(30, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openSalesAdjustment()
{

    if(LoginValues::Privilege<8)
        return;

    SalesAdjustmentWidget *SalesVoucherWidget = new SalesAdjustmentWidget();
    SalesVoucherWidget->setWindowTitle("Sales Adjustment");
    SalesVoucherWidget->setAttribute(Qt::WA_DeleteOnClose);
    SalesVoucherWidget->setWindowFlags(Qt::Window);
    SalesVoucherWidget->showMaximized();
}

void RestuarantCounterMainWindow::openReceiptEntry()
{
    //    ReceiptEntry* ReceiptEntryWidget = new ReceiptEntry(this);
    //    ReceiptEntryWidget->setWindowTitle("Receipts");
    //    ReceiptEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    ReceiptEntryWidget->setWindowFlags(Qt::Window);
    //    ReceiptEntryWidget->showMaximized();

    if(!uiCtrl->setUiData(49, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openReceiptNote()
{
    qDebug()<<Q_FUNC_INFO;
    if(!uiCtrl->setUiData(52, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openJournalEntry()
{
    //    journal* journalWidget = new journal(this);
    //    journalWidget->setWindowTitle("Journals");
    //    journalWidget->setWindowFlags(Qt::Window);
    //    journalWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    journalWidget->showMaximized();

    if(!uiCtrl->setUiData(50, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openPurchaseReturn()
{
    //    debitNote *debitNoteWidget = new debitNote(this);
    //    debitNoteWidget->setWindowTitle("Purchase Returns");
    //    debitNoteWidget->setWindowFlags(Qt::Window);
    //    debitNoteWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    debitNoteWidget->showMaximized();


    if(!uiCtrl->setUiData(54, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

//void RestuarantCounterMainWindow::openReceiptNote()
//{
//    if(LoginValues::Privilege<3)
//        return;
//    ReceiptNotes *receiptNoteWidget = new ReceiptNotes(this);
//    receiptNoteWidget->setWindowTitle("Receipt Notes");
//    receiptNoteWidget->setWindowFlags(Qt::Window);
//    receiptNoteWidget->setAttribute(Qt::WA_DeleteOnClose);
//    receiptNoteWidget->showMaximized();
//}


void RestuarantCounterMainWindow::openCreditNote()
{
    //    creditNote *debitNoteWidget = new creditNote(this);
    //    debitNoteWidget->setWindowTitle("Sales Returns");
    //    debitNoteWidget->setWindowFlags(Qt::Window);
    //    debitNoteWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    debitNoteWidget->showMaximized();

    if(!uiCtrl->setUiData(53, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openStockEntry()
{


    //    StockEntry *StockEntryWidget = new StockEntry(this);
    //    StockEntryWidget->setWindowTitle("Stock");
    //    StockEntryWidget->setWindowFlags(Qt::Window);
    //    StockEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    StockEntryWidget->showMaximized();

    if(!uiCtrl->setUiData(59, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openStockJournal()
{


    //    stockJournals *stockJournalsWidget = new stockJournals(this);
    //    stockJournalsWidget->setWindowTitle("Stock Journal");
    //    stockJournalsWidget->setWindowFlags(Qt::Window);
    //    stockJournalsWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    stockJournalsWidget->showMaximized();

    if(!uiCtrl->setUiData(58, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openDefectiveEntry()
{


    //    DefectiveList *defectiveListWidget = new DefectiveList(this);
    //    defectiveListWidget->setWindowTitle("Wastage Entry");
    //    defectiveListWidget->setWindowFlags(Qt::Window);
    //    defectiveListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    defectiveListWidget->showMaximized();

    if(!uiCtrl->setUiData(56, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openPurchaseOrders()
{


    qDebug()<<"PO order CLICKED";
    //    purchaseOrders *defectiveListWidget = new purchaseOrders(this);
    //    defectiveListWidget->setWindowTitle("Purchase Orders");
    //    defectiveListWidget->setWindowFlags(Qt::Window);
    //    defectiveListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    defectiveListWidget->showMaximized();

    if(!uiCtrl->setUiData(47, true))
        return;
    uiCtrl->reportWid->showMaximized();
}


void RestuarantCounterMainWindow::openReservations()
{
    if(!uiCtrl->setUiData(5020, true))
        return;
    uiCtrl->reportWid->showMaximized();

}


void RestuarantCounterMainWindow::openIndents()
{



    //    qDebug()<<"POS CLICKED";
    //    StockRequestList *defectiveListWidget = new StockRequestList(this);
    //    defectiveListWidget->setWindowTitle("Indents");
    //    defectiveListWidget->setWindowFlags(Qt::Window);
    //    defectiveListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    defectiveListWidget->showMaximized();

    if(!uiCtrl->setUiData(57, true))
        return;
    uiCtrl->reportWid->showMaximized();
}



void RestuarantCounterMainWindow::openStockRequest()
{


    //    StockRequestList *StockRequestListWidget = new StockRequestList(this);
    //    StockRequestListWidget->setWindowTitle("Stock Request");
    //    StockRequestListWidget->setWindowFlags(Qt::Window);
    //    StockRequestListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    StockRequestListWidget->showMaximized();

    if(!uiCtrl->setUiData(57, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_F9)  && (e->modifiers().testFlag(Qt::ControlModifier))
        && (e->modifiers().testFlag(Qt::AltModifier))
        ){
        MainWindow *mw = new MainWindow();
        mw->setAttribute(Qt::WA_DeleteOnClose);
        mw->setWindowTitle(LoginValues::getCompany().branch);
        mw->showMaximized();
        return;
    }

    if ((e->key() == Qt::Key_F12)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        bool v = settings->getValue("hideCustomerScreen",true).toBool();
        hideCustomerScreen(!v);
    }

    switch (e->key ()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:{
        qDebug()<<"Enter Pressed";
        if(!qtyFlag){
            if(!isKot)
                on_modeOfPayToolButton_clicked();
            else
                qtyFlag = false;
        }
        break;
    }
    case Qt::Key_F12 :
    {
        openTill();
        break;
    }
    case Qt::Key_F5 :
    {
        refreshPage();
        break;
    }
    case Qt::Key_F1 :{
        editQtyByKeyboard();
        break;
    }
    case  Qt::Key_F2 :{
        int row = ui->tableWidget->currentRow();
        openItemDetail(row);
        break;
    }
    case Qt::Key_F11:{
        emit switchCompany();
    }
    case Qt::Key_Plus:{
        on_plusToolButton_clicked();
        break;
    }
    case Qt::Key_Minus:{
        on_minusToolButton_clicked();
        break;
    }
    case Qt::Key_Asterisk:{
        on_itemDeleteToolButton_clicked();
        break;
    }
    case Qt::Key_Escape:
        emit minimizeMain();
    default:
        QMainWindow::keyPressEvent (e);
    }


}

void RestuarantCounterMainWindow::openPaymentEntry()
{
    //    Payment* PaymentEntryWidget = new Payment(this);
    //    PaymentEntryWidget->setWindowTitle("Payments");
    //    PaymentEntryWidget->setWindowFlags(Qt::Window);
    //    PaymentEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    PaymentEntryWidget->show();

    if(!uiCtrl->setUiData(48, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::closeApplication(){

    QMessageBox::StandardButton reply;

    reply= QMessageBox::question(this,"Exit??","Are you Sure You want to Close",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        qApp->exit();
    }
}

void RestuarantCounterMainWindow::openLedgers()
{

    //    ledgerMaster* ledgerMasterWidget = new ledgerMaster(this);
    //    ledgerMasterWidget->setWindowTitle("Ledger Masters");
    //    ledgerMasterWidget->setWindowFlags(Qt::Window);
    //    ledgerMasterWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    ledgerMasterWidget->showMaximized();
    if(!uiCtrl->setUiData(69, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openPriceLists()
{



    //    priceList* priceListWidget = new priceList(this);
    //    priceListWidget->setWindowTitle("Price List");
    //    priceListWidget->setWindowFlags(Qt::Window);
    //    priceListWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    priceListWidget->showMaximized();
    if(!uiCtrl->setUiData(66, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openLedgerReport()
{


    //    LedgerReport* LedgerReportWidget = new LedgerReport(this);
    //    LedgerReportWidget->setWindowTitle("Ledger Report");
    //    LedgerReportWidget->setWindowFlags(Qt::Window);
    //    LedgerReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    LedgerReportWidget->show();

    if(!uiCtrl->setUiData(14, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openGroupSummary()
{


    //    GroupwiseOutstanding* GroupSummaryWidget = new GroupwiseOutstanding(this);
    //    GroupSummaryWidget->setWindowTitle("Group Summary");
    //    GroupSummaryWidget->setWindowFlags(Qt::Window);
    //    GroupSummaryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    GroupSummaryWidget->show();

    if(!uiCtrl->setUiData(17, true))
        return;
    uiCtrl->reportWid->showMaximized();


}

void RestuarantCounterMainWindow::openDayBook()
{
    //    Daybook* DaybookWidget = new Daybook(this);
    //    DaybookWidget->setWindowTitle("DayBook");
    //    DaybookWidget->setWindowFlags(Qt::Window);
    //    DaybookWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    DaybookWidget->showMaximized();

    if(!uiCtrl->setUiData(13, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openTrialBalance()
{


    //    TrialBalance* TrialBalanceWidget = new TrialBalance(this);
    //    TrialBalanceWidget->setWindowTitle("Trial Balance");
    //    TrialBalanceWidget->setWindowFlags(Qt::Window);
    //    TrialBalanceWidget->show();

    if(!uiCtrl->setUiData(23, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openProfitLoss()
{
    //    if(LoginValues::Privilege<3)
    //        return;

    ProfitAndLossReport* ProfitLossWidget = new ProfitAndLossReport(this);
    ProfitLossWidget->setWindowTitle("Profit and Loss");
    ProfitLossWidget->setWindowFlags(Qt::Window);
    ProfitLossWidget->show();
}

void RestuarantCounterMainWindow::openBalanceSheet()
{
    //    if(LoginValues::Privilege<3)
    //        return;

    BalanceSheet* BalanceSheetWidget = new BalanceSheet(this);
    BalanceSheetWidget->setWindowTitle("Balance Sheet");
    BalanceSheetWidget->setWindowFlags(Qt::Window);
    BalanceSheetWidget->show();
}

void RestuarantCounterMainWindow::openNotifications()
{
    //    if(! userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1005)->allowRead){
    //        showPermissionDenied();
    //        return;
    //    }


    notificationWidget = new NotificationsList();
    notificationWidget->setWindowTitle("Notifications");
    notificationWidget->setWindowFlags(Qt::Window);
    notificationWidget->show();

    //    if(!uiCtrl->setUiData(1005, false))
    //        return;
    //    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openVATReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    VATReport* VATReportWidget = new VATReport(this);
    //    VATReportWidget->setWindowTitle("VAT Report");
    //    VATReportWidget->setWindowFlags(Qt::Window);

    //    VATReportWidget->show();
}

void RestuarantCounterMainWindow::openSalesMarginReport()
{

    //    SalesMarginReport* VATReportWidget = new SalesMarginReport(this);
    //    VATReportWidget->setWindowTitle("Sales Margin Report");
    //    VATReportWidget->setWindowFlags(Qt::Window);
    //    VATReportWidget->show();

    if(!uiCtrl->setUiData(32, true))
        return;
    uiCtrl->reportWid->showMaximized();
}



void RestuarantCounterMainWindow::openInventoryGroups()
{

    //        inventorygroup *InventoryGroupWidget = new inventorygroup(this);
    //        InventoryGroupWidget->setWindowTitle("Inventory Groups");
    //        InventoryGroupWidget->setWindowFlags(Qt::Window);
    //        InventoryGroupWidget->show();
    if(!uiCtrl->setUiData(62, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openTodaysBills()
{

    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 30);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }

    TodaysBills* TodaysBillsWidget = new TodaysBills(this);
    TodaysBillsWidget->showDateWidget(false);
    if(!perm->allowUpdate)
        TodaysBillsWidget->disableEdit();
    if(!perm->allowDelete)
        TodaysBillsWidget->disableDelete();

    TodaysBillsWidget->setWindowFlags(Qt::Window);
    TodaysBillsWidget->showFullScreen();
    TodaysBillsWidget->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(TodaysBillsWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),this,SLOT(setVoucher(GeneralVoucherDataObject*)));
    QObject::connect(TodaysBillsWidget,SIGNAL(deleteVoucher(GeneralVoucherDataObject*)),this,SLOT(deleteVoucher(GeneralVoucherDataObject*)));

}

void RestuarantCounterMainWindow::deleteVoucher(GeneralVoucherDataObject *obj)
{
    salesHelper->deleteVoucher(obj);

}


void RestuarantCounterMainWindow::openStockReport()
{
    //    StockReport* StockReportWidget = new StockReport(this);
    //    StockReportWidget->setWindowTitle("Stock Report");
    //    StockReportWidget->setWindowFlags(Qt::Window);
    //    StockReportWidget->show();
    //    if(LoginValues::Privilege<2)
    //        return;

    if(! userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1000)->allowRead){
        showPermissionDenied();
        return;
    }


    RestaurantClosingStock* restClose = new RestaurantClosingStock(this);
    //    ClsTestWid* restClose = new ClsTestWid(this);
    restClose->setWindowTitle("Stock Report");
    restClose->setAttribute(Qt::WA_DeleteOnClose);
    restClose->setWindowFlags(Qt::Window);
    restClose->showMaximized();


    //    if(!uiCtrl->setUiData(1000, false))
    //        return;
    //    uiCtrl->reportWid->showMaximized();

    //    emit uiSelected(1000,false);

}

void RestuarantCounterMainWindow::openItemwiseReport()
{


    //    ItemwiseReport* ItemwiseReportWidget = new ItemwiseReport(this);
    //    ItemwiseReportWidget->setWindowTitle("Item-wise Report");
    //    ItemwiseReportWidget->setWindowFlags(Qt::Window);
    //    ItemwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    ItemwiseReportWidget->showMaximized();
    if(lang == 0){
        if(!uiCtrl->setUiData(97, true))
            return;
    }
    else{
        if(!uiCtrl->setUiData(250, true))
            return;
    }

    uiCtrl->setDefaultFilters("f2", GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher), "voucherType");
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openGroupwiseReport()
{

    if(! userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1021)->allowRead){

        showPermissionDenied();
        return;
    }


    GroupwiseReport* GroupwiseReportWidget = new GroupwiseReport(this);
    GroupwiseReportWidget->setWindowTitle("Groupwise Report");
    GroupwiseReportWidget->setWindowFlags(Qt::Window);
    GroupwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    GroupwiseReportWidget->showMaximized();

    //        if(!uiCtrl->setUiData(1021, false))
    //            return;
    //        uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openServicewiseReport()
{


    //    ModeOfServiceWiseReport* ModeOfServiceWiseReportWidget = new ModeOfServiceWiseReport(this);
    //    ModeOfServiceWiseReportWidget->setWindowTitle("Service Wise Report");
    //    ModeOfServiceWiseReportWidget->setWindowFlags(Qt::Window);
    //    ModeOfServiceWiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    ModeOfServiceWiseReportWidget->showMaximized();

    if(!uiCtrl->setUiData(11, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openPaymentSummary()
{


    //    LedgerTransactionSummary* transSummaryWidget = new LedgerTransactionSummary(GMVoucherTypes::PaymentVoucher, this);
    //    transSummaryWidget->setWindowTitle("Ledgerwise Payments Summary");
    //    transSummaryWidget->setWindowFlags(Qt::Window);
    //    transSummaryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    transSummaryWidget->showMaximized();

    if(!uiCtrl->setUiData(31, true))
        return;
    uiCtrl->setDefaultFilters("f1", GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher), "voucherType");
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openReceiptSummary()
{


    //    LedgerTransactionSummary* transSummaryWidget = new LedgerTransactionSummary(GMVoucherTypes::ReceiptVoucher, this);
    //    transSummaryWidget->setWindowTitle("Ledgerwise Receipts Summary");
    //    transSummaryWidget->setWindowFlags(Qt::Window);
    //    transSummaryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    transSummaryWidget->showMaximized();

    if(!uiCtrl->setUiData(31, true))
        return;
    uiCtrl->setDefaultFilters("f1", GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher), "voucherType");
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::loadLanguage()
{
    if(lang==1){
        qDebug()<<"Language  to arabic";
        bool loadf = l.load(":/fonts/fonts/lang_ar.qm");
        bool aaw = qApp->installTranslator(&l);
        qDebug()<<loadf<<aaw;
    }
    else{
        bool loadf = l.load(":/font/fonts/lang_ar.qm");
        bool aaw = qApp->installTranslator(&l);
    }
    refreshPage();
}

void RestuarantCounterMainWindow::switchLang()
{
    if( lang == 0 ){
        lang = 1;
    }
    else {
        lang = 0;
    }

    qDebug()<<" switching to "<<lang;

    ConfigurationSettingsDatabaseHelper::upsertSettings(POSLang,lang);
    SalesGroupsWidget->setTable();
    setTable();



    loadLanguage();

    // qApp->exit();
    // QStringList args;
    // args<<"100";
    // args<<QString::number(LoginValues::userID)<<LoginValues::voucherPrefix<<DatabaseValues::dbName;
    // qDebug()<<Q_FUNC_INFO<<__LINE__<<args;
    // QProcess::startDetached(qApp->arguments()[0], args);

}

void RestuarantCounterMainWindow::openDailyClosingReport()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1023);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }

    DailyClosingReport* DailyClosingSalesReportWidget = new DailyClosingReport(this);
    // DailyClosingSalesReportWidget->setShift(shift);
    DailyClosingSalesReportWidget->setWindowTitle("Daily Closing Report");
    DailyClosingSalesReportWidget->setWindowFlags(Qt::Window);
    DailyClosingSalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    DailyClosingSalesReportWidget->setShowReport(true);
    DailyClosingSalesReportWidget->setData();
    DailyClosingSalesReportWidget->showMaximized();


}

void RestuarantCounterMainWindow::openWaiterwiseService()
{
    WaiterwiseServicewiseReport *widget = new WaiterwiseServicewiseReport;
    widget->setWindowTitle("Salesman-wise Servicewise Sales");
    widget->setWindowFlags(Qt::Window);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->showMaximized();


}

void RestuarantCounterMainWindow::openDailyClosing()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1023);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }
    ShiftDatabaseHelper shiftHelper;
    ShiftDataModel* shift = new ShiftDataModel;
    shift->voucherPrefix = "%";

    int lastBill = shiftHelper.getlastShiftDetailByVoucherPrefix("%").BillsTo.toInt();
    int currVNo = salesHelper->getMaxID();

    int soPending = salesOrderHelper->getSOPending();

    bool shiftNoPendingDraftVal =  ConfigurationSettingsDatabaseHelper::getValue(shiftNoPendingDraft,false).toBool();

    if(soPending >0 && shiftNoPendingDraftVal){
        QMessageBox box; box.setText(QString::number(soPending) + " Orders Pending to Checkout"); box.exec();
        return;
    }

    qDebug()<<"Curr : "<<currVNo;
    qDebug()<<"last Bill : "<<lastBill;

    //    if(currVNo>lastBill){
    //        QMessageBox box; box.setText("Un Closed Shifts Pending"); box.exec();
    //        return;
    //    }

    DailyClosingReport* DailyClosingSalesReportWidget = new DailyClosingReport(this);
    // DailyClosingSalesReportWidget->setShift(shift);
    if(!perm->allowCreate)
        DailyClosingSalesReportWidget->disableSave();
    DailyClosingSalesReportWidget->setWindowTitle("Daily Closing Report");
    DailyClosingSalesReportWidget->setWindowFlags(Qt::Window);
    DailyClosingSalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    DailyClosingSalesReportWidget->setData();
    DailyClosingSalesReportWidget->showMaximized();

}

void RestuarantCounterMainWindow::openShiftwiseReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;

    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1027);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }

    ShiftWiseReport* ShiftWiseReportWidget = new ShiftWiseReport(this);
    ShiftWiseReportWidget->setWindowTitle("Shift Wise Report");
    ShiftWiseReportWidget->setDeleteAllowed(perm->allowDelete);
    ShiftWiseReportWidget->setWindowFlags(Qt::Window);
    ShiftWiseReportWidget->showMaximized();
}

void RestuarantCounterMainWindow::openTimeBasedReport()
{
    //    TimeBasedSales* TimeBasedSalesWidget = new TimeBasedSales(this);
    //    TimeBasedSalesWidget->setWindowTitle("Time based Sales Report");
    //    TimeBasedSalesWidget->setWindowFlags(Qt::Window);
    //    TimeBasedSalesWidget->showMaximized();

    if(!uiCtrl->setUiData(12, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openWaiterwiseReport()
{

    //    WaiterWiseSalesReport* WaiterWiseSalesReportWidget = new WaiterWiseSalesReport(this);
    //    WaiterWiseSalesReportWidget->setWindowTitle("Waiterwise Sales Report");
    //    WaiterWiseSalesReportWidget->setWindowFlags(Qt::Window);
    //    WaiterWiseSalesReportWidget->showMaximized();

    if(!uiCtrl->setUiData(18, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openZomatoReport()
{
    //    if(LoginValues::Privilege<2)
    //        return;
    ZomatoReport* ZomatoReportWidget = new ZomatoReport(this);
    ZomatoReportWidget->setWindowTitle("Potafo Report");
    ZomatoReportWidget->setWindowFlags(Qt::Window);
    ZomatoReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    ZomatoReportWidget->showMaximized();
}

void RestuarantCounterMainWindow::openCustomerwiseReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;

    //    CustomerList* CustReportWidget = new CustomerList(this);
    //    CustReportWidget->setWindowTitle("Customer List");
    //    CustReportWidget->setWindowFlags(Qt::Window);
    //    CustReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    CustReportWidget->showMaximized();

    if(!uiCtrl->setUiData(34, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openComplimentaryReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    ComplimentarySalesReport* ComplimentarySalesReportWidget = new ComplimentarySalesReport(this);
    //    ComplimentarySalesReportWidget->setWindowTitle("Complimentary Report");
    //    ComplimentarySalesReportWidget->setWindowFlags(Qt::Window);
    //    ComplimentarySalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    ComplimentarySalesReportWidget->show();

    if(!uiCtrl->setUiData(21, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openDiscountReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    DiscountsReport* DiscountsReportWidget = new DiscountsReport(this);
    //    DiscountsReportWidget->setWindowTitle("Discounts Report");
    //    DiscountsReportWidget->setWindowFlags(Qt::Window);
    //    DiscountsReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    DiscountsReportWidget->show();

    if(!uiCtrl->setUiData(20, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openBillwiseReport()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 30);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }

    BillwiseReport* BillwiseReportWidget = new BillwiseReport(this);
    BillwiseReportWidget->setWindowTitle("Billwise Report");
    if(!perm->allowDelete)
        BillwiseReportWidget->disableDelete();
    BillwiseReportWidget->setValueTable();
    BillwiseReportWidget->setWindowFlags(Qt::Window);
    BillwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(BillwiseReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),this,SLOT(setVoucher(GeneralVoucherDataObject*)));
    BillwiseReportWidget->showMaximized();
}

void RestuarantCounterMainWindow::openDeletedBills()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    BillwiseReport* BillwiseReportWidget = new BillwiseReport(this);
    BillwiseReportWidget->setWindowTitle("Deleted Bills Report");
    BillwiseReportWidget->setDeleted(true);
    BillwiseReportWidget->setSalesHelper(deletedSalesHelper);
    BillwiseReportWidget->setValueTable();
    BillwiseReportWidget->setWindowFlags(Qt::Window);
    BillwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(BillwiseReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),this,SLOT(setVoucher(GeneralVoucherDataObject*)));
    BillwiseReportWidget->showMaximized();
}

void RestuarantCounterMainWindow::openDatewiseReport()
{

    //    DatewiseSalesReport* BillwiseReportWidget = new DatewiseSalesReport(this);
    //    BillwiseReportWidget->setWindowTitle("Billwise Report");
    //    BillwiseReportWidget->setWindowFlags(Qt::Window);
    //    BillwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    BillwiseReportWidget->show();

    if(!uiCtrl->setUiData(33, true))
        return;
    uiCtrl->reportWid->showMaximized();
}


void RestuarantCounterMainWindow::openSalesReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    TodaysBills* SalesReportWidget = new TodaysBills(this);
    SalesReportWidget->showDateWidget(true);
    SalesReportWidget->setWindowFlags(Qt::Window);
    SalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    SalesReportWidget->showFullScreen();
}

void RestuarantCounterMainWindow::openPendingOrders()
{
    //        if(LoginValues::Privilege<3)
    //            return;
    PendingOrders* PendingOrdersReportWidget = new PendingOrders(this);
    PendingOrdersReportWidget->setWindowFlags(Qt::Window);
    PendingOrdersReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    PendingOrdersReportWidget->showMaximized();

    //    uiCtrl->setUiData(60);
    //    uiCtrl->reportWid->showMaximized();
    QObject::connect(PendingOrdersReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(setVoucher(GeneralVoucherDataObject*)));
}

void RestuarantCounterMainWindow::openKOTReport()
{
    KOTReport *kotReportWidget = new KOTReport(this);
    kotReportWidget->setWindowFlags(Qt::Window);
    kotReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    kotReportWidget->showMaximized();
}

void RestuarantCounterMainWindow::openSalesOrders()
{
    //    if(LoginValues::Privilege<1)
    //        return;
    //    salesOrders* SalesOrdersWidget = new salesOrders(this);
    //    SalesOrdersWidget->setWindowFlags(Qt::Window);
    //    SalesOrdersWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    SalesOrdersWidget->showMaximized();

    if(!uiCtrl->setUiData(46, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openReOrderReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    ReOrderLevel* ReOrderLevelWidget = new ReOrderLevel(this);
    //    ReOrderLevelWidget->setWindowTitle("Re Order Level");
    //    ReOrderLevelWidget->setWindowFlags(Qt::Window);
    //    ReOrderLevelWidget->show();

    if(!uiCtrl->setUiData(9, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openOutstandingPayable()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    PaymentOutstanding* PaymentOutstandingWidget = new PaymentOutstanding(this);
    //    PaymentOutstandingWidget->setWindowTitle("Payment Outstanding");
    //    PaymentOutstandingWidget->setWindowFlags(Qt::Window);
    //    PaymentOutstandingWidget->show();
}

void RestuarantCounterMainWindow::openOutstandingReceivable()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    ReceivableOutstanding* ReceivableOutstandingWidget = new ReceivableOutstanding(this);
    //    ReceivableOutstandingWidget->setWindowTitle("Receivable Outstanding");
    //    ReceivableOutstandingWidget->setWindowFlags(Qt::Window);
    //    ReceivableOutstandingWidget->show();
}

void RestuarantCounterMainWindow::openPurchaseSummaryReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    ItemwisePurchaseReport* PurchaseReportWidget = new ItemwisePurchaseReport(this);
    //    PurchaseReportWidget->setWindowTitle("Itemwise Purchase Report");
    //    PurchaseReportWidget->setWindowFlags(Qt::Window);
    //    PurchaseReportWidget->showMaximized();

    if(!uiCtrl->setUiData(40, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openPurchaseOrderSummaryReport()
{

    //    if(LoginValues::Privilege<3)
    //        return;
    //    POItemwiseReport* PurchaseReportWidget = new POItemwiseReport(this);
    //    PurchaseReportWidget->setWindowTitle("Purchase Orders Item wise Report");
    //    PurchaseReportWidget->setWindowFlags(Qt::Window);
    //    PurchaseReportWidget->showMaximized();

}


void RestuarantCounterMainWindow::openSMSWidget()
{
    //    if(LoginValues::Privilege<15)
    //        return;
    SMSWidget* PurchaseReportWidget = new SMSWidget(this);
    PurchaseReportWidget->setWindowTitle("SMS Widget");
    PurchaseReportWidget->setWindowFlags(Qt::Window);
    PurchaseReportWidget->showMaximized();

}

void RestuarantCounterMainWindow::openPurchaseItemReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    ItemwisePurchaseReport* PurchaseReportWidget = new ItemwisePurchaseReport(this);
    //    PurchaseReportWidget->setWindowTitle("Purchase Item Report");
    //    PurchaseReportWidget->setWindowFlags(Qt::Window);
    //    PurchaseReportWidget->showMaximized();

    if( !uiCtrl->setUiData(89, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openLedgerwiseItemsReport()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    LedgerwiseItemMovement* PurchaseReportWidget = new LedgerwiseItemMovement(this);
    //    PurchaseReportWidget->setWindowTitle("Ledgerwise Items Report");
    //    PurchaseReportWidget->setWindowFlags(Qt::Window);
    //    PurchaseReportWidget->showMaximized();

    if(!uiCtrl->setUiData(28, true))
        return;
    uiCtrl->setDefaultFilters("f1", GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher), "voucherType");
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openCustomersLedger()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    CustomerReport* widget = new CustomerReport(this);
    //    widget->setWindowTitle("Customer Report");
    //    widget->setWindowFlags(Qt::Window);
    //    widget->showMaximized();

    //    uiCtrl->setUiData(3);
    //    uiCtrl->setDefaultFilters("f1", GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher), "voucherType");
    //    uiCtrl->reportWid->showMaximized();

    if(!uiCtrl->setUiData(81, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::openVisitorsList()
{
    //    if(LoginValues::Privilege<3)
    //        return;
    //    VisitorsRegisterWidget* widget = new VisitorsRegisterWidget(this);
    //    widget->setWindowTitle("Visitors Report");
    //    widget->setWindowFlags(Qt::Window);
    //    widget->showMaximized();

    if(!uiCtrl->setUiData(38, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::hideAdjustAction()
{
    ui->actionSalesAdjustment->setVisible(false);
}

void RestuarantCounterMainWindow::showDayNight()
{
    QDateTime t = QDateTime::currentDateTime();
    QString tText = t.toString("hh:mm:ss");
    QString tpli = tText.split(":").at(0);

    QRect rect;
    QImage img;

}

void RestuarantCounterMainWindow::showTime()
{
    QDateTime t = QDateTime::currentDateTime();
    QString tText = t.toString("hh:mm:ss");
    QString tpli = tText.split(":").at(0);
    int hh = tpli.toInt();

    if(hh>12){
        hh=hh-12;
        if(hh == 0) { hh=12;}
        tText = QString::number(hh)+":"+tText.split(":").at(1)+":"+tText.split(":").at(2)+" PM";
    }

    else{
        if(hh == 0) { hh=12;}
        tText = QString::number(hh)+":"+tText.split(":").at(1)+":"+tText.split(":").at(2)+ " AM";
    }

    tText = t.toString("dd-MM-yyyy ")+tText;
    tText = t.toString("dd-MM-yyyy hh:mm:ss ap");
    ui->dateLabel->setText(tText);

}
void RestuarantCounterMainWindow::tabSync()
{
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query2(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query3(QSqlDatabase::database(DatabaseValues::connectionString));

    QString tableName;
    QString print;
    QString addons;
    QString printerName="";
    QString voucherPrefix;
    int id;
    //    qDebug()<<"SYNC";

    bool saveAsInvoice= false;

    QString prefStr="";//

    if(serveOnlyPrefix == true)
        prefStr = " where Voucher_Prefix='"+LoginValues::voucherPrefix+"' ";

    query.prepare("Select itemname,groupname,printer,addons,Voucher_Prefix,id from tempbill "+prefStr+" order by id limit 1 ");
    if(query.exec())
    {
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<query.lastQuery();
        if(query.next()){
            tableName = query.value(0).toString();
            print = query.value(1).toString();
            printerName = query.value(2).toString();
            QString v = query.value(3).toString();
            //            qDebug()<<"v is "<<v;
            // qDebug()<<Q_FUNC_INFO<<__LINE__<<tableName;
            if(v.contains("Bill"/*GMVoucherTypes::getVoucherString( GMVoucherTypes::SalesVoucher)*/,Qt::CaseInsensitive)){
                saveAsInvoice = true;
            }
            voucherPrefix = query.value(4).toString();
            id= query.value(5).toInt();

        }
        else{
            // qDebug()<<Q_FUNC_INFO<<__LINE__;
        }

        QString prName = printerName;
        bool printKOTForWaiter = false;
        if(prName.length()<1){
            prName = ConfigurationSettingsDatabaseHelper::getValue(tabBillPrinter,"Counter").toString();
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<prName;
        }
        else{
            QStringList ps = printerName.split("|");
            prName = ps[0];
            if(ps.length()>1)
                if(ps[1].compare("1") == 0)
                    printKOTForWaiter = true;
            //            qDebug()<<Q_FUNC_INFO<<__LINE__;
        }

        if(saveAsInvoice == false){
            if(tableName.length()>0){

                // qDebug()<<Q_FUNC_INFO<<__LINE__<<tableName;

                QPair<QString,QString> VchrNo = salesOrderHelper->getVoucherNoByTransactionId(tableName);
                GeneralVoucherDataObject *v = salesOrderHelper->getVoucherByVoucherNoPtr(VchrNo.first,VchrNo.second);
                for(int i =0;i<v->InventoryItems.size();i++){
                    v->InventoryItems[i].BaseItem.taxRate = itemsHelper->getTaxRate(v->InventoryItems[i].BaseItem.ItemID);
                }
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                TransactionCalculator::VoucherCalculateSales(v);
                if(v->TransactionId.length() == 0)
                    v->TransactionId = DatabaseHelper::getUUID();
                if(v->kotNumber.length()==0)
                    v->kotNumber = QString::number(salesOrderHelper->getNewKotNumber());
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                tabKotGenerator->setVoucher(v);
                tabKotGenerator->generateWorkOrders();
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                tabKotGenerator->printKot();
                if(prName.length()>0 && printKOTForWaiter)
                {
                    // qDebug()<<Q_FUNC_INFO<<__LINE__;
                    printer->printKOTToPrinter(v,prName);
                }
                salesOrderHelper->updateVoucher(v,v->voucherNumber);
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                delete v;
                v = salesOrderHelper->getVoucherByVoucherNoPtr(VchrNo.first,VchrNo.second);

                if( print == "1" && allowOrderCopyPrint ){
                    printer->printSalesOrder(v,prName);
                }
                // query3.prepare("Delete from tempbill where itemname like '"+tableName+"' AND ADDONS = 'Order'");
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                // if(query3.exec()){

                // }
                delete v;
            }
        }
        else {
            if(tableName.length()>0){

                qDebug()<<Q_FUNC_INFO<<__LINE__<<tableName;

                QPair<QString,QString> VchrNo = salesHelper->getVoucherNoByTransactionId(tableName);
                GeneralVoucherDataObject *v = salesHelper->getVoucherByVoucherNoPtr(VchrNo.first,VchrNo.second);
                for(int i =0;i<v->InventoryItems.size();i++){
                    v->InventoryItems[i].BaseItem.taxRate = itemsHelper->getTaxRate(v->InventoryItems[i].BaseItem.ItemID);
                }
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                TransactionCalculator::VoucherCalculateSales(v);
                if(v->TransactionId.length() == 0)
                    v->TransactionId = DatabaseHelper::getUUID();
                if(v->kotNumber.length()==0)
                    v->kotNumber = QString::number(salesOrderHelper->getNewKotNumber());
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                tabKotGenerator->setVoucher(v);
                tabKotGenerator->generateWorkOrders();
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                tabKotGenerator->printKot();
                if(prName.length()>0 && printKOTForWaiter)
                {
                    qDebug()<<Q_FUNC_INFO<<__LINE__;
                    printer->printKOTToPrinter(v,prName);
                }
                salesHelper->updateVoucher(v,v->voucherNumber);
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                delete v;
                v = salesHelper->getVoucherByVoucherNoPtr(VchrNo.first,VchrNo.second);

                if( print == "1" && allowOrderCopyPrint ){
                    printer->printSalesOrder(v,prName);
                }
                // query3.prepare("Delete from tempbill where itemname like '"+tableName+"' AND ADDONS = 'BILL'");
                // qDebug()<<Q_FUNC_INFO<<__LINE__;
                // if(query3.exec()){

                // }
                delete v;
            }
        }

        query3.prepare("Delete from tempbill where id =:id ");
        query3.bindValue(":id",id);
        // qDebug()<<Q_FUNC_INFO<<__LINE__;
        if(query3.exec()){

        }

    }
    else{
        qDebug()<<query.lastError()<<query.lastQuery();
    }


}
void RestuarantCounterMainWindow::addNewItemBySearch(inventoryItemDataModel BaseItem)
{

    CompoundItemDataObject item;
    item.BaseItem = BaseItem;
    if(item.BaseItem.ItemID.length()>0){
        if(item.BaseItem.quantity <= 0)
            item.BaseItem.quantity = 1;
        item.BaseItem.PriceLevel = QString::number(voucher->ModeOfService);
        addNewItemToList(item);
        itemLine->clear();
    }

}

void RestuarantCounterMainWindow::connectMenuActions()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QObject::connect(ui->actionInventory_Groups,SIGNAL(triggered()),this,SLOT(openInventoryGroups()));
    QObject::connect(ui->actionInventory_Items,SIGNAL(triggered()),this,SLOT(openInventoryItems()));
    QObject::connect(ui->actionUom,SIGNAL(triggered()),this,SLOT(openUOMList()));
    QObject::connect(ui->actionAccount_Groups,SIGNAL(triggered()),this,SLOT(openAccountGroups()));
    QObject::connect(ui->actionLedgers,SIGNAL(triggered()),this,SLOT(openLedgers()));
    QObject::connect(ui->actionPriceList,SIGNAL(triggered()),this,SLOT(openPriceLists()));
    QObject::connect(ui->actionGodowns,SIGNAL(triggered()),this,SLOT(openGodowns()));
    QObject::connect(ui->actionEmployee,SIGNAL(triggered()),this,SLOT(openEmployees()));
    QObject::connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(openSettings()));
    QObject::connect(ui->actionUserGroups,SIGNAL(triggered()),this,SLOT(openUserGroups()));
    QObject::connect(ui->actionTables,SIGNAL(triggered()),this,SLOT(openTablesMaster()));
    QObject::connect(ui->actionMeasurements,SIGNAL(triggered()),this,SLOT(openMeasurements()));



    QObject::connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(closeApplication()));
    QObject::connect(ui->actionPurchase,SIGNAL(triggered()),this,SLOT(openPurchaseEntry()));
    QObject::connect(ui->actionSales, SIGNAL(triggered()), this, SLOT(openSalesEntry()));
    QObject::connect(ui->actionSalesAdjustment, SIGNAL(triggered()), this, SLOT(openSalesAdjustment()));
    QObject::connect(ui->actionPayment,SIGNAL(triggered()),this,SLOT(openPaymentEntry()));
    QObject::connect(ui->actionReceipt,SIGNAL(triggered()),this,SLOT(openReceiptEntry()));
    QObject::connect(ui->actionJournal,SIGNAL(triggered()),this,SLOT(openJournalEntry()));
    QObject::connect(ui->actionPurchase_Return,SIGNAL(triggered()),this,SLOT(openPurchaseReturn()));
    QObject::connect(ui->actionReceiptNote,SIGNAL(triggered()),this,SLOT(openReceiptNote()));
    QObject::connect(ui->actionReceiptNote_2,SIGNAL(triggered()),this,SLOT(openRceiptNoteSummary()));
    QObject::connect(ui->actionSales_Return,SIGNAL(triggered()),this,SLOT(openCreditNote()));
    QObject::connect(ui->actionStock_Entry,SIGNAL(triggered()),this,SLOT(openStockEntry()));
    QObject::connect(ui->actionStock_Journal,SIGNAL(triggered()),this,SLOT(openStockJournal()));
    QObject::connect(ui->actionShift,SIGNAL(triggered()),this,SLOT(openDenominations()));
    QObject::connect(ui->actionDaily_Closing,SIGNAL(triggered()),this,SLOT(openDailyClosing()));
    QObject::connect(ui->actionStockRequest,SIGNAL(triggered()),this,SLOT(openStockRequest()));
    QObject::connect(ui->actionWastage_Entry,SIGNAL(triggered()),this,SLOT(openDefectiveEntry()));
    QObject::connect(ui->actionPO_Itemwise,SIGNAL(triggered()),this,SLOT(openPurchaseOrderSummaryReport()));
    QObject::connect(ui->actionPurchase_Summary,SIGNAL(triggered()),this,SLOT(openPurchaseSummaryReport()));
    QObject::connect(ui->actionIndent,SIGNAL(triggered()),this,SLOT(openIndents()));
    QObject::connect(ui->actionPurchase_Orders,SIGNAL(triggered()),this,SLOT(openPurchaseOrders()));
    QObject::connect(ui->actionReservations,SIGNAL(triggered()),this,SLOT(openReservations()));

    QObject::connect(ui->actionToday_s_Bills,SIGNAL(triggered()),this,SLOT(openTodaysBills()));
    QObject::connect(ui->actionBillwise_Report,SIGNAL(triggered()),this,SLOT(openBillwiseReport()));
    QObject::connect(ui->actionDatewise_Sales,SIGNAL(triggered()),this,SLOT(openDatewiseReport()));

    QObject::connect(ui->actionItemwise_Sales,SIGNAL(triggered()),this,SLOT(openItemwiseReport()));
    QObject::connect(ui->actionGroupwise_Sales,SIGNAL(triggered()),this,SLOT(openGroupwiseReport()));
    QObject::connect(ui->actionWaiterwise_Sales,SIGNAL(triggered()),this,SLOT(openWaiterwiseReport()));
    QObject::connect(ui->actionServicewise_Sales,SIGNAL(triggered()),this,SLOT(openServicewiseReport()));
    QObject::connect(ui->actionTime_based_Sales,SIGNAL(triggered()),this,SLOT(openTimeBasedReport()));
    QObject::connect(ui->actionZomato_Report,SIGNAL(triggered()),this,SLOT(openZomatoReport()));
    QObject::connect(ui->actionCustomerwise_Report,SIGNAL(triggered()),this,SLOT(openCustomerwiseReport()));

    QObject::connect(ui->actionShiftwise_Report,SIGNAL(triggered()),this,SLOT(openShiftwiseReport()));
    QObject::connect(ui->actionDaily_Closing_Report,SIGNAL(triggered()),this,SLOT(openDailyClosingReport()));
    QObject::connect(ui->actionSalesmanwiseServicewiseSales, SIGNAL(triggered()),this,SLOT(openWaiterwiseService()) );
    QObject::connect(ui->actionSales_Orders,SIGNAL(triggered()),this,SLOT(openSalesOrders()));
    QObject::connect(ui->actionPendingOrders_2,SIGNAL(triggered()),this,SLOT(openPendingOrders()));
    QObject::connect(ui->actionPendingKOT,SIGNAL(triggered()),this,SLOT(openKOTReport()));
    QObject::connect(ui->actionClosing_Stock,SIGNAL(triggered()),this,SLOT(openStockReport()));
    QObject::connect(ui->actionRe_Order_Report,SIGNAL(triggered()),this,SLOT(openReOrderReport()));
    QObject::connect(ui->actionPayable,SIGNAL(triggered()),this,SLOT(openOutstandingPayable()));
    QObject::connect(ui->actionReceivable,SIGNAL(triggered()),this,SLOT(openOutstandingReceivable()));
    QObject::connect(ui->actionPurchaseItemReport,SIGNAL(triggered()),this,SLOT(openPurchaseItemReport()));

    QObject::connect(ui->actionLedgerwise_Items,SIGNAL(triggered()),this,SLOT(openLedgerwiseItemsReport()));
    QObject::connect(ui->actionLedger_Report,SIGNAL(triggered()),this,SLOT(openLedgerReport()));
    QObject::connect(ui->actionGroup_Summary,SIGNAL(triggered()),this,SLOT(openGroupSummary()));
    QObject::connect(ui->actionDay_Book,SIGNAL(triggered()),this,SLOT(openDayBook()));
    QObject::connect(ui->actionTrial_Balance,SIGNAL(triggered()),this,SLOT(openTrialBalance()));
    QObject::connect(ui->actionProfit_and_Loss,SIGNAL(triggered()),this,SLOT(openProfitLoss()));
    QObject::connect(ui->actionBalance_Sheet,SIGNAL(triggered()),this,SLOT(openBalanceSheet()));
    QObject::connect(ui->actionVAT_Report,SIGNAL(triggered()),this,SLOT(openVATReport()));

    QObject::connect(ui->actionSales_Margin_Report,SIGNAL(triggered()),this,SLOT(openSalesMarginReport()));
    QObject::connect(ui->actionComplimentary,SIGNAL(triggered()),this,SLOT(openComplimentaryReport()));
    QObject::connect(ui->actionDiscount_Report,SIGNAL(triggered()),this,SLOT(openDiscountReport()));\
        QObject::connect(ui->actionContents,SIGNAL(triggered()),this,SLOT(openHelpContents()));
    QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(openAbout()));
    QObject::connect(ui->actionKotPrinters,SIGNAL(triggered()),this,SLOT(openKotSettings()));
    QObject::connect(ui->actionNotifications, SIGNAL(triggered()), this, SLOT(openNotifications()));
    QObject::connect(ui->actionContents,SIGNAL(triggered()),this,SLOT(ChangeFont()));
    QObject::connect(ui->actionSMS,SIGNAL(triggered()),this,SLOT(openSMSWidget()));

    QObject::connect(ui->actionGST_Report,SIGNAL(triggered()),this,SLOT(openGSTReport()));
    QObject::connect(ui->actionDatewisePurchase, SIGNAL(triggered()),this,SLOT(openDatewisePurchase()));
    QObject::connect(ui->actionCustomer_Ledger,SIGNAL(triggered()),this,SLOT(openCustomersLedger()));
    QObject::connect(ui->actionVisitors_Register,SIGNAL(triggered()),this,SLOT(openVisitorsList()));
    QObject::connect(ui->actionDeleted_Sales,SIGNAL(triggered()),this,SLOT(openDeletedBills()));
    QObject::connect(ui->actionPayments,SIGNAL(triggered()),this,SLOT(openPaymentSummary()));
    QObject::connect(ui->actionReceipts,SIGNAL(triggered()),this,SLOT(openReceiptSummary()));

    QObject::connect(ui->actionSwitch_Language,SIGNAL(triggered()),this,SLOT(switchLang()));


    //    QObject::connect(ui->searchBox,SIGNAL(activated(QString)),this,SLOT(addNewItemBySearch(QString)));

    //    QObject::connect(completer, SIGNAL(activated(QModelIndex)),this, SLOT(addNewItemBySearch(QModelIndex)),Qt::QueuedConnection);
    //    QObject::connect(completer, SIGNAL(activated(const QString&)),ui->searchBox, SLOT(clear()),Qt::QueuedConnection);
}

void RestuarantCounterMainWindow::openKotSettings(){

    KOTPrinterSettings* kotwindow = new KOTPrinterSettings();
    kotwindow->setWindowFlags(Qt::Window);
    //    kotwindow->setAttribute(Qt::WA_DeleteOnClose);
    kotwindow->showMaximized();
}

void RestuarantCounterMainWindow::ChangeFont(){

    //    KOTPrinterSettings* kotwindow = new KOTPrinterSettings(this);
    //    kotwindow->setWindowFlags(Qt::Window);
    //    kotwindow->setAttribute(Qt::WA_DeleteOnClose);
    //    kotwindow->showMaximized();
}

void RestuarantCounterMainWindow::SalesDeletedReport(){


}


void RestuarantCounterMainWindow::saveVoucher()
{
    //    if(saveVoucherStarted)
    //        return;

    //    saveVoucherStarted = true;

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    cashOutToolButton->setDisabled(true);

    if(!allowEmptyAddressFlag){
        if(voucher->Contact.ContactName.length() == 0 ||
            voucher->Contact.PhoneNumber.length() == 0){
            //            blockSignals(AddressDialogWidget);
            connect(AddressDialogWidget, &AddressDialog::addrSelectedForInvoice, this, &RestuarantCounterMainWindow::saveVoucher , Qt::UniqueConnection);
            //            AddressDialogWidget->disableCancel();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;
            AddressDialogWidget->setVoucher(voucher);
            AddressDialogWidget->setVoucherType(GMVoucherTypes::SalesVoucher);
            AddressDialogWidget->show();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            //            saveVoucherStarted = false;
            return;


        }
    }
    disconnect(AddressDialogWidget, &AddressDialog::addrSelectedForInvoice, this, &RestuarantCounterMainWindow::saveVoucher);

    //    blockSignals(AddressDialogWidget);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    //    QTime dieTime= QTime::currentTime().addSecs(100);
    //    while (QTime::currentTime() < dieTime)
    //        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    if(!checkAllStocks())
    {
        QMessageBox box; box.setText("Stock Does not exist"); box.exec();
        return;
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;


    if(deletedvoucher->InventoryItems.size()>0){


        qDebug()<<Q_FUNC_INFO<<__LINE__;

        PasswordEntry *pwdEntry = new PasswordEntry();
        pwdEntry->setKotDeleted(true);
        pwdEntry->show();
        connect(pwdEntry, &PasswordEntry::loginSuccessWithComment, this, [=](QString val, QString comment, int priv){
                if(priv > -1){
                    qDebug()<<"saveDel"<<deletedvoucher->InventoryItems.size();
                    //                if(!deleteKOTItemWithoutRemarksFlag && comment.length() <3)
                    //                    return;
                    saveDeleted(true, comment);
                    saveVoucher();
                }
            },Qt::QueuedConnection);

        return;
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    billsCount++;
    if(voucher->kotNumber.toInt()==0)
        voucher->kotNumber = QString::number(salesOrderHelper->getNewKotNumber());

    if(voucher->reference.length()==0){
        voucher->reference = voucher->kotNumber;
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(voucher->SalesmanID == 0){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        AssignSalesman *SelectWaiterWidget = new AssignSalesman(voucher,this);
        SelectWaiterWidget->setWindowFlags(Qt::Window);
        SelectWaiterWidget->setAttribute(Qt::WA_DeleteOnClose);
        SelectWaiterWidget->setGroupName("");
        QObject::connect(SelectWaiterWidget,SIGNAL(selectEmployeeID(int)),
                         this,SLOT(changeWaiterNameAndSave(int)));
        SelectWaiterWidget->setTable();
        SelectWaiterWidget->showFullScreen();
        //        saveVoucherStarted = false;
        return;
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher->AddedBy = QString::number(LoginValues::getUserID());


    //    QString SoNo = salesOrderHelper->getVoucherNumberByRef(voucher->reference);
    //    if((voucher->status == QuotationStatus::SalesOrderCreated
    //        ||voucher->status == QuotationStatus::SalesOrderDeliveryCreated
    //        ||voucher->status == QuotationStatus::DeliveryReady
    //        ||voucher->status == QuotationStatus::DeliveryCompleted)){
    //        qDebug()<<"change voucher status";
    //        QString SoNo = salesOrderHelper->getVoucherNumberByRef(voucher->reference);
    //        salesOrderHelper->setSalesOrderStatus(QuotationStatus::SalesInvoiceRaised,
    //                                              SoNo,LoginValues::voucherPrefix);
    //    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->TransactionId;

    if(voucher->TransactionId.length() == 0)
        voucher->TransactionId = DatabaseHelper::getUUID();

    bool showMod =  showModFlag;

    if(showMod && !modeSet){
        ModeOfServiceWidget *modeOfSvcWid = new ModeOfServiceWidget(voucher,priceHelper,ledHelper);
        QObject::connect(modeOfSvcWid,&ModeOfServiceWidget::itemPricesChanged,[=]{
            setTable();
            modeSet = true;
            modeOfSvcWid->close();
            saveVoucher();
        });
        modeOfSvcWid->showFullScreen();
        //        saveVoucherStarted = false;
        return;
    }
    modeSet = false;

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    bool saveStatus = false;

    if(voucher->status == QuotationStatus::Started){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        voucher->VoucherPrefix = LoginValues::voucherPrefix;
        //        salesOrderHelper->insertVoucher(voucher);
        //        voucher->ConvertedToSalesOrder = voucher->voucherNumber;
        voucher->VoucherDate = QDate::currentDate();
        voucher->timestamp = QDateTime::currentDateTime();
        saveStatus = salesHelper->insertVoucher(voucher);
    }
    else{
        if(voucher->status != QuotationStatus::SalesInvoiceRaised){
            
            voucher->status = QuotationStatus::SalesInvoiceRaised;
            QString orderPrefix = voucher->VoucherPrefix;
            voucher->VoucherPrefix = LoginValues::voucherPrefix;
            voucher->ConvertedToSalesOrder = voucher->voucherNumber;
            voucher->VoucherDate = QDate::currentDate();
            voucher->timestamp  = QDateTime::currentDateTime();
            saveStatus =  salesHelper->insertVoucher(voucher);
            if(saveStatus)
                salesOrderHelper->setSalesOrderStatus(QuotationStatus::SalesInvoiceRaised, voucher->ConvertedToSalesOrder,orderPrefix);
        }
        else
        {
            saveStatus = salesHelper->updateVoucher(voucher,voucher->voucherNumber);
        }
    }

    if(saveStatus){
        setBillwiseMapping();

        printVoucher();
        printer->openTillByCode();

        printKotCopy = false;
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        sendToKOT(); //--fr

        refreshPage();
    }

    if(LoginValues::warningBillCount > 0 && LoginValues::warningMessage.length() > 0 ){

        if(billsCount % LoginValues::warningBillCount == 0){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<billsCount<<LoginValues::warningBillCount;
            QMessageBox box; box.setText(LoginValues::warningMessage); box.exec();
        }
    }

    if(billsCount % 40 == 0){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        qApp ->exit();
        QStringList args;
        args<<"100";
        args<<QString::number(LoginValues::userID)<<LoginValues::voucherPrefix<<DatabaseValues::dbName;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<args;
        QProcess::startDetached(qApp->arguments()[0], args);
    }


    qDebug()<<"Save Voucher Completed";
}


void RestuarantCounterMainWindow::setBillwiseMapping()
{
    if( voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Sundry Debtors")
        || voucher->ledgerObject.LedgerGroupId == accHelper->getGroupIDByName("Sundry Creditors") ){
        BillwiseRecordDataModel* bill = new BillwiseRecordDataModel;
        bill->VoucherNo = voucher->voucherNumber;
        bill->VoucherDate = voucher->VoucherDate;
        bill->VoucherType = voucher->voucherType;
        bill->VoucherPrefix = voucher->VoucherPrefix;
        bill->LedgerID = voucher->ledgerObject.LedgerID;
        bill->DebitAmount = voucher->grandTotal;
        bill->CreditAmount = 0;
        bill->DueDate = voucher->VoucherDate.addDays(voucher->ledgerObject.Credit_Period);

        voucher->ledgerObject.mapList.append(bill);
        voucher->mapList.append(bill);
    }
}

void RestuarantCounterMainWindow::openRceiptNoteSummary()
{
    //    ItemTransactionSummaryReportGenerator *summaryWidget = new ItemTransactionSummaryReportGenerator(this);
    //    summaryWidget->setVoucherType(GMVoucherTypes::ReceiptNote);
    //    summaryWidget->setTableView();
    //    summaryWidget->setReportName(GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote));
    //    summaryWidget->setWindowTitle("Receipt Note Summary");
    //    summaryWidget->setWindowFlags(Qt::Window);
    //    summaryWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    summaryWidget->showMaximized();

    if(!uiCtrl->setUiData(3, true))
        return;
    uiCtrl->setDefaultFilters("f2", GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote), "voucherType");
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::closeEvent(QCloseEvent *event)
{
    emit closeMain();
}

void RestuarantCounterMainWindow::changeEvent(QEvent *event)
{
    if(event!=nullptr){
        switch(event->type()){
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        }
    }
    QMainWindow::changeEvent(event);

}


void RestuarantCounterMainWindow::openDenominations()
{
    UiAccessControlDataModel *perm = userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1024);
    if(! perm->allowRead){
        showPermissionDenied();
        return;
    }

    int soPending = salesOrderHelper->getSOPending();
    bool checkShiftFlag = ConfigurationSettingsDatabaseHelper::getValue(checkShift,false).toBool();
    //    qDebug()<< "SO PEngins ="<<soPending;
    if(soPending>0 && checkShiftFlag){
        QMessageBox box; box.setText(QString::number(soPending) + " Orders Pending to Checkout"); box.exec();
        return;
    }

    //    Denominations *DenominationsWidget = new Denominations(this);
    //    DenominationsWidget->setWindowFlags(Qt::Window);
    //    DenominationsWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    DenominationsWidget->showFullScreen();
    //    QObject::connect(DenominationsWidget,SIGNAL(totalSales(DenominationsDataModel *)),this,SLOT(openShiftWidget(DenominationsDataModel *)));

    ShiftEntryWidget *shiftWid = new ShiftEntryWidget;
    shiftWid->setWindowFlags(Qt::Window);
    shiftWid->setAttribute(Qt::WA_DeleteOnClose);
    if(!perm->allowCreate)
        shiftWid->disablSave();
    shiftWid->showFullScreen();
    QObject::connect(shiftWid,SIGNAL(shiftClosingStarted(ShiftDataModel *)),this,SLOT(openShiftWidget(ShiftDataModel *)));


}
void RestuarantCounterMainWindow::changeTableToDraft(){

    //    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$ Called Change Table";
    ui->selectTableToolButton->setText(voucher->reference);

    previousTable = voucher->reference;
    QPair<QString,QString> voucherNumber = salesOrderHelper->getVoucherNumberByRef(voucher->reference);
    if(voucherNumber.first.length()>0){
        GeneralVoucherDataObject *tempVoucher =  salesOrderHelper->getVoucherByVoucherNoPtr(voucherNumber.first,voucherNumber.second);

        voucher->voucherNumber = tempVoucher->voucherNumber;
        voucher->VoucherPrefix = tempVoucher->VoucherPrefix;
        voucher->TransactionId = tempVoucher->TransactionId;
        voucher->RequirementVoucherNo = tempVoucher->RequirementVoucherNo;
        voucher->ModeOfService = tempVoucher->ModeOfService;
        voucher->status        = tempVoucher->status;
        voucher->POCPhone      = tempVoucher->POCPhone;
        voucher->POCName       = tempVoucher->POCPhone;
        voucher->Contact.ContactName = tempVoucher->Contact.ContactName;
        voucher->Contact.PhoneNumber = tempVoucher->Contact.PhoneNumber;
        voucher->Location       =tempVoucher->Location;
        //        voucher->SalesmanID = tempVoucher->SalesmanID;

        for(int i =0;i< tempVoucher->InventoryItems.size();i++){
            voucher->InventoryItems.insert(i,tempVoucher->InventoryItems[i]);
        }
        delete tempVoucher;
        setTable();
    }
}

void RestuarantCounterMainWindow::changeTable()
{
    //    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$ Called Change Table";
    QString ref = voucher->reference;
    //    QString waiter = voucher->SalesmanID;
    refreshPage();
    voucher->reference = ref;
    ui->selectTableToolButton->setText(ref);
    //    ui->selectWaiterToolButton->setText(waiter);

    previousTable = voucher->reference;
    QPair<QString,QString> voucherNumber = salesOrderHelper->getVoucherNumberByRef(voucher->reference);
    if(voucherNumber.first.length()>0){
        GeneralVoucherDataObject *tempVoucher = salesOrderHelper->getVoucherByVoucherNoPtr(voucherNumber.first,voucherNumber.second);
        voucher->voucherNumber = tempVoucher->voucherNumber;
        voucher->ModeOfService = tempVoucher->ModeOfService;
        voucher->status        = tempVoucher->status;
        voucher->SalesmanID    = tempVoucher->SalesmanID;
        voucher->Contact.PhoneNumber = tempVoucher->Contact.PhoneNumber;
        voucher->Contact.address = tempVoucher->Contact.address;
        voucher->Contact.ContactName = tempVoucher->Contact.ContactName;


        for(int i =0;i< tempVoucher->InventoryItems.size();i++){
            voucher->InventoryItems.insert(i,tempVoucher->InventoryItems[i]);
        }

        //        if(voucher->ModeOfService==ServiceMode::DineIn){
        //            on_dineInToolButton_clicked();
        //        }
        //        else if (voucher->ModeOfService==ServiceMode::TakeAway){
        //            on_takeAwayToolButton_clicked();
        //        }
        //        else if (voucher->ModeOfService==ServiceMode::Delivery){
        //            on_deliveryToolButton_clicked();
        //        }
        delete tempVoucher;

        setTable();
    }
}

void RestuarantCounterMainWindow::changeWaiterNameAndDraft(int salesManID)
{
    SwitchSalesMan(salesManID);
    on_draftToolButton_clicked();
}

void RestuarantCounterMainWindow::SwitchSalesMan(int salesManID)
{
    voucher->SalesmanID=salesManID;

    ui->selectWaiterToolButton->setText(userHelper->getEmployeeByID(voucher->SalesmanID).Name);
}

void RestuarantCounterMainWindow::sendToKOT()
{
    bool flag = false;

    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->InventoryItems[i].BaseItem.listId==0){
            flag = true;
            break;
        }
    }

    if(flag){
        if(askToPrintKOT){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"Print Kot","Do you Want to print KOT",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                kotGenerator->printKot();
            }
            else{
                qDebug()<<"Not Printing";
            }
        }
        else{
            kotGenerator->printKot();
        }

        int numKOt = CounterSettingsDataModel::KOTPrintCopies;

        if(voucher->ModeOfService==ServiceMode::Delivery)
            numKOt = CounterSettingsDataModel::DeliveryPrintCopies;

        if(printKotCopy){
            for(int i=0;i<numKOt;i++)
                printer->printSalesOrder(voucher);
        }
        printKotCopy= true;
    }

    // Send to Work Order
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    kotGenerator->generateWorkOrders();



    QSqlDatabase db(QSqlDatabase::database(DatabaseValues::connectionString));
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));

    db.transaction();

    query.prepare("INSERT INTO `kot_details`( `item_id`,`Item_Name`,`Quantity`, `Narration`, `Service_Type`,"
                  " `table_name`, `kot_num`,`waiterID`,`Voucher_Date`,`elapsedTime`, `section`,`sono`) VALUES"
                  "(:id,:itemName,:qty,:narr,:sType,:tabName,:kot,:waiter,NOW(),:elapsed,:section,:sono)");

    QString itemID;
    QString NonGroup = "|Teax|Coffeex|SOFT DRINKxS|STAxFF DRINK|";
    for(int i =0;i<voucher->InventoryItems.size();i++){
        QString gpName = "|"+voucher->InventoryItems[i].BaseItem.GroupName+"|";
        if(voucher->InventoryItems[i].BaseItem.listId==0 ){

            query.bindValue(":id",voucher->InventoryItems[i].BaseItem.ItemID);
            query.bindValue(":itemName",voucher->InventoryItems[i].BaseItem.ItemName);
            query.bindValue(":qty",voucher->InventoryItems[i].BaseItem.quantity);
            query.bindValue(":narr",voucher->InventoryItems[i].BaseItem.narration);
            query.bindValue(":sType",ServiceMode::getStatusString(voucher->ModeOfService));
            query.bindValue(":tabName",voucher->reference +" - "+ voucher->Contact.ContactName);
            query.bindValue(":kot",voucher->kotNumber.toInt());
            query.bindValue(":waiter", userHelper->getEmployeeNameFromID(voucher->SalesmanID));
            query.bindValue(":elapsed","00:00");
            query.bindValue(":section", voucher->InventoryItems[i].BaseItem.Category);
            query.bindValue(":sono",voucher->LPO);

            if(!query.exec()){
                qDebug()<<query.lastQuery();
                qDebug()<<query.lastError();
            }
        }
    }

    db.commit();
}

void RestuarantCounterMainWindow::groupButtonClicked(QString grpID)
{
    SalesItemsWidget->grpID = grpID;
    SalesItemsWidget->setTable();
    SalesItemsWidget->show();
}

void RestuarantCounterMainWindow::itemButtonClicked(QString itmID)
{
    CompoundItemDataObject compitem;
    compitem.BaseItem = itemsHelper->getInventoryItemByID(itmID);
    if(compitem.BaseItem.ItemID.length()>0){
        compitem.BaseItem.quantity = 1;
        addNewItemToList(compitem);
    }
}

void RestuarantCounterMainWindow::saveDraft()
{
    if(!allowEmptyAddressFlag){
        if(voucher->Contact.ContactName.length() == 0 ||
            voucher->Contact.PhoneNumber.length() == 0){
            //            blockSignals(AddressDialogWidget);
            connect(AddressDialogWidget, &AddressDialog::addrSelectedForDraft, this, &RestuarantCounterMainWindow::saveDraft);
            //            AddressDialogWidget->disableCancel();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;
            AddressDialogWidget->setVoucher(voucher);
            AddressDialogWidget->setVoucherType(GMVoucherTypes::SalesOrder);
            AddressDialogWidget->show();
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            return;
        }
    }

    if(!checkAllStocks())
    {
        QMessageBox box; box.setText("Stock Does not exist"); box.exec();
        return;
    }
    draftsCount++;
    voucher->kotNumber = QString::number(salesOrderHelper->getNewKotNumber());
    if(voucher->reference.length()<1){
        voucher->reference = voucher->kotNumber;
    }
    qDebug()<<"$%^&*(&^#$%^==========>"<<voucher->status<<voucher->voucherNumber<<"<====vno";
    if(voucher->status != QuotationStatus::Started && voucher->voucherNumber.length()>0){
        salesOrderHelper->updateVoucher(voucher,voucher->voucherNumber);
    }
    else
    {
        if(voucher->ModeOfService == ServiceMode::Delivery)
            voucher->status = QuotationStatus::DeliveryReady;

        if(voucher->status == QuotationStatus::Started){
            voucher->status = QuotationStatus::SalesOrderCreated;
        }

        if(voucher->TransactionId.length() == 0)
            voucher->TransactionId = DatabaseHelper::getUUID();
        voucher->timestamp = QDateTime::currentDateTime();
        salesOrderHelper->insertVoucher(voucher);
    }
    printKotCopy = true;

    sendToKOT();
    refreshPage();


    if(draftsCount%40==0){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        qApp ->exit();
        QStringList args;
        args<<"100";
        args<<QString::number(LoginValues::userID)<<LoginValues::voucherPrefix<<DatabaseValues::dbName;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<args;
        QProcess::startDetached(qApp->arguments()[0], args);
    }

}

void RestuarantCounterMainWindow::setVoucher(GeneralVoucherDataObject *obj)
{
    //    refreshPage();

    delete voucher;
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher = obj;

    kotGenerator->setVoucher(voucher);
    ModeOfService->setVoucher(voucher);


    AddressDialogWidget->setVoucher(voucher);
    AddressDialogWidget->hide();

    deletedvoucher->reference = voucher->reference;
    deletedvoucher->ModeOfService = voucher->ModeOfService;
    deletedvoucher->SalesmanID = voucher->SalesmanID;

    ui->BillNumber->setText(voucher->VoucherPrefix+" - " +voucher->voucherNumber);
    ui->selectTableToolButton->setText(voucher->reference);

    ui->selectWaiterToolButton->setText(userHelper->getEmployeeByID(voucher->SalesmanID).Name);

    setTable();
}

//void RestuarantCounterMainWindow::SearchInventoryItems()
//{
//    inventoryItem* inventoryItemWidget = new inventoryItem(this);
//    inventoryItemWidget->setWindowFlags(Qt::Window);
//    inventoryItemWidget->setWindowTitle("Inventory");
//    inventoryItemWidget->getBackFlag =1;
//    inventoryItemWidget->show();
//    inventoryItemWidget->setAttribute(Qt::WA_DeleteOnClose);
//    inventoryItemWidget->setFocus();
//    inventoryItemWidget->setFocusonSearchLine();
//    QObject::connect(inventoryItemWidget,SIGNAL(SelectedItem(CompoundItemDataObject)),this,
//                     SLOT(addNewItemToList(CompoundItemDataObject)));
//}

void RestuarantCounterMainWindow::addNewItemToList(CompoundItemDataObject item)
{
    qDebug()<<"Dont Check Stock "<<allowNegativeStock;
    if(!allowNegativeStock)
        if(  item.BaseItem.ClosingStock <= 0 && item.BaseItem.isStockItem == true )
        {
            QMessageBox box;
            box.setText("Current Stock is " + QString::number(item.BaseItem.ClosingStock,'f',2) + " for " + item.BaseItem.ItemName + " !");
            box.exec();
            return;
        }

    if(item.BaseItem.quantity<=0)
        item.BaseItem.quantity = 1;

    item.BaseItem.price = priceHelper->getPriceByItemID(item.BaseItem.ItemID,QString::number(voucher->ModeOfService));


    if(Inclusive){
        item.BaseItem.price = item.BaseItem.price * 100 / (100+item.BaseItem.taxRate);
    }

    if(item.BaseItem.price == 0 ){
        bool ok;
        QString text = QInputDialog::getText(this, tr("Enter Price"),
                                             tr("Amount : "), QLineEdit::Normal,
                                             "0", &ok);
        if (ok && !text.isEmpty())
        {
            item.BaseItem.price = text.toFloat();
        }
    }


    if(item.BaseItem.Category.contains("4",Qt::CaseInsensitive)){
        freeCount +=0;
    }
    else{
        if( freeUsed < freeCount ){
            if( freeCount > 0){
                item.BaseItem.price=0;
                freeUsed++;
            }
        }
    }

    int comboIndex = voucher->InventoryItems.length();
    qDebug()<<"Index = "<<comboIndex;
    if(comboIndex>0){
        if(comboIndex==1){
            if(voucher->InventoryItems[0].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
                item.BaseItem.price = 0;
            }
        }
        else{
            if(voucher->InventoryItems[comboIndex-1].BaseItem.Category.contains("4",Qt::CaseInsensitive)
                ||
                voucher->InventoryItems[comboIndex-2].BaseItem.Category.contains("4",Qt::CaseInsensitive)
                ){
                item.BaseItem.price = 0;
            }
        }
    }

    voucher->InventoryItems.append(item);
    setTable();
    ui->tableWidget->scrollToBottom();
    selectRow(voucher->InventoryItems.length()-1);
}

void RestuarantCounterMainWindow::createNewItem(QString text)
{
    AddItems *additemWidget= new AddItems(this);
    additemWidget->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
    additemWidget->setItemCode(text);
    additemWidget->setAttribute(Qt::WA_DeleteOnClose);
    additemWidget->show();
    QObject::connect(additemWidget,SIGNAL(savedItem(CompoundItemDataObject)),this,SLOT(addNewItemToList(CompoundItemDataObject)));
}

void RestuarantCounterMainWindow::printVoucher()
{
    if(printOn){
        CounterPrinterHelper pHelper;
        for(int i=0;i<voucher->NoOfCopies;i++){
            pHelper.printSalesVoucher(voucher);
        }
    }
}

void RestuarantCounterMainWindow::openRestaurantItemDetail()
{
    int row = sender()->property("row").toInt();

    RestaurantItemDetail *RestaurantItemDetailWidget = new RestaurantItemDetail(voucher,row,this);
    RestaurantItemDetailWidget->setWindowFlags(Qt::Window);
    RestaurantItemDetailWidget->setWindowTitle("Item Detail");
    RestaurantItemDetailWidget->setAttribute(Qt::WA_DeleteOnClose);

    RestaurantItemDetailWidget->setItem(&voucher->InventoryItems[row]);
    RestaurantItemDetailWidget->show();

    QObject::connect(RestaurantItemDetailWidget,SIGNAL(saveRestaurantItemDetails()),this,SLOT(setTable()));

    QObject::connect(RestaurantItemDetailWidget, &RestaurantItemDetail::itemDeletedFromDetail,
                     this,&RestuarantCounterMainWindow::appendDeletedItem);

}

void RestuarantCounterMainWindow::appendDeletedItem(CompoundItemDataObject c){
    deletedvoucher->InventoryItems.append(c);
}



void RestuarantCounterMainWindow::openGodowns()
{
    if(!uiCtrl->setUiData(67, true))
        return;
    uiCtrl->reportWid->showMaximized();
}
void RestuarantCounterMainWindow::openEmployees()
{
    if(!uiCtrl->setUiData(70, true))
        return;
    uiCtrl->reportWid->showMaximized();

}

void RestuarantCounterMainWindow::selectRow(int row)
{
    ui->tableWidget->selectRow(row);
}



void RestuarantCounterMainWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    openItemDetail(index.row());
}
void RestuarantCounterMainWindow::openItemDetail(int row){

    if(voucher->InventoryItems[row].BaseItem.price!=0)
        if(voucher->InventoryItems[row].BaseItem.listId >0 && disableItemEdit)
            return;

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ItemDetail *itemDetail = new ItemDetail(voucher,
                                                row, 1
                                                , itemsHelper
                                                ,uomHelper
                                                , priceHelper
                                                , new GodownDatabaseHelper
                                                ,QString::number(voucher->ModeOfService),this);

        itemDetail->setWindowFlags(Qt::Window);
        //        itemDetail->setWindowTitle("Item Detail");
        itemDetail->setAttribute(Qt::WA_DeleteOnClose);
        itemDetail->show();
        QObject::connect(itemDetail,SIGNAL(updateItem()),this,SLOT(setTable()));
    }
    else{
        RestaurantItemDetail *RestaurantItemDetailWidget = new RestaurantItemDetail(voucher,row,this);
        RestaurantItemDetailWidget->setWindowFlags(Qt::Window);
        RestaurantItemDetailWidget->setWindowTitle("Item Detail");
        RestaurantItemDetailWidget->setAttribute(Qt::WA_DeleteOnClose);
        //    int row = sender()->property("row").toInt();
        RestaurantItemDetailWidget->setItem(&voucher->InventoryItems[row]);
        RestaurantItemDetailWidget->show();
        QObject::connect(RestaurantItemDetailWidget,SIGNAL(saveRestaurantItemDetails()),this,SLOT(setTable()));

    }



}

void RestuarantCounterMainWindow::openGSTReport()
{
    if(! userGroupHelper->getPermissionsByUserGroupAndUi(LoginValues::model._id, 1009)->allowRead){

        showPermissionDenied();
        return;
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    VatReport *widget = new VatReport(this);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    widget->setWindowFlags(Qt::Window);
    widget->setWindowTitle("VAT Report");
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->show();

    //    if(!uiCtrl->setUiData(1009, false))
    //        return;
    //    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::openDatewisePurchase()
{

    if(!uiCtrl->setUiData(61, true))
        return;
    uiCtrl->reportWid->showMaximized();
}

void RestuarantCounterMainWindow::changeWaiterNameAndSave(int salesManID)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    SwitchSalesMan(salesManID);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    saveVoucher();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void RestuarantCounterMainWindow::on_switchButton_clicked()
{
    if(voucher->InventoryItems.size()>0 && voucher->reference.length()>0 ){
        previousTable = voucher->reference;
        TableSelect* tabSelect = new TableSelect(voucher,salesOrderHelper,ledHelper,accHelper, priceHelper,this);
        tabSelect ->setWindowFlags(Qt::Window);
        tabSelect->setSwitchFlag(true);
        tabSelect->setLineFocus();
        tabSelect ->showFullScreen();
        QObject::connect(tabSelect ,SIGNAL(switchTable()),
                         this,SLOT(switchOption()));
    }
}

void RestuarantCounterMainWindow::switchOption()
{
    qDebug()<<Q_FUNC_INFO;
    if(previousTable!=voucher->reference){
        QPair<QString,QString> vNumber = salesOrderHelper->getVoucherNumberByRef(voucher->reference);

        if(vNumber.first.length()>0){
            GeneralVoucherDataObject *temp = salesOrderHelper->getVoucherByVoucherNoPtr(vNumber.first,vNumber.second);

            for(int i=0;i<voucher->InventoryItems.size();i++){
                temp->InventoryItems.append(voucher->InventoryItems[i]);
                //                salesOrderHelper->deleteSalesOrderItemByListID(voucher->InventoryItems[i].BaseItem.listId);
            }

            qDebug()<<"Deleting voucher 2009";
            qDebug()<<salesOrderHelper->getInventoryItemsCount(voucher->voucherNumber);

            //            if(salesOrderHelper->getInventoryItemsCount(voucher->voucherNumber)==0)
            //            {
            // qDebug()<<"Deleting voucher 2012";
            TransactionCalculator::VoucherCalculateSales(temp);
            salesOrderHelper->deleteVoucher(voucher);
            salesOrderHelper->updateVoucher(temp,vNumber.first);
            // qDebug()<<"Temp Total : "<<temp->grandTotal;

            //            }

        }
        else{
            salesOrderHelper->updateReference(voucher->voucherNumber,voucher->reference);
        }
        refreshPage();
    }
}


void RestuarantCounterMainWindow::showExtrasWidget()
{
    RestaurantExtras *RestaurantExtrasWidget = new RestaurantExtras(this);
    RestaurantExtrasWidget->setWindowFlags(Qt::Window);
    RestaurantExtrasWidget->setWindowTitle("Extras");
    RestaurantExtrasWidget->setAttribute(Qt::WA_DeleteOnClose);
    RestaurantExtrasWidget->show();

    QTimer::singleShot(300  * 1000, this, SLOT(hideAdjustAction()));
    QObject::connect(RestaurantExtrasWidget,SIGNAL(adminButtonClicked()),this,SLOT(showMenuBar()));
    QObject::connect(RestaurantExtrasWidget,SIGNAL(screenLockClicked()),this,SLOT(lockScreen()));
    QObject::connect(RestaurantExtrasWidget,SIGNAL(reportsButtonClicked()),this,SLOT(openTodaysBills()));
}

void RestuarantCounterMainWindow::openTill()
{
    printer->openTillByCode();
}

void RestuarantCounterMainWindow::hideMenuBar()
{
    QRect r= ui->tableWidget->geometry();
    int h = ui->menubar->height();
    ui->menubar->hide();
}

void RestuarantCounterMainWindow::showMenuBar()
{
    if(ui->menubar->isHidden()){
        ui->menubar->show();
        // this->showMaximized();
        // int h = ui->menubar->height();
        // QRect r= ui->tableWidget->geometry();
        // r.setHeight(r.height()-(2*h));
        // ui->tableWidget->setGeometry(r);
        QTimer::singleShot(1000*60*30,
                           this,
                           hideMenuBar
                           );
        // QTimer::singleShot(300000,&RestuarantCounterMainWindow::hideMenuBar);
    }
}

void RestuarantCounterMainWindow::lockScreen()
{
    LoginPage *LoginPageWidget = new LoginPage();
    LoginPageWidget->setWindowFlags(Qt::FramelessWindowHint);
    LoginPageWidget->show();
    emit closeMain();
}

void RestuarantCounterMainWindow::openSerialPort()
{
    bool cnfFlag =  ConfigurationSettingsDatabaseHelper::getValue(openSerialPortCnf,false).toBool();
    if(!cnfFlag)
        return;
    serial->setPortName(CounterSettingsDataModel::PortNo);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(QSerialPort::Data8));
    serial->setParity(static_cast<QSerialPort::Parity>(QSerialPort::NoParity));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(QSerialPort::OneStop));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(QSerialPort::NoFlowControl));

    //    for caller id
    if (serial->open(QIODevice::ReadWrite)) {
        qDebug()<<"Connected TO caller ID!!!";
        QByteArray str = "\r\nAT+VCID=1\r\n";
        writeData(str);
    }

    //for vfd
    //    if (serial->open(QIODevice::WriteOnly)) {
    //        qDebug()<<"Connected!!!";
    //        QByteArray str = "WELCOME TO          CAKE STUDIO         ";
    //        writeData(str);
    //    }
    //    else {
    //        //        QMessageBox::critical(this, tr("Error"), serial->errorString()+"\nCalled ID not Connected");

    //    }
}

void RestuarantCounterMainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    // console->setEnabled(false);
    //    ui->actionConnect->setEnabled(true);
    //    ui->actionDisconnect->setEnabled(false);
    //    ui->actionConfigure->setEnabled(true);
    //    showStatusMessage(tr("Disconnected"));
}
//! [5]


//! [6]
void RestuarantCounterMainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void RestuarantCounterMainWindow::readData()
{
    //for caller id uncomment
    QByteArray data = serial->readAll();
    //console->putData(data);
    QString dStr1 = data;
    // QMessageBox box; box.setText(dStr1); box.exec();
    if(dStr1.contains("NMBR") && dStr1.split('=').size()>3){
        ui->pickCallToolButton->setHidden(false);
        dStr = dStr1.right(dStr1.length()-1);
        QString number = dStr.split('=')[3];

        number=number.left(number.length() - 2);

        number = number.right(number.length()-1);


        QString sstr = "INSERT INTO `tempbill`(`itemname`,addons,voucher_prefix ) VALUES ('"+number.right(number.length()-1)+"','phone',"+LoginValues::voucherPrefix+")";
        //        QMessageBox box; box.setText(sstr); box.exec();
        qDebug()<<sstr;
        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
        query.exec(sstr);
    }
}

void RestuarantCounterMainWindow::setLoginValues(int username)
{
    return;
    UserGroupDatabaseHelper *urHelper = new UserGroupDatabaseHelper;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    qDebug()<<"username";

    QString qStr = "SELECT ";
    qStr += userHelper->Employee_Details_Username + ", ";
    qStr += userHelper->Employee_Details_Password + ", ";
    qStr += userHelper->Employee_Details_Email    + ", ";
    qStr += userHelper->Employee_Details_Name +", ";
    qStr += userHelper->Employee_Details_id + ", ";
    qStr += userHelper->Employee_Details_Phone + ", ";
    qStr += userHelper->Employee_Details_User_Group + " ";
    qStr += " FROM ";
    qStr += userHelper->Employee_Details_TableName + " WHERE ";
    qStr += userHelper->Employee_Details_id +"='"+QString::number(username)+"'";

    if(query.exec(qStr)){
        while(query.next()){
            LoginValues::setUserName(query.value(0).toString());
            LoginValues::setUserEmail(query.value(2).toString());
            LoginValues::setEmpName(query.value(3).toString());
            LoginValues::setUserID(query.value(4).toInt());
            LoginValues::setUserPhone(query.value(5).toString());
            LoginValues::setModel(urHelper->getUserGroup(query.value(6).toInt()));
        }
    }
}


void RestuarantCounterMainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void RestuarantCounterMainWindow::on_airportButton_clicked()
{

    this->showMinimized();
    //    on_refreshToolButton_clicked();
    //    ui->cashOutToolButton->hide();
    //    ui->modeOfPayToolButton->hide();
    //    MuroorClicked = true;
    //    DatabaseValues dv;
    //    dv.switchDB("gmmuroor","teabreakmr.dyndns.org",3308,"user2kit","user2kit");

    //    SalesGroupsWidget->setTable();
    //    SalesItemsWidget->setTable();

    //    delete salesOrderHelper;
    //    salesOrderHelper = new SalesOrderDatabaseHelper();
    //    delete salesHelper;
    //    salesHelper = new salesVoucherDatabaseHelper();
    //    delete itemsHelper;
    //    itemsHelper = new SalesInventoryItemDatabaseHelper();
    //    delete ledHelper;
    //    ledHelper = new LedgerMasterDatabaseHelper();
    //    delete userHelper;
    //    userHelper = new UserProfileDatabaseHelper();

    //    ui->airportButton->setStyleSheet("background-color:#5dfd08");
    //    on_deliveryToolButton_clicked();
}




void RestuarantCounterMainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    itemLine->setFocus();
}


void RestuarantCounterMainWindow::on_printOnOff_clicked()
{
    if(ui->printOnOff->isChecked()){
        ui->printOnOff->setText("PRINT OFF");
        printOn = false;
    }
    else{
        ui->printOnOff->setText("PRINT ON");
        printOn = true;
    }
}

bool RestuarantCounterMainWindow::checkAllStocks()
{

    if(allowNegativeStock)
        return true;
    QList<CompoundItemDataObject> itemsList;
    for(int i=0;i<voucher->InventoryItems.size();i++){ qDebug()<<i;
        int j=0;
        voucher->InventoryItems[i].BaseItem.ClosingStock = itemsHelper->getClsStkOfItemTillDate(voucher->InventoryItems[i].BaseItem.ItemID,QDate::currentDate());
        for(;j<itemsList.size();j++){
            qDebug()<<j;
            if(itemsList[j].BaseItem.ItemID == voucher->InventoryItems[i].BaseItem.ItemID){
                qDebug()<<"adding to old "<<voucher->InventoryItems[i].BaseItem.ItemName;
                itemsList[j].BaseItem.quantity += voucher->InventoryItems[i].BaseItem.quantity;
                if(itemsList[j].BaseItem.isStockItem  && itemsList[j].BaseItem.quantity > itemsList[j].BaseItem.ClosingStock)
                {
                    //                    bar->setAutoHideDuration();
                    bar->addMessage(itemsList[j].BaseItem.ItemName +
                                    " is Out Of Stock (" +
                                    QString::number(itemsList[j].BaseItem.quantity>itemsList[j].BaseItem.ClosingStock,'f',2)+
                                    ")");
                    return false;
                }
                break;
            }
        }
        if( j == itemsList.size()){
            if(voucher->InventoryItems[i].BaseItem.isStockItem && voucher->InventoryItems[i].BaseItem.quantity>voucher->InventoryItems[i].BaseItem.ClosingStock)
                return false;
            qDebug()<<"new Item "<<voucher->InventoryItems[i].BaseItem.ItemName;
            itemsList.append(voucher->InventoryItems[i]);
        }
    }
    return true;
}

void RestuarantCounterMainWindow::editQtyByKeyboard()
{
    int row = ui->tableWidget->currentRow();

    if(voucher->InventoryItems[row].BaseItem.listId>0 && disableItemEdit)
        return;
    qDebug()<<"ROW = "<<row;
    qDebug()<<"Inv size = "<<voucher->InventoryItems.size();
    if(voucher->InventoryItems.length()>row){
        QLineEdit* edit = new QLineEdit(this);
        int dec = voucher->InventoryItems[row].BaseItem.uomObject.decimalPoints;
        edit->setValidator(new QDoubleValidator(0,999,dec,this));
        ui->tableWidget->setCellWidget(row,1,edit);
        edit->setFocus();
        qDebug()<<"Quantity edit set to true";
        qtyFlag = true;
        QObject::connect(edit,&QLineEdit::returnPressed,
                         this,&RestuarantCounterMainWindow::qtyEditCompleted);

    }
}

void RestuarantCounterMainWindow::qtyEditCompleted()
{
    int row = ui->tableWidget->currentRow();
    QLineEdit *edit = qobject_cast <QLineEdit*> (sender());
    bool ok = false;
    voucher->InventoryItems[row].BaseItem.quantity = edit->text().toFloat(&ok);

    setTable();
    selectRow(row);

    qDebug()<<" Quantity flag is made to false";
}



void RestuarantCounterMainWindow::on_refreshToolButton_clicked()
{
    if(saveOnRefreshFlag){
        for(int i =0;i<voucher->InventoryItems.size();i++)
            deletedvoucher->InventoryItems.append(voucher->InventoryItems[i]);
        if(deletedvoucher->InventoryItems.size()>0)
            saveDeleted(false);
    }
    refreshPage();
}

void RestuarantCounterMainWindow::showPermissionDenied()
{
    QMessageBox box;
    box.setText("User " + LoginValues::userName + " does not have permissions;\n Please contact Admin");
    box.exec();
}


void RestuarantCounterMainWindow::hideCustomerScreen(bool show)
{
    qDebug()<<"showHideCustomerScreen"<<show<<QGuiApplication::screens().length();
    int len = QGuiApplication::screens().length();
    customerScreenWidget->setHidden(show);
    settings->setValue("hideCustomerScreen",show);
    if(!show && len>1){
        QScreen *screen = QGuiApplication::screens()[len-1]; // specify which screen to use
        qDebug()<<"SCreen Moved"<<screen->geometry().x()<< screen->geometry().y();

        customerScreenWidget->move(screen->geometry().x(), screen->geometry().y());
        customerScreenWidget->resize(screen->geometry().width(), screen->geometry().height());
        customerScreenWidget->showMaximized();
        QRect screenRect = QApplication::desktop()->screenGeometry(len-1);
        customerScreenWidget->move(QPoint(screenRect.x(), screenRect.y()));
    }
    else{
        qDebug()<<"SCreen NOT Moved";
    }
}

