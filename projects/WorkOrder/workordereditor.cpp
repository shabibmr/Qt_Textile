#include "workordereditor.h"
#include "ui_workordereditor.h"


WorkOrderEditor::WorkOrderEditor(VoucherDBAbstract *dbhelper, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    this->dbHelper = dbhelper;

//    initDbHelpers();
    setDrawerLayout();
    initVoucher();
    initGlobalObjects();
//    setWidgets();
    initExtraActions();

}

WorkOrderEditor::~WorkOrderEditor()
{
}

void WorkOrderEditor::initExtraActions()
{
    contactButton->hide();
    moreButton->hide();

//    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
//    saveButton->setMini(true);
//    saveButton->setXOffset(10);
//    saveButton->setYOffset(10);
//    saveButton->setParent(this);
//    saveButton->setToolTip("Save Voucher");


    completedButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "check_circle"));
    completedButton->setMini(true);
    completedButton->setXOffset(80);
    completedButton->setYOffset(10);
    completedButton->setBackgroundColor(QColor::fromRgb(46,125,50));
    completedButton->setParent(this);
    completedButton->setToolTip("Mark Completed");
    completedButton->hide();

    undoButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "undo"));
    undoButton->setMini(true);
    undoButton->setXOffset(10);
    undoButton->setYOffset(10);
    undoButton->setBackgroundColor(QColor::fromRgb(255, 202, 40));
    undoButton->setParent(this);
    undoButton->setToolTip("Undo");
    undoButton->setCorner(Qt::BottomLeftCorner);

    snackBar = new QtMaterialSnackbar(this);
    drawer = new QtMaterialDrawer(this);
    drawer->setClickOutsideToClose(true);
    drawer->setOverlayMode(true);


    QVBoxLayout *drawerLayout = new QVBoxLayout;
    drawer->setDrawerLayout(drawerLayout);

//    printButton = new QtMaterialIconButton(QtMaterialTheme::icon("action", "print"));
//    printButton->setColor(QColor(255,250,0));
//    printButton->setIconSize(QSize(30,30));


    //    printButton->set

//    appBar->appBarLayout()->addWidget(printButton);

    QObject::connect(saveButton,&QPushButton::clicked,
                     this,&WorkOrderEditor::saveVoucher);
    QObject::connect(completedButton,&QPushButton::clicked,
                     this,&WorkOrderEditor::markCompleted);
    QObject::connect(undoButton,&QPushButton::clicked,
                     this,&WorkOrderEditor::undoTask);

//    QObject::connect(printButton,&QPushButton::clicked,
//                     this,&WorkOrderEditor::askForPrint);


    //    QObject::connect(toGodownWidget,&GMComboBox::selected, this,&WorkOrderEditor::getVouchersToImport);
}


void WorkOrderEditor::initVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    editorStatus = EditorStatus::NewVoucher;

    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;
    voucher->toGodownID = godownHelper->getDefaultGodown();

    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::WorkOrder));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);


    v1 = new GeneralVoucherDataObject();

}

void WorkOrderEditor::initGlobalObjects()
{
    selectVoucherWidget = new ShowVouchersWidget();

    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Completed"<<"Stock"<<"Pending";
    DataTableWidget = new StockEntryTableView(voucher);
    //    DataTableWidget->setEnabled(false);

    WorkOrderDelegate *del = new WorkOrderDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                uomHelper);
    DataTableWidget->setItemDelegate(del);
    connect(del, &WorkOrderDelegate::printBarcode, this, &WorkOrderEditor::printBarcode);

    model = new GMWorkOrderModel(voucher,lis.size(),true,this);
    model->setHeaderStrings(lis);

    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();


    table = new StockEntryTableView(v1);
    //    table->setItemDelegate(new WorkOrderDelegate(
    //                               itemsHelper,itemsHelper->getInventoryItemsQueryModel(1),
    //                               uomHelper));

    model2 = new GMWorkOrderModel(v1,lis.size(),false,this);
    model2->setHeaderStrings(lis);

    table->setModel(model2);
    table->setColumns();
    table->hideColumn(4);

    QObject::connect(model,&GMWorkOrderModel::updateValues,
                     this,&WorkOrderEditor::restSummaryTable);

    QPointer<QWidget> Widget1 = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout1 = new QVBoxLayout();
    Widget1->setLayout(mainLayout1);


    QPointer<QWidget> Widget2 = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout2 = new QVBoxLayout();
    Widget2->setLayout(mainLayout2);

    mainLayout1->addWidget(DataTableWidget);
    mainLayout2->addWidget(table);

    tablesTab = new QTabWidget(this);
    tablesTab->addTab(Widget1,"Data");
    tablesTab->addTab(Widget2,"Summary");

    setTableLayout(tablesTab);

    godownWidget->setGodown(&voucher->toGodownID);
    godownWidget->setLabel("Section");
    connect(godownWidget, &GMGodownLineMaterial::itemSelectedwithItem, this, [=](QString godown){

        getVouchersToImport();
        bool useSectionFlag = ConfigurationSettingsDatabaseHelper::getValue(useDefaultItemSection,false).toBool();
        if(!useSectionFlag){
            for(int i=0;i<voucher->InventoryItems.size();  i++){
                if(voucher->InventoryItems[i].BaseItem.godownList.size() == 0)
                    voucher->InventoryItems[i].BaseItem.toGodownID = voucher->toGodownID;
                else if (voucher->InventoryItems[i].BaseItem.godownList.size() == 1) {
                    voucher->InventoryItems[i].BaseItem.godownList[0].toGodown = voucher->toGodownID;
                }
            }
        }

    });
    godownWidget->show();
    mainLedgerWidget->hide();

}


