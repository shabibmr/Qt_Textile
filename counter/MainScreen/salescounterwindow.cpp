#include "salescounterwindow.h"
#include "ui_salescounterwindow.h"
#include <math.h>
#include <windows.h>


SalesCounterWindow::SalesCounterWindow(int id,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalesCounterWindow)
{
    ui->setupUi(this);



    CounterSettingsDatabaseHelper* settingsDB = new CounterSettingsDatabaseHelper();
    settingsDB->getCounterSettings(LoginValues::voucherPrefix);
    CompanyProfileDataBaseHelper* companyHelper = new CompanyProfileDataBaseHelper();
    LoginValues::setCompany(companyHelper->getCompanyDetails());

    itemsHelper = new SalesInventoryItemDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    salesOrderHelper = new SalesOrderDatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper();
    voucher = new GeneralVoucherDataObject();
    printer = new CounterPrinterHelper();
    lineNameLine = new GMInventoryItemSearch(itemsHelper, itemsHelper->getInventoryItemsQueryModel());
    UomDataBaseHelper *uomHelper = new UomDataBaseHelper();
    GMUOMSearchWidget* uline = new GMUOMSearchWidget(uomHelper,uomHelper->getUomModelByItem("0x4"));

    QObject::connect(lineNameLine,SIGNAL(itemSelectedwithItem(inventoryItemDataModel)),this,
                     SLOT(addNewItemComp(inventoryItemDataModel)));


    ui->verticalLayout_6->addWidget(lineNameLine);
    //    ui->verticalLayout_6->addWidget(uline);



    UserProfileDatabaseHelper *userHelper = new UserProfileDatabaseHelper();

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

    //    ui->actionProfit_and_Loss->setVisible(false);
    //    ui->actionTrial_Balance->setVisible(false);
    //    ui->actionBalance_Sheet->setVisible(false);

    ui->tabWidget->removeTab(1);

    //     ui->menubar->hide();         // branch


    ui->priceQuantityToggleButton->setCheckable(true);
    //    ui->pickCallToolButton->setCheckable(true);

    ui->vATAt5LineEdit->setAlignment(Qt::AlignRight);
    ui->subTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->grandTotalLineEdit->setAlignment(Qt::AlignRight);

    ui->pickCallToolButton->setText("Delivery");
    ui->pickCallToolButton->setIcon(QIcon(":/images/delivery.png"));
    ui->pickCallToolButton->setIconSize(QSize(50,50));
    ui->pickCallToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->pickCallToolButton->setAutoRaise(true);

    ui->itemDeleteToolButton->setText("Delete");
    ui->itemDeleteToolButton->setIcon(QIcon(":/images/delete.png"));
    ui->itemDeleteToolButton->setIconSize(QSize(50,50));
    ui->itemDeleteToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->itemDeleteToolButton->setAutoRaise(true);

    ui->draftToolButton->setText("Draft");
    ui->draftToolButton->setIcon(QIcon(":/images/save.png"));
    ui->draftToolButton->setIconSize(QSize(50,50));
    ui->draftToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->draftToolButton->setAutoRaise(true);

    ui->refreshToolButton->setText("Refresh");
    ui->refreshToolButton->setIcon(QIcon(":/images/refresh.png"));
    ui->refreshToolButton->setIconSize(QSize(50,50));
    ui->refreshToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->refreshToolButton->setAutoRaise(true);

    ui->draftedBillsToolButton->setText("Drafted Bills");
    ui->draftedBillsToolButton->setIcon(QIcon(":/images/drafted.png"));
    ui->draftedBillsToolButton->setIconSize(QSize(50,50));
    ui->draftedBillsToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->draftedBillsToolButton->setAutoRaise(true);

    ui->modeOfPayToolButton->setText("Mode of Pay");
    ui->modeOfPayToolButton->setIcon(QIcon(":/images/paymentmode.png"));
    ui->modeOfPayToolButton->setIconSize(QSize(50,50));
    ui->modeOfPayToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->modeOfPayToolButton->setAutoRaise(true);

    ui->openTillButton->setText("Open Till");
    ui->openTillButton->setIcon(QIcon(":/images/opentill.png"));
    ui->openTillButton->setIconSize(QSize(50,50));
    ui->openTillButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->openTillButton->setAutoRaise(true);

    ui->searchItemButton->setText("Search Item");
    ui->searchItemButton->setIcon(QIcon(":/images/searchitem.png"));
    ui->searchItemButton->setIconSize(QSize(50,50));
    ui->searchItemButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->searchItemButton->setAutoRaise(true);

    ui->shiftCloseButton->setText("Shift Close");
    ui->shiftCloseButton->setIcon(QIcon(":/images/shift.png"));
    ui->shiftCloseButton->setIconSize(QSize(50,50));
    ui->shiftCloseButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->shiftCloseButton->setAutoRaise(true);

    ui->screenLockButton->setText("Screen Lock");
    ui->screenLockButton->setIcon(QIcon(":/images/screenlock.png"));
    ui->screenLockButton->setIconSize(QSize(50,50));
    ui->screenLockButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->screenLockButton->setAutoRaise(true);

    ui->adminButton->setText("Admin");
    ui->adminButton->setIcon(QIcon(":/images/admin.png"));
    ui->adminButton->setIconSize(QSize(50,50));
    ui->adminButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->adminButton->setAutoRaise(true);

    ui->priceQuantityToggleButton->setText("Price/Qty");
    ui->priceQuantityToggleButton->setIcon(QIcon(":/images/priceqty.png"));
    ui->priceQuantityToggleButton->setIconSize(QSize(50,50));
    ui->priceQuantityToggleButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->priceQuantityToggleButton->setAutoRaise(true);

    ui->miscellaneousButton->setText("Misc");
    ui->miscellaneousButton->setIcon(QIcon(":/images/misc.png"));
    ui->miscellaneousButton->setIconSize(QSize(50,50));
    ui->miscellaneousButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->miscellaneousButton->setAutoRaise(true);

    ui->reportsToolButton->setText("Reports");
    ui->reportsToolButton->setIcon(QIcon(":/images/graph.png"));
    ui->reportsToolButton->setIconSize(QSize(50,50));
    ui->reportsToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->reportsToolButton->setAutoRaise(true);

    ui->cashOutToolButton->setText("Print Bill");
    ui->cashOutToolButton->setIcon(QIcon(":/images/print.png"));
    ui->cashOutToolButton->setIconSize(QSize(50,50));
    ui->cashOutToolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->cashOutToolButton->setAutoRaise(true);

    ui->customerWidget->setHidden(true);

    ui->companyNameLabel->setPixmap(QPixmap(":/images/greymatter.png"));
    ui->companyNameLabel->setScaledContents(true);
    ui->companyNameLabel->setFixedSize(130,100);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    ui->dateLabel->setStyleSheet("color:#000000;");

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

    ui->dateLabel->setText(tText);

    QHBoxLayout *numLay = new QHBoxLayout(ui->numberPadWidget);
    numPad = new NumberPad(ui->numberPadWidget);
    numLay->addWidget(numPad);

    QObject::connect(numPad,SIGNAL(clicked(QString)),this,SLOT(numpadSlots(QString)));
    QObject::connect(numPad,SIGNAL(plus()),this,SLOT(plusClicked()));
    QObject::connect(numPad,SIGNAL(minus()),this,SLOT(minusClicked()));

    QObject::connect(numPad,SIGNAL(dot()),this,SLOT(dotClicked()));
    QObject::connect(numPad,SIGNAL(deleted()),this,SLOT(deleteClicked()));
    QObject::connect(numPad,SIGNAL(clearAll()),this,SLOT(clearClicked()));


    connectMenuActions();


    serial = new QSerialPort(this);
    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SalesCounterWindow::handleError);


    connect(serial, &QSerialPort::readyRead, this, &SalesCounterWindow::readData);

    openSerialPort();
    on_refreshToolButton_clicked();

}

