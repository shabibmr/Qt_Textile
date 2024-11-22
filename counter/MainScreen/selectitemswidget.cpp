#include "selectitemswidget.h"
#include "ui_selectitemswidget.h"

SelectItemsWidget::SelectItemsWidget(GeneralVoucherDataObject *voucher,
                                     QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectItemsWidget)
{
    ui->setupUi(this);

    this->voucher = voucher;

    ui->tableWidget->setColumnCount(4);

    int i = 0;
    QTableWidgetItem* Item = new QTableWidgetItem;
    Item->setText("Item Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,Item);

    QTableWidgetItem* qty = new QTableWidgetItem;
    qty->setText("Qty");
    ui->tableWidget->setHorizontalHeaderItem(i++,qty);

    QTableWidgetItem* price = new QTableWidgetItem;
    price->setText("Price");
    ui->tableWidget->setHorizontalHeaderItem(i++,price);

    QTableWidgetItem* total = new QTableWidgetItem;
    total->setText("Total");
    ui->tableWidget->setHorizontalHeaderItem(i++,total);

    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,50);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    settable();

}

SelectItemsWidget::~SelectItemsWidget()
{
    delete ui;
}

void SelectItemsWidget::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
}

GeneralVoucherDataObject *SelectItemsWidget::getVoucher() const
{
    return voucher;
}

void SelectItemsWidget::settable()
{

    qDebug()<<"Calc Total Completer";
    int rowCount =0;
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(voucher->InventoryItems.length());

    for(int i =0;i<voucher->InventoryItems.length();i++){
        ui->tableWidget->setRowHeight(rowCount,30);
        QTableWidgetItem* itemName = new QTableWidgetItem();
        QTableWidgetItem* Qty = new QTableWidgetItem();
        QTableWidgetItem* Price = new QTableWidgetItem();
        QTableWidgetItem* total = new QTableWidgetItem();
        QToolButton* edit = new QToolButton;

        edit->setProperty("row",i);
        QObject::connect(edit,SIGNAL(clicked(bool)),this,SLOT(openRestaurantItemDetail()));

        itemName->setText(voucher->InventoryItems[i].BaseItem.ItemName);
        Qty->setText(QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2));
        Price->setText(QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2));
        total->setText(QString::number(voucher->InventoryItems[i].BaseItem.subTotal,'f',2));

        Qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        Price->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        total->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

        itemName->setFlags(itemName->flags() ^ Qt::ItemIsEditable);
        Qty->setFlags(Qty->flags() ^ Qt::ItemIsEditable);
        Price->setFlags(Price->flags() ^ Qt::ItemIsEditable);
        total->setFlags(total->flags() ^ Qt::ItemIsEditable);

        ui->tableWidget->setItem(i,0,itemName);
        ui->tableWidget->setItem(i,1,Qty);
        ui->tableWidget->setItem(i,2,Price);
        ui->tableWidget->setItem(i,3,total);

        rowCount++;
    }

}

void SelectItemsWidget::on_cancelButton_clicked()
{
    this->close();
}

void SelectItemsWidget::on_okButton_clicked()
{
    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();
    QList<int> rows;
    // Multiple rows can be selected
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        //qDebug() << index.row();
        rows<<index.row();
    }
    emit selectedRows(rows);
    this->close();
}

void SelectItemsWidget::on_tableWidget_cellClicked(int row, int column)
{
    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();

    float total = 0;
    for(int i=0; i< selection.count(); i++)
    {
        QModelIndex index = selection.at(i);
        total += voucher->InventoryItems[index.row()].BaseItem.subTotal;
    }
    ui->lineEdit->setText(QString::number(total,'f',2));
}
