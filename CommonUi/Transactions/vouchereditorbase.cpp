#include "vouchereditorbase.h"
#include <QApplication>

VoucherEditorBase::VoucherEditorBase(QWidget *parent) :
    QWidget(parent)

{

    //    editorStatus = EditorStatus::EntryStatus;

    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");

    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    height = screenGeometry.height();
    width = screenGeometry.width();

    initDbHelpers();
    initGlobalObjects();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setExtrasLayout();

    setVoucherDataLayout();
    setWidgets();
    initExtraActions();
    setAppBarLayout();

    //    disableUpdate();

}

VoucherEditorBase::~VoucherEditorBase()
{

}

void VoucherEditorBase::setEditorStatus(int value)
{
    editorStatus = value;
}

void VoucherEditorBase::setPermissions(UiAccessControlDataModel *value)
{
    permissions = value;

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<permissions->allowDelete<<permissions->allowUpdate<<permissions->allowCreate;
    if(!permissions->allowDelete)
        deleteButton->setDisabled(true);

    if(!permissions->allowCreate)
        saveButton->setDisabled(true);
}

void VoucherEditorBase::showVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    this->setWindowTitle(voucher->voucherType + " : " + voucher->voucherNumber );
    this->editorStatus = EditorStatus::EditVoucher;
    if(!permissions->allowUpdate)
        saveButton->setDisabled(true);

    if(voucher->fromExternal){
        rejectButton->show();
        //        importButton->hide();
    }
    voucher->priceListName = priceHelper->getPriceListNameByID(voucher->priceListId);
    //    RequirementVoucherDataObject::GetVoucherToVoucherMappingByType(voucher->ReqVoucherList, "ADV");
}

void VoucherEditorBase::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
//    setTitle(voucher->voucherType);
}

void VoucherEditorBase::saveVoucher()
{
    //    if(voucher->ledgerObject.LedgerID.length()<1){
    //        snackBar->addMessage("Please Set Ledger");
    //        return;
    //    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->ledgerObject.LedgerID;

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

    if(editorStatus == EditorStatus::NewVoucher || ( voucher->fromExternal  && voucher->action == 1001 ))
    {
        if(voucher->TransactionId.length() == 0)
            voucher->TransactionId = DatabaseHelper::getUUID();
        voucher->timestamp = QDateTime::currentDateTime();

//        voucher->status = QuotationStatus::SalesInvoiceRaised;
        saveStatus = dbHelper->insertVoucher(voucher);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<saveStatus<<voucher->ObjToJson(voucher);

        //        if(saveStatus && voucher->advanceCash > 0)
        //            coupledVoucherHandler->createCoupledReceiptVoucher();
    }
    else if(editorStatus == EditorStatus::EditVoucher || (voucher->fromExternal  && voucher->action == 1002)){
        //        qDebug()<<"save price list is"<<priceHelper->getPriceListNameByID(voucher->priceListId);
        saveStatus = dbHelper->updateVoucher(voucher,voucher->voucherNumber);
        //        if(saveStatus ){
        //            if(voucher->isCoupled)
        //                coupledVoucherHandler->updateCoupledReceiptVoucher();
        //            else if(voucher->advanceCash>0){
        //                coupledVoucherHandler->createCoupledReceiptVoucher();
        //            }
        //        }
    }

    if(voucher->fromExternal){
        emit NotificationAccepted();
    }
//    emit saveCompleted();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;

    //    closeEditor();
}

void VoucherEditorBase::initVoucher()
{

}

void VoucherEditorBase::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    qDebug()<<" With V prefix : "<<voucher->VoucherPrefix;
    voucherNumberWidget->setVariable(new QString(voucher->VoucherPrefix+" - "+voucher->voucherNumber));
    voucherNarrationWidget->setVariable(&voucher->narration);
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(voucher->SalesmanID));
    priceListWidget->setText(priceHelper->getPriceListNameByID(voucher->priceListId));
    mainLedgerWidget->setItem(&voucher->ledgerObject);
    invoiceDateWidget->setVariable(&voucher->invoiceDate);
    invoiceNumberWidget->setVariable(&voucher->invoiceNumber);
    ReasonWidget->setVariable(&voucher->QuotationDroppedReason);

}

void VoucherEditorBase::setTitle(const QString &value)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<value;
    windowTitle = value;
    windowLabel->setText(windowTitle);
    setWindowTitle(windowTitle);
}

