#include "godownallocationwidget.h"
#include "ui_godownallocationwidget.h"

GodownAllocationWidget::GodownAllocationWidget(GeneralVoucherDataObject *voucher, int row, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GodownAllocationWidget)
{
    ui->setupUi(this);
    this->Item = &voucher->InventoryItems[row];
    this->itemRow = row;
    this->voucher = voucher;
    this->transactionType = GMVoucherTypes::getTransactionType
            (GMVoucherTypes::getVoucherIdFromString(voucher->voucherType));
    qDebug()<<Q_FUNC_INFO<<transactionType<<voucher->voucherType;

    this->setWindowTitle("Godown Allocation");
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:#e0f7fa}");
    QPalette pal = palette();

    // set black background
    pal.setColor(QPalette::Background,QColor("#e0f7fa"));
    setAutoFillBackground(true);
    setPalette(pal);

    dbHelper = new ClosingStockHelper;
    fifoCalc = new FifoCalculator;

    setWidgets();
    setTable();
}

GodownAllocationWidget::~GodownAllocationWidget()
{
    delete ui;
}

void GodownAllocationWidget::setTable()
{
    qDebug()<<Q_FUNC_INFO;
    allGodownModel = dbHelper->getGodownwiseStockOfItem(Item->BaseItem.ItemID, voucher->VoucherDate);

    int i;

    godownTable->setRowCount(0);

    for(int row=0; row<allGodownModel->rowCount(); row++){
        godownTable->insertRow(row);
        i=0;

        QTableWidgetItem* Godown = new QTableWidgetItem;
        Godown->setText(allGodownModel->record(row).value(1).toString());
        Godown->setFlags(Godown->flags() ^ Qt::ItemIsEditable);
        godownTable->setItem(row,i++,Godown);

        QTableWidgetItem* stock = new QTableWidgetItem;
        float stockVal = allGodownModel->record(row).value(2).toFloat() * Item->BaseItem.uomObject.convRate;
        stock->setText(QString::number(stockVal, 'f',2));
        stock->setFlags(Godown->flags() ^ Qt::ItemIsEditable);
        godownTable->setItem(row,i++,stock);

        QLineEdit *qtySelected = new QLineEdit;
        qtySelected->setValidator(new QDoubleValidator(0.0000,999999,2,this));
        qtySelected->setFrame(QFrame::NoFrame);
        qtySelected->setProperty("row",row);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<qtySelected;
        godownTable->setCellWidget(row,i++,qtySelected);
        connect(qtySelected, &QLineEdit::returnPressed, this, &GodownAllocationWidget::setGodownAllocation);

        if(Item->BaseItem.godownList.size()>0){
            qDebug()<<"showing godown entry : "<<Item->BaseItem.godownList.size();
            float qty = getSetQtyFromFifo(row);
            qDebug()<<"of row"<<row<<qty;
            qtySelected->setText(QString::number(qty,'f',2));
        }

    }
    calcTotal();
}

