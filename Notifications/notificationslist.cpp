#include "notificationslist.h"

#include "customwidgets/Delegates/Notifications/notificationsdelegate.h"

NotificationsList::NotificationsList(QWidget *parent) :
    QWidget(parent)
{
    this->setWindowTitle("Notifications");

    historyButton = new QtMaterialFlatButton("Show History");
    connect(historyButton, &QtMaterialFlatButton::clicked, this, [=](){
       NotificationsHistory *historyWidget = new NotificationsHistory;
       connect(historyWidget, &NotificationsHistory::refresh, this, &NotificationsList::setTableView);
       historyWidget->setWindowTitle("Notifications");
       historyWidget->setAttribute(Qt::WA_DeleteOnClose);
       historyWidget->show();
    });

    QVBoxLayout *tableLayout = new QVBoxLayout;
    this->setLayout(tableLayout);

    tableView = new ReportsTableView("");
    tableView->lastRow->hide();

    connect(tableView, &QTableView::doubleClicked, this, &NotificationsList::on_tableView_doubleClicked);

    tableLayout->addWidget(historyButton,0, Qt::AlignRight);
    tableLayout->addWidget(tableView);

    vEditor = new VoucherEditorSelector;
    connect(vEditor, &VoucherEditorSelector::NotificationAccepted, this, &NotificationsList::NotificationAccepted);
    connect(vEditor, &VoucherEditorSelector::NotificationRejected, this, &NotificationsList::NotificationRejected);
    connect(vEditor, &VoucherEditorSelector::refresh, this, &NotificationsList::setTableView);


    rHelper = new receiveChannelDatabaseHelper();
    ledHelper = new LedgerMasterDatabaseHelper();
    //    vHelper = new GeneralVoucherDatabaseHelper();
    voucher = new GeneralVoucherDataObject();
    invItem = new inventoryItemDataModel();
    invGroup = new InventoryItemGroupDataModel();
    ledger = new LedgerMasterDataModel();
    accGroup = new AccountGroupDataModel();
    notificationObj = new NotificationDataModel();

    setMinimumSize(600,400);
    tableLayout->setContentsMargins(0,0,0,0);

    model = new QSqlQueryModel(this);

    setTableView();

    QTimer *syncNotification = new QTimer(this);
    connect(syncNotification,&QTimer::timeout,this,&NotificationsList::resetModel);
//    syncNotification->start(3*1000);

    tableView->horizontalHeader() ->setSectionResizeMode(QHeaderView::Stretch);
    tableView->hideColumn(0);

}

NotificationsList::~NotificationsList()
{
}

void NotificationsList::ShowMissingDialog()
{

    QDialog *MissingItemsList = new QDialog;
    MissingItemsList->setFixedSize(300,150);
    MissingItemsList->setWindowTitle("Missing Items");
    MissingItemsList->setWindowFlags(Qt::WindowStaysOnTopHint);

    QVBoxLayout *vLayout = new QVBoxLayout(MissingItemsList);


    QLabel *textLabel = new QLabel(MissingItemsList);
    textLabel->setGeometry(QRect(50, 20, 221, 31));
    textLabel->clear();
    textLabel->setText("MISSING ITEMS LIST");

    StockEntryTableView *DataTableWidget = new StockEntryTableView(voucher);
    QStringList lis;
    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"Status";
    GMNotificationListModel *model2 = new GMNotificationListModel(missingItems,lis.size(),false,this);
    model2->setHeaderStrings(lis);

    DataTableWidget->setModel(model2);
    DataTableWidget->hideColumn(3);

    vLayout->addWidget(DataTableWidget);
    MissingItemsList->setLayout(vLayout);
    connect(DataTableWidget,&QTableView::doubleClicked,
            this,[=](QModelIndex index){
        MissingItemsList->hide();
        int row = index.row();
        missingItems->InventoryItems[row].BaseItem.fromExternal = true;
        missingItems->InventoryItems[row].BaseItem.action = 1001;

        AddItems *additemWidget = new AddItems(this);
        additemWidget->setWindowFlags(Qt::Window);
        additemWidget->setAttribute(Qt::WA_DeleteOnClose);
        additemWidget->setItem(&missingItems->InventoryItems[row].BaseItem);
        additemWidget->edit();
        additemWidget->showMaximized();

        QObject::connect(additemWidget, &AddItems::savedItem,
                         this, [=] (CompoundItemDataObject invItem) {
            missingItems->InventoryItems.removeAt(row) ;
            MissingItemsList->deleteLater();
            if(missingItems->InventoryItems.size()>0)
                ShowMissingDialog();
        });


    });

    MissingItemsList->exec();

}