SalesCounterWindow::~SalesCounterWindow()
{
    delete ui;
}

void SalesCounterWindow::setTableHeading()
{
    int i =0;
    ui->tableWidget->setColumnCount(7);

    QTableWidgetItem* Item = new QTableWidgetItem;
    Item->setText("ITEM");
    ui->tableWidget->setHorizontalHeaderItem(i++,Item);

    QTableWidgetItem* uom = new QTableWidgetItem;
    uom->setText("UOM");
    ui->tableWidget->setHorizontalHeaderItem(i++,uom);

    QTableWidgetItem* qty = new QTableWidgetItem;
    qty->setText("QTY");
    ui->tableWidget->setHorizontalHeaderItem(i++,qty);

    QTableWidgetItem* price = new QTableWidgetItem;
    price->setText("PRICE");
    ui->tableWidget->setHorizontalHeaderItem(i++,price);

    QTableWidgetItem* Vat = new QTableWidgetItem;
    Vat->setText("GST");
    ui->tableWidget->setHorizontalHeaderItem(i++,Vat);

    QTableWidgetItem* amount = new QTableWidgetItem;
    amount->setText("AMOUNT");
    ui->tableWidget->setHorizontalHeaderItem(i++,amount);

    QTableWidgetItem* edit = new QTableWidgetItem;
    edit->setText("EDIT");
    ui->tableWidget->setHorizontalHeaderItem(i++,edit);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,80);
    ui->tableWidget->setColumnWidth(3,80);
    ui->tableWidget->setColumnWidth(4,80);
    ui->tableWidget->setColumnWidth(5,80);
    ui->tableWidget->setColumnWidth(6,50);
}

void SalesCounterWindow::setTable()
{
    if(voucher->InventoryItems.size()>0)
        calcTotals();
    int rowCount =0;

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(voucher->InventoryItems.length());
    for(int i =0;i<voucher->InventoryItems.length();i++){
        ui->tableWidget->setRowHeight(rowCount,40);
        QTableWidgetItem* itemName = new QTableWidgetItem;
        QTableWidgetItem* uomName = new QTableWidgetItem;
        QTableWidgetItem* Qty = new QTableWidgetItem;
        QTableWidgetItem* Price = new QTableWidgetItem;
        QTableWidgetItem* vat = new QTableWidgetItem;
        QTableWidgetItem* amount = new QTableWidgetItem;
        QToolButton* edit = new QToolButton;
        edit->setProperty("row",i);
        QObject::connect(edit,SIGNAL(clicked(bool)),this,SLOT(openItemDetail()));

        itemName->setText(voucher->InventoryItems[i].BaseItem.ItemName);
        uomName->setText(voucher->InventoryItems[i].BaseItem.uomObject.UomName);
        Qty->setText(QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2));
        Price->setText(QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2));
        vat->setText(QString::number(voucher->InventoryItems[i].BaseItem.taxAmount,'f',2));
        amount->setText(QString::number(voucher->InventoryItems[i].BaseItem.subTotal+
                                        voucher->InventoryItems[i].BaseItem.taxAmount,'f',2));
        edit->setIcon(QIcon(":/images/edit.png"));
        edit->setAutoRaise(true);

        Qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        Price->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        vat->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        amount->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);
        uomName->setFlags(uomName->flags() ^ Qt::ItemIsEditable);
        Qty->setFlags(Qty->flags() ^ Qt::ItemIsEditable);
        Price->setFlags(Price->flags() ^ Qt::ItemIsEditable);
        vat->setFlags(vat->flags() ^ Qt::ItemIsEditable);
        amount->setFlags(amount->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget->setItem(i,0,itemName);
        ui->tableWidget->setItem(i,1,uomName);
        ui->tableWidget->setItem(i,2,Qty);
        ui->tableWidget->setItem(i,3,Price);
        ui->tableWidget->setItem(i,4,vat);
        ui->tableWidget->setItem(i,5,amount);
        ui->tableWidget->setCellWidget(i,6,edit);
        rowCount++;
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
        qDebug()<<"substring = "<<subString;
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
        qDebug()<<"WORD22 = "<<word;
        str= word.toLocal8Bit();
        //        str.prepend(0x0D);
        writeData(str);
        //        writeData(1B 46 42 [DATA X 40] 0D");
    }
    lineNameLine->setFocus();
}

void SalesCounterWindow::setAdditionalTable()
{

}

void SalesCounterWindow::on_cashPaidLineEdit_textChanged(const QString &arg1)
{
    //float balance = ui->grandTotalLineEdit->text().toFloat() - ui->cashPaidLineEdit->text().toFloat();
    //ui->balanceLineEdit->setText(QString::number(balance,'f',2));
}

void SalesCounterWindow::on_itemDeleteToolButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    voucher->InventoryItems.removeAt(row);
    setTable();
}

void SalesCounterWindow::on_refreshToolButton_clicked()
{
    if(voucher!=NULL){
        //        qDebug()<<"deleting Voucher";
        delete voucher;
        //        qDebug()<<"Voucher Deleted";
    }
    voucher = new GeneralVoucherDataObject();

    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->AddedById = LoginValues::getUserID();
    voucher->AddedBy = LoginValues::getUserName();
    voucher->SalesmanID = LoginValues::getUserID();
    voucher->VoucherDate = QDate::currentDate();
    voucher->ModeOfService = ServiceMode::Store;
    ui->voucherDate->setText(QDate::currentDate().toString("dd-MM-yyyy"));
    ui->voucherNo->setText(QString::number(salesHelper->getMaxID()+1));
    ui->cashierNameLabel->setText(LoginValues::userName);
    ui->POSNoLabel->setText(CounterSettingsDataModel::POSNumber);

    ui->customerWidget->setHidden(true);
    //    ui->subTotalLineEdit->clear();
    //    ui->vATAt5LineEdit->clear();
    //    ui->grandTotalLineEdit->clear();
    ui->tableWidget->clear();
    setTableHeading();
    setTable();
    QByteArray str = "NEXT CUSTOMER";
    str.prepend(0x0C);
    writeData(str);

}

void SalesCounterWindow::on_cashOutToolButton_clicked()
{
    if(voucher->InventoryItems.size()>0){
        LedgerMasterDatabaseHelper *LedgerHelper = new LedgerMasterDatabaseHelper;
        voucher->ledgerObject = LedgerHelper->getLedgerObjectByID(CounterSettingsDataModel::DefaultCash);
        voucher->NoOfCopies = CounterSettingsDataModel::CashOutPrintCopies;
        saveVoucher();
    }
}

void SalesCounterWindow::on_draftToolButton_clicked()
{
    if(voucher->InventoryItems.size()>0){
        saveDraft();
    }
}

void SalesCounterWindow::on_modeOfPayToolButton_clicked()
{
    if(voucher->InventoryItems.size()>0){
        ModeOfPayWidget = new ModeOfPay(ledHelper, this);
        ModeOfPayWidget->setWindowFlags(Qt::Window);
        ModeOfPayWidget->setAttribute(Qt::WA_DeleteOnClose);
        ModeOfPayWidget->showFullScreen();
        ModeOfPayWidget->setVoucher(voucher);
        QObject::connect(ModeOfPayWidget,SIGNAL(modeOfPaySignal()),this,SLOT(ModeOfPaySlot()));
    }
}

void SalesCounterWindow::ModeOfPaySlot()
{
    calcTotals();
    saveVoucher();
}

void SalesCounterWindow::on_draftedBillsToolButton_clicked()
{
    SavedDraftsWidget = new SavedDrafts(this);
    SavedDraftsWidget->setWindowFlags(Qt::Window);
    SavedDraftsWidget->setAttribute(Qt::WA_DeleteOnClose);
    SavedDraftsWidget->showFullScreen();
    QObject::connect(SavedDraftsWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(setVoucher(GeneralVoucherDataObject*)));
}