void GodownAllocationWidget::setWidgets()
{
    godownTable = new QTableWidget;
    godownTable->setFixedHeight(300);
    godownTable->setStyleSheet(
                "QTableWidget::item{"
                "gridline-color : #ffffff;"
                "border: 0px solid #fffff8;"
                "}"
                "QTableWidget::item:selected{"
                "color:#00acc1;"
                "background-color:#e0f7fa;"
                "}"
                "QTableWidget{"
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

    godownTable->setColumnCount(3);

    int i=0;
    QTableWidgetItem *Godown = new QTableWidgetItem;
    Godown->setText("Godown");
    godownTable->setHorizontalHeaderItem(i++,Godown);


    QTableWidgetItem* stock = new QTableWidgetItem;
    stock->setText("Stock");
    godownTable->setHorizontalHeaderItem(i++,stock);


    QTableWidgetItem* qty = new QTableWidgetItem;
    qty->setText("Quantity");
    godownTable->setHorizontalHeaderItem(i++,qty);

    godownTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    godownTable->verticalHeader()->hide();


    GMMaterialNumberEdit *voucherQty = new GMMaterialNumberEdit(&Item->BaseItem.quantity);
    voucherQty->setLabel("Quantity in Voucher");
    voucherQty->setReadOnly(true);

    selectedQty = new GMMaterialNumberEdit(&totalQtySelected);
    selectedQty->setLabel("Quantity Selected");
    selectedQty->setReadOnly(true);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(voucherQty);
    layout->addWidget(selectedQty);

    ui->verticalLayout->addWidget(godownTable);
    ui->verticalLayout->addStretch(1);
    ui->verticalLayout->addLayout(layout);


}

void GodownAllocationWidget::setGodownAllocation()
{
    qDebug()<<Q_FUNC_INFO;

    int row= sender()->property("row").toInt();
    bool updatedGodownEntry = false;

    float quantity = qobject_cast<QLineEdit*>(godownTable->cellWidget(row,2))->text().toFloat();
    qDebug()<<"Entered : "<<quantity;
    calcTotal();
    if(totalQtySelected > Item->BaseItem.quantity){
        quantity = Item->BaseItem.quantity - (totalQtySelected - quantity);
        calcTotal();
    }

    qDebug()<<"Updated qty "<<quantity;

    // below loop checks if godown entry already added for the godown

    for(int i=0; i< Item->BaseItem.godownList.size(); i++)
    {
        qDebug()<<"compare left"<<allGodownModel->record(row).value(0).toString()<<"right"<<Item->BaseItem.godownList[i].fromGodown<<Item->BaseItem.godownList[i].toGodown;
        if(Item->BaseItem.godownList[i].fromGodown == allGodownModel->record(row).value(0).toString())
        {
            qDebug()<<"mtched from godown";
            if(quantity > 0){
                qDebug()<<"change "<<Item->BaseItem.godownList[i].crQty;
                Item->BaseItem.godownList[i].crQty = quantity;
                qDebug()<<"to "<<Item->BaseItem.godownList[i].crQty;
                fifoCalc->setItemQtyByFifo(voucher, itemRow, i);
            }
            else {
                Item->BaseItem.godownList.removeAt(i);
            }
            updatedGodownEntry = true;
        }
        else if(Item->BaseItem.godownList[i].toGodown == allGodownModel->record(row).value(0).toString())
        {
            qDebug()<<"mtched to godown";
            if(quantity > 0){
                qDebug()<<"change "<<Item->BaseItem.godownList[i].drQty;
                Item->BaseItem.godownList[i].drQty = quantity;
                qDebug()<<"to "<<Item->BaseItem.godownList[i].drQty;
                qDebug()<<voucher->InventoryItems[itemRow].BaseItem.ItemName;
                fifoCalc->setItemQtyByFifo(voucher, itemRow, i);
            }
            else {
                Item->BaseItem.godownList.removeAt(i);
            }
            updatedGodownEntry = true;
        }
    }

    qDebug()<<"godown updated? "<<updatedGodownEntry;

    if(!updatedGodownEntry){
        qDebug()<<"adding new godown entry. current size is"<<Item->BaseItem.godownList.size()<<quantity
               <<transactionType;
        if(quantity >= 0){
            GodownTransactionDataModel godownObj ;
            switch (transactionType) {
            case 1:
            {
                godownObj.fromGodown = allGodownModel->record(row).value(0).toString();
                godownObj.toGodown = "OUT";
                godownObj.crQty = quantity;
                break;
            }
            case 2:
            {
                godownObj.fromGodown = "OUT";
                godownObj.toGodown = allGodownModel->record(row).value(0).toString();
                godownObj.drQty = quantity;
                break;
            }
            case 3:
            {
                qDebug()<<"stock journal"<<Item->BaseItem.drQty<<Item->BaseItem.crQty;
                if(Item->BaseItem.drQty > 0){
                    qDebug()<<"add stock";
                    godownObj.fromGodown = "OUT";
                    godownObj.toGodown = allGodownModel->record(row).value(0).toString();
                    godownObj.drQty = quantity;
                    break;
                }
                else if(Item->BaseItem.crQty > 0){
                    qDebug()<<"reduce stock";
                    godownObj.fromGodown = allGodownModel->record(row).value(0).toString();
                    godownObj.toGodown = "OUT";
                    godownObj.crQty = quantity;
                    break;
                }
                break;
            }
            case 4:
            {

                qDebug()<<Q_FUNC_INFO<<__LINE__<<"transa type 4 ";
                float diff = allGodownModel->record(row).value(2).toFloat() - quantity;
                godownObj.enteredQty = quantity;
                qDebug()<<"stock adj is"<<diff<<godownObj.enteredQty;
                if(diff > 0){

                    godownObj.fromGodown = allGodownModel->record(row).value(0).toString();
                    godownObj.toGodown = "OUT";
                    godownObj.crQty = diff;


                }
                else{
                    godownObj.toGodown = allGodownModel->record(row).value(0).toString();
                    godownObj.fromGodown = "OUT";
                    godownObj.drQty = -1 * diff;

                }
                qDebug()<<"cr/dr set"<<godownObj.crQty<<godownObj.drQty;
                //                if(Item->BaseItem.drQty > 0){
                //                    qDebug()<<"add stock";
                //                    godownObj.fromGodown = "OUT";
                //                    godownObj.toGodown = allGodownModel->record(row).value(0).toString();
                //                    godownObj.drQty = quantity;
                //                    break;
                //                }
                //                else if(Item->BaseItem.crQty > 0){
                //                    qDebug()<<"reduce stock";
                //                    godownObj.fromGodown = allGodownModel->record(row).value(0).toString();
                //                    godownObj.toGodown = "OUT";
                //                    godownObj.crQty = quantity;
                //                    break;
                //                }
            }
            }
            qDebug()<<"new entry is"<<godownObj.fromGodown<<godownObj.toGodown<<godownObj.crQty<<godownObj.drQty;

            Item->BaseItem.godownList.append(godownObj);
            qDebug()<<Item->BaseItem.godownList.size();
            qDebug()<<voucher->InventoryItems[this->itemRow].BaseItem.ItemName;
            fifoCalc->setItemQtyByFifo(voucher, this->itemRow, Item->BaseItem.godownList.size()-1);
        }

    }

    godownTable->setFocus(Qt::NoFocusReason);

    setTable();

}

float GodownAllocationWidget::getSetQtyFromFifo(int row)
{

    switch (transactionType) {
    case 1:{
        for(int i=0; i<Item->BaseItem.godownList.size();i++){
            if(Item->BaseItem.godownList[i].fromGodown == allGodownModel->record(row).value(0).toString()){
                // uncomment if need to restrict sale qty by stock
                //                if(Item->BaseItem.godownList[i].crQty > allGodownModel->record(row).value(2).toFloat()){
                //                    Item->BaseItem.godownList[i].crQty = allGodownModel->record(row).value(2).toFloat();
                //                    fifoCalc->setItemQtyByFifo(voucher, itemRow, i);
                //                }
                return Item->BaseItem.godownList[i].crQty;
            }

        }
        break;
    }
    case 2:{
        qDebug()<<"case 2";
        for(int i=0; i<Item->BaseItem.godownList.size();i++){
            qDebug()<<"each entry"<<Item->BaseItem.godownList[i].toGodown<<Item->BaseItem.godownList[i].drQty;
            if(Item->BaseItem.godownList[i].toGodown == allGodownModel->record(row).value(0).toString()) {
                return Item->BaseItem.godownList[i].drQty;
            }
        }
        break;
    }

    case 3:{
        if(Item->BaseItem.drQty > 0 ){
            for(int i=0; i<Item->BaseItem.godownList.size();i++){
                if(Item->BaseItem.godownList[i].toGodown == allGodownModel->record(row).value(0).toString()) {
                    return Item->BaseItem.godownList[i].drQty;
                }
            }
            break;
        }
        else if(Item->BaseItem.crQty > 0 ){
            for(int i=0; i<Item->BaseItem.godownList.size();i++){
                if(Item->BaseItem.godownList[i].fromGodown == allGodownModel->record(row).value(0).toString()){
                    return Item->BaseItem.godownList[i].crQty;
                }
            }
            break;
        }
    }
        break;
    case 4:{
        qDebug()<<Q_FUNC_INFO<<__LINE__<<Item->BaseItem.drQty<<Item->BaseItem.crQty;

        for(int i=0; i<Item->BaseItem.godownList.size();i++){
            if(Item->BaseItem.godownList[i].toGodown == allGodownModel->record(row).value(0).toString()) {
                qDebug()<<"returning entered qty"<<Item->BaseItem.godownList[i].enteredQty;
                return Item->BaseItem.godownList[i].enteredQty;
            }
            else if(Item->BaseItem.godownList[i].fromGodown == allGodownModel->record(row).value(0).toString()) {
                qDebug()<<"returning entered qty"<<Item->BaseItem.godownList[i].enteredQty;
                return Item->BaseItem.godownList[i].enteredQty;
            }
        }
        break;

    }

    }

    return 0;
}

void GodownAllocationWidget::calcTotal()
{
    qDebug()<<Q_FUNC_INFO;
    totalQtySelected = 0;

    for(int i=0;i<allGodownModel->rowCount();i++){
        totalQtySelected += qobject_cast<QLineEdit*>(godownTable->cellWidget(i,2))->text().toFloat();
    }

    selectedQty->setVariable(&totalQtySelected);
}


