#include "vouchereditorselector.h"
#include <QPair>


VoucherEditorSelector::VoucherEditorSelector(UiAccessControlDataModel *permissions, QWidget *parent):QWidget(parent)
{

    this->permissions = permissions;

    salesHelper = new salesVoucherDatabaseHelper;
    purchaseHelper = new PurchaseVoucherDatabaseHelper;
    paymentHelper = new PaymentVoucherDatabaseHelper;
    receiptHelper = new ReceiptVoucherDatabaseHelper;
    journalHelper = new JournalVoucherDatabaseHelper;
    creditHelper = new CreditNoteVoucherDatabaseHelper;
    debitHelper = new DebitNoteVoucherDatabaseHelper;
    proformaHelper = new ProformaDatabaseHelper;
    contraHelper = new ContraVoucherDatabaseHelper;
    deliveryHelper = new DeliveryNoteDatabaseHelper;
    receiptNoteHelper = new ReceiptNoteDatabaseHelper;
    POHelper = new PurchaseOrderDatabaseHelper;
    SOHelper = new SalesOrderDatabaseHelper;
    WOHelper = new WorkOrderDatabaseHelper;

    stockJournalHelper = new StockJournalDatabaseHelper;
    stockEntryHelper = new StockEntryDatabaseHelper;
    materialHelper = new materialinoutDatabaseHelper;
    wastageHelper = new DefectiveDatabaseHelper;
    stockRequestHelper = new StockRequestDatabaseHelper;
    godownTransferHelper = new GodownTransferDatabaseHelper;

    userGroupHelper = new UserGroupDatabaseHelper;



}