void VoucherEditorBase::initGlobalObjects()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    voucher = new GeneralVoucherDataObject;

    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor());
    appBar->setMinimumHeight(50);
    appBar->setMaximumHeight(300);

    appBar->setMaximumWidth(width-20);

    windowLabel = new QLabel(windowTitle);
    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);
    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    snackBar = new QtMaterialSnackbar(this);
    drawer = new QtMaterialDrawer(this);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);


    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);

    mainLayout = new QVBoxLayout;
    tableLayout = new QVBoxLayout;
    voucherDataLayout = new QHBoxLayout;
    extrasLayout = new QHBoxLayout;
    extrasLayout2 = new QHBoxLayout;



    voucherDateWidget = new GMDateEdit(&voucher->VoucherDate);
    voucherDateWidget->setFixedWidth(80);
    voucherNumberWidget = new GMLineEdit(&voucher->voucherNumber);
    voucherNumberWidget->setReadOnly(true);

    invDateLabel = new QLabel("Invoice Date");
    invNumberLabel = new QLabel("Invoice No");
    invoiceDateWidget = new GMDateEdit(&voucher->invoiceDate);
    invoiceNumberWidget = new GMLineEdit(&voucher->invoiceNumber);

    QVBoxLayout *lay1 = new QVBoxLayout;
    lay1->addWidget(invDateLabel);
    lay1->addWidget(invoiceDateWidget);

    QVBoxLayout *lay2 = new QVBoxLayout;
    lay2->addWidget(invNumberLabel);
    lay2->addWidget(invoiceNumberWidget);

    QHBoxLayout *invLayout = new QHBoxLayout;
    invLayout->addLayout(lay1);
    invLayout->addLayout(lay2);

    invoiceDataWidget = new QWidget;
    invoiceDataWidget->setLayout(invLayout);
    invoiceDataWidget->hide();


    voucherNarrationWidget = new GMPlainTextEdit(&voucher->narration);
    voucherNarrationWidget->setPlaceholderText("Narration...");
    voucherNarrationWidget->setMinimumWidth(10);

    mainLedgerWidget = new GMLedgerLineMaterial(ledgerHelper,ledgerHelper->getLedgerQueryString(),&voucher->ledgerObject);
    mainLedgerWidget->setLabel("Customer Name");
    mainLedgerWidget->setParent(this);
    mainLedgerWidget->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");
    //    mainLedgerWidget->setFixedWidth(200);

    SalesmanSearchBox = new GMEmployeeLineMaterial(userHelper,userHelper->getEmployeeQueryForSearch());
    voucher->AddedById = LoginValues::userID;
//        SalesmanSearchBox->setItem(userHelper->getEmployeeByID(voucher->SalesmanID));
    SalesmanSearchBox->setLabel("SalesMan");
    SalesmanSearchBox->setParent(this);
    SalesmanSearchBox->setFixedWidth(200);
    SalesmanSearchBox->setObjectName("GMEmployeeLineMaterial");
    SalesmanSearchBox->setStyleSheet("GMEmployeeLineMaterial{background-color:#e0f7fa;}");
    connect(SalesmanSearchBox,&GMEmployeeLineMaterial::itemSelectedwithItem,
            this,[=](UserProfileDataModel emp){
        qDebug()<<Q_FUNC_INFO<<"Salesman"<<voucher->SalesmanID<<emp._id;
        voucher->SalesmanID = emp._id;
    });

    godownWidget = new GMGodownLineMaterial (godownHelper,godownHelper->getGodownForSearchModel());
    godownWidget->setLabel("Godown");
    godownWidget->setParent(this);
    godownWidget->setFixedWidth(200);
    godownWidget->setObjectName("GMGodownLineMaterial");
    godownWidget->setStyleSheet("GMGodownLineMaterial{background-color:#e0f7fa;}");
    godownWidget->hide();




    priceListWidget = new GMPriceListLineMaterial(priceHelper,
                                                  priceHelper->getPriceListsAsQueryModel());
    priceListWidget->setLabel("Pricelist");
    priceListWidget->setParent(this);
    priceListWidget->setStyleSheet("GMPriceListLineMaterial{background-color:#e0f7fa;}");