void NotificationsList::setTableView()
{

    model = rHelper->getUnreadNotifications();
    NotificationsDelegate *delegate = new NotificationsDelegate(model);
    connect(delegate, &NotificationsDelegate::markRead, this, &NotificationsList::markRead);

    tableView->setItemDelegate(delegate);
    tableView->setModel(model);

    tableView->setColumnWidth(6,25);


    //    ui->tableLayout->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
 }

void NotificationsList::NotificationAccepted()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    rHelper->updateNotificationStatus(notificationObj->id, notificationObj->GeneratedTransactionId, QuotationStatus::NotificationAccepted);

    NotificationDataModel *acknowledgement = new NotificationDataModel();
    QJsonObject jsonObject;
    jsonObject.insert("Status", "Accepted");
//    if(notificationObj->TypeOfData == "Voucher" ){
//        jsonObject.insert("VoucherType", voucher->voucherType);
//        jsonObject.insert("VoucherNo", voucher->voucherNumber);
//        jsonObject.insert("VoucherPrefix", voucher->VoucherPrefix);
//    }
//    else if(notificationObj->TypeOfData == "Inventory Item" ){
//        jsonObject.insert("Item", compItem.BaseItem.ItemName);
//    }
//    else if(notificationObj->TypeOfData == "Inventory Group" ){
//        jsonObject.insert("InventoryGroup", invGroup->GroupName);
//    }
//    else if(notificationObj->TypeOfData == "Ledger" ){
//        jsonObject.insert("Ledger", ledger->LedgerName);
//    }
//    else if(notificationObj->TypeOfData == "Account Group" ){
//        jsonObject.insert("AccountGroup", accGroup->groupName);
//    }
    QJsonDocument doc(jsonObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    acknowledgement->NotificationData = strJson;
    acknowledgement->ToDBName = notificationObj->FromDBName;
    acknowledgement->GeneratedTransactionId = notificationObj->GeneratedTransactionId;
    acknowledgement->TypeOfData = "Acknowledgement";
    acknowledgement->Action = 1004; // no action


    sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
    sendHelper->insertNotification(acknowledgement);

    setTableView();

}

void NotificationsList::NotificationRejected(QString reason)
{
    rHelper->updateNotificationStatus(notificationObj->id, notificationObj->GeneratedTransactionId, QuotationStatus::NotificationRejected);


    NotificationDataModel *acknowledgement = new NotificationDataModel();
    QJsonObject jsonObject;
    jsonObject.insert("Status", "Rejected");
    jsonObject.insert("Reason", reason);
    QJsonDocument doc(jsonObject);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    acknowledgement->NotificationData = strJson;
    acknowledgement->ToDBName = notificationObj->FromDBName;
    acknowledgement->GeneratedTransactionId = notificationObj->GeneratedTransactionId;
    acknowledgement->TypeOfData = "Acknowledgement";
    acknowledgement->Action = 1004; // no action

    sendChannelDatabaseHelper *sendHelper = new sendChannelDatabaseHelper();
    sendHelper->insertNotification(acknowledgement);

    setTableView();
}

void NotificationsList::markRead(int row)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Notifications","Do you want to mark notification as read?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        int id = model->record(row).value(0).toInt();
        QString transId = model->record(row).value(6).toString();

        rHelper->updateNotificationStatus(id, transId, QuotationStatus::NotificationRead);

        setTableView();


    }

    else
        return;

}

void NotificationsList::ShowSalesOrderVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::SalesOrder);
    SalesOrderDatabaseHelper *vHelper = new SalesOrderDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        qDebug()<<voucher->voucherNumber;
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;
            }

            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);


//    SalesOrderVoucherEditor *voucherWidget = new SalesOrderVoucherEditor();
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}


void NotificationsList::ShowPurchaseOrderVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::PurchaseOrder);
    PurchaseOrderDatabaseHelper *vHelper = new PurchaseOrderDatabaseHelper();
    if(notificationObj->Action == 1001){
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;
            }

            else
                return;


        }
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    vEditor->openExternalVoucher(voucher);

