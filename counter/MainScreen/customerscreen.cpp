#include "customerscreen.h"
#include "ui_customerscreen.h"

CustomerScreen::CustomerScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomerScreen)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:#e0f7fa");

    settings = new SettingsHelper();

    ui->vat5LineEdit->setAlignment(Qt::AlignRight);
    ui->subTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->totalLineEdit->setAlignment(Qt::AlignRight);
    ui->discountLineEdit->setAlignment(Qt::AlignRight);

    ui->discountLabel->hide();
    ui->discountLineEdit->hide();

    ui->tableWidget->verticalHeader()->hide();

    ui->tableWidget->setColumnCount(4);

    ui->tableWidget->verticalScrollBar()->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
                                                        " stop:1 rgba(85, 60, 56, 255));");

    int fntSize =  ConfigurationSettingsDatabaseHelper::getValue(customerscreenfontsize,12).toInt();
    fnt.setPointSize(fntSize);

    int i = 0;

    QPixmap pixmap;
    pixmap.loadFromData(LoginValues::getCompany().logo);
    ui->logoLabel->setPixmap(pixmap);

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

    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    Item->setFont(fnt);
    qty->setFont(fnt);
    price->setFont(fnt);
    total->setFont(fnt);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    int x = settings->getValue("CustomerScreenX",this->geometry().x()).toInt();
    int y = settings->getValue("CustomerScreenY",this->geometry().y()).toInt();

    this->move(x,y);

}

CustomerScreen::~CustomerScreen()
{
    delete ui;
}

void CustomerScreen::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
}

void CustomerScreen::setTable()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(voucher->InventoryItems.length());
    int rowCount =0;
    for(int i =0;i<voucher->InventoryItems.length();i++){
        if(voucher->InventoryItems[i].BaseItem.hideInPrint)
            continue;
        ui->tableWidget->setRowHeight(rowCount,30);
        QTableWidgetItem* itemName = new QTableWidgetItem();
        QTableWidgetItem* Qty = new QTableWidgetItem();
        QTableWidgetItem* Price = new QTableWidgetItem();
        QTableWidgetItem* total = new QTableWidgetItem();



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

        itemName->setFont(fnt);
        Qty->setFont(fnt);
        Price->setFont(fnt);
        total->setFont(fnt);



        ui->tableWidget->setItem(i,0,itemName);
        ui->tableWidget->setItem(i,1,Qty);
        ui->tableWidget->setItem(i,2,Price);
        ui->tableWidget->setItem(i,3,total);

        rowCount++;


    }


    ui->subTotalLineEdit   -> setText(QString::number(voucher->subTotal,'f',2));
    ui->vat5LineEdit     -> setText(QString::number(voucher->taxTotalAmount,'f',2));
    ui->totalLineEdit ->setText(QString::number(voucher->grandTotal,'f',2));
    ui->discountLineEdit  -> setText(QString::number(voucher->discountinAmount,'f',2));

    ui->subTotalLineEdit->setFont(fnt);
    ui->subTotalLabel->setFont(fnt);
    ui->discountLabel->setFont(fnt);
    ui->discountLineEdit->setFont(fnt);
    ui->totalLineEdit->setFont(fnt);
    ui->grandTotalLabel->setFont(fnt);

    ui->vat5Label->setFont(fnt);
    ui->vat5LineEdit->setFont(fnt);

    if(voucher->discountinAmount>0){
        ui->discountLabel->show();
        ui->discountLineEdit->show();
    }
    else{
        ui->discountLabel->hide();
        ui->discountLineEdit->hide();
    }

    ui->tableWidget->scrollToBottom();
}



void CustomerScreen::refreshScreen()
{
    ui->subTotalLineEdit->clear();
    ui->vat5LineEdit->clear();
    ui->totalLineEdit->clear();
    ui->tableWidget->setRowCount(0);
}

void CustomerScreen::closeEvent(QCloseEvent *event)
{
    settings->setValue("CustomerScreenX",this->geometry().x());
    settings->setValue("CustomerScreenY",this->geometry().y());

    QWidget::closeEvent(event);
}
