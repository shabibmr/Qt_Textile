#include "itemtrasactionlist.h"
#include "ui_itemtrasactionlist.h"
#include <QDate>
#include "voucherOperations/vouchereditorselector.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
ItemTrasactionList::ItemTrasactionList(QString itemID, SalesInventoryItemDatabaseHelper *itemhelper, InventoryHelper *invHelper, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ItemTrasactionList)
{
    ui->setupUi(this);

    tableView = new ReportsTableView("Total");
    tableView->setItemDelegate(new ReportCommonDelegate);
    ui->tableLayout->addWidget(tableView);
    connect(tableView, &ReportsTableView::doubleClicked, this, &ItemTrasactionList::on_tableView_doubleClicked);

    this->invHelper=invHelper;
    itemHelper = itemhelper;
    item=itemHelper->getInventoryItemByIDasPtr(itemID);
    setWindowTitle("Transactions");
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());

    ui->fromDateEdit->setDisabled(true);
    ui->toDateEdit->setDisabled(true);

    tableView->setItemDelegate(new ReportCommonDelegate());

}

ItemTrasactionList::~ItemTrasactionList()
{
    delete ui;
}

void ItemTrasactionList::setValue()
{
    if(voucherType.length()==0)
        voucherType="%";
    model= invHelper->getItemCrDrBetDate(item->ItemID,
                                         ui->fromDateEdit->date(),
                                         ui->toDateEdit->date(),
                                         voucherType,
                                         godownID);
    ui->itemName->setText(item->ItemName);
    tableView->setModel(model);
    tableView->hideColumn(0);
}

void ItemTrasactionList::setDateFilters(QDate fromDate, QDate toDate)
{
    ui->fromDateEdit->setDate(fromDate);
    ui->toDateEdit->setDate(toDate);
}

void ItemTrasactionList::setVoucherTypeFilter(QString voucher_Type)
{
    voucherType = voucher_Type;
}

void ItemTrasactionList::setGodownFilter(QString godown_ID)
{
    godownID= godown_ID;
}

void ItemTrasactionList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    VoucherEditorSelector* vSel = new VoucherEditorSelector();
    QString vno = model->record(row).value(1).toString();
    QString vpref = model->record(row).value(0).toString();
    QString vType = model->record(row).value(2).toString();
    vSel->openEditor(vno,vpref,vType);
}