//    PurchaseOrderVoucherEditor *voucherWidget = new PurchaseOrderVoucherEditor(new PurchaseOrderDatabaseHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));
}


void NotificationsList::ShowSalesInvoiceVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::SalesVoucher);
    salesVoucherDatabaseHelper *vHelper = new salesVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
        voucher->status = QuotationStatus::Started;
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;


        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);

//    SalesVoucherEditor *voucherWidget = new SalesVoucherEditor(new salesVoucherDatabaseHelper());
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}

void NotificationsList::ShowPurchaseInvoiceVoucher()
{
    qDebug()<<"Show Purchase Invoice";
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::PurchaseVoucher);
    PurchaseVoucherDatabaseHelper *vHelper = new PurchaseVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();



    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }

            else
                return;

        }
    }

    vEditor->openExternalVoucher(voucher);

//    PurchaseVoucherEditor *voucherWidget = new PurchaseVoucherEditor(new PurchaseVoucherDatabaseHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));


}

void NotificationsList::ShowDebitNoteVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::DebitNote);
    DebitNoteVoucherDatabaseHelper *vHelper = new DebitNoteVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }

            else
                return;


        }
    }
    DebitNoteVoucherEditor *voucherWidget = new DebitNoteVoucherEditor(new DebitNoteVoucherDatabaseHelper);
    voucherWidget->setVoucher(voucher);
    voucherWidget->showVoucher();
    voucherWidget->showMaximized();
    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}


void NotificationsList::ShowCreditNoteVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::CreditNote);
    CreditNoteVoucherDatabaseHelper *vHelper = new CreditNoteVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }
    CreditNoteVoucherEditor *voucherWidget = new CreditNoteVoucherEditor(new CreditNoteVoucherDatabaseHelper);
    voucherWidget->setVoucher(voucher);
    voucherWidget->showVoucher();
    voucherWidget->showMaximized();
    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}

void NotificationsList::ShowDeliveryNoteVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::DeliveryNote);
    DeliveryNoteDatabaseHelper *vHelper = new DeliveryNoteDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);

//    DeliveryNoteEditor *voucherWidget = new DeliveryNoteEditor(new DeliveryNoteDatabaseHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}


void NotificationsList::ShowReceiptNoteVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::ReceiptNote);
    ReceiptNoteDatabaseHelper *vHelper = new ReceiptNoteDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);

//    ReceiptNoteVoucherEditor *voucherWidget = new ReceiptNoteVoucherEditor(new ReceiptNoteDatabaseHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}


void NotificationsList::ShowReceiptVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::ReceiptVoucher);
    ReceiptVoucherDatabaseHelper *vHelper = new ReceiptVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            qDebug()<<"88888888888888888888888"<<pair.first;
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                qDebug()<<" voucher not exist to update***************";
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);

//    ReceiptVoucherEditor *voucherWidget = new ReceiptVoucherEditor(new ReceiptVoucherDatabaseHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

}

void NotificationsList::ShowPaymentVoucher()
{
    voucher->voucherType = GMVoucherTypes::getVoucherString( GMVoucherTypes::PaymentVoucher);
    PaymentVoucherDatabaseHelper *vHelper = new PaymentVoucherDatabaseHelper();
    if(notificationObj->Action == 1001){
        voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
        voucher->VoucherDate = QDate::currentDate();
    }
    else if (notificationObj->Action == 1002) {
        pair = vHelper->getVoucherNoByTransactionId(voucher->TransactionId);
        if(pair.first != "-1"){
            voucher->voucherNumber = pair.first;
            voucher->VoucherPrefix = pair.second;
        }
        else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"New Voucher","Voucher doesn't exist, Would You Like to create?",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                voucher->voucherNumber =  vHelper->getNextVoucherNo(voucher->VoucherPrefix);
                voucher->VoucherDate = QDate::currentDate();
                voucher->action = 1001;

            }
            else
                return;


        }
    }

    vEditor->openExternalVoucher(voucher);

//    PaymentVoucherEditor *voucherWidget = new PaymentVoucherEditor(vHelper);
//    voucherWidget->setVoucher(voucher);
//    voucherWidget->showVoucher();
//    voucherWidget->showMaximized();
//    QObject::connect(voucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
//    QObject::connect(voucherWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));


}

