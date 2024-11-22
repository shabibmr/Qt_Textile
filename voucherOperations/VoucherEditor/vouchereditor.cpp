#include "vouchereditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/itemdetail.h"

#include <QDesktopWidget>
#include <QScreen>

VoucherEditor::VoucherEditor(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle(windowTitle);
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:" + backgroundColor + "}");
    QPalette pal = palette();

    //    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
    //                                          qApp->primaryScreen()->geometry()));

    // set black background
    pal.setColor(QPalette::Background,QColor(backgroundColor));
    setAutoFillBackground(true);
    setPalette(pal);

    initDbHelpers();
    initVoucher();
    initGlobalObjects();
    setWidgets();
    initExtraActions();
}

VoucherEditor::~VoucherEditor()
{
}

void VoucherEditor::initExtraActions()
{
    shareOptionsWidget = new ShareTools("Voucher", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup);

    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");

    rejectButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("navigation", "cancel"));
    rejectButton->setMini(true);
    rejectButton->setXOffset(1315);
    rejectButton->setYOffset(10);
    rejectButton->setParent(this);
    rejectButton->setToolTip("Reject Voucher");
    rejectButton->hide();

    shareButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setMini(true);
    shareButton->setBackgroundColor(QColor(backgroundColor));
    shareButton->setCorner(Qt::TopRightCorner);
    shareButton->setParent(this);
    shareButton->setXOffset(45);
    shareButton->setYOffset(5);

    snackBar = new QtMaterialSnackbar(this);
    drawer = new QtMaterialDrawer(this);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);

    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);

    printButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "print"));
    printButton->setColor(QColor(iconColor));
    printButton->setIconSize(iconSize);

    contactButton = new QtMaterialIconButton(QtMaterialTheme::icon("communication", "contacts"));
    contactButton->setColor(QColor(iconColor));
    contactButton->setIconSize(iconSize);

    importButton = new QtMaterialIconButton(QtMaterialTheme::icon("communication", "import_export"));
    importButton->setColor(QColor(iconColor));
    importButton->setIconSize(iconSize);

    appBar->appBarLayout()->addWidget(contactButton);
    appBar->appBarLayout()->addWidget(printButton);
    appBar->appBarLayout()->addWidget(importButton);

    QObject::connect(backButton,&QPushButton::clicked,
                     this,&VoucherEditor::closeEditor);
    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&VoucherEditor::saveVoucher);
    QObject::connect(rejectButton,&QPushButton::clicked,
                     this,&VoucherEditor::rejectVoucher);
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&VoucherEditor::showShareOptions);
    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&VoucherEditor::printVoucher);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&VoucherEditor::printVoucher);
    QObject::connect(printButton,&QPushButton::clicked,
                     this,&VoucherEditor::askForPrint);
    QObject::connect(contactButton,&QPushButton::clicked,
                     this,&VoucherEditor::showContactDetails);
    QObject::connect(importButton,&QPushButton::clicked,
                     this,&VoucherEditor::getVoucherstoImport);
}

void VoucherEditor::initDbHelpers()
{
    ledgerHelper = new LedgerMasterDatabaseHelper;
    accHelper = new AccountGroupMasterDatabaseHelper;
    dbHelper = new salesVoucherDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
    rHelper = new receiveChannelDatabaseHelper;
    priceHelper = new PricelistDatabaseHelper;
}

void VoucherEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;
    voucher->priceListName = priceHelper->getPriceListNameByID(priceHelper->getDefaultPriceList());
}

