#include "itemchecklistwidget.h"
#include "ui_itemchecklistwidget.h"

#include <QScreen>

ItemCheckListWidget::ItemCheckListWidget(GeneralVoucherDataObject *voucher, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemCheckListWidget)
{
    ui->setupUi(this);
    qDebug()<<Q_FUNC_INFO<<voucher;
    //    GeneralVoucherDataObject voucher1 = *voucher;
    //    qDebug()<<"voucher obj loc"<<&voucher1;
    //    this->voucher = &voucher1;
    this->voucher = voucher;
    qDebug()<<"voucher ptr "<<this->voucher<<this->voucher->InventoryItems.size();
    this->setMinimumSize(700,600);

    itemHelper = new SalesInventoryItemDatabaseHelper;
    this->setWindowTitle("Item CheckList");
    this->setObjectName("itemCheckList");
    this->setStyleSheet("QWidget#itemCheckList{background-color:" + bgcolor + "}");
    //    this->setWindowFlag(Qt::Popup);
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));
    //    initCheckingList();
    setWidgets();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

ItemCheckListWidget::~ItemCheckListWidget()
{
    delete ui;
}

void ItemCheckListWidget::setVoucher(GeneralVoucherDataObject *value)
{
    this->voucher->InventoryItems = value->InventoryItems;
}

void ItemCheckListWidget::setWidgets()
{

    //    rejectButton->hide();

    QSqlQueryModel* searchmodel = itemHelper->getInventoryItemsQueryModelForSearch();
    itemSearch = new GMItemSearchLite(searchmodel);
    itemSearch->setText("");
    itemSearch->setPlaceholderText("Search Item...");
    QWidget *SearchWid = new QWidget(this);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    QLabel *itemLabel = new QLabel("Item");
    itemLabel->setStyleSheet("QLabel{color:" + textColor + "}");
    lay->addWidget(itemLabel);
    lay->addWidget(itemSearch);
    SearchWid->setLayout(lay);
    SearchWid->setStyleSheet("GMItemSearchLite {color : " + textColor + "}");
    SearchWid->setFixedWidth(200);

    QObject::connect(itemSearch,&GMItemSearchLite::SelectedItemID,
                     this,[=](inventoryItemDataModel itemObj){

        int pos = findItemInVoucher(itemObj.ItemID, itemObj.uomObject._id);
        if(pos > -1){
            voucher->InventoryItems[pos].BaseItem.quantityFull++;
            qDebug()<<"item set";
        }
        else{
            CompoundItemDataObject comp;
            itemObj.quantityFull = 1;
            itemObj.removeItem = true;
            comp.BaseItem = itemObj;
            voucher->InventoryItems.append(comp);
        }

        resetWidgets();
    });

    ui->verticalLayout->addWidget(SearchWid);


    tableView = new QTableView;
    tableView->setObjectName("EditInvTable");
    tableView->setStyleSheet(

                "QTableView::item{"
                "gridline-color : #ffffff;"
                "border : 0px solid #fffff8;"
                "}"
                "QTableView::item:selected{"
                "color:#00acc1;"
                "background-color:#e0f7fa;"
                "}"
                "QTableView#EditInvTable{"
                "background-color: #ffffff;"
                "color: #006064;"
                "border : none;"
                "/*selection-background-color: #999*/}"
                "QHeaderView::section:horizontal"
                "{"
                "background-color: #00bcd4;"
                "color: white;"
                "border: 1px solid #ffffff;"
                "}"
                );
    tableView->setItemDelegate(new QStyledItemDelegate);

    qDebug()<<"model start"<<Q_FUNC_INFO;

    model = new GMItemCheckListModel(voucher);
    connect(model, &GMItemCheckListModel::checkQtyUpdated, this, &ItemCheckListWidget::resetWidgets);
    tableView->setModel(model);
    qDebug()<<__LINE__<<"model init done";
    tableView->setColumnWidth(0,10);
    tableView->setColumnWidth(2,20);
    tableView->setColumnWidth(3,20);
    tableView->setColumnWidth(4,20);
    tableView->setColumnWidth(5,10);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    //    tableView->setColumns();

    ui->verticalLayout->addWidget(tableView);

    qDebug()<<__LINE__<<"tableview done";

    //    okButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "check_circle"));
    //    okButton->setMini(true);
    //    okButton->setXOffset(10);
    //    okButton->setYOffset(10);
    //    okButton->setParent(this);
    //    okButton->setToolTip("Check OK");

    //    connect(okButton, &QPushButton::clicked, this, [=](){
    //        removeItems();
    //       emit checkCompleted(true) ;
    //        this->close();
    //    });

    //    rejectButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("navigation", "cancel"));
    //    rejectButton->setMini(true);
    //    rejectButton->setXOffset(70);
    //    rejectButton->setYOffset(10);
    //    rejectButton->setParent(this);
    //    rejectButton->setToolTip("Reject");
    //    connect(rejectButton, &QPushButton::clicked, this, [=](){
    //        removeItems();
    //       emit checkCompleted(false) ;
    //        this->close();
    //    });

    saveButton = new QtMaterialFlatButton("Save");
    saveButton->setIcon(QtMaterialTheme::icon("content", "save"));
    saveButton->setForegroundColor(QColor("#00bcd4"));
    saveButton->setFontSize(10);
    saveButton->setTextAlignment(Qt::AlignLeft);
    saveButton->setIconSize(QSize(20,20));

    ui->verticalLayout->addWidget(saveButton, 1, Qt::AlignRight);

    connect(saveButton, &QPushButton::clicked, this, [=](){
        removeItems();
        emit checkCompleted(true);
        this->close();
    });

    if(!ConfigurationSettingsDatabaseHelper::getValue(allowSaveWithoutChecklist,true).toBool())
        saveButton->hide();
    //    ui->verticalLayout->addWidget(okButton);

    qDebug()<<__LINE__<<"end of"<<Q_FUNC_INFO;

}

