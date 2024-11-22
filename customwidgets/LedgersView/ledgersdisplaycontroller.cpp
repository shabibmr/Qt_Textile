#include "ledgersdisplaycontroller.h"
#include "ui_ledgersdisplaycontroller.h"

LedgersDisplayController::LedgersDisplayController(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LedgersDisplayController)
{
    ui->setupUi(this);

    this->showMaximized();

    defaultHeader<<"Ledger"<<"Debit"<<"Credit";

    voucher = new GeneralVoucherDataObject();

    crAmount = new QLineEdit();
    drAmount = new QLineEdit();

    setTableHeaders(defaultHeader);

    uisetups();

    ledHelper = new LedgerMasterDatabaseHelper();

    ledModel = ledHelper->getLedgerQueryString();

    ledgerLine = new GMLedgerSearchLite(ledModel);

    mainLed =  new GMLedgerSearchLite(ledModel);

    ui->tableWidget->setFrameStyle(QFrame::NoFrame);

    ui->tableWidget->installEventFilter(this);

    setTable();

    ui->voucherDate->setFocus();
}

void LedgersDisplayController::uisetups()
{
    ui->narrationPlainText->setMaximumHeight(100);

    ui->voucherNoGroupBox->setFlat(false);

    ui->voucherDate->setDate(QDate::currentDate());


    //    QWidget *SearchWid = new QWidget(this);
    //    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    //    lay->setContentsMargins(0,0,0,0);
    //    lay->setSpacing(0);
    //    lay->addWidget(mainLed);
    //    SearchWid->setLayout(lay);

    //    QObject::connect(mainLed,SIGNAL(selectedLedger(LedgerMasterDataModel)),
    //                     this,SLOT(setLedgerDetails(LedgerMasterDataModel)));

    //    ui->mailLedLayout->addWidget(SearchWid);

}

LedgersDisplayController::~LedgersDisplayController()
{
    delete ui;
}

void LedgersDisplayController::calcTotal()
{

}

void LedgersDisplayController::setTable()
{
    qDebug()<<"setTable() called";
    float crTotal =0;
    float drTotal=0;
    ui->tableWidget->setRowCount(voucher->ledgersList.size()+3);
    int i=0;
    for(;i<voucher->ledgersList.size();i++){
        QTableWidgetItem *text1 = new QTableWidgetItem;
        QTableWidgetItem *text2 = new QTableWidgetItem;
        QTableWidgetItem *text3 = new QTableWidgetItem;
        QToolButton* editButton = new QToolButton;
        QToolButton* deleteButton = new QToolButton;

        editButton->setProperty("row",i);
        deleteButton->setProperty("row",i);

        QObject::connect(editButton,SIGNAL(clicked(bool)),this,SLOT(openItemDetail()));
        QObject::connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteLedgerRow()));


        text1->setFlags(text1->flags() ^ Qt::ItemIsEditable);
        text2->setFlags(text2->flags() ^ Qt::ItemIsEditable);
        text3->setFlags(text3->flags() ^ Qt::ItemIsEditable);

        text1->setText(voucher->ledgersList[i].LedgerName);
        text2->setText(QString::number(voucher->ledgersList[i].drAmount,'f',currencyDecimal));
        text3->setText(QString::number(voucher->ledgersList[i].crAmount,'f',currencyDecimal));


        crTotal += voucher->ledgersList[i].crAmount;
        drTotal += voucher->ledgersList[i].drAmount;

        editButton->setIcon(QIcon(":/images/edit.png"));
        editButton->setAutoRaise(true);

        deleteButton->setIcon(QIcon(":/icons/trash.ico"));
        deleteButton->setAutoRaise(true);


        ui->tableWidget->setItem(i,0,text1);
        ui->tableWidget->setItem(i,1,text2);
        ui->tableWidget->setItem(i,2,text3);

    }
    // row #i wil be the next entry row
    // row #i+1 will be a blank big row
    //row #i+2 will have totoals
    ui->tableWidget->setRowHeight(i+1,200);

    if(voucher->ledgersList.size()>0){
        QTableWidgetItem *text1 = new QTableWidgetItem;
        QTableWidgetItem *text2 = new QTableWidgetItem;
        QTableWidgetItem *text3 = new QTableWidgetItem;

        text1->setFlags(text1->flags() ^ Qt::ItemIsEditable);
        text2->setFlags(text2->flags() ^ Qt::ItemIsEditable);
        text3->setFlags(text3->flags() ^ Qt::ItemIsEditable);

        text1->setText("Grand Total");
        text2->setText(QString::number(drTotal,'f',currencyDecimal));
        text3->setText(QString::number(crTotal,'f',currencyDecimal));

        ui->tableWidget->setItem(i+2,0,text1);
        ui->tableWidget->setItem(i+2,1,text2);
        ui->tableWidget->setItem(i+2,2,text3);
    }

    if(!selectByClick){
        focusRow = i;
        focusColumn = 0;
    }

    selectByClick = false;

    createEditableRow();
}