void VoucherEditorSelector::openEditor(QString voucherNo, QString voucherPrefix, QString voucherType)
{
    permissions = userGroupHelper->getPermissionsByUserGroupAndVoucherType(LoginValues::model._id,
                                                                           voucherType);
    if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PurchaseVoucher) ){

        voucherWidget = new PurchaseVoucherEditor( purchaseHelper);
        GeneralVoucherDataObject *voucher = purchaseHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType();
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::SalesVoucher)){
        voucherWidget = new SalesVoucherEditor(salesHelper);
        GeneralVoucherDataObject* voucher = salesHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType();
        voucherWidget->showVoucher();
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        //        connect(voucherWidget, &VoucherEditorBase::closing, this, &VoucherEditorSelector::refreshList);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PaymentVoucher)){
        PaymentVoucherEditor* voucherWidget = new PaymentVoucherEditor(paymentHelper);
        GeneralVoucherDataObject* voucher = paymentHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ReceiptVoucher)){
        GeneralVoucherDataObject* voucher = receiptHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        ReceiptVoucherEditor* voucherWidget = new ReceiptVoucherEditor(receiptHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::JournalVoucher)){
        GeneralVoucherDataObject* voucher = journalHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        JournalVoucherEditor* voucherWidget = new JournalVoucherEditor(journalHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::CreditNote) ){
        voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::CreditNote, creditHelper);
        GeneralVoucherDataObject* voucher = creditHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType();
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DebitNote)){
        GeneralVoucherDataObject* voucher = debitHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::DebitNote, debitHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType();
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DeliveryNote) ){
        GeneralVoucherDataObject* voucher = deliveryHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget = new DeliveryNoteEditor(deliveryHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType(GMVoucherTypes::DebitNote);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ReceiptNote)){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        GeneralVoucherDataObject* voucher = receiptNoteHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget = new ReceiptNoteVoucherEditor(receiptNoteHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType(GMVoucherTypes::ReceiptNote);
        voucherWidget->showVoucher();
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PurchaseOrder)){
        GeneralVoucherDataObject* voucher = POHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget = new PurchaseOrderVoucherEditor(POHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType(GMVoucherTypes::PurchaseOrder);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::SalesOrder)){
        GeneralVoucherDataObject* voucher = SOHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget = new SalesOrderVoucherEditor(SOHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType(GMVoucherTypes::SalesOrder);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::WorkOrder)  ){
        GeneralVoucherDataObject* voucher = WOHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        WorkOrderEditor* voucherWidget = new WorkOrderEditor(WOHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ContraVoucher)){
        ContraVoucherEditor* voucherWidget= new ContraVoucherEditor(contraHelper);
        GeneralVoucherDataObject* voucher = contraHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  proformaHelper->VoucherType ){
        voucherWidget = new ProformaInvoiceEditor( proformaHelper);
        GeneralVoucherDataObject *voucher = proformaHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType();
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DefectiveVoucher) ){
        GeneralVoucherDataObject* voucher = wastageHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        WastageEntryEditor* voucherWidget= new WastageEntryEditor(wastageHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::IndentRequest)  ){
        GeneralVoucherDataObject* voucher = stockRequestHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        StockRequestEditor* voucherWidget= new StockRequestEditor(stockRequestHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::StockJournal)){
        GeneralVoucherDataObject* voucher = stockJournalHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        StockJournalEditor* voucherWidget= new StockJournalEditor(stockJournalHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::StockEntry)  ){
        StockEntryEditor* voucherWidget= new StockEntryEditor(stockEntryHelper);
        GeneralVoucherDataObject* voucher = stockEntryHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  materialHelper->VoucherType ){
        MaterialInOutEditor* voucherWidget= new MaterialInOutEditor;
        GeneralVoucherDataObject* voucher = materialHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  godownTransferHelper->VoucherType ){
        GodownTransferEditor* voucherWidget= new GodownTransferEditor;
        GeneralVoucherDataObject* voucher = godownTransferHelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);
        voucherWidget->showVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::itemwiseOrder)){
        ItemwiseSalesOrderDatabaseHelper *dbHelper = new ItemwiseSalesOrderDatabaseHelper(SalesOrderDatabaseHelper::Sales_Order_Main_TableName,
                                                                                          SalesOrderDatabaseHelper::Sales_Order_Details_TableName);
        GeneralVoucherDataObject *voucher = dbHelper->getVoucherByVoucherNoAsPtr(voucherNo,voucherPrefix);
        AddItemwiseSalesOrder *voucherWidget = new AddItemwiseSalesOrder(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwiseOrder),
                                                                         GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder)
                                                                         ,SOHelper,
                                                                         dbHelper,
                                                                         this);
        //        AddVoucher->setWindowTitle(title);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::itemwisePurchaseOrder)){
        ItemwiseSalesOrderDatabaseHelper *dbHelper = new ItemwiseSalesOrderDatabaseHelper(PurchaseOrderDatabaseHelper::Purchase_Order_Main_TableName,
                                                                                          PurchaseOrderDatabaseHelper::Purchase_Order_Details_TableName);
        GeneralVoucherDataObject *voucher = dbHelper->getVoucherByVoucherNoAsPtr(voucherNo,voucherPrefix);
        AddItemwiseSalesOrder *voucherWidget = new AddItemwiseSalesOrder(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwisePurchaseOrder),
                                                                         GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder)
                                                                         ,POHelper,
                                                                         dbHelper,
                                                                         this);
        //        AddVoucher->setWindowTitle(title);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::TableReservation)){
        TableReservationDatabaseHelper *dbHelper = new TableReservationDatabaseHelper();
        TableReservationDataModel *voucher = dbHelper->getTableReservationById(voucherNo);
        TableReservationEditor *voucherWidget = new TableReservationEditor(voucher,
                                                                         dbHelper,
                                                                         this);
        //        AddVoucher->setWindowTitle(title);

        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
}

void VoucherEditorSelector::setPermissions(UiAccessControlDataModel *value)
{
    permissions = value;
}

