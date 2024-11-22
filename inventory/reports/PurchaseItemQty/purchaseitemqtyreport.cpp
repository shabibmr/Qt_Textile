#include "purchaseitemqtyreport.h"
#include "ui_purchaseitemqtyreport.h"

PurchaseItemQtyReport::PurchaseItemQtyReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PurchaseItemQtyReport)
{
    ui->setupUi(this);
    itemsHelper = new SalesInventoryItemDatabaseHelper();
    ingHelper = new BillOfMaterialsIngredientsDataBaseHelper();

    itemsModel = itemsHelper->getInventoryItemsQueryModelForSearch(2);
    QPointer<GMItemSearchLite> itemLineLite = new GMItemSearchLite(itemsModel);

    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(itemLineLite);
    SearchWid->setLayout(lay);
    QObject::connect(itemLineLite,SIGNAL(SelectedItemID(inventoryItemDataModel)),
                     this,SLOT(getItemDetails(inventoryItemDataModel)));

    ui->itemNameLay->addWidget(SearchWid);

}

PurchaseItemQtyReport::~PurchaseItemQtyReport()
{
    delete ui;
}

void PurchaseItemQtyReport::getItemDetails(inventoryItemDataModel item)
{
    inventoryItemDataModel ITEM = item;
    qDebug()<<ITEM.ItemName;
    itemID = item.ItemID;
    setTableView();

}

void PurchaseItemQtyReport::setTableView()
{
    QSqlQueryModel* model = ingHelper->getSalesItemByPurchaseItemModel(itemID);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PurchaseItemQtyReport::on_pushButton_clicked()
{
    setTableView();
}