//    priceListWidget->setPriceListId(&voucher->priceListId);
    priceListWidget->hide();

    QStringList reasonList;
    reasonList<<"Used in kitchen"<<"Expiry"<<"Rejected as not per order"<<"Rejected as not per order";
    reasonList<<"Too much stock"<<"Packing damage"<<"Production Damage"<<"Others";

    ReasonWidget = new GMMaterialComboBox(&voucher->QuotationDroppedReason, reasonList);
    ReasonWidget->setLabel("Reason");
    ReasonWidget->setFixedWidth(200);
    ReasonWidget->setStyleSheet("QWidget{background-color:#e0f7fa;}");
    ReasonWidget->hide();
    ReasonWidget->setParent(this);
    connect(ReasonWidget, &GMMaterialComboBox::selectedItem, this, [=](){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->QuotationDroppedReason;
       voucher->QuotationDroppedReason = ReasonWidget->text();
    });
    connect(ReasonWidget, &GMMaterialComboBox::editFinished, this, [=](){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->QuotationDroppedReason;
       voucher->QuotationDroppedReason = ReasonWidget->text();
    });


}

void VoucherEditorBase::initExtraActions()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(appBar);
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));

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

    contactButton = new QtMaterialIconButton(QtMaterialTheme::icon("communication", "contacts"));
    contactButton->setColor(QColor(255,250,0));
    contactButton->setIconSize(QSize(30,30));

    moreButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation","more_vert"));
    moreButton->setColor(QColor(255,250,0));
    moreButton->setIconSize(QSize(30,30));

    deleteButton = new QtMaterialIconButton(QtMaterialTheme::icon("action","delete"));
    deleteButton->setColor(QColor(255,250,0));
    deleteButton->setIconSize(QSize(30,30));
//    deleteButton->hide();



    exportButton = new QtMaterialIconButton(QtMaterialTheme::icon("communication", "import_export"));
    exportButton->setColor(QColor(255,250,0));
    exportButton->setIconSize(QSize(30,30));
    exportButton->hide();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));

    paymentButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "payment"));
    paymentButton->setColor(QColor(255,250,0));
    paymentButton->setIconSize(QSize(30,30));
    paymentButton->hide();


    shareOptionsWidget = new ShareTools("Voucher", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup);
    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&VoucherEditorBase::printVoucher);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&VoucherEditorBase::exportToPdf);

    //    connect(saveButton, &QtMaterialFloatingActionButton::clicked, this,
    //            &VoucherEditorBase::saveVoucher);

    setDrawerLayout();

    connect(backButton,&QPushButton::clicked,
            this,&VoucherEditorBase::closeEditor);
    connect(rejectButton,&QPushButton::clicked,
            this,&VoucherEditorBase::rejectVoucher);
    connect(shareButton,&QPushButton::clicked,
            this,&VoucherEditorBase::showShareOptions);
    QObject::connect(contactButton,&QPushButton::clicked,
                     this,&VoucherEditorBase::showContactDetails);
    QObject::connect(deleteButton,&QPushButton::clicked,
                     this, &VoucherEditorBase::deleteVoucher);
    QObject::connect(contactButton,&QPushButton::clicked,
                     this,&VoucherEditorBase::showContactDetails);
    QObject::connect(exportButton,&QPushButton::clicked,
                     drawer,&QtMaterialDrawer::openDrawer);

}

void VoucherEditorBase::initDbHelpers()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    ledgerHelper = new LedgerMasterDatabaseHelper;
    accHelper = new AccountGroupMasterDatabaseHelper;
    //    dbHelper = new salesVoucherDatabaseHelper;
    uomHelper = new UomDataBaseHelper;
    userHelper = new UserProfileDatabaseHelper;
    itemsHelper = new SalesInventoryItemDatabaseHelper;
    rHelper = new receiveChannelDatabaseHelper;
    priceHelper = new PricelistDatabaseHelper;
    godownHelper = new GodownDatabaseHelper;
}

void VoucherEditorBase::setWidgets()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);

    QPointer<QWidget> Widget = new QWidget(this);
    Widget->setLayout(mainLayout);

    Widget->setMaximumWidth(width-10);

    mainLayout->addWidget(canvas);
    mainLayout->addLayout(voucherDataLayout);
    mainLayout->addLayout(extrasLayout2);
    mainLayout->addLayout(tableLayout);
    mainLayout->addLayout(extrasLayout);

    mainLayout->setStretch(3,1);
    verticalLayout = new QVBoxLayout(this);
    this->setLayout(verticalLayout);
    verticalLayout->addWidget(Widget);
}

void VoucherEditorBase::setVoucherDataLayout()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucherDataLayout->addWidget(new QLabel("Date"));
    voucherDataLayout->addWidget(voucherDateWidget);
    voucherDataLayout->addWidget(new QLabel("Voucher No"));
    voucherDataLayout->addWidget(voucherNumberWidget);
    voucherDataLayout->addWidget(mainLedgerWidget);
    voucherDataLayout->addWidget(invoiceDataWidget);