void VoucherEditorSelector::exportVoucher(GeneralVoucherDataObject *voucher, QString voucherType)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->voucherType<<voucher->SalesmanID;
    permissions = userGroupHelper->getPermissionsByUserGroupAndVoucherType(LoginValues::model._id,
                                                                           voucherType);

    if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DeliveryNote)){
        QPair<QString, QString> vPair =
                deliveryHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(vPair.first == "-1"){ // no voucher existing
            voucherWidget = new DeliveryNoteEditor(deliveryHelper);
            connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
            voucherWidget->setWindowFlags(Qt::Window);
            voucherWidget->importItemsFromVoucher(voucher);
            voucherWidget->setMainLedger(voucher->ledgerObject);
            voucherWidget->setSalesMan(voucher->SalesmanID);
            voucherWidget->setGodown(voucher->toGodownID);
            voucherWidget->showMaximized();
        }
        else {
            openEditor(vPair.first,vPair.second, voucherType);
        }
    }
    else if(GMVoucherTypes::compareVoucherType(voucherType, GMVoucherTypes::SalesVoucher)){
        QPair<QString, QString> vPair =
                salesHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(vPair.first == "-1"){ // no voucher existing
            voucherWidget = new SalesVoucherEditor(salesHelper);
            voucherWidget->setWindowTitle("Sales Voucher Form");
            voucherWidget->setWindowFlags(Qt::Window);
            if(voucher->advanceCash > 0)
                voucherWidget->setAdvancePaid(voucher->grandTotal - voucher->advanceCash);
            voucherWidget->importItemsFromVoucher(voucher);
            voucherWidget->setMainLedger(voucher->ledgerObject);
            voucherWidget->setSalesMan(voucher->SalesmanID);
            voucherWidget->setGodown(voucher->fromGodownID);
            voucherWidget->showMaximized();
            connect(voucherWidget, &VoucherEditorBase::saveCompleted, this, &VoucherEditorSelector::emitSaveCompleted);
        }
        else {
            openEditor(vPair.first,vPair.second, voucherType);
        }
    }

    else if(GMVoucherTypes::compareVoucherType(voucherType, GMVoucherTypes::PurchaseVoucher)){
        QPair<QString, QString> vPair =
                purchaseHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(vPair.first == "-1"){
            voucherWidget = new PurchaseVoucherEditor(purchaseHelper);
            voucherWidget->setWindowTitle("Purchase Voucher Form");
            voucherWidget->setWindowFlags(Qt::Window);
            if(voucher->advanceCash > 0)
                voucherWidget->setAdvancePaid(voucher->grandTotal - voucher->advanceCash);
            voucherWidget->importItemsFromVoucher(voucher);
            voucherWidget->setMainLedger(voucher->ledgerObject);
            voucherWidget->setSalesMan(voucher->SalesmanID);
            voucherWidget->setGodown(voucher->toGodownID);
            voucherWidget->showMaximized();
            connect(voucherWidget, &VoucherEditorBase::saveCompleted, this, &VoucherEditorSelector::emitSaveCompleted);
        }
        else {
            openEditor(vPair.first,vPair.second, voucherType);
        }
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PurchaseOrder)){
        voucherWidget = new PurchaseOrderVoucherEditor(POHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->setVoucher(voucher);
        voucherWidget->importItemsFromVoucher(voucher);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ReceiptNote)){
        QPair<QString, QString> vPair =
                receiptNoteHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(vPair.first == "-1"){ // no voucher existing
            voucherWidget = new ReceiptNoteVoucherEditor(receiptNoteHelper);
            voucherWidget->setWindowFlags(Qt::Window);
            voucherWidget->importItemsFromVoucher(voucher);
            voucherWidget->setMainLedger(voucher->ledgerObject);
            voucherWidget->setSalesMan(voucher->SalesmanID);
            voucherWidget->setGodown(voucher->toGodownID);
            voucherWidget->showMaximized();
            connect(voucherWidget, &VoucherEditorBase::saveCompleted, this,&VoucherEditorSelector::emitSaveCompleted);
        }
        else {
            openEditor(vPair.first,vPair.second, voucherType);
        }
    }
}

void VoucherEditorSelector::openExternalVoucher(GeneralVoucherDataObject *voucher)
{

    permissions = userGroupHelper->getPermissionsByUserGroupAndVoucherType(LoginValues::model._id,
                                                                           voucher->voucherType);
    if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::SalesOrder)){

        voucherWidget = new SalesOrderVoucherEditor(SOHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::PurchaseOrder)){

        qDebug()<<Q_FUNC_INFO<<__LINE__<<"PO";

        voucherWidget = new PurchaseOrderVoucherEditor(POHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));
//        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::SalesVoucher)){

        voucherWidget = new SalesVoucherEditor(salesHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::PurchaseVoucher)){

        voucherWidget = new PurchaseVoucherEditor(purchaseHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::DeliveryNote)){

        voucherWidget = new DeliveryNoteEditor(deliveryHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::ReceiptNote)){

        voucherWidget = new ReceiptNoteVoucherEditor(receiptNoteHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::PaymentVoucher)){

        voucherWidget = new PaymentVoucherEditor(paymentHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->setEditorStatus(EditorStatus::NewVoucher);
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }

    else if(GMVoucherTypes::compareVoucherType(voucher->voucherType, GMVoucherTypes::ReceiptVoucher)){

        voucherWidget = new ReceiptVoucherEditor(receiptHelper);
        voucherWidget->setVoucher(voucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showVoucher();
        voucherWidget->setEditorStatus(EditorStatus::NewVoucher);
        voucherWidget->showMaximized();

        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
        QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(emitNotificationAccepted()));
        QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(emitNotificationRejected(QString)));

    }
}