void SalesCounterWindow::on_pickCallToolButton_clicked()
{
    voucher->ModeOfService = ServiceMode::Delivery;
//    AddressDialogWidget = new AddressDialog(new LedgerMasterDatabaseHelper(),this);
//    AddressDialogWidget->setAttribute(Qt::WA_DeleteOnClose);
//    AddressDialogWidget->setWindowFlags(Qt::Window);
//    AddressDialogWidget->show();
//    QObject::connect(AddressDialogWidget,SIGNAL(setAddress(QString,QString,QString)),
//                     this,SLOT(getCustomerDetails(QString,QString,QString)));
}

void SalesCounterWindow::calcTotals()
{

    TransactionCalculator::VoucherCalculateSales(voucher);

    ui->subTotalLineEdit   -> setText(QString::number(voucher->subTotal,'f',2));
    ui->vATAt5LineEdit     -> setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->grandTotalLineEdit ->setText(QString::number(voucher->grandTotal,'f',2));



}

float SalesCounterWindow::getRoundOff(float Amount,float multiple)
{
    if(multiple == 0){
        return Amount;
    }

    float result = Amount + multiple/2;

    result -= fmod(result,multiple);
    return result;
}

void SalesCounterWindow::openInventoryItems()
{
    inventoryItem* inventoryItemWidget = new inventoryItem(this);
    inventoryItemWidget->setWindowFlags(Qt::Window);
    inventoryItemWidget->setWindowTitle("Inventory");
    inventoryItemWidget->setAttribute(Qt::WA_DeleteOnClose);
    inventoryItemWidget->show();
}

void SalesCounterWindow::openUOMList()
{

    qDebug()<<"Uom clicked";
    UOM* UOMWidget = new UOM(this);
    UOMWidget->setWindowFlags(Qt::Window);
    UOMWidget->setAttribute(Qt::WA_DeleteOnClose);
    UOMWidget->setWindowTitle("UOM List");
    UOMWidget->show();
}

void SalesCounterWindow::openAccountGroups()
{
    accountGroups* accountGroupsWidget = new accountGroups(this);
    accountGroupsWidget->setWindowFlags(Qt::Window);
    accountGroupsWidget->setWindowTitle("Account Groups");
    accountGroupsWidget->setAttribute(Qt::WA_DeleteOnClose);
    accountGroupsWidget->show();
}

void SalesCounterWindow::openSettings()
{
    Settings* SettingsWidget = new Settings(this);
    SettingsWidget->setWindowFlags(Qt::Window);
    SettingsWidget->setWindowTitle("Settings");
    SettingsWidget->setAttribute(Qt::WA_DeleteOnClose);
    SettingsWidget->show();
}

void SalesCounterWindow::openHelpContents()
{
    HelpContents* HelpContentsWidget = new HelpContents(this);
    HelpContentsWidget->setWindowFlags(Qt::Window);
    HelpContentsWidget->setWindowTitle("Help Contents");
    HelpContentsWidget->setAttribute(Qt::WA_DeleteOnClose);
    HelpContentsWidget->show();
}

void SalesCounterWindow::openAbout()
{
    About* AboutWidget = new About(this);
    AboutWidget->setWindowFlags(Qt::Window);
    AboutWidget->setWindowTitle("About");
    AboutWidget->setAttribute(Qt::WA_DeleteOnClose);
    AboutWidget->show();
}
void SalesCounterWindow::openDash()
{
    LedgersDisplayController* AboutWidget = new LedgersDisplayController(this);
    AboutWidget->setWindowFlags(Qt::Window);
    AboutWidget->setWindowTitle("WORKSHOP");
    AboutWidget->setAttribute(Qt::WA_DeleteOnClose);
    AboutWidget->show();
}

void SalesCounterWindow::openShiftWidget(float value)
{
    qDebug()<<value;
    ShiftEntryWidget = new ShiftEntry(this);
    ShiftEntryWidget->setDenominationValues(value);
    ShiftEntryWidget->setWindowFlags(Qt::Window);
    ShiftEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    ShiftEntryWidget ->showFullScreen();
    QObject::connect(ShiftEntryWidget,SIGNAL(shiftClosed()),this,SLOT(on_refreshToolButton_clicked()));
}

void SalesCounterWindow::openOrdersEntry()
{
//    IteemwiseSalesOrderList* SalesEntryWidget = new IteemwiseSalesOrderList(new SalesOrderDatabaseHelper ,this);
//    SalesEntryWidget->setWindowTitle("Sales");
//    SalesEntryWidget->setWindowFlags(Qt::Window);
//    SalesEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
//    SalesEntryWidget->showMaximized();
}

void SalesCounterWindow::openSalesEntry()
{
    SalesVoucherList* SalesEntryWidget = new SalesVoucherList(this);
    SalesEntryWidget->setWindowTitle("Sales");
    SalesEntryWidget->setWindowFlags(Qt::Window);
    SalesEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    SalesEntryWidget->showMaximized();
}

void SalesCounterWindow::openPurchaseEntry()
{
    PurchaseEntry* PurchaseEntryWidget = new PurchaseEntry(this);
    PurchaseEntryWidget->setWindowTitle("Purchases");
    PurchaseEntryWidget->setWindowFlags(Qt::Window);
    PurchaseEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    PurchaseEntryWidget->showMaximized();
}

void SalesCounterWindow::openSalesordersVoucher()
{
    salesOrders* billwiseWidget = new salesOrders() ;
    billwiseWidget->setWindowFlags(Qt::Window);
    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    billwiseWidget->show();

}

void SalesCounterWindow::openWorkOrder()
{
    //    WorkOrderList* billwiseWidget = new WorkOrderList ;
    //    billwiseWidget->setWindowFlags(Qt::Window);
    //    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    //    billwiseWidget->show();

}



void SalesCounterWindow::openLedgerwise()
{
    AddBillwiseRecord* billwiseWidget = new AddBillwiseRecord() ;
    billwiseWidget->setWindowFlags(Qt::Window);
    billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
    LedgerMasterDataModel ledger = ledHelper->getLedgerObjectByID("0x2x15x173");
    billwiseWidget->setLedger(&ledger);
    billwiseWidget->showPendingBills(); //ledgerID
    billwiseWidget->show();

}

void SalesCounterWindow::openReceiptEntry()
{
    ReceiptEntry* ReceiptEntryWidget = new ReceiptEntry(this);
    ReceiptEntryWidget->setWindowTitle("Receipts");
    ReceiptEntryWidget->setWindowFlags(Qt::Window);
    ReceiptEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    ReceiptEntryWidget->show();
}

void SalesCounterWindow::openJournalEntry()
{
    journal* journalWidget = new journal(this);
    journalWidget->setWindowTitle("Journals");
    journalWidget->setWindowFlags(Qt::Window);
    journalWidget->setAttribute(Qt::WA_DeleteOnClose);
    journalWidget->show();
}

void SalesCounterWindow::openStockEntry()
{
    StockEntry* StockEntryWidget = new StockEntry(this);
    StockEntryWidget->setWindowTitle("Stock");
    StockEntryWidget->setWindowFlags(Qt::Window);
    StockEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    StockEntryWidget->show();
}

void SalesCounterWindow::openStockJournal()
{
    stockJournals* stockJournalsWidget = new stockJournals(this);
    stockJournalsWidget->setWindowTitle("Stock Journal");
    stockJournalsWidget->setWindowFlags(Qt::Window);
    stockJournalsWidget->setAttribute(Qt::WA_DeleteOnClose);
    stockJournalsWidget->show();
}