void LedgersDisplayController::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;

    focusRow = voucher->ledgersList.size();

    setTable();
}

void LedgersDisplayController::setTableHeaders(QStringList headers)
{

    this->columnCount = headers.count()+2;
    ui->tableWidget->setColumnCount(this->columnCount);
    for(int i=0;i<headers.count();i++){
        QTableWidgetItem *headerText = new QTableWidgetItem;
        headerText->setText(headers[i]);
        ui->tableWidget->setHorizontalHeaderItem(i,headerText);
        ui->tableWidget->setColumnWidth(i,150);
    }

    QTableWidgetItem* editItem = new QTableWidgetItem;
    editItem->setIcon(QIcon(":/images/edit.png"));
    editItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setHorizontalHeaderItem(columnCount-2,editItem);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    deleteItem->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setHorizontalHeaderItem(columnCount-1,deleteItem);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(columnCount-1,50);
    ui->tableWidget->setColumnWidth(columnCount-2,50);

}

void LedgersDisplayController::setFocusRowColumn(int row, int column)
{
    this->focusRow        = row;
    this->focusColumn     = column;
}

void LedgersDisplayController::setnextFocus()
{
    int newRow = 0;
    int newCol = 0;

    // DO SOMETHING

    setFocusRowColumn(newRow,newCol);
}

void LedgersDisplayController::createEditableRow()
{
    qDebug()<<"Create Editable Row";
    qDebug()<<"At Row = "<<focusRow<<" Column = "<<focusColumn;
    if(focusColumn >= defaultHeader.size()){
        focusColumn = 0;
        focusRow++;
    }
    if(focusRow>voucher->InventoryItems.size())
        return;

    delete ledgerLine;
    delete crAmount;
    delete drAmount;

    qDebug()<<"Deleted All QWidgets";

    ledgerLine = new GMLedgerSearchLite(ledModel);

    qDebug()<<"Ledger Search Created";
    crAmount   = new QLineEdit(this);
    drAmount   = new QLineEdit(this);

    crAmount->setFrame(QFrame::NoFrame);
    drAmount->setFrame(QFrame::NoFrame);

    drAmount->setValidator(new QDoubleValidator(0,99999,2,this));
    drAmount->setProperty("row",focusRow);

    crAmount->setValidator(new QDoubleValidator(0,99999,2,this));
    crAmount->setProperty("row",focusRow);

    qDebug()<<"Created and set all new QWidgets";


    ledgerLine->setText("");
    ledgerLine->setProperty("row",focusRow);
    QWidget *SearchWid = new QWidget(this);
    SearchWid->setProperty("row",focusRow);
    QVBoxLayout *lay = new QVBoxLayout(SearchWid);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(0);
    lay->addWidget(ledgerLine);
    SearchWid->setLayout(lay);

    qDebug()<<"Created and set all new QWidgets";
    QObject::connect(ledgerLine,SIGNAL(selectedLedger(LedgerMasterDataModel)),
                     this,SLOT(setLedgerDetails(LedgerMasterDataModel)));


    QObject::connect(drAmount,SIGNAL(editingFinished()),this,SLOT(drAmtEntered()));
    QObject::connect(drAmount,SIGNAL(returnPressed()),this,SLOT(drAmtEntered()));

    QObject::connect(crAmount,SIGNAL(editingFinished()),this,SLOT(crAmtEntered()));
    QObject::connect(crAmount,SIGNAL(returnPressed()),this,SLOT(crAmtEntered()));

    qDebug()<<" set all new QWidgets func";
    ui->tableWidget->setCellWidget(focusRow,0,SearchWid);
    ui->tableWidget->setCellWidget(focusRow,1,drAmount);
    ui->tableWidget->setCellWidget(focusRow,2,crAmount);



    if(focusRow<voucher->InventoryItems.size()){
        qDebug()<<"Setting data";
        crAmount->setEnabled(true);
        ledgerLine->setText(voucher->ledgersList[focusRow].LedgerName);
        drAmount->setText(QString::number(voucher->ledgersList[focusRow].drAmount,'f',currencyDecimal));
        crAmount->setText(QString::number(voucher->ledgersList[focusRow].crAmount,'f',currencyDecimal));
    }
    else{
        focusColumn=1;
    }
    qDebug()<<"Table Created ; About to focus";
    setWidgetFocused();
}