void VoucherEditorSelector::createNewVoucher(QString voucherType)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucherType;
    if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PurchaseVoucher)){

        voucherWidget = new PurchaseVoucherEditor( purchaseHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::PurchaseVoucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));

    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::SalesVoucher)){
        voucherWidget = new SalesVoucherEditor(salesHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::SalesVoucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PaymentVoucher)){
        voucherWidget = new PaymentVoucherEditor(paymentHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::PaymentVoucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ReceiptVoucher)){
        voucherWidget = new ReceiptVoucherEditor(receiptHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::JournalVoucher)){
        voucherWidget = new JournalVoucherEditor(journalHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::CreditNote)){
        voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::CreditNote, creditHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::CreditNote);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DebitNote)){
        voucherWidget = new InventoryLedgerTransactionsEditor(GMVoucherTypes::DebitNote, debitHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::DebitNote);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DeliveryNote)){
        voucherWidget = new DeliveryNoteEditor(deliveryHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::DeliveryNote);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::ReceiptNote)){
        voucherWidget = new ReceiptNoteVoucherEditor(receiptNoteHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::ReceiptNote);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::PurchaseOrder)){
        voucherWidget = new PurchaseOrderVoucherEditor( POHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::PurchaseOrder);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::SalesOrder)  ){
        voucherWidget = new SalesOrderVoucherEditor(SOHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::SalesOrder);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::WorkOrder)  ){
        WorkOrderEditor* voucherWidget = new WorkOrderEditor(WOHelper);
        voucherWidget->setPermissions(permissions);
        //        voucherWidget->setVoucherType(GMVoucherTypes::SalesOrder);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }


    else if(voucherType==  contraHelper->VoucherType ){
        ContraVoucherEditor* voucherWidget= new ContraVoucherEditor(contraHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  proformaHelper->VoucherType ){
        voucherWidget = new ProformaInvoiceEditor(proformaHelper);
        //        voucherWidget->setVoucherType(GMVoucherTypes::SalesVoucher);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::DefectiveVoucher) ){
        WastageEntryEditor* voucherWidget= new WastageEntryEditor(wastageHelper);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::IndentRequest) ){
        StockRequestEditor* voucherWidget= new StockRequestEditor(stockRequestHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::StockJournal) ){
        StockJournalEditor* voucherWidget= new StockJournalEditor(stockJournalHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::StockEntry) ){
        StockEntryEditor* voucherWidget= new StockEntryEditor(stockEntryHelper);
        voucherWidget->setPermissions(permissions);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  materialHelper->VoucherType ){
        MaterialInOutEditor* voucherWidget= new MaterialInOutEditor;
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(voucherType==  godownTransferHelper->VoucherType ){
        GodownTransferEditor* voucherWidget= new GodownTransferEditor;
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }

    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::itemwiseOrder)){
        AddItemwiseSalesOrder *voucherWidget = new AddItemwiseSalesOrder(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwiseOrder),
                                                                         GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder)
                                                                         ,SOHelper,
                                                                         new ItemwiseSalesOrderDatabaseHelper
                                                                         (SalesOrderDatabaseHelper::Sales_Order_Main_TableName,
                                                                          SalesOrderDatabaseHelper::Sales_Order_Details_TableName),
                                                                         this);
        //        AddVoucher->setWindowTitle(title);
        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::itemwisePurchaseOrder)){
        AddItemwiseSalesOrder *voucherWidget = new AddItemwiseSalesOrder(GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwisePurchaseOrder),
                                                                         GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder)
                                                                         ,POHelper,
                                                                         new ItemwiseSalesOrderDatabaseHelper
                                                                         (PurchaseOrderDatabaseHelper::Purchase_Order_Main_TableName,
                                                                          PurchaseOrderDatabaseHelper::Purchase_Order_Details_TableName),
                                                                         this);
        //        AddVoucher->setWindowTitle(title);
        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
    else if(GMVoucherTypes::compareVoucherType( voucherType,  GMVoucherTypes::TableReservation)){
        TableReservationEditor *voucherWidget = new TableReservationEditor();

        //        AddVoucher->setWindowTitle(title);
        voucherWidget->setWindowFlags(Qt::Window);
        voucherWidget->showMaximized();
        QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(refreshList()));
    }
}

void VoucherEditorSelector::refreshList()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit refresh();
}

void VoucherEditorSelector::emitSaveCompleted()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    emit saveCompleted();
}

void VoucherEditorSelector::emitNotificationAccepted()
{
    emit NotificationAccepted();
}

void VoucherEditorSelector::emitNotificationRejected(QString reason)
{
    emit NotificationRejected(reason);
}