void SalesCounterWindow::openPurchaseReturn()
{
    debitNote *debitNoteWidget = new debitNote(this);
    debitNoteWidget->setWindowTitle("Purchase Returns");
    debitNoteWidget->setWindowFlags(Qt::Window);
    debitNoteWidget->setAttribute(Qt::WA_DeleteOnClose);
    debitNoteWidget->show();
}

void SalesCounterWindow::openSalesReturn()
{
    creditNote *creditNoteWidget = new creditNote(this);
    creditNoteWidget->setWindowTitle("Sales Returns");
    creditNoteWidget->setWindowFlags(Qt::Window);
    creditNoteWidget->setAttribute(Qt::WA_DeleteOnClose);
    creditNoteWidget->show();
}

void SalesCounterWindow::openPaymentEntry()
{
    Payment* PaymentEntryWidget = new Payment(this);
    PaymentEntryWidget->setWindowTitle("Payments");
    PaymentEntryWidget->setWindowFlags(Qt::Window);
    PaymentEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    PaymentEntryWidget->show();
}

void SalesCounterWindow::openDenominations()
{
    Denominations *DenominationsWidget = new Denominations();
    DenominationsWidget->setWindowFlags(Qt::Window);
    DenominationsWidget->setAttribute(Qt::WA_DeleteOnClose);
    DenominationsWidget->showFullScreen();
    QObject::connect(DenominationsWidget,SIGNAL(totalSales(float)),this,SLOT(openShiftWidget(float)));
}

void SalesCounterWindow::openClosingEntry()
{
    ClosingEntryWidget = new ClosingEntry(this);
    ClosingEntryWidget->setWindowTitle("Daily Closing");
    ClosingEntryWidget->setWindowFlags(Qt::Window);
    ClosingEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    ClosingEntryWidget->showFullScreen();
}

void SalesCounterWindow::closeApplication()
{
    QMessageBox::StandardButton reply;

    reply= QMessageBox::question(this,"Exit??","Are you Sure You want to Close",QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        qApp->exit();
    }
}

void SalesCounterWindow::openLedgers()
{
    ledgerMaster* ledgerMasterWidget = new ledgerMaster(this);
    ledgerMasterWidget->setWindowTitle("Ledger Masters");
    ledgerMasterWidget->setWindowFlags(Qt::Window);
    ledgerMasterWidget->setAttribute(Qt::WA_DeleteOnClose);
    ledgerMasterWidget->showMaximized();
}

void SalesCounterWindow::openLedgerReport()
{
    LedgerReport* LedgerReportWidget = new LedgerReport(this);
    LedgerReportWidget->setWindowTitle("Ledger Report");
    LedgerReportWidget->setWindowFlags(Qt::Window);
    LedgerReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    LedgerReportWidget->showMaximized();
}

void SalesCounterWindow::openDayBook()
{
//    Daybook* DaybookWidget = new Daybook(this);
//    DaybookWidget->setWindowTitle("DayBook");
//    DaybookWidget->setWindowFlags(Qt::Window);
//    DaybookWidget->setAttribute(Qt::WA_DeleteOnClose);
//    DaybookWidget->showMaximized();
}

void SalesCounterWindow::openTrialBalance()
{
    TrialBalance* TrialBalanceWidget = new TrialBalance(this);
    TrialBalanceWidget->setWindowTitle("Trial Balance");
    TrialBalanceWidget->setWindowFlags(Qt::Window);
    TrialBalanceWidget->setAttribute(Qt::WA_DeleteOnClose);
    TrialBalanceWidget->showMaximized();
}

void SalesCounterWindow::openProfitLoss()
{
    ProfitAndLossReport* ProfitLossWidget = new ProfitAndLossReport(this);
    ProfitLossWidget->setWindowTitle("Profit and Loss");
    ProfitLossWidget->setWindowFlags(Qt::Window);
    ProfitLossWidget->setAttribute(Qt::WA_DeleteOnClose);
    ProfitLossWidget->showMaximized();
}

void SalesCounterWindow::openBalanceSheet()
{
    BalanceSheet* BalanceSheetWidget = new BalanceSheet(this);
    BalanceSheetWidget->setWindowTitle("Balance Sheet");
    BalanceSheetWidget->setWindowFlags(Qt::Window);
    BalanceSheetWidget->setAttribute(Qt::WA_DeleteOnClose);
    BalanceSheetWidget->showMaximized();
}

void SalesCounterWindow::openVATReport()
{
    VATReport* VATReportWidget = new VATReport(this);
    VATReportWidget->setWindowTitle("VAT Report");
    VATReportWidget->setWindowFlags(Qt::Window);
    VATReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    VATReportWidget->showMaximized();
}

void SalesCounterWindow::openOutstandingReport()
{
//    GroupwiseOutstanding* OutstandingWidget = new GroupwiseOutstanding(this);
//    OutstandingWidget->setWindowTitle("Outstanding Report");
//    OutstandingWidget->setWindowFlags(Qt::Window);
//    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
//    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openPurchaseItemwise()
{
    ItemwisePurchaseReport* OutstandingWidget = new ItemwisePurchaseReport(this);
    OutstandingWidget->setWindowTitle("Purchase Itemwise");
    OutstandingWidget->setWindowFlags(Qt::Window);
    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openLedgerwiseItemMovement()
{
    LedgerwiseItemMovement* OutstandingWidget = new LedgerwiseItemMovement(this);
    OutstandingWidget->setWindowTitle("Ledgerwise Item Movement");
    OutstandingWidget->setWindowFlags(Qt::Window);
    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openGroupwiseItemMovement()
{
    GroupwiseItemMovement* OutstandingWidget = new GroupwiseItemMovement(this);
    OutstandingWidget->setWindowTitle("Groupwise Item Movement");
    OutstandingWidget->setWindowFlags(Qt::Window);
    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openItemTransactions()
{
    MovementAnalysis* OutstandingWidget = new MovementAnalysis(this);
    OutstandingWidget->setWindowTitle("Item Transactions");
    OutstandingWidget->setWindowFlags(Qt::Window);
    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openNegativeStockList()
{
    NegativeStock* OutstandingWidget = new NegativeStock(this);
    OutstandingWidget->setWindowTitle("Negative Stock");
    OutstandingWidget->setWindowFlags(Qt::Window);
    OutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    OutstandingWidget->showMaximized();
}

void SalesCounterWindow::openClosingStock()
{

}

void SalesCounterWindow::openInventoryGroups()
{
    inventorygroup *InventoryGroupWidget = new inventorygroup(this);
    InventoryGroupWidget->setWindowTitle("Inventory Groups");
    InventoryGroupWidget->setWindowFlags(Qt::Window);
    InventoryGroupWidget->setAttribute(Qt::WA_DeleteOnClose);
    InventoryGroupWidget->showMaximized();
}

void SalesCounterWindow::openTodaysBills()
{
    TodaysBills* TodaysBillsWidget = new TodaysBills(this);
    TodaysBillsWidget->showDateWidget(false);
    TodaysBillsWidget->setWindowFlags(Qt::Window);
    TodaysBillsWidget->setAttribute(Qt::WA_DeleteOnClose);
    TodaysBillsWidget->showFullScreen();
}

void SalesCounterWindow::openStockReport()
{
    StockReport* StockReportWidget = new StockReport(this);
    StockReportWidget->setWindowTitle("Stock Report");
    StockReportWidget->setWindowFlags(Qt::Window);
    StockReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    StockReportWidget->showMaximized();
}

void SalesCounterWindow::openItemwiseReport()
{
    ItemwiseReport* ItemwiseReportWidget = new ItemwiseReport(this);
    ItemwiseReportWidget->setWindowTitle("Itemwise Report");
    ItemwiseReportWidget->setWindowFlags(Qt::Window);
    ItemwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    ItemwiseReportWidget->showMaximized();
}


void SalesCounterWindow::openSalesMarginReport()
{
    SalesMarginReport* GroupwiseReportWidget = new SalesMarginReport(this);
    GroupwiseReportWidget->setWindowTitle("Sales Margin Report");
    GroupwiseReportWidget->setWindowFlags(Qt::Window);
    GroupwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    GroupwiseReportWidget->showMaximized();
}

void SalesCounterWindow::openGroupwiseReport()
{
    GroupwiseReport* GroupwiseReportWidget = new GroupwiseReport(this);
    GroupwiseReportWidget->setWindowTitle("Groupwise Report");
    GroupwiseReportWidget->setWindowFlags(Qt::Window);
    GroupwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    GroupwiseReportWidget->showMaximized();
}

void SalesCounterWindow::openSalesReport()
{
    TodaysBills* SalesReportWidget = new TodaysBills(this);
    SalesReportWidget->showDateWidget(true);
    SalesReportWidget->setWindowFlags(Qt::Window);
    SalesReportWidget->setTitle("Sales Report");
    SalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    SalesReportWidget->showFullScreen();
}

void SalesCounterWindow::openSalesOrders()
{
    SalesOrdersReport* SalesOrdersWidget = new SalesOrdersReport(this);
    SalesOrdersWidget->showDateWidget(true);
    SalesOrdersWidget->setWindowFlags(Qt::Window);
    SalesOrdersWidget->setAttribute(Qt::WA_DeleteOnClose);
    SalesOrdersWidget->showFullScreen();
}

void SalesCounterWindow::openReOrderReport()
{
    ReOrderLevel* ReOrderLevelWidget = new ReOrderLevel(this);
    ReOrderLevelWidget->setWindowTitle("Re Order Level");
    ReOrderLevelWidget->setWindowFlags(Qt::Window);
    ReOrderLevelWidget->setAttribute(Qt::WA_DeleteOnClose);
    ReOrderLevelWidget->showMaximized();
}

void SalesCounterWindow::numpadSlots(QString text)
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                QString qty = QString::number(voucher->InventoryItems[row].BaseItem.quantity) + text;
                voucher->InventoryItems[row].BaseItem.quantity = qty.toFloat();
            }
            else if(cnt ==3){
                QString price = QString::number(voucher->InventoryItems[row].BaseItem.price) + text;
                voucher->InventoryItems[row].BaseItem.price = price.toFloat();
            }
        }

        setTable();
        selectRow(row);
    }
}

void SalesCounterWindow::plusClicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                voucher->InventoryItems[row].BaseItem.quantity++;
            }
            else if(cnt == 3){
                voucher->InventoryItems[row].BaseItem.price += 0.25;
            }
            setTable();
            selectRow(row);
        }
    }
}