void VoucherEditor::initGlobalObjects()
{
    coupledVoucherHandler = new CoupledVoucherHandler(this);
    coupledVoucherHandler->setParentVoucher(voucher);
    coupledVoucherHandler->setMaximumWidth(300);
    coupledVoucherHandler->setWidgets();
    coupledVoucherHandler->hide();


    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Price"<<"Tax"<<"Tax Amount"<<"Disc %"<<"Disc. Amount"<<"Amount";
    DataTableWidget = new InventoryVoucherTableView(voucher);
    DataTableWidget->setEnabled(false);
    DataTableWidget->setStyleSheet(
                "QTableView::item{"
                "gridline-color : #ffffff;"
                "border: 0px solid #fffff8;"
                "}"
                "QTableView::item:selected{"
                "color:" + highlightedColor +";"
                                             "background-color:" + selectionColor + ";"
                                                                                    "}"
                                                                                    "QTableView#EditInvTable{"
                                                                                    "background-color: " + backgroundColor + ";"
                                                                                                                             "color: " + textColor + ";"
                                                                                                                                                     "/*selection-background-color: #999*/}"
                                                                                                                                                     "QHeaderView::section:horizontal"
                                                                                                                                                     "{"
                                                                                                                                                     "background-color: " + headerBgColor + ";"
                                                                                                                                                                                            "color: white;"
                                                                                                                                                                                            "border: 1px solid #ffffff;"
                                                                                                                                                                                            "}"
                );

    QStringList salesGroups;
    salesGroups.append(accHelper->getGroupIDByName("Cash-in-Hand"));
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    salesGroups.append(accHelper->getGroupIDByName("Sundry Debtors"));
    salesGroups.append(accHelper->getGroupIDByName("Bank Accounts"));
    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Sundry Creditors"))){
        salesGroups.append(acc.groupID);
    }

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Sundry Debtors"))){
        salesGroups.append(acc.groupID);
    }

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Bank Accounts"))){
        salesGroups.append(acc.groupID);
    }

    mainLedgerWidget = new GMLedgerLineMaterial(ledgerHelper,ledgerHelper->getLedgerQueryString(salesGroups),&voucher->ledgerObject);
    mainLedgerWidget->setLabel("Customer Name");
    mainLedgerWidget->setParent(this);
    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:" + backgroundColor + ";}");



    SalesmanSearchBox = new GMEmployeeLineMaterial(userHelper,userHelper->getEmployeeQueryForSearch());
    SalesmanSearchBox->setLabel("SalesMan Name");
    SalesmanSearchBox->setParent(this);
    SalesmanSearchBox->setFixedWidth(200);
    SalesmanSearchBox->setStyleSheet("GMEmployeeLineMaterial{background-color:" + backgroundColor + ";}");
    connect(SalesmanSearchBox,&GMEmployeeLineMaterial::itemSelectedwithItem,
            this,[=](UserProfileDataModel emp){
        voucher->SalesmanID = emp._id;
    });

    QStringList priceLists;

    for(PriceListDataObject plObj: priceHelper->getAllPriceLists())
        priceLists.append(plObj.priceListName);
    priceListWidget = new GMMaterialComboBox(&voucher->priceListName, priceLists);
    priceListWidget->setLabel("Pricelist");
    priceListWidget->setParent(this);
    priceListWidget->setStyleSheet("GMMaterialComboBox{background-color:" + backgroundColor + ";}");
    connect(priceListWidget,&GMMaterialComboBox::selectedItem,
            this, [=](){
        voucher->priceListId = QString::number(priceHelper->getPriceListIDByName(voucher->priceListName));
        itemDelegate->setPriceListId(voucher->priceListId);
        model->setVoucher(voucher);
    });


    itemDelegate = new GMInventoryTransactionDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                uomHelper, voucher->priceListId);
    DataTableWidget->setItemDelegate(itemDelegate);

    model = new GMInventoryItemModel(voucher,1,this);
    //    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    DataTableWidget->hideColumn(5);
    DataTableWidget->hideColumn(6);


    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherDateWidget->setFixedWidth(80);
    voucherDateWidget->setFrame(false);
    voucherDateWidget->setStyleSheet("GMDateEdit{background-color:" + backgroundColor + ";border: none; color: " + textColor + "} "
                                                                                                                               "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right top;}"
                                                                                                                               "QDateEdit::down-arrow {image: url(:/icons/icons/navigation/svg/production/ic_arrow_drop_down_circle_24px.svg);height:15px;width:15px; }");
    voucherDateWidget->setFont(QFont("Calibri", 9, QFont::Normal));
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNumberWidget->setStyleSheet("GMLineEdit{background-color:" + backgroundColor + ";border: none; color: " + textColor + "}");
    voucherNumberWidget->setReadOnly(true);
    voucherNumberWidget->setFont(QFont("Calibri", 11, QFont::Normal));
    voucherNumberWidget->setAlignment(Qt::AlignLeft);

    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration");
    //    voucherNarrationWidget->setMinimumWidth(450);

    AddressDialogWidget = new AddressDialog(ledgerHelper,this);

}

