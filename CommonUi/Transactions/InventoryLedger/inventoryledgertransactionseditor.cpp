#include "inventoryledgertransactionseditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "counter/MainScreen/itemdetail.h"
#include <QScreen>



InventoryLedgerTransactionsEditor::InventoryLedgerTransactionsEditor(int voucherType, VoucherDBAbstract *dbhelper, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    this->setTitle(windowTitle);
    this->setObjectName("editor");
    this->dbHelper = dbhelper;

    setVoucherType(voucherType);
    initVoucher();
    initGlobalObjects();
    initExtraActions();

}

void InventoryLedgerTransactionsEditor::setVoucherType(int value)
{
    voucherType = value;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType<<GMVoucherTypes::Proforma;

    if(voucherType == GMVoucherTypes::SalesVoucher || voucherType == GMVoucherTypes::SalesOrder
            || voucherType == GMVoucherTypes::DeliveryNote || voucherType == GMVoucherTypes::Proforma ){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType;
        transType = 1;
    }
    else if (voucherType == GMVoucherTypes::PurchaseVoucher || voucherType == GMVoucherTypes::PurchaseOrder
             ||voucherType == GMVoucherTypes::ReceiptNote ) {
        qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType;

        transType = 2;
    }
    else if (voucherType == GMVoucherTypes::CreditNote ) {

        transType = 3;
    }
    else if (voucherType == GMVoucherTypes::DebitNote ) {

        transType = 4;
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType<<transType;


}


void InventoryLedgerTransactionsEditor::initExtraActions()
{
    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&InventoryLedgerTransactionsEditor::saveVoucher);
    QObject::connect(moreButton,&QtMaterialIconButton::clicked,
                     this, [=](bool Checked){
        qDebug()<<"more checked"<<Checked;

        ledgerTable->setHidden(!ledgerTable->isHidden());
    });

    connect(paymentButton, &QtMaterialIconButton::clicked, this, &InventoryLedgerTransactionsEditor::showCoupledWidget);

    if(voucherType != GMVoucherTypes::SalesOrder && voucherType != GMVoucherTypes::PurchaseOrder )
        godownWidget->show();

    if(voucherType == GMVoucherTypes::SalesOrder || voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::PurchaseOrder || voucherType == GMVoucherTypes::PurchaseVoucher)
        //        addToExtrasLayout(coupledVoucherHandler);
        paymentButton->show();

    if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::SalesVoucher)){
        priceListWidget->setPriceListId(&voucher->priceListId);
        priceListWidget->show();
    }

    if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::PurchaseVoucher))
        invoiceDataWidget->show();
    if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::DebitNote)
            || GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::CreditNote))
        ReasonWidget->show();

//    connect(godownWidget, &GMGodownLineMaterial::itemSelectedwithItem, this,
//            [=](QString godownID) {
//        qDebug()<<"changed godown"<<godownID<<voucher->fromGodownID;
//        switch (transType) {
//        case 1: case 4:
//        {
//            voucher->fromGodownID = (godownID);
//            for(int i=0;i<voucher->InventoryItems.size();  i++){
//                if(voucher->InventoryItems[i].BaseItem.godownList.size() == 0)
//                    voucher->InventoryItems[i].BaseItem.fromGodownID = voucher->fromGodownID;
//                else if (voucher->InventoryItems[i].BaseItem.godownList.size() == 1) {
//                    voucher->InventoryItems[i].BaseItem.fromGodownID = voucher->fromGodownID;
//                    voucher->InventoryItems[i].BaseItem.godownList[0].fromGodown = voucher->fromGodownID;
//                }

//            }
//            break;
//        }
//        case 2: case 3:
//        {
//            voucher->toGodownID = (godownID);
//            for(int i=0;i<voucher->InventoryItems.size();  i++){
//                if(voucher->InventoryItems[i].BaseItem.godownList.size() == 0)
//                    voucher->InventoryItems[i].BaseItem.toGodownID = voucher->toGodownID;
//                else if (voucher->InventoryItems[i].BaseItem.godownList.size() == 1) {
//                    voucher->InventoryItems[i].BaseItem.toGodownID = voucher->toGodownID;
//                    voucher->InventoryItems[i].BaseItem.godownList[0].toGodown = voucher->toGodownID;
//                }

//            }
//            break;

//        }
//        }
//    });


}

void InventoryLedgerTransactionsEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    editorStatus = EditorStatus::NewVoucher;
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(voucherType);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    voucher->priceListName = priceHelper->getPriceListNameByID(priceHelper->getDefaultPriceList());
    voucher->priceListId = priceHelper->getDefaultPriceList();
    switch (transType) {
    case 1: case 4:{
        voucher->fromGodownID = godownHelper->getDefaultGodown();
        break;
    }
    case 2: case 3:{
        voucher->toGodownID = godownHelper->getDefaultGodown();
    }

    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;
    voucher->SalesmanID = LoginValues::userID;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;
    setTitle(GMVoucherTypes::getVoucherTitle(voucherType));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    invoiceDateWidget->setVariable(&voucher->invoiceDate);
    invoiceNumberWidget->setVariable(&voucher->invoiceNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);

}

void InventoryLedgerTransactionsEditor::initGlobalObjects()
{
    coupledVoucherHandler = new CoupledVoucherHandler(this);
    //    coupledVoucherHandler->setWindowProps();
    coupledVoucherHandler->setWindowFlags(Qt::Popup);
    coupledVoucherHandler->setParentVoucher(voucher);
    coupledVoucherStatus = EditorStatus::NewVoucher;
    coupledVoucherHandler->setMaximumWidth(300);
    coupledVoucherHandler->setWidgets();
    //    coupledVoucherHandler->hide();

    connect(priceListWidget, &GMPriceListLineMaterial::itemSelectedwithItem, this,
            &InventoryLedgerTransactionsEditor::setPrices);

    //    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Price"<<"Disc %"<<"Disc. Amount"<<"Tax"<<"Tax Amount"<<"Amount";

    QStringList ledGroups;
    if(voucherType == GMVoucherTypes::SalesVoucher || voucherType == GMVoucherTypes::SalesOrder
            ||voucherType == GMVoucherTypes::DeliveryNote || voucherType == GMVoucherTypes::CreditNote){

        ledGroups.append("0x5x23");
        ledGroups.append("0x5x19");
        ledGroups.append(accHelper->getGroupIDByName("Bank Accounts"));

        for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive("0x5x19")){
            ledGroups.append(acc.groupID);
        }

        for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Bank Accounts"))){
            ledGroups.append(acc.groupID);
        }
    }
    else if (voucherType == GMVoucherTypes::PurchaseVoucher || voucherType == GMVoucherTypes::PurchaseOrder
             ||voucherType == GMVoucherTypes::ReceiptNote || voucherType == GMVoucherTypes::DebitNote) {
        ledGroups.append("0x5x23");
        ledGroups.append("0x2x15");
        ledGroups.append(accHelper->getGroupIDByName("Bank Accounts"));

        for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive("0x2x15")){
            ledGroups.append(acc.groupID);
        }

        for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accHelper->getGroupIDByName("Bank Accounts"))){
            ledGroups.append(acc.groupID);
        }

    }

    mainLedgerWidget->setSrcModel(ledgerHelper->getLedgerQueryString(ledGroups));



    DataTableWidget = new InventoryVoucherTableView(voucher);
    DataTableWidget->setEnabled(false);

    itemDelegate = new GMInventoryTransactionDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                uomHelper, voucher->priceListId);
    connect(itemDelegate, &GMInventoryTransactionDelegate::showItemDetailClicked,
            this, &InventoryLedgerTransactionsEditor::showItemDetail);

    DataTableWidget->setItemDelegate(itemDelegate);

    model = new GMInventoryItemModel(voucher,transType,this);
    //    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    DataTableWidget->hideColumn(2);
    DataTableWidget->hideColumn(4);
    bool showDiscountFlag = ConfigurationSettingsDatabaseHelper::getValue(showDiscount,false).toBool();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<showDiscountFlag;
    if(!showDiscountFlag)
    {
        DataTableWidget->hideColumn(9);
        DataTableWidget->hideColumn(10);
    }

    if(!ConfigurationSettingsDatabaseHelper::getValue(showTaxinVoucher,true).toBool())
    {
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        DataTableWidget->hideColumn(11);
        DataTableWidget->hideColumn(12);
    }

    AddressDialogWidget = new AddressDialog(ledgerHelper);

    ledgerTable = new LedgerwiseVoucherTableView(voucher,ledgerHelper,this);
    ledgerTable->hide();
    connect(model,SIGNAL(updateValues()),ledgerTable,SLOT(updateData()));

    connect(ledgerTable,&LedgerwiseVoucherTableView::ledUpdated,[=](){
        //        qDebug()<<"Update model at voucher";
        model->setVoucher(voucher);
        DataTableWidget->setValues();
    });

    setTableLayout(DataTableWidget);
    setTableLayout(ledgerTable);

    connect(mainLedgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,
            this,[=](LedgerMasterDataModel led){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<led.LedgerID;
        voucher->ledgerObject = led;
        mainLedgerWidget->clearFocus();
        DataTableWidget->setEnabled(true);
        DataTableWidget->clearFocus();
        DataTableWidget->setFocusToFirstRow();
        //        showCoupledWidget();



    });
    switch (transType) {
    case 1: case 4:{
        //        connect(godownWidget, &GMGodownLineMaterial::itemSelected, this, [=](){
        //           voucher->fromGodownID = *godownWidget->godown;
        //        });
        godownWidget->setGodown(&voucher->fromGodownID);
        mainLedgerWidget->setLabel("Customer Name");
        break;
    }
    case 2: case 3:{
        //        connect(godownWidget, &GMGodownLineMaterial::itemSelected, this, [=](){
        //           voucher->toGodownID = *godownWidget->godown;
        //        });
        godownWidget->setGodown(&voucher->toGodownID);
        mainLedgerWidget->setLabel("Vendor Name");
    }

    }





}


void InventoryLedgerTransactionsEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();
    delete model;

    model = new GMInventoryItemModel(voucher,transType,this);
    //    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    DataTableWidget->setMaximumWidth(width-20);
    if(voucher->ledgerObject.LedgerID.length()>0)
        DataTableWidget->setEnabled(true);
    ledgerTable->setVoucher(voucher);
    connect(model,SIGNAL(updateValues()),ledgerTable,SLOT(updateData()));

    connect(ledgerTable,&LedgerwiseVoucherTableView::ledUpdated,[=](){
        //        qDebug()<<"Update model at voucher";
        model->setVoucher(voucher);
        DataTableWidget->setValues();
    });

    godownWidget->blockSignals(true);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<transType<<voucher->fromGodownID<<voucher->toGodownID;
    switch (transType) {
    case 1: case 4:{
        godownWidget->setGodown(&voucher->fromGodownID);
        break;
    }
    case 2: case 3:{
        godownWidget->setGodown(&voucher->toGodownID);
    }

    }
    godownWidget->blockSignals(false);
    if(voucher->advanceCash > 0){

        initCoupledWidget();
        //    resetWidgets();

        //        extrasLayout->addStretch(1);

    }

}

void InventoryLedgerTransactionsEditor::setMainLedger(LedgerMasterDataModel led)
{

    mainLedgerWidget->setText(led.LedgerName);
    voucher->ledgerObject = led;
    mainLedgerWidget->setItem(&voucher->ledgerObject);
    DataTableWidget->setEnabled(true);
    DataTableWidget->setFocusToFirstRow();

    initCoupledWidget();
}

void InventoryLedgerTransactionsEditor::setSalesMan(int id)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;
    SalesmanSearchBox->setText(userHelper->getEmployeeNameFromID(id));
    voucher->SalesmanID = id;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->SalesmanID;
}

void InventoryLedgerTransactionsEditor::setGodown(QString godownId)
{
    if(godownId.length() == 0)
        godownId = godownHelper->getDefaultGodown();
    switch (transType) {
    case 1: case 4:{
        voucher->fromGodownID = godownId;
        break;
    }
    case 2: case 3:{
        voucher->toGodownID = godownId;
    }

    }

    resetWidgets();
}



void InventoryLedgerTransactionsEditor::setAdvancePaid(float amount)
{
    voucher->advanceCash = amount;

    qDebug()<<"&&&&&&&&&&Balance is "<<voucher->advanceCash;

    resetWidgets();
}

void InventoryLedgerTransactionsEditor::getVoucherstoImport()
{
    //    if(voucher->ledgerObject.LedgerID.length()<1){
    //        snackBar->addMessage("Please Set Ledger");
    //        return;
    //    }

    //    ShowVouchersWidget *selectVoucherWidget = new ShowVouchersWidget();
    //    selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::SalesVoucher);
    //    selectVoucherWidget->setLedger(voucher->ledgerObject.LedgerID);
    //    selectVoucherWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
    //    //    selectVoucherWidget->setWindowState(Qt::WindowMaximized);
    //    selectVoucherWidget->resize(500,200);
    //    selectVoucherWidget->show();
    //    QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),
    //                     this, SLOT(importItemsFromVoucher(GeneralVoucherDataObject *)));

}