void SalesCounterWindow::minusClicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                if(voucher->InventoryItems[row].BaseItem.quantity-1 >0)
                    voucher->InventoryItems[row].BaseItem.quantity--;

            }else if(cnt == 3){
                voucher->InventoryItems[row].BaseItem.price -= 0.25;
            }
        }
        setTable();
        selectRow(row);
    }
}

void SalesCounterWindow::dotClicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                voucher->InventoryItems[row].BaseItem.quantity /= 10;
            }
            else if(cnt == 3){
            }
        }
        setTable();
        selectRow(row);
    }
}

void SalesCounterWindow::deleteClicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                voucher->InventoryItems[row].BaseItem.quantity =
                        QString::number(voucher->InventoryItems[row].BaseItem.quantity/10,'f',2).toFloat();
            }
            else if(cnt == 3){
                voucher->InventoryItems[row].BaseItem.price =
                        QString::number(voucher->InventoryItems[row].BaseItem.price/10,'f',2).toFloat();
            }
        }
        setTable();
        selectRow(row);
    }
}

void SalesCounterWindow::clearClicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        int cnt = ui->priceQuantityToggleButton->isChecked()?3:2;

        if(row>-1 && voucher->InventoryItems.length()>row ){
            if(cnt == 2){
                voucher->InventoryItems[row].BaseItem.quantity = 0;
            }
            else if(cnt == 3){
                voucher->InventoryItems[row].BaseItem.price = 0;
            }
        }
        setTable();
        selectRow(row);
    }
}

void SalesCounterWindow::showTime()
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

    tText = t.toString("dd-MM-yyyy    ")+tText;

    ui->dateLabel->setText(tText);
}

void SalesCounterWindow::addNewItem()
{
    QString text = ui->searchLine->text();
    if(text.length()>0){
        ui->searchLine->clear();

        if(text.startsWith("99")){
            QString itemID = text.mid(2,5);
            //            float amount = QString(text.mid(7,3)+"."+ text.mid(10,2)).toFloat();
            //            float qty = QString(text.mid(12,2)+"."+ text.mid(14,3)).toFloat() ;
            float amount = QString(text.mid(7,3)+"."+ text.mid(10,2)).toFloat();
            float qty = 1.00 ;
            float price = amount/qty;
            CompoundItemDataObject item= itemsHelper->getInventoryItemByBarcode(itemID);
            item.BaseItem.price = price;
            item.BaseItem.quantity = qty;
            addNewItemToList(item);
        }
        else{
            CompoundItemDataObject item= itemsHelper->getInventoryItemByBarcode(text);
            if(item.BaseItem.ItemID.length()>0){
                item.BaseItem.quantity = 1;
                addNewItemToList(item);
            }
            else{
                createNewItem(text);
            }
        }
    }
}