void LedgersDisplayController::readStaticWidgets()
{
    voucher->VoucherDate ;//= ui->voucherDateEdit->date();
    voucher->ProjectId ;//= this->projectID;
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->grandTotal = voucher->grandTotal;
    voucher->narration = ui->narrationPlainText->toPlainText();
}

void LedgersDisplayController::saveVoucher()
{

}

void LedgersDisplayController::setWidgetFocused(){

    qDebug()<<" focus at "<<focusRow<<" col "<< focusColumn;
    drAmount->blockSignals(true);
    crAmount->blockSignals(true);
    if(focusColumn==0){
        ledgerLine->setFocus();
        ledgerLine->selectAll();
    }
    else if (focusColumn==1) {
        drAmount->setFocus();
        drAmount->selectAll();
    }
    else if (focusColumn==2) {
        crAmount->setFocus();
        crAmount->selectAll();
    }
    drAmount->blockSignals(false);
    crAmount->blockSignals(false);
    qDebug()<<"widgetFocused";
}

void LedgersDisplayController::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    int col = index.column();

    qDebug()<< " ROW = "<<row<< " COL + "<<col;
    focusRow = row;
    focusColumn = col;
    selectByClick = true;
    setTable();

    emit tableClicked(index);
}

void LedgersDisplayController::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    emit tableDoubleClicked(index);
}

void LedgersDisplayController::deleteLedgerRow()
{
    int x= sender()->property("row").toInt();

    ui->tableWidget->removeRow(x);
    voucher->InventoryItems.removeAt(x);

    setTable();
}

void LedgersDisplayController::drAmtEntered()
{
    int row = sender()->property("row").toInt();

    QLineEdit* line = qobject_cast<QLineEdit*>(sender());

    voucher->ledgersList[row].drAmount = line->text().toFloat();

//    QObject::disconnect(ledgerLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
//                        this,SLOT(getItemDetails(inventoryItemDataModel)));
//    QObject::disconnect(line,SIGNAL(editingFinished()),this,SLOT(drQtyEntered()));

    qDebug()<<"DR Qty Entred";
    focusColumn++;
    setTable();
}

void LedgersDisplayController::crAmtEntered()
{
    int row = sender()->property("row").toInt();

    QLineEdit* line = qobject_cast<QLineEdit*>(sender());

    voucher->ledgersList[row].crAmount    = line->text().toFloat();

//    QObject::disconnect(ledgerLine,SIGNAL(SelectedItemID(inventoryItemDataModel)),
//                        this,SLOT(getItemDetails(inventoryItemDataModel)));
//    QObject::disconnect(line,SIGNAL(editingFinished()),this,SLOT(crQtyEntered()));
    qDebug()<<"DR Qty Entred";

    focusColumn++;
    setTable();
}

void LedgersDisplayController::setLedgerDetails(LedgerMasterDataModel ledger)

{
    qDebug()<<"set ledger slot";
    int row = sender()->property("row").toInt();

    qDebug()<<"ROW NO : "<<row;
    qDebug()<<"Led List Size : "<<voucher->ledgersList.size();

    if(row == voucher->ledgersList.size()){

        qDebug()<<"Creating new Ledger  at "<<row;
        voucher->ledgersList.append(ledger);
    }
    else{
        voucher->ledgersList[row]=ledger;
    }

    focusColumn++;

    setTable();
}

bool LedgersDisplayController::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Up)
        {
            if(focusRow>0){
                focusRow--;
                setTable();
            }
            //            return true;
        }
        else if(keyEvent->key() == Qt::Key_Down)
        {
            qDebug()<<"focusrow "<<focusRow;
            qDebug()<<"Row Cnt"<<ui->tableWidget->rowCount();
            if(focusRow < ui->tableWidget->rowCount()-1){
                focusRow++;
                setTable();
            }


        }

        else if(keyEvent->key() == Qt::Key_Enter||keyEvent->key() == Qt::Key_Return||
                keyEvent->key() == Qt::Key_Tab)
        {
            if(focusColumn<4){
                focusColumn++;
                setTable();
            }
            else{
                focusColumn=1;
                focusRow++;
                setTable();
            }
            qDebug()<<"return Pressed";
        }

    }

}