void WorkOrderEditor::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    VoucherEditorBase::resetWidgets();

    godownWidget->setGodown(&voucher->toGodownID);
    delete model;

    model = new GMWorkOrderModel(voucher,lis.size(),true,this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    QObject::connect(model,&GMWorkOrderModel::updateValues,
                     this,&WorkOrderEditor::restSummaryTable);
    restSummaryTable();
}

void WorkOrderEditor::showItemChecklistDialog()
{

    ItemCheckListWidget *checkList = new ItemCheckListWidget(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    checkList->setWindowFlags(Qt::Popup);
    checkList->show();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(checkList, &ItemCheckListWidget::checkCompleted, this, [=](bool val){
        itemsChecked = val;
        saveVoucher();
    });

}
void WorkOrderEditor::saveVoucher()
{
    if(ConfigurationSettingsDatabaseHelper::getValue(promptItemChecklist,false).toBool()){
        if(!itemsChecked && voucher->status == QuotationStatus::WorkOrderCompleted){
            showItemChecklistDialog();
            return;

        }
    }

    if(voucher->toGodownID.length() == 0){
        snackBar->addMessage("Section not selected!!!");
        return;

    }

    if( voucher->InventoryItems.size()<1 ){
        snackBar->addMessage("No Items Found!!!");
        return;
    }


    //    for(CompoundItemDataObject comp:voucher->InventoryItems){
    //        if(comp.BaseItem.isPurchaseItem ){
    //            snackBar->addMessage(comp.BaseItem.ItemName + " - Purchase Item selected !!");
    //            return;
    //        }
    //        if( comp.BaseItem.section != voucher->toGodownName){
    //            snackBar->addMessage(comp.BaseItem.ItemName + " - Item not in selected section !!");
    //            return;
    //        }
    //    }


    if(editorStatus == EditorStatus::NewVoucher){
        qDebug()<<"355";
        voucher->status = QuotationStatus::WorkOrderRaised;
        dbHelper->insertVoucher(voucher);
        qDebug()<<"358";
    }
    else if(editorStatus == EditorStatus::EditVoucher ){
        dbHelper->updateVoucher(voucher,voucher->voucherNumber);
    }

    if(voucher->fromExternal){
        emit NotificationAccepted();
    }

    closeEditor();
}

//void WorkOrderEditor::deleteVoucher()
//{
//    if(true && voucher->status == QuotationStatus::SalesInvoiceRaised)    //Check Authority
//    {

//        dbHelper->deleteVoucher(voucher);
//    }
//    else{
//        snackBar->addInstantMessage("Cannot Delete");
//    }
//}

void WorkOrderEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void WorkOrderEditor::exportVoucher()
{
    if(voucher->status ==  QuotationStatus::Started){
        snackBar->addMessage("Work Order not Saved!!!");
        return;
    }

    StockRequestEditor *editor = new StockRequestEditor(new StockRequestDatabaseHelper);
    editor->setWindowTitle("Stock Request Form");
    editor->setWindowFlags(Qt::Window);

    GeneralVoucherDataObject *voucherExport = new GeneralVoucherDataObject();


    BillOfMaterialsIngredientsDataBaseHelper *bomHelper = new BillOfMaterialsIngredientsDataBaseHelper();

    voucherExport->InventoryItems = bomHelper->getBOMBySalesItemList(v1->InventoryItems);
    voucherExport->toGodownID = voucher->toGodownID;
    voucherExport->fromGodownID = "Godown";
    voucherExport->RequirementVoucherNo = voucher->voucherNumber;
    voucherExport->TransactionId = voucher->TransactionId;

    StockRequestDatabaseHelper *stockreqHelper = new StockRequestDatabaseHelper;
    QPair<QString, QString> pair = stockreqHelper->getVoucherNoByTransactionId(voucher->TransactionId);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<pair;
    if(pair.first != "-1")
    {
        GeneralVoucherDataObject *requestVoucher = stockreqHelper->getVoucherByVoucherNoPtr(pair.first, pair.second);

        voucherExport->voucherNumber = requestVoucher->voucherNumber;
        voucherExport->VoucherPrefix = requestVoucher->VoucherPrefix;
        voucherExport->VoucherDate = requestVoucher->VoucherDate;
        voucherExport->status = requestVoucher->status;

        if(voucherExport->InventoryItems.size() > 0){
            for(int i=0; i<requestVoucher->InventoryItems.size(); i++){
                for(int j=0; j<voucherExport->InventoryItems.size(); j++){
                    // check if item exists in saved voucher
                    if(requestVoucher->InventoryItems[i].BaseItem.ItemReqUuid ==
                            voucherExport->InventoryItems[j].BaseItem.ItemReqUuid ||
                            requestVoucher->InventoryItems[i].BaseItem.ItemID ==
                            voucherExport->InventoryItems[j].BaseItem.ItemID ){
                        voucherExport->InventoryItems[j].BaseItem.quantity =
                                requestVoucher->InventoryItems[i].BaseItem.quantity; // set issued qty
                        break;
                    }
                    voucherExport->InventoryItems.append(requestVoucher->InventoryItems[i]);

                }
            }
        }
        else {
            voucherExport->InventoryItems = requestVoucher->InventoryItems;
        }
        editor->setVoucher(voucherExport);
        editor->showVoucher(); // show existing issue request
    }
    else{
        editor->importWorkOrder(voucherExport); // create new issue request
    }
    editor->showMaximized();
}

void WorkOrderEditor::getVouchersToImport()
{
    qDebug()<<"Status : "<<voucher->status;

    if(voucher->status == QuotationStatus::Started && voucher->InventoryItems.size() == 0){ // added condition for items to avoid reopening of the widget while exporting from sales order

        delete  selectVoucherWidget;

        qDebug()<<"section"<<voucher->toGodownID;

        selectVoucherWidget = new ShowVouchersWidget(this);
        selectVoucherWidget->setWindowFlags(Qt::Window);
        selectVoucherWidget->setVoucherTypes(GMVoucherTypes::SalesOrder, GMVoucherTypes::WorkOrder);
        selectVoucherWidget->setSection(voucher->toGodownID);
        //    selectVoucherWidget->show();
        //    selectVoucherWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
        //    selectVoucherWidget->resize(500,200);
        QObject::connect(selectVoucherWidget,SIGNAL(importVoucher(GeneralVoucherDataObject *)),
                         this, SLOT(importItemsFromVoucher(GeneralVoucherDataObject *)));

        QObject::connect(selectVoucherWidget, &ShowVouchersWidget::noVoucherToExport,
                         this, &WorkOrderEditor::noVoucherToImport);

    }

}

void WorkOrderEditor::noVoucherToImport()
{
    qDebug()<<"No voucher to export";
    snackBar->addMessage("No vouchers to import!!!");
    return;
}

void WorkOrderEditor::setDrawerLayout()
{
    QPushButton *button = new QPushButton(this);
    button->setText("Create Stock Request");
    drawer->drawerLayout()->addWidget(button);


    QObject::connect(button,&QPushButton::clicked,
                     this,&WorkOrderEditor::exportVoucher);
}

void WorkOrderEditor::printVoucher()
{
    printHelper = new PrinterHelper();
    printHelper->PrintWorkOrderVoucher(voucher);
}

void WorkOrderEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{
    setVoucher(voucher2);
    showVoucher();
}

void WorkOrderEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{
    voucher->InventoryItems.append(voucher2->InventoryItems);
    for(CompoundItemDataObject c:voucher->InventoryItems){
        c.BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
    }
    voucher->toGodownID = voucher2->toGodownID;
    resetWidgets();
}

void WorkOrderEditor::sendEmail()
{

}

void WorkOrderEditor::markCompleted()
{
    qDebug()<<"475";

    for(int i=0;i< voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.itemProductionStatus = QuotationStatus::WorkOrderCompleted;
        voucher->InventoryItems[i].BaseItem.orderCompletedDate = QDate::currentDate();
        voucher->InventoryItems[i].BaseItem.drQty = voucher->InventoryItems[i].BaseItem.quantity;
        bool useSectionFlag = ConfigurationSettingsDatabaseHelper::getValue(useDefaultItemSection,false).toBool();
        qDebug()<<Q_FUNC_INFO<<__LINE__<<useSectionFlag<<voucher->InventoryItems[i].BaseItem.section;
        if(useSectionFlag){
            voucher->InventoryItems[i].BaseItem.toGodownID = voucher->InventoryItems[i].BaseItem.section;
        }
        else
        model->updateGodownList(i);
    }
    qDebug()<<"482";
    voucher->status = QuotationStatus::WorkOrderCompleted;
    voucher->completedTimeStamp = QDateTime::currentDateTime();
    resetWidgets();
}

void WorkOrderEditor::undoTask()
{
    for(int i=0;i< voucher->InventoryItems.size();i++){
        voucher->InventoryItems[i].BaseItem.itemProductionStatus = QuotationStatus::WorkOrderRaised;
    }
    voucher->status = QuotationStatus::WorkOrderRaised;
    resetWidgets();
}

void WorkOrderEditor::restSummaryTable()
{

    qDebug()<<"Reset Summary Table;";
    QList<CompoundItemDataObject> itemsList;
    for(int i=0;i<voucher->InventoryItems.size();i++){ qDebug()<<i;
        int j=0;
        for(;j<itemsList.size();j++){
            qDebug()<<j;
            if(itemsList[j].BaseItem.ItemID == voucher->InventoryItems[i].BaseItem.ItemID){
                qDebug()<<"adding to old "<<voucher->InventoryItems[i].BaseItem.ItemName;
                itemsList[j].BaseItem.quantity += voucher->InventoryItems[i].BaseItem.quantity;
                break;
            }
        }
        if( j == itemsList.size()){
            qDebug()<<"new Item "<<voucher->InventoryItems[i].BaseItem.ItemName;
            itemsList.append(voucher->InventoryItems[i]);
        }
    }

    v1->InventoryItems=itemsList;


    if(model2 != nullptr)
    delete model2;
    model2 = new GMWorkOrderModel(v1,lis.size(),false,this);
    model2->setHeaderStrings(lis);
    table->setVoucher(v1);
    table->setModel(model2);
    table->setColumns();

    qDebug()<<"from external"<<voucher->fromExternal;
    //    qDebug()<<"item"<<voucher->InventoryItems[0].BaseItem.ItemName;
}

void WorkOrderEditor::printBarcode(int row)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<row<<voucher->InventoryItems[row].BaseItem.itemProductionStatus<<QuotationStatus::WorkOrderCompleted;
    if(voucher->InventoryItems[row].BaseItem.itemProductionStatus == QuotationStatus::WorkOrderCompleted){
        BarcodePrintWidget *barcodePrint = new BarcodePrintWidget(&voucher->InventoryItems[row].BaseItem);
        barcodePrint->show();

        connect(barcodePrint, &BarcodePrintWidget::closing, this, &WorkOrderEditor::closeEditor);
    }
    else {

        snackBar->addMessage("Work order not completed for Item");
    }
}


void WorkOrderEditor::showVoucher()
{

    VoucherEditorBase::showVoucher();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->toGodownID<<voucher->InventoryItems.size();
    completedButton->show();
    if(voucher->toGodownID.length() == 0)
        voucher->toGodownID = godownHelper->getDefaultGodown();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->toGodownID<<voucher->InventoryItems.size();
    resetWidgets();
    exportButton->show();
    //    qDebug()<<"from external"<<voucher->fromExternal;
    //    qDebug()<<"item"<<voucher->InventoryItems[0].BaseItem.ItemName;
}