void SalesCounterWindow::connectMenuActions()
{
    QObject::connect(ui->actionInventory_Groups,SIGNAL(triggered()),this,SLOT(openInventoryGroups()));
    QObject::connect(ui->actionInventory_Items,SIGNAL(triggered()),this,SLOT(openInventoryItems()));
    QObject::connect(ui->actionUOM,SIGNAL(triggered()),this,SLOT(openUOMList()));
    QObject::connect(ui->actionAccount_Groups,SIGNAL(triggered()),this,SLOT(openAccountGroups()));
    QObject::connect(ui->actionLedgers_2,SIGNAL(triggered()),this,SLOT(openLedgers()));
    QObject::connect(ui->actionEmployee,SIGNAL(triggered()),this,SLOT(openEmployees()));
    QObject::connect(ui->actionPrice_List,SIGNAL(triggered()),this,SLOT(openPriceList()));



    QObject::connect(ui->actionGodowns,SIGNAL(triggered()),this,SLOT(openGodowns()));
    QObject::connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(openSettings()));
    QObject::connect(ui->actionExit_2,SIGNAL(triggered()),this,SLOT(closeApplication()));

    QObject::connect(ui->actionOrders,SIGNAL(triggered()),this,SLOT(openOrdersEntry()));
    QObject::connect(ui->actionSales,SIGNAL(triggered()),this,SLOT(openSalesEntry()));
    QObject::connect(ui->actionPurchase,SIGNAL(triggered()),this,SLOT(openPurchaseEntry()));
    QObject::connect(ui->actionLedger,SIGNAL(triggered()),this,SLOT(openLedgerwise()));

    QObject::connect(ui->actionSales_Orders_2,SIGNAL(triggered()),this,SLOT(openSalesordersVoucher()));

    QObject::connect(ui->actionWork_Order,SIGNAL(triggered()),this,SLOT(openWorkOrder()));

    QObject::connect(ui->actionPayment,SIGNAL(triggered()),this,SLOT(openPaymentEntry()));
    QObject::connect(ui->actionReceipt,SIGNAL(triggered()),this,SLOT(openReceiptEntry()));
    QObject::connect(ui->actionJournal,SIGNAL(triggered()),this,SLOT(openJournalEntry()));
    QObject::connect(ui->actionStock_Entry,SIGNAL(triggered()),this,SLOT(openStockEntry()));
    QObject::connect(ui->actionStock_Journal,SIGNAL(triggered()),this,SLOT(openStockJournal()));
    QObject::connect(ui->actionPurchase_Return,SIGNAL(triggered()),this,SLOT(openPurchaseReturn()));
    QObject::connect(ui->actionSales_Return,SIGNAL(triggered()),this,SLOT(openSalesReturn()));
    QObject::connect(ui->actionShift,SIGNAL(triggered()),this,SLOT(openDenominations()));
    QObject::connect(ui->actionDaily_Closing,SIGNAL(triggered()),this,SLOT(openClosingEntry()));


    QObject::connect(ui->actionToday_s_Bills,SIGNAL(triggered()),this,SLOT(on_reportsToolButton_clicked()));
    QObject::connect(ui->actionBillwise_Report,SIGNAL(triggered()),this,SLOT(openBillwiseReport()));
    QObject::connect(ui->actionItemwise_Sales,SIGNAL(triggered()),this,SLOT(openItemwiseReport()));
    QObject::connect(ui->actionGroupwise_Sales,SIGNAL(triggered()),this,SLOT(openGroupwiseReport()));
    QObject::connect(ui->actionSales_Margin_Report,SIGNAL(triggered()),this,SLOT(openSalesMarginReport()));

    QObject::connect(ui->actionWaiterwise_Sales,SIGNAL(triggered()),this,SLOT(openWaiterwiseReport()));
    QObject::connect(ui->actionServicewise_Sales,SIGNAL(triggered()),this,SLOT(openServicewiseReport()));
    QObject::connect(ui->actionTime_based_Sales,SIGNAL(triggered()),this,SLOT(openTimeBasedReport()));

    QObject::connect(ui->actionShiftwise_Report,SIGNAL(triggered()),this,SLOT(openShiftwiseReport()));
    QObject::connect(ui->actionDaily_Closing_Report,SIGNAL(triggered()),this,SLOT(openDailyClosing()));
    QObject::connect(ui->actionSales_Orders,SIGNAL(triggered()),this,SLOT(openSalesOrders()));
    QObject::connect(ui->actionClosing_Stock,SIGNAL(triggered()),this,SLOT(openStockReport()));
    QObject::connect(ui->actionRe_Order_Report,SIGNAL(triggered()),this,SLOT(openReOrderReport()));
    QObject::connect(ui->actionPayable,SIGNAL(triggered()),this,SLOT(openOutstandingPayable()));
    QObject::connect(ui->actionReceivable,SIGNAL(triggered()),this,SLOT(openOutstandingReceivable()));

    QObject::connect(ui->actionLedger_Report,SIGNAL(triggered()),this,SLOT(openLedgerReport()));
    QObject::connect(ui->actionGroup_Summary,SIGNAL(triggered()),this,SLOT(openGroupSummary()));
    QObject::connect(ui->actionDay_Book,SIGNAL(triggered()),this,SLOT(openDayBook()));
    QObject::connect(ui->actionTrial_Balance,SIGNAL(triggered()),this,SLOT(openTrialBalance()));
    QObject::connect(ui->actionProfit_and_Loss,SIGNAL(triggered()),this,SLOT(openProfitLoss()));
    QObject::connect(ui->actionBalance_Sheet,SIGNAL(triggered()),this,SLOT(openBalanceSheet()));
    QObject::connect(ui->actionVAT_Report,SIGNAL(triggered()),this,SLOT(openVATReport()));
    QObject::connect(ui->actionOutstanding_4, SIGNAL(triggered()), this, SLOT(openOutstandingReport()));

    QObject::connect(ui->actionItemwise_Purchase, SIGNAL(triggered()), this, SLOT(openPurchaseItemwise()));
    QObject::connect(ui->actionLedger_Movement, SIGNAL(triggered()), this, SLOT(openLedgerwiseItemMovement()));
    QObject::connect(ui->actionGroupwise_Movement, SIGNAL(triggered()), this, SLOT(openGroupwiseItemMovement()));
    QObject::connect(ui->actionItem_Movement, SIGNAL(triggered()), this, SLOT(openItemTransactions()));
    QObject::connect(ui->actionNegative_Stock, SIGNAL(triggered()), this, SLOT(openNegativeStockList()));



    QObject::connect(ui->actionComplimentary,SIGNAL(triggered()),this,SLOT(openComplimentaryReport()));
    QObject::connect(ui->actionDiscount_Report,SIGNAL(triggered()),this,SLOT(openDiscountReport()));

    QObject::connect(ui->actionContents,SIGNAL(triggered()),this,SLOT(openHelpContents()));
    QObject::connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(openAbout()));

    QObject::connect(ui->searchLine,SIGNAL(editingFinished()),this,SLOT(addNewItem()));

    QObject::connect(ui->actionDashboard,SIGNAL(triggered()),this,SLOT(openDash()));





}

void SalesCounterWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"single click";
    selectRow(index.row());
    lineNameLine->setFocus();
}

void SalesCounterWindow::saveVoucher()
{
    billsCount++;
    if((voucher->status != QuotationStatus::SalesInvoiceRaised && voucher->voucherNumber.length()>0)){
        salesOrderHelper->setSalesOrderStatus(QuotationStatus::SalesInvoiceRaised,voucher->voucherNumber,voucher->VoucherPrefix);
    }

    if(voucher->status == QuotationStatus::SalesInvoiceRaised){
        qDebug()<<"updating sales order";
        salesHelper->updateVoucher(voucher,voucher->voucherNumber);
        qDebug()<<voucher->voucherNumber;
    }
    else{
        voucher->status = QuotationStatus::SalesInvoiceRaised;
        salesHelper->insertVoucher(voucher);
        qDebug()<<"Saving Ledgers Count :"<<voucher->ledgersList.size();
    }

    for(int i=0;i<voucher->NoOfCopies;i++){
        printer->printSalesVoucher(voucher);
    }

    printer->openTillUSB();

    on_refreshToolButton_clicked();

    if(billsCount%20==0){
        qApp->quit();
        QStringList args;
        args<<"100";
        args<<QString::number(LoginValues::userID);
        QProcess::startDetached(qApp->arguments()[0], args);
    }
}

void SalesCounterWindow::saveDraft()
{
    if(voucher->status == QuotationStatus::SalesOrderCreated ||
            voucher->status == QuotationStatus::DeliveryReady ||
            voucher->status == QuotationStatus::DeliveryCompleted){
        salesOrderHelper->updateVoucher(voucher,voucher->voucherNumber);
    }
    else{
        if(voucher->ModeOfService == ServiceMode::Delivery)
            voucher->status = QuotationStatus::DeliveryReady;
        else
            voucher->status = QuotationStatus::SalesOrderCreated;

        salesOrderHelper->insertVoucher(voucher);
    }
    on_refreshToolButton_clicked();
}

void SalesCounterWindow::setVoucher(GeneralVoucherDataObject *obj)
{
    on_refreshToolButton_clicked();
    delete voucher;
    voucher = obj;

    ui->voucherDate->setText(voucher->VoucherDate.toString("dd-MM-yyyy"));
    ui->voucherNo->setText(voucher->voucherNumber);
    setTable();
}

void SalesCounterWindow::SearchInventoryItems()
{
    qDebug()<<"Search clicked";
    inventoryItem* inventoryItemWidget = new inventoryItem(this);
    inventoryItemWidget->setWindowFlags(Qt::Window);
    inventoryItemWidget->setWindowTitle("Inventory");
    inventoryItemWidget->getBackFlag =1;
    inventoryItemWidget->setAttribute(Qt::WA_DeleteOnClose);
    inventoryItemWidget->show();
    inventoryItemWidget->setFocus();
    inventoryItemWidget->setFocusonSearchLine();
    QObject::connect(inventoryItemWidget,SIGNAL(SelectedItem(CompoundItemDataObject)),this,
                     SLOT(addNewItemToList(CompoundItemDataObject)));
}