void InventoryLedgerTransactionsEditor::saveVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType;
    if(voucher->InventoryItems.size()<1 && voucher->ledgersList.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    if(voucher->ledgerObject.LedgerID.length()<1){
        snackBar->addMessage("Please Set Ledger");
        return;
    }


    setBillwiseMapping();


    calcTotals();

    qDebug()<<voucher->advanceCash<<"adv";
    //    voucher->PaidAmount = voucher->advanceCash;
    if(coupledVoucherHandler != nullptr){
        if(!coupledVoucherHandler->checkModeOfPayEntered() && voucher->advanceCash > 0)
        {
            snackBar->addMessage("Please Set Mode Of Pay");
            return;
        }
    }

    VoucherEditorBase::saveVoucher();
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    if(saveStatus && voucher->advanceCash > 0 ){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        if(voucherType == GMVoucherTypes::SalesOrder || voucherType == GMVoucherTypes::SalesVoucher ||
                voucherType == GMVoucherTypes::PurchaseOrder || voucherType == GMVoucherTypes::PurchaseVoucher){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<EditorStatus::NewVoucher<<coupledVoucherStatus;
            if(coupledVoucherStatus == EditorStatus::NewVoucher)
                coupledVoucherHandler->createCoupledVoucher();
            else {
                coupledVoucherHandler->saveCoupledVoucher();
            }
        }
    }
    else {
        qDebug()<<Q_FUNC_INFO<<saveStatus;
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;


    emit saveCompleted();
    VoucherEditorBase::closeEditor();


}

void InventoryLedgerTransactionsEditor::printVoucher(QPrinter *printer)
{
    int val = sender()->property("PrintType").toInt();
    qDebug()<<"Print Type = "<<val;
    qDebug()<<"Public Type "<<shareOptionsWidget->printType ;
    switch (transType) {
    case 1: case 4:
    {


        if(val == PrintTypes::Default){
            printHelper = new PrinterHelper();
            //    printHelper->testPrint(voucher);
            printHelper->setPrinter(printer);
            printHelper->PrintInventorySalesVoucher(voucher);
        }
        else if(val == PrintTypes::DotMatrix){
            PrintDotMatrix printer;
            printer.printSalesVoucher(voucher);
        }
        else if(val == PrintTypes::Thermal){
            CounterPrinterHelper counter;
            counter.setPrinter(printer);
            counter.printSalesVoucher(voucher);
        }
    }
        break;
    case 2: case 3:
    {

        printHelper = new PrinterHelper();
        //    printHelper->testPrint(voucher);
        printHelper->setPrinter(printer);
        printHelper->PrintInventoryPurchaseVoucher(voucher);

    }
    }
}

void InventoryLedgerTransactionsEditor::exportToPdf(QPrinter *printer)
{
    printHelper = new PrinterHelper();
    //    printHelper->testPrint(voucher);
    printHelper->setPrinter(printer);
    printHelper->PrintInventorySalesVoucher(voucher);
}

void InventoryLedgerTransactionsEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    setVoucher(voucher2);
}

void InventoryLedgerTransactionsEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{
    // used when creating new voucher to import
    voucher->InventoryItems.append(voucher2->InventoryItems);
    voucher->SalesmanID = voucher2->SalesmanID;
    voucher->ledgerObject = voucher2->ledgerObject;
    voucher->ModeOfService = voucher2->ModeOfService;
    voucher->TransactionId = voucher2->TransactionId;
    voucher->narration = voucher2->narration;

    switch (transType) {
    case 1:{

        for(int i=0; i<voucher2->InventoryItems.size(); i++){
            voucher2->InventoryItems[i].BaseItem.fromGodownID = voucher2->InventoryItems[i].BaseItem.section;
        }
        voucher->fromGodownID = voucher2->fromGodownID;
    }
    case 2:{
        for(int i=0; i<voucher2->InventoryItems.size(); i++){
            voucher2->InventoryItems[i].BaseItem.toGodownID = voucher2->InventoryItems[i].BaseItem.section;
        }
        voucher->fromGodownID = voucher2->toGodownID;
    }

    }
    calcTotals();

    initCoupledWidget();
    if(coupledVoucherHandler != nullptr){
        //        if(coupledVoucherHandler->retrieveCoupledVoucher())
        //            coupledVoucherStatus  = EditorStatus::EditVoucher;

    }
    //    coupledVoucherHandler->addWidgetsInParentVoucher(advanceLayout);
    resetWidgets();

}

void InventoryLedgerTransactionsEditor::setBillwiseMapping()
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

