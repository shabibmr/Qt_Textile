#include "stockjournaleditor.h"
//#include "ui_stockjournaleditor.h"

StockJournalEditor::StockJournalEditor(VoucherDBAbstract *db, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{

    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");

    this->dbHelper = db;


    initVoucher();
    initGlobalObjects();
    initExtraActions();
}

StockJournalEditor::~StockJournalEditor()
{
}

void StockJournalEditor::initExtraActions()
{
    contactButton->hide();
    moreButton->hide();
    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&StockJournalEditor::saveVoucher);

}

void StockJournalEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::StockJournal);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    editorStatus = EditorStatus::NewVoucher;
    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::StockJournal));
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);

}

void StockJournalEditor::initGlobalObjects()
{
    mainLedgerWidget->hide();

    lis<<"#"<<"Particulars"<<"UOM"<<"Consumed"<<"Produced";
    DataTableWidget = new StockEntryTableView(voucher);
    qDebug()<<"Init table";

    StockJournalDelegate *itemDelegate = new StockJournalDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                uomHelper);
    connect(itemDelegate, &StockJournalDelegate::showItemDetailClicked,
            this, &StockJournalEditor::showItemDetail);


    DataTableWidget->setItemDelegate(itemDelegate);
    model = new GMStockJournalModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    setTableLayout(DataTableWidget);

}

void StockJournalEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();
    delete model;

    model = new GMStockJournalModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
}

void StockJournalEditor:: saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

//    qDebug()<<voucher->ObjToJson(voucher);
    VoucherEditorBase::saveVoucher();
    closeEditor();
}

void StockJournalEditor::askForPrint()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        printVoucher();
    }
}

void StockJournalEditor::printVoucher()
{
    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
}

void StockJournalEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void StockJournalEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void StockJournalEditor::sendEmail()
{

}

void StockJournalEditor::showVoucher()
{
    VoucherEditorBase::showVoucher();
    qDebug()<<"Status : "<<voucher->status;
    resetWidgets();
}



void StockJournalEditor::showItemDetail(int row)
{
    qDebug()<<Q_FUNC_INFO;
    ItemDetail *itemDetailWid = new ItemDetail(voucher, row,3,itemsHelper,
            uomHelper,new PricelistDatabaseHelper,new GodownDatabaseHelper, voucher->priceListId);
    itemDetailWid->show();

    connect(itemDetailWid, &ItemDetail::updateItem, this, [=](){
        model->setVoucher(voucher);
    });
}
