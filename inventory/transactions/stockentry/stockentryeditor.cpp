#include "stockentryeditor.h"
//#include "ui_stockentryeditor.h"

StockEntryEditor::StockEntryEditor(VoucherDBAbstract *dbHelper, VoucherEditorBase *parent) :
    VoucherEditorBase(parent)
{
    this->setTitle(windowTitle);
    this->setObjectName("editor");
    this->dbHelper = dbHelper;

    initVoucher();
    initGlobalObjects();
    initExtraActions();
    setTitle(GMVoucherTypes::getVoucherTitle(GMVoucherTypes::StockEntry));

}

void StockEntryEditor::initExtraActions()
{

    QPushButton *button = new QPushButton(this);
    button->setText("Import All Stock Items");

    addToDrawerLayout(button);

    QObject::connect(saveButton,&QPushButton::clicked,
                           this,&StockEntryEditor::saveVoucher);

    QObject::connect(button,&QPushButton::clicked,
                           this,&StockEntryEditor::importAllStockItems);
    mainLedgerWidget->hide();

    exportButton->show();
    contactButton->hide();
    moreButton->hide();

//    godownWidget->show();
}


void StockEntryEditor::initVoucher()
{
    voucher = new GeneralVoucherDataObject();
    editorStatus = EditorStatus::NewVoucher;
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::StockEntry);
    voucher->voucherNumber = dbHelper->getNextVoucherNo(voucher->VoucherPrefix);
    voucher->status = QuotationStatus::Started;

    setTitle(voucher->voucherType);
    voucherDateWidget->setVariable(&voucher->VoucherDate);
    voucherNumberWidget->setVariable(&voucher->voucherNumber);
    voucherNarrationWidget->setVariable(&voucher->narration);
//    resetWidgets();

}

void StockEntryEditor::initGlobalObjects()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    mainLedgerWidget->hide();



    lis<<"#"<<"Particulars"<<"UOM"<<"Quantity"<<"f";
    DataTableWidget = new StockEntryTableView(voucher);
    qDebug()<<"Init table";

    StockEntryDelegate *itemDelegate = new StockEntryDelegate(
                itemsHelper,itemsHelper->getInventoryItemsQueryModel(),
                uomHelper);
    connect(itemDelegate, &StockEntryDelegate::showItemDetailClicked,
            this, &StockEntryEditor::showItemDetail);


    DataTableWidget->setItemDelegate(itemDelegate);

    model = new GMStockEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    setTableLayout(DataTableWidget);

}
void StockEntryEditor::resetWidgets()
{
    VoucherEditorBase::resetWidgets();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    delete model;

    model = new GMStockEntryModel(voucher,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setVoucher(voucher);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void StockEntryEditor::saveVoucher()
{
    if(voucher->InventoryItems.size()<1){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    VoucherEditorBase::saveVoucher();

    closeEditor();
}


void StockEntryEditor::printVoucher()
{
    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryVoucher(voucher);
}

void StockEntryEditor::importVoucher(GeneralVoucherDataObject *voucher2)
{

}

void StockEntryEditor::importItemsFromVoucher(GeneralVoucherDataObject *voucher2)
{

}

void StockEntryEditor::importAllStockItems()
{
    drawer->closeDrawer();
    QSqlQueryModel* m1 = itemsHelper->getInventoryItemsQueryModel(3);

    for(int i=0;i<m1->rowCount();i++){
        CompoundItemDataObject item;
        item.BaseItem = itemsHelper->getInventoryItemByID(m1->record(i).value(0).toString());
        voucher->InventoryItems.append(item);
    }
    resetWidgets();
}

void StockEntryEditor::sendEmail()
{

}

void StockEntryEditor::showVoucher()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    VoucherEditorBase::showVoucher();
    resetWidgets();
    saveButton->setDisabled(true);
}

void StockEntryEditor::showItemDetail(int row)
{
    qDebug()<<Q_FUNC_INFO;
    ItemDetail *itemDetailWid = new ItemDetail(voucher, row,3,itemsHelper,
            uomHelper,new PricelistDatabaseHelper,godownHelper, voucher->priceListId);
    qDebug()<<__LINE__<<Q_FUNC_INFO;
    itemDetailWid->show();

    connect(itemDetailWid, &ItemDetail::updateItem, this, [=](){
        model->setVoucher(voucher);
    });
}