void SalesCounterWindow::addNewItemComp(inventoryItemDataModel item)
{
    CompoundItemDataObject c;
    c.BaseItem=item;
    c.BaseItem.quantity=1;
    addNewItemToList(c);
    lineNameLine->setText("");

}

void SalesCounterWindow::addNewItemToList(CompoundItemDataObject item)
{
    int y = QDate::currentDate().year();

    if(item.BaseItem.price == 0){
        bool ok;
        QString text = QInputDialog::getText(this, tr("Rate : "),
                                             tr("Amount:"), QLineEdit::Normal,
                                             "0", &ok);
        if (ok && !text.isEmpty())
        {
            item.BaseItem.price =text.toFloat();

        }
    }

    qDebug()<<"PSST";
    if(item.BaseItem.taxRate == -1)
    {
        item.BaseItem.taxRate=0;
        bool ok;
        QString text = QInputDialog::getText(this, tr("GST : "),
                                             tr("GST RATE:"), QLineEdit::Normal,
                                             "0", &ok);
        if (ok && !text.isEmpty())
        {
            item.BaseItem.taxRate =text.toFloat();
            if(item.BaseItem.taxRate>=0)
            {
                item.BaseItem.price = item.BaseItem.price * 100 /(100+item.BaseItem.taxRate);
                itemsHelper->updateGST(item.BaseItem.ItemID,item.BaseItem.taxRate);
                itemsHelper->updatePrice(item.BaseItem.ItemID,item.BaseItem.price);
            }
        }
    }
    voucher->InventoryItems.append(item);
    qDebug()<<"Vat Rate = "<<item.BaseItem.taxRate;

    setTable();
    selectRow(voucher->InventoryItems.length()-1);
    ui->tableWidget->scrollToBottom();
}

void SalesCounterWindow::createNewItem(QString text)
{
    AddItems *additemWidget= new AddItems(this);
    additemWidget->setWindowFlags(Qt::Window |Qt::WindowStaysOnTopHint);
    additemWidget->setAttribute(Qt::WA_DeleteOnClose);
    additemWidget->setItemCode(text);
    additemWidget->show();
    QObject::connect(additemWidget,SIGNAL(savedItem(CompoundItemDataObject)),this,SLOT(addNewItemToList(CompoundItemDataObject)));
}

void SalesCounterWindow::on_reportsToolButton_clicked()
{
    TodaysBills *ReportsWidget = new TodaysBills(this);
    ReportsWidget->showDateWidget(false);
    ReportsWidget->setWindowFlags(Qt::Window);
    ReportsWidget->setAttribute(Qt::WA_DeleteOnClose);
    ReportsWidget->showFullScreen();
    QObject::connect(ReportsWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(setVoucher(GeneralVoucherDataObject*)));
}

void SalesCounterWindow::on_searchItemButton_clicked()
{
    SearchInventoryItems();
}

void SalesCounterWindow::on_shiftCloseButton_clicked()
{
    DenominationsWidget = new Denominations();
    DenominationsWidget->setWindowFlags(Qt::Window);
    DenominationsWidget->setAttribute(Qt::WA_DeleteOnClose);
    DenominationsWidget->showFullScreen();
    QObject::connect(DenominationsWidget,SIGNAL(totalSales(float)),this,SLOT(openShiftWidget(float)));
}

void SalesCounterWindow::on_screenLockButton_clicked()
{
    LoginPage *LoginPageWidget = new LoginPage();
    LoginPageWidget->setWindowFlags(Qt::FramelessWindowHint);
    LoginPageWidget->setAttribute(Qt::WA_DeleteOnClose);
    LoginPageWidget->show();
    this->close();
}

void SalesCounterWindow::on_adminButton_clicked()
{
    PasswordEntry *PasswordEntryWidget = new PasswordEntry(this);
    PasswordEntryWidget->setWindowTitle("Enter Password");
    PasswordEntryWidget->setWindowFlags(Qt::Window);
    PasswordEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    PasswordEntryWidget->show();
    QObject::connect(PasswordEntryWidget,SIGNAL(loginSuccess()),this,SLOT(showMenuBar()));
    QObject::connect(PasswordEntryWidget,SIGNAL(loginFail()),this,SLOT(on_adminButton_clicked()));
}

void SalesCounterWindow::on_openTillButton_clicked()
{
    PasswordEntry *PasswordEntryWidget = new PasswordEntry(this);
    PasswordEntryWidget->setWindowTitle("Enter Password");
    PasswordEntryWidget->setWindowFlags(Qt::Window);
    PasswordEntryWidget->setAttribute(Qt::WA_DeleteOnClose);
    PasswordEntryWidget->show();
    QObject::connect(PasswordEntryWidget,SIGNAL(loginSuccess()),this,SLOT(openTillUSB()));
    QObject::connect(PasswordEntryWidget,SIGNAL(loginFail()),this,SLOT(on_openTillButton_clicked()));
}

void SalesCounterWindow::on_priceQuantityToggleButton_clicked()
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        selectRow(row);
    }
}

void SalesCounterWindow::on_miscellaneousButton_clicked()
{
    MiscellaneousItems *MiscellaneousItemsWidget = new MiscellaneousItems(this);
    MiscellaneousItemsWidget->setWindowTitle("Miscellaneous Item");
    MiscellaneousItemsWidget->setWindowFlags(Qt::Window);
    MiscellaneousItemsWidget->setAttribute(Qt::WA_DeleteOnClose);
    MiscellaneousItemsWidget->show();
    QObject::connect(MiscellaneousItemsWidget,SIGNAL(misc(CompoundItemDataObject)),this,SLOT(addNewItemToList(CompoundItemDataObject)));
}

void SalesCounterWindow::getCustomerDetails(QString Address, QString Phone, QString Name)
{
    ui->customerNameLineEdit->setText(Name);
    ui->phoneNoLineEdit->setText(Phone);
    ui->addressTextEdit->setPlainText(Address);
    ui->customerWidget->show();
    voucher->Contact.address= Address;
    voucher->Contact.PhoneNumber= Phone;
    voucher->Contact.ContactName= Name;
}

void SalesCounterWindow::printVoucher()
{
    CounterPrinterHelper pHelper;
    for(int i=0;i<voucher->NoOfCopies;i++){
        pHelper.printSalesVoucher(voucher);
    }
}

void SalesCounterWindow::openItemDetail()
{
//    ItemDetail *ItemDetailWidget = new ItemDetail(this);
//    ItemDetailWidget->setWindowFlags(Qt::Window);
//    ItemDetailWidget->setWindowTitle("Item Detail");
//    ItemDetailWidget->setAttribute(Qt::WA_DeleteOnClose);
//    int row = sender()->property("row").toInt();

//    ItemDetailWidget->setItem(&voucher->InventoryItems[row]);
//    ItemDetailWidget->show();

//    QObject::connect(ItemDetailWidget,SIGNAL(saveItemDetails()),this,SLOT(setTable()));
}

void SalesCounterWindow::openEmployees()
{
    UsersList *UsersListWidget = new UsersList(this);
    UsersListWidget->setWindowTitle("Employees");
    UsersListWidget->setWindowFlags(Qt::Window);
    UsersListWidget->setAttribute(Qt::WA_DeleteOnClose);
    UsersListWidget->show();
}

void SalesCounterWindow::openGodowns()
{
    godown *GodownWidget = new godown(this);
    GodownWidget->setWindowTitle("Godowns");
    GodownWidget->setWindowFlags(Qt::Window);
    GodownWidget->setAttribute(Qt::WA_DeleteOnClose);
    GodownWidget->show();
}

void SalesCounterWindow::openPriceList()
{
    priceList *priceListWidget = new priceList(this);
    priceListWidget->setWindowTitle("Price List");
    priceListWidget->setWindowFlags(Qt::Window);
    priceListWidget->setAttribute(Qt::WA_DeleteOnClose);
    priceListWidget->show();
}