void InventoryLedgerTransactionsEditor::showVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    VoucherEditorBase::showVoucher();
    calcTotals();

    resetWidgets();

    if(voucherType == GMVoucherTypes::SalesOrder || voucherType == GMVoucherTypes::PurchaseOrder ||
            voucherType == GMVoucherTypes::DeliveryNote || voucherType == GMVoucherTypes::ReceiptNote )
        exportButton->show();

    initCoupledWidget();
    if(coupledVoucherHandler != nullptr){

        qDebug()<<Q_FUNC_INFO<<__LINE__;
        if(coupledVoucherHandler->retrieveCoupledVoucher()){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            coupledVoucherStatus = EditorStatus::EditVoucher;
        }

    }
}

void InventoryLedgerTransactionsEditor::setPrices(QString priceList)
{
    qDebug()<<Q_FUNC_INFO<<voucher->priceListId;
    voucher->priceListId = priceList;
    itemDelegate->setPriceListId(voucher->priceListId);
    qDebug()<<"reset model"<<voucher->priceListId;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        qDebug()<<"updating price of item :"<<voucher->InventoryItems[i].BaseItem.ItemName;
        voucher->InventoryItems[i].BaseItem.PriceLevel = voucher->priceListId;
        float price = priceHelper->getPriceByItemIDAndUOM(voucher->InventoryItems[i].BaseItem.ItemID,
                                                          voucher->InventoryItems[i].BaseItem.PriceLevel,
                                                          voucher->InventoryItems[i].BaseItem.uomObject._id);
        qDebug()<<"got price"<<price;
        if(price != -1){
            voucher->InventoryItems[i].BaseItem.price = price;
            voucher->InventoryItems[i].BaseItem.uomObject.convRate = 1;
        }
        else
            voucher->InventoryItems[i].BaseItem.price = voucher->InventoryItems[i].BaseItem.price_1;

        qDebug()<<"***********Price"<<voucher->InventoryItems[i].BaseItem.ItemName<<voucher->InventoryItems[i].BaseItem.price;

    }

    calcTotals();

    model->setVoucher(voucher);
    qDebug()<<"current price list is"<<priceHelper->getPriceListNameByID(voucher->priceListId);
}

void InventoryLedgerTransactionsEditor::showItemDetail(int row)
{
    ItemDetail *itemDetailWid = new ItemDetail(voucher, row,1,itemsHelper,
                                               uomHelper,priceHelper,godownHelper, voucher->priceListId);
    itemDetailWid->show();

    connect(itemDetailWid, &ItemDetail::updateItem, this, [=](){
        calcTotals();
        model->setVoucher(voucher);
    });
}

void InventoryLedgerTransactionsEditor::calcTotals()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<transType;
    switch (transType) {

    case 1: {
        TransactionCalculator::VoucherCalculateSales(voucher);
        break;
    }
    case 2: {

        TransactionCalculator::VoucherCalculatePurchase(voucher);
        break;
    }
    case 3: {

        TransactionCalculator::VoucherCalculateSalesReturn(voucher);
        break;
    }
    case 4: {

        TransactionCalculator::VoucherCalculatePurchaseReturn(voucher);
        break;
    }
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->grandTotal;
}

void InventoryLedgerTransactionsEditor::showCoupledWidget()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType;
    if(voucherType == GMVoucherTypes::SalesOrder || voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::PurchaseOrder || voucherType == GMVoucherTypes::PurchaseVoucher){
        if(!(voucher->ledgerObject.LedgerGroupName.contains("Cash-in-Hand") ||
             voucher->ledgerObject.LedgerGroupName.contains("Bank Accounts"))){
            qDebug()<<Q_FUNC_INFO<<__LINE__;

            coupledVoucherHandler->show();
            //            coupledVoucherHandler->resetWidgets();
        }

        else{
            coupledVoucherHandler->hide();
        }
    }
}

void InventoryLedgerTransactionsEditor::initCoupledWidget()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType;
    if(voucherType == GMVoucherTypes::SalesOrder || voucherType == GMVoucherTypes::SalesVoucher ||
            voucherType == GMVoucherTypes::PurchaseOrder || voucherType == GMVoucherTypes::PurchaseVoucher){
        if(!(voucher->ledgerObject.LedgerGroupName.contains("Cash-in-Hand") ||
             voucher->ledgerObject.LedgerGroupName.contains("Bank Accounts"))){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            coupledVoucherHandler->setParentVoucher(voucher);
            coupledVoucherHandler->resetWidgets();
            //            coupledVoucherHandler->resetWidgets();
        }

        else{
            //            coupledVoucherHandler->hide();
        }
    }
}

void InventoryLedgerTransactionsEditor::setDrawerLayout()
{

}

//void InventoryLedgerTransactionsEditor::setDrawerLayout()
//{

//}

