#include "addpricelist.h"
#include "ui_addpricelist.h"
#include <QComboBox>
#include <QCompleter>
#include  "customwidgets/gmsearchlineedit.h"
#include <QIcon>
#include <QDesktopWidget>

addPriceList::addPriceList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addPriceList)
{

    ui->setupUi(this);

    QString bgcolor = "#e0f7fa";
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:" + bgcolor + "}");
    dbHelper = new PricelistDatabaseHelper();
    uomHelper = new UomDataBaseHelper();
    priceList = new PriceListDataObject();
    itemsHelper = new SalesInventoryItemDatabaseHelper();
    setWidgets();
    ui->addItemsPushButton->hide();
    ui->dateWidget->hide();
}

addPriceList::~addPriceList()
{
    delete ui;
}

void addPriceList::setId(int value)
{
    id = value;
}

void addPriceList::setItemPrice(QString itemId, float price)
{
    inventoryItemDataModel Item = itemsHelper->getInventoryItemByID(itemId);
    Item.price = price;

    priceList->items.append(Item);

}

void addPriceList::savePriceList()
{
    //    PriceListDataObject *priceList = new PriceListDataObject();

    priceList->priceListID = id;

    priceList->priceListName = ui->priceListNameLineEdit->text();
    if(ui->basedOnDate->isChecked()){
        priceList->dateFrom = ui->fromDate->date();
        priceList->dateto = ui->toDate->date();
    }
    else{
        priceList->dateFrom = QDate::fromString("0000:00:00","yyyy-MM-dd");
        priceList->dateto= QDate::fromString("0000:00:00","yyyy-MM-dd");
    }

    if(id == 0){
        dbHelper->insertPriceList(priceList);
    }
    else{
        dbHelper->updatePriceList(priceList,id);
    }
    this->close();
    emit closing();
}

void addPriceList::disableSave()
{
    ui->buttonBox->hide();
}

void addPriceList::setWidgets()
{
    lis<<"#"<<"Particulars"<<"UOM"<<"Price"<<"";
    DataTableWidget = new PriceListTableView(priceList);
    DataTableWidget->setParent(this);
    DataTableWidget->setItemDelegate(new PriceListDelegate(
                                         itemsHelper,itemsHelper->getInventoryItemsQueryModel(),uomHelper));
    model = new PriceListModel(priceList,lis.size(),this);
    model->setHeaderStrings(lis);

    sort = new PriceListSortModel(this);
    sort->setSourceModel(model);
    sort->setDynamicSortFilter(true);

    DataTableWidget->setModel(sort);
    DataTableWidget->setColumns();
    ui->tableLay->addWidget(DataTableWidget);

    ui->showInPos->setChecked(true);
}


void addPriceList::on_buttonBox_accepted()
{

    //    PriceListDataObject *priceList = new PriceListDataObject();

    priceList->priceListID = id;

    priceList->priceListName = ui->priceListNameLineEdit->text();
    if(ui->basedOnDate->isChecked()){
        priceList->dateFrom = ui->fromDate->date();
        priceList->dateto = ui->toDate->date();
    }
    else{
        priceList->dateFrom = QDate::fromString("0000:00:00","yyyy-MM-dd");
        priceList->dateto= QDate::fromString("0000:00:00","yyyy-MM-dd");
    }
    priceList->isDefault = ui->checkBox->isChecked();
    priceList->showinPOS = ui->showInPos->isChecked();

    if(id == 0){
        dbHelper->insertPriceList(priceList);
    }
    else{
        dbHelper->updatePriceList(priceList,id);
    }
    this->close();
    emit closing();

}

void addPriceList::on_buttonBox_rejected()
{
    this->close();
}

void addPriceList::edit(PriceListDataObject *obj)
{
    delete  priceList;
    priceList = new PriceListDataObject();
    priceList = obj;
    model = new PriceListModel(priceList,lis.size(),this);
    model->setHeaderStrings(lis);
    sort->setSourceModel(model);

    sort->setDynamicSortFilter(true);
    DataTableWidget->setPriceList(priceList);
    DataTableWidget->setModel(sort);

    //    DataTableWidget->setSortingEnabled(true);

    DataTableWidget->setColumns();
    ui->checkBox->setChecked(priceList->isDefault);
    ui->showInPos->setChecked(priceList->showinPOS);
}

void addPriceList::setTitle(QString text)
{
    ui->titleLabel->setText(text);
}

void addPriceList::setPriceName(QString name)
{
    ui->priceListNameLineEdit->setText(name);
}


void addPriceList::on_basedOnDate_toggled(bool checked)
{
    if(checked == true){
        ui->datesWidget->setHidden(false);
    }
    else{
        ui->datesWidget->setHidden(true);
    }
}

void addPriceList::on_searchLine_textChanged(const QString &arg1)
{
    qDebug()<<"Price : "<<arg1;
    QRegExp regExp(arg1);
    sort->setFilterRegExp(regExp);
}