void ItemCheckListWidget::resetWidgets()
{
    qDebug()<<Q_FUNC_INFO;
    delete model;

    model = new GMItemCheckListModel(voucher);
    connect(model, &GMItemCheckListModel::checkQtyUpdated, this, &ItemCheckListWidget::resetWidgets);
    tableView->setModel(model);
    //    tableView->setAvlcols(QList<int>{3,4});
    //    tableView->setColumns();

    itemSearch->clear();
    itemSearch->setPlaceholderText("Search Item...");

    if(!ConfigurationSettingsDatabaseHelper::getValue(allowSaveWithoutChecklist,true).toBool()){
        qDebug()<<"save hidden";
        if(checklistOk()){
            qDebug()<<"check ok";
            saveButton->show();
        }
        else {
            saveButton->hide();
        }
    }

}

int ItemCheckListWidget::findItemInVoucher(QString itemID, QString uom)
{
    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->InventoryItems[i].BaseItem.ItemID == itemID &&
                voucher->InventoryItems[i].BaseItem.uomObject._id == uom){
            qDebug()<<"found item at "<<i;
            return i;
        }
    }

    return -1;
}


void ItemCheckListWidget::removeItems()
{
    qDebug()<<Q_FUNC_INFO;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->InventoryItems[i].BaseItem.removeItem ){
            qDebug()<<"removing item "<<voucher->InventoryItems[i].BaseItem.ItemName<<voucher->InventoryItems[i].BaseItem.uomObject.UomName<<voucher->InventoryItems[i].BaseItem.quantityFull;
            voucher->InventoryItems.removeAt(i);
        }
    }
}

bool ItemCheckListWidget::checklistOk()
{
    qDebug()<<Q_FUNC_INFO;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        if(voucher->InventoryItems[i].BaseItem.quantity != voucher->InventoryItems[i].BaseItem.quantityFull )
            return false;
    }
    return true;
}