//    voucherDataLayout->addWidget(invDateLabel);
//    voucherDataLayout->addWidget(invoiceDateWidget);
//    voucherDataLayout->addWidget(invNumberLabel);
//    voucherDataLayout->addWidget(invoiceNumberWidget);
    voucherDataLayout->addStretch(1);
    voucherDataLayout->addWidget(godownWidget);
    voucherDataLayout->addWidget(priceListWidget);
    voucherDataLayout->addWidget(SalesmanSearchBox);
    voucherDataLayout->addWidget(ReasonWidget);
}

void VoucherEditorBase::setAppBarLayout()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    appBar->appBarLayout()->addWidget(backButton);
    appBar->appBarLayout()->addWidget(windowLabel);
    appBar->appBarLayout()->addWidget(shareButton);
    appBar->appBarLayout()->addWidget(exportButton);
    appBar->appBarLayout()->addWidget(contactButton);
    appBar->appBarLayout()->addWidget(deleteButton);
    appBar->appBarLayout()->addWidget(paymentButton);
    appBar->appBarLayout()->addWidget(moreButton);

}

void VoucherEditorBase::setExtrasLayout()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    extrasLayout->addWidget(voucherNarrationWidget, Qt::AlignLeft);
    //     extrasLayout->addWidget(coupledVoucherHandler);

}

void VoucherEditorBase::addToExtrasLayout(QWidget *wid)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    extrasLayout->addWidget(wid);

}

void VoucherEditorBase::addToExtrasLayout2(QWidget *wid)
{
    extrasLayout2->addWidget(wid, 0, Qt::AlignLeft);
}

void VoucherEditorBase::addToDrawerLayout(QWidget *wid)
{
    drawer->drawerLayout()->addWidget(wid);

}

void VoucherEditorBase::closeEditor()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;
    this->close();
    emit closing();


}

void VoucherEditorBase::disableSave()
{
//    if(!permissions->allowUpdate)
        saveButton->setDisabled(true);

}

void VoucherEditorBase::rejectVoucher()
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


void VoucherEditorBase::deleteVoucher()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete Voucher","Would You Like to Delete?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        if(editorStatus== EditorStatus::EditVoucher)    //Check Authority
        {
            dbHelper->deleteVoucher(voucher);
            closeEditor();
        }
        else{
            //TODO snackbar
            snackBar->addInstantMessage("Cannot Delete");
        }
    }
}

void VoucherEditorBase::setDrawerLayout()
{

}

void VoucherEditorBase::printVoucher(QPrinter *printer)
{

}

void VoucherEditorBase::exportToPdf(QPrinter *printer)
{

}

void VoucherEditorBase::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void VoucherEditorBase::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void VoucherEditorBase::setMainLedger(LedgerMasterDataModel led)
{

}

void VoucherEditorBase::setSalesMan(int user)
{

}

void VoucherEditorBase::setGodown(QString godownId)
{

}

void VoucherEditorBase::setAdvancePaid(float amount)
{

}

void VoucherEditorBase::setTableLayout(QWidget *widget)
{
    tableLayout->addWidget(widget);
}

void VoucherEditorBase::showShareOptions()
{
    if(editorStatus == EditorStatus::NewVoucher){
        snackBar->addMessage("Please save the voucher to continue");
        return;
    }

    shareOptionsWidget->move(shareButton->pos().x(), shareButton->pos().y() + 90);
    shareOptionsWidget->show();
}

void VoucherEditorBase::showContactDetails()
{
    AddressDialogWidget = new AddressDialog(ledgerHelper);

    AddressDialogWidget->setWindowFlags(Qt::Window);
    AddressDialogWidget->setVoucher(voucher);

    AddressDialogWidget->setFocus();
    AddressDialogWidget->show();
    AddressDialogWidget->setFocustoPhone();

    QObject::connect(AddressDialogWidget,&AddressDialog::setAddress,
                     this, [=](QString Address, QString Phone, QString Name, QString route){
        voucher->Contact.address= Address;
        voucher->Contact.PhoneNumber= Phone;
        voucher->Contact.ContactName= Name;
        voucher->ledgerObject.ContactPersonNumber = Phone;
        voucher->ledgerObject.emailAddress = Address;
        voucher->ledgerObject.ContactPersonName = Name;
        voucher->Contact.route = route;
    });

}




void VoucherEditorBase::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F8){
        QMessageBox box ; box.setText("F8"); box.exec();
    }
}