void VoucherEditor::setWidgets()
{

    QLabel *windowLabel = new QLabel(windowTitle);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));



    appBar->appBarLayout()->addWidget(backButton);

    QHBoxLayout *vNobox = new QHBoxLayout;
    vNobox->addWidget(new QLabel("Voucher No"));
    vNobox->addWidget(voucherNumberWidget);

    QHBoxLayout *vDatebox = new QHBoxLayout;
    vDatebox->addWidget(new QLabel("Voucher Date"));
    vDatebox->addWidget(voucherDateWidget);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addLayout(vNobox);
    vbox->addLayout(vDatebox);

    //    appBar->appBarLayout()->addLayout(vbox);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    appBar->appBarLayout()->addStretch(1);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Calibri", 18, QFont::Normal));

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);
    //    layout->addStretch(1);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    QPointer<QHBoxLayout> voucherData = new QHBoxLayout;
    QPointer<QHBoxLayout> extrasLayout = new QHBoxLayout;
    QPointer<QVBoxLayout> tablesLayout = new QVBoxLayout;
    QPointer<QVBoxLayout> advLayout = new QVBoxLayout;

    //    voucherData->setContentsMargins(20,0,0,0);
    mainLayout->addWidget(canvas);
    mainLayout->addLayout(voucherData);
    mainLayout->addLayout(tablesLayout);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(advLayout);

    tablesLayout->addWidget(DataTableWidget);


    extrasLayout->addWidget(voucherNarrationWidget,Qt::AlignLeft);
    extrasLayout->addWidget(coupledVoucherHandler);

    //    voucherData->addWidget(new QLabel("Date"));
    //    voucherData->addWidget(voucherDateWidget);
    //    voucherData->addWidget(new QLabel("Voucher No"));
    //    voucherData->addWidget(voucherNumberWidget);

    voucherData->addLayout(vbox);

    voucherData->addWidget(mainLedgerWidget);
    voucherData->addStretch(1);
    voucherData->addWidget(priceListWidget);
    voucherData->addWidget(SalesmanSearchBox);

    //    ui->verticalLayout_2->addWidget(Widget);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    this->setLayout(verticalLayout);
    verticalLayout->addWidget(Widget);

}

void VoucherEditor::resetWidgets()
{
    delete model;

    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);

    mainLedgerWidget->setText(voucher->ledgerObject.LedgerName);
    SalesmanSearchBox->setText( userHelper->getEmployeeNameFromID(voucher->SalesmanID));

    model = new GMInventoryItemModel(voucher,1,this);
    //    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    if(voucher->ledgerObject.LedgerID.length()>0)
        DataTableWidget->setEnabled(true);
    mainLedgerWidget->setItem(&voucher->ledgerObject);

}

void VoucherEditor::setMainLedger(LedgerMasterDataModel( led))
{
    mainLedgerWidget->setText(led.LedgerName);
    voucher->ledgerObject = led;
    DataTableWidget->setEnabled(true);
    DataTableWidget->setFocusToFirstRow();
}

void VoucherEditor::setAdvancePaid(float amount)
{
    voucher->advanceCash = amount;

    qDebug()<<"&&&&&&&&&&Balance is "<<voucher->advanceCash;

    //    resetWidgets();
}

void VoucherEditor::showShareOptions()
{
    shareOptionsWidget->move(shareButton->pos().x(), shareButton->pos().y() + 90);
    shareOptionsWidget->show();
}

void VoucherEditor::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Close?","Would You Like to Close the voucher? ",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){

        emit closing();
        event->accept();
    }
    else{
        event->ignore();
    }
}

void VoucherEditor::getVoucherstoImport()
{
    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }

    ShowVouchersWidget *selectVoucherWidget = new ShowVouchersWidget();
    selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::SalesVoucher);
    selectVoucherWidget->setLedger(voucher->ledgerObject.LedgerID);
    selectVoucherWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    //    selectVoucherWidget->setWindowState(Qt::WindowMaximized);
    selectVoucherWidget->resize(500,200);
    selectVoucherWidget->show();
    QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),
                     this, SLOT(importItemsFromVoucher(GeneralVoucherDataObject *)));

}

void VoucherEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }

    setBillwiseMapping();

    if(!voucher->fromExternal && voucher->ledgerObject.dbName!=""){
        if(ConfigurationSettingsDatabaseHelper::getValue(promptToSendToOtherLedgers,true).toBool()){
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"Send Voucher","Would You Like to send the voucher to "+voucher->ledgerObject.LedgerName+"?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes)
                voucher->sendFlag = true;
        }
        else {
            voucher->sendFlag = true;
        }
    }

    //    voucher->PaidAmount = voucher->advanceCash;
    TransactionCalculator::VoucherCalculateSales(voucher);

    if(! coupledVoucherHandler->checkModeOfPayEntered() && voucher->advanceCash > 0){
        snackBar->addMessage("Please Set Mode Of Pay");

        return;
    }

    if(voucher->status == QuotationStatus::Started || ( voucher->fromExternal  && voucher->action == 1001 )){
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        int saveStatus = dbHelper->insertVoucher(voucher);

        if(saveStatus && voucher->advanceCash > 0)
            coupledVoucherHandler->createCoupledReceiptVoucher();
    }
    else if(voucher->status != QuotationStatus::Started || (voucher->fromExternal  && voucher->action == 1002)){
        int saveStatus = dbHelper->updateVoucher(voucher,voucher->voucherNumber);
        if(saveStatus ){
            if(voucher->isCoupled)
                coupledVoucherHandler->updateCoupledReceiptVoucher();
            else if(voucher->advanceCash>0){
                coupledVoucherHandler->createCoupledReceiptVoucher();
            }
        }
    }


    if(voucher->fromExternal){
        emit NotificationAccepted();
    }
    emit saveCompleted();

    closeEditor();

}

void VoucherEditor::rejectVoucher()
{
    bool ok;
    QString reason = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                           tr("Reason for rejecting:"), QLineEdit::Normal,
                                           "0", &ok);
    if(ok && !reason.isEmpty()){
        emit NotificationRejected(reason);
        closeEditor();
    }
}

void VoucherEditor::deleteVoucher()
{
    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
    {

        dbHelper->deleteVoucher(voucher);
    }
    else{
        //TODO snackbar
        snackBar->addInstantMessage("Cannot Delete");
    }
}

void VoucherEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void VoucherEditor::showContactDetails()
{


    AddressDialogWidget->setWindowFlags(Qt::Window);
    AddressDialogWidget->setVoucher(voucher);
//    AddressDialogWidget->setAddressText(voucher->Contact.address,
//                                        voucher->Contact.PhoneNumber,
//                                        voucher->Contact.ContactName,
//                                        voucher->Contact.route);
    AddressDialogWidget->setFocus();
    AddressDialogWidget->show();
    AddressDialogWidget->setFocustoPhone();

//    QObject::connect(AddressDialogWidget,&AddressDialog::setAddress,
//                     this, [=](QString Address, QString Phone, QString Name, QString route){
//        voucher->Contact.address= Address;
//        voucher->Contact.PhoneNumber= Phone;
//        voucher->Contact.ContactName= Name;
//        voucher->ledgerObject.ContactPersonNumber = Phone;
//        voucher->ledgerObject.emailAddress = Address;
//        voucher->ledgerObject.ContactPersonName = Name;
//        voucher->Contact.route = route;
//    });

}

void VoucherEditor::printVoucher()
{
    if(DatabaseValues::isPOS){
        CounterPrinterHelper printer;
        printer.printSalesVoucher(voucher);
    }
    else{
        printHelper = new PrinterHelper();
        printHelper->setPrinter(new QPrinter(QPrinter::ScreenResolution));
        printHelper->PrintInventorySalesVoucher(voucher);

    }
}

void VoucherEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    showVoucher(voucher2);
}

void VoucherEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{
    voucher->InventoryItems.append(voucher2->InventoryItems);
    TransactionCalculator::VoucherCalculateSales(voucher);

    coupledVoucherHandler->setParentVoucher(voucher);
    coupledVoucherHandler->resetWidgets();
    if(coupledVoucherHandler->retrieveCoupledReceiptVoucher()){
        voucher->isCoupled = true;
        coupledVoucherHandler->show();
    }
    //    coupledVoucherHandler->addWidgetsInParentVoucher(advanceLayout);
    resetWidgets();

}

void VoucherEditor::setBillwiseMapping()
{
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

void VoucherEditor::sendEmail()
{

}

void VoucherEditor::showVoucher(GeneralVoucherDataObject *obj)
{
    delete  voucher;
    voucher = new GeneralVoucherDataObject();
    voucher = obj;
    TransactionCalculator::VoucherCalculateSales(voucher);
    resetWidgets();
    if(voucher->fromExternal)
        rejectButton->show();

    coupledVoucherHandler->setParentVoucher(voucher);
    if(coupledVoucherHandler->retrieveCoupledReceiptVoucher()){
        voucher->isCoupled = true;
        coupledVoucherHandler->show();
    }
    //    coupledVoucherHandler->addWidgetsInParentVoucher(advanceLayout);

    //    RequirementVoucherDataObject::GetVoucherToVoucherMappingByType(voucher->ReqVoucherList, "ADV");

}

void VoucherEditor::closeEditor()
{
    qDebug()<<"Closing editor";
    emit closing();
    this->close();
}