void SalesCounterWindow::on_priceQuantityToggleButton_toggled(bool checked)
{
    if(ui->tableWidget->currentRow()>-1){
        int row = ui->tableWidget->currentRow();
        selectRow(row);
    }
}

void SalesCounterWindow::selectRow(int row)
{
    for(int i =0;i<voucher->InventoryItems.size();i++){
        QTableWidgetItem* Qty = new QTableWidgetItem;
        QTableWidgetItem* Price = new QTableWidgetItem;

        Qty->setText(QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2));
        Price->setText(QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2));

        Qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        Price->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

        Qty->setFlags(Qty->flags() ^ Qt::ItemIsEditable);
        Price->setFlags(Price->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget->setItem(i,2,Qty);
        ui->tableWidget->setItem(i,3,Price);
    }

    int col = ui->priceQuantityToggleButton->isChecked()?3:2;
    int noCol = ui->priceQuantityToggleButton->isChecked()?2:3;
    QTableWidgetItem *item = ui->tableWidget->item(row,col);
    QTableWidgetItem *noitem = ui->tableWidget->item(row,noCol);

    item->setIcon(QIcon(":/images/edit.png"));
    noitem->setIcon(QIcon(":/images/noedit.png"));

    ui->tableWidget->selectRow(row);
}

void SalesCounterWindow::openBillwiseReport()
{
    BillwiseReport* BillwiseReportWidget = new BillwiseReport(this);
    BillwiseReportWidget->setWindowTitle("Billwise Report");
    BillwiseReportWidget->setWindowFlags(Qt::Window);
    BillwiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    BillwiseReportWidget->show();
}

void SalesCounterWindow::openServicewiseReport()
{
    ModeOfServiceWiseReport* ModeOfServiceWiseReportWidget = new ModeOfServiceWiseReport(this);
    ModeOfServiceWiseReportWidget->setWindowTitle("Service Wise Report");
    ModeOfServiceWiseReportWidget->setWindowFlags(Qt::Window);
    ModeOfServiceWiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    ModeOfServiceWiseReportWidget->show();
}

void SalesCounterWindow::openDailyClosing()
{
    DailyClosingSalesReport* DailyClosingSalesReportWidget = new DailyClosingSalesReport(this);
    DailyClosingSalesReportWidget->setWindowTitle("Daily Closing Report");
    DailyClosingSalesReportWidget->setWindowFlags(Qt::Window);
    DailyClosingSalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    DailyClosingSalesReportWidget->show();
}

void SalesCounterWindow::openShiftwiseReport()
{
    ShiftWiseReport* ShiftWiseReportWidget = new ShiftWiseReport(this);
    ShiftWiseReportWidget->setWindowTitle("Shift Wise Report");
    ShiftWiseReportWidget->setWindowFlags(Qt::Window);
    ShiftWiseReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    ShiftWiseReportWidget->show();
}

void SalesCounterWindow::openTimeBasedReport()
{
    TimeBasedSales* TimeBasedSalesWidget = new TimeBasedSales(this);
    TimeBasedSalesWidget->setWindowTitle("Time based Sales Report");
    TimeBasedSalesWidget->setWindowFlags(Qt::Window);
    TimeBasedSalesWidget->setAttribute(Qt::WA_DeleteOnClose);
    TimeBasedSalesWidget->show();
}

void SalesCounterWindow::openWaiterwiseReport()
{
    WaiterWiseSalesReport* WaiterWiseSalesReportWidget = new WaiterWiseSalesReport(this);
    WaiterWiseSalesReportWidget->setWindowTitle("Waiterwise Sales Report");
    WaiterWiseSalesReportWidget->setWindowFlags(Qt::Window);
    WaiterWiseSalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    WaiterWiseSalesReportWidget->show();
}

void SalesCounterWindow::openComplimentaryReport()
{
    ComplimentarySalesReport* ComplimentarySalesReportWidget = new ComplimentarySalesReport(this);
    ComplimentarySalesReportWidget->setWindowTitle("Complimentary Report");
    ComplimentarySalesReportWidget->setWindowFlags(Qt::Window);
    ComplimentarySalesReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    ComplimentarySalesReportWidget->show();
}

void SalesCounterWindow::openDiscountReport()
{
    DiscountsReport* DiscountsReportWidget = new DiscountsReport(this);
    DiscountsReportWidget->setWindowTitle("Discounts Report");
    DiscountsReportWidget->setWindowFlags(Qt::Window);
    DiscountsReportWidget->setAttribute(Qt::WA_DeleteOnClose);
    DiscountsReportWidget->show();
}

void SalesCounterWindow::openGroupSummary()
{
    GroupSummary* GroupSummaryWidget = new GroupSummary(this);
    GroupSummaryWidget->setWindowTitle("Group Summary");
    GroupSummaryWidget->setWindowFlags(Qt::Window);
    GroupSummaryWidget->setAttribute(Qt::WA_DeleteOnClose);
    GroupSummaryWidget->show();
}

void SalesCounterWindow::openOutstandingPayable()
{
    PaymentOutstanding* PaymentOutstandingWidget = new PaymentOutstanding(this);
    PaymentOutstandingWidget->setWindowTitle("Payment Outstanding");
    PaymentOutstandingWidget->setWindowFlags(Qt::Window);
    PaymentOutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    PaymentOutstandingWidget->show();
}

void SalesCounterWindow::openOutstandingReceivable()
{
    ReceivableOutstanding* ReceivableOutstandingWidget = new ReceivableOutstanding(this);
    ReceivableOutstandingWidget->setWindowTitle("Receivable Outstanding");
    ReceivableOutstandingWidget->setWindowFlags(Qt::Window);
    ReceivableOutstandingWidget->setAttribute(Qt::WA_DeleteOnClose);
    ReceivableOutstandingWidget->show();
}

void SalesCounterWindow::showMenuBar()
{
    ui->menubar->show();
}

void SalesCounterWindow::openTillUSB()
{
    printer->openTillUSB();
}

void SalesCounterWindow::on_phoneNoLineEdit_returnPressed()
{

}


void SalesCounterWindow::openSerialPort()
{
    serial->setPortName(CounterSettingsDataModel::PortNo);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(QSerialPort::Data8));
    serial->setParity(static_cast<QSerialPort::Parity>(QSerialPort::NoParity));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(QSerialPort::OneStop));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(QSerialPort::NoFlowControl));

    if (serial->open(QIODevice::WriteOnly)) {
        qDebug()<<"Connected!!!";
        QByteArray str = "NEXT CUSTOMER";
        writeData(str);
    }
    else {
        //        QMessageBox::critical(this, tr("Error"), serial->errorString()+"\nCalled ID not Connected");

    }
}

void SalesCounterWindow::closeSerialPort()
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
void SalesCounterWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void SalesCounterWindow::readData()
{
    //    QByteArray data = serial->readAll();
    //    //console->putData(data);
    //    QString dStr1 = data;
    ////     QMessageBox box; box.setText(dStr1); box.exec();
    //    if(dStr1.contains("NMBR") && dStr1.split('=').size()>3){
    //        ui->pickCallToolButton->setHidden(false);
    //        dStr = dStr1.right(dStr1.length());
    //        QString number = dStr.split('=')[3];

    //        number=number.left(number.length()-2);

    ////        number = number.right(number.length()-1);


    //        QString sstr = "INSERT INTO `call_log`(`Number` ) VALUES ('"+number.right(number.length())+"')";
    //        //        QMessageBox box; box.setText(sstr); box.exec();
    //        QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    //        query.exec(sstr);
    //    }
}

void SalesCounterWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void SalesCounterWindow::on_tableWidget_doubleClicked(const QModelIndex &index)
{

}