void NotificationsList::resetModel()
{

   setTableView();
   qDebug()<<"Model Reset Count : "<<model->rowCount();
   emit notificationCount(model->rowCount());
}


void NotificationsList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int r = index.row();

    int id = model->record(r).value(0).toInt();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<id;

    notificationObj = rHelper->getNotificationById(id);
    QJsonDocument doc = QJsonDocument::fromJson(notificationObj->NotificationData.toUtf8());

    if(notificationObj->TypeOfData.contains("Acknowledgement"))
    {
        QJsonObject ack = doc.object();
        if(ack["Status"].toString() == "Rejected"){
            QMessageBox box; box.setText("Reason for Rejection: " + ack["Reason"].toString()); box.exec();
        }
        rHelper->updateNotificationStatus(notificationObj->id, notificationObj->GeneratedTransactionId,
                                          QuotationStatus::NotificationAccepted);
        setTableView();
    }

    SalesInventoryItemDatabaseHelper *sHelper = new SalesInventoryItemDatabaseHelper();

    if(notificationObj->TypeOfData == "Voucher"){

        bool showVoucher = true;
        voucher = voucher->JsonToObj(doc.object());


        missingItems = new GeneralVoucherDataObject();
        for(int i=0;i<voucher->InventoryItems.size();i++){
            CompoundItemDataObject c = voucher->InventoryItems[i];
            inventoryItemDataModel voucherItem = sHelper->getInventoryItemByID(c.BaseItem.ItemID);
            qDebug()<<"item"<<c.BaseItem.ItemID<<voucherItem.ItemID;
            if(c.BaseItem.ItemID != voucherItem.ItemID){

                showVoucher = false;
                missingItems->InventoryItems.append(c);
            }
            else{
                voucherItem.quantity = c.BaseItem.quantity;
                voucherItem.crQty = c.BaseItem.crQty;
                voucherItem.drQty = c.BaseItem.drQty;
                voucherItem.ItemReqUuid = c.BaseItem.ItemReqUuid;
                voucherItem.ItemDescription = c.BaseItem.ItemDescription;
                if(GMVoucherTypes::compareVoucherType( voucher->voucherType , GMVoucherTypes::PurchaseOrder)
                        || GMVoucherTypes::compareVoucherType( voucher->voucherType , GMVoucherTypes::PurchaseVoucher)
                        || GMVoucherTypes::compareVoucherType( voucher->voucherType , GMVoucherTypes::ReceiptNote))
                    voucherItem.price = c.BaseItem.price;
            }
        }

        if(!showVoucher){
            qDebug()<<"missing items";
            ShowMissingDialog();
        }
        else {
            voucher->fromExternal = true;
            voucher->action = notificationObj->Action;
            voucher->VoucherPrefix = LoginValues::voucherPrefix;
            voucher->ledgerObject = ledHelper->getLedgerObjectByID(rHelper->getLedgerIdByDbName(notificationObj->FromDBName));


            if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher), Qt::CaseInsensitive))
                ShowSalesInvoiceVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher), Qt::CaseInsensitive))
                ShowPurchaseInvoiceVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder), Qt::CaseInsensitive))
                ShowPurchaseOrderVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder), Qt::CaseInsensitive))
                ShowSalesOrderVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::CreditNote), Qt::CaseInsensitive))
                ShowDebitNoteVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::DebitNote), Qt::CaseInsensitive))
                ShowCreditNoteVoucher();
            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher), Qt::CaseInsensitive))
                ShowReceiptVoucher();
            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher), Qt::CaseInsensitive))
                ShowPaymentVoucher();

            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote), Qt::CaseInsensitive))
                ShowReceiptNoteVoucher();
            else if(!QString::compare(voucher->voucherType, GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote), Qt::CaseInsensitive))
                ShowDeliveryNoteVoucher();
        }

    }
    else if (notificationObj->TypeOfData == "Inventory Item") {

        invItem = invItem->JsonToObjPtr(doc.object()["BaseItem"].toObject());
        invItem->fromExternal = true;
        invItem->action = notificationObj->Action;

        AddItems *additemWidget = new AddItems(this);
        additemWidget->setWindowFlags(Qt::Window);
        additemWidget->setAttribute(Qt::WA_DeleteOnClose);
        additemWidget->setItem(invItem);
        additemWidget->edit();
        additemWidget->show();
        QObject::connect(additemWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(additemWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(additemWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

    else if (notificationObj->TypeOfData == "Ledger") {
        ledger = ledger->JsonToObjPtr(doc.object());
        ledger->fromExternal = true;
        ledger->action = notificationObj->Action;

        addLedger *addLedgerWidget = new addLedger(this);
        addLedgerWidget->setWindowFlags(Qt::Window);
        addLedgerWidget->setAttribute(Qt::WA_DeleteOnClose);
        addLedgerWidget->setLedger(ledger);
        addLedgerWidget->edit();
        addLedgerWidget->show();
        QObject::connect(addLedgerWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(addLedgerWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(addLedgerWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

    else if (notificationObj->TypeOfData == "Inventory Group") {
        invGroup = invGroup->JsonToObjPtr(doc.object());
        invGroup->fromExternal = true;
        invGroup->action = notificationObj->Action;

        AddItemGroups *addItemGroupWidget = new AddItemGroups(this);
        addItemGroupWidget->setWindowFlags(Qt::Window);
        addItemGroupWidget->setAttribute(Qt::WA_DeleteOnClose);
        addItemGroupWidget->setInvGroup(invGroup);
        addItemGroupWidget->edit(invGroup);
        addItemGroupWidget->show();
        QObject::connect(addItemGroupWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(addItemGroupWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(addItemGroupWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

    else if (notificationObj->TypeOfData == "Account Group") {
        accGroup = accGroup->JsonToObjPtr(doc.object());
        accGroup->fromExternal = true;
        accGroup->action = notificationObj->Action;

        addAccountGroup *addAccGroupWidget = new addAccountGroup(this);
        addAccGroupWidget->setWindowFlags(Qt::Window);
        addAccGroupWidget->setAttribute(Qt::WA_DeleteOnClose);
        //        addAccGroupWidget->setAccountGroup(accGroup);
        addAccGroupWidget->editAccountGroup(accGroup);
        addAccGroupWidget->show();
        QObject::connect(addAccGroupWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(addAccGroupWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(addAccGroupWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

    else if (notificationObj->TypeOfData == "Godown") {
        godown = godown->JsonToObjPtr(doc.object());
        godown->fromExternal = true;
        godown->action = notificationObj->Action;

        addGodown *godownWidget = new addGodown(this);
        godownWidget->setWindowFlags(Qt::Window);
        godownWidget->setAttribute(Qt::WA_DeleteOnClose);
        godownWidget->setGodown(godown);
        godownWidget->edit();
        godownWidget->show();
        QObject::connect(godownWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(godownWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(godownWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

    else if (notificationObj->TypeOfData == "Brand") {
        brand = brand->JsonToObjPtr(doc.object());
        brand->fromExternal = true;
        brand->action = notificationObj->Action;

        AddBrands *AddBrandsWidget = new AddBrands(this);
        AddBrandsWidget->setWindowFlags(Qt::Window);
        AddBrandsWidget->setAttribute(Qt::WA_DeleteOnClose);
        AddBrandsWidget->setBrand(brand);
        AddBrandsWidget->edit();
        AddBrandsWidget->show();

        QObject::connect(AddBrandsWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(AddBrandsWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(AddBrandsWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }


    else if (notificationObj->TypeOfData == "UOM") {
        uom = uom->JsonToObjPtr(doc.object());
        uom->fromExternal = true;
        uom->action = notificationObj->Action;

        addUOM *addUOMWidget = new addUOM(this);
        addUOMWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
        addUOMWidget->setAttribute(Qt::WA_DeleteOnClose);
        addUOMWidget->setTitle("Update UOM");
        addUOMWidget->editUOM(uom);
        addUOMWidget->show();

        QObject::connect(addUOMWidget,SIGNAL(closing()),this,SLOT(setTableView()));
        QObject::connect(addUOMWidget,SIGNAL(NotificationAccepted()),this,SLOT(NotificationAccepted()));
        QObject::connect(addUOMWidget,SIGNAL(NotificationRejected(QString)),this,SLOT(NotificationRejected(QString)));

    }

}
