#include "addbillwiserecord.h"
#include "ui_addbillwiserecord.h"
#include "voucherOperations/vouchereditorselector.h"

AddBillwiseRecord::AddBillwiseRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBillwiseRecord)
{
    ui->setupUi(this);
    qDebug()<<"Billwise";

    this->setWindowTitle("Billwise Mapping");

    refObj = new BillwiseRecordDataModel();
    ledger = new LedgerMasterDataModel();
    ui->DueDateDateEdit->setDate(QDate::currentDate().addDays(ledger->Credit_Period));
    ui->VoucherDateEdit->setDate(QDate::currentDate());
    ui->enteredAmountLineEdit->setEnabled(false);
    billHelper = new BillwiseRecordsDatabaseHelper();

    //    setTableHeader(flag);
    //    setTable(flag);

    //    qDebug()<<"Flag = "<<flag;

    ui->clearDuesPushButton->hide();
    ui->widget->hide();
    qDebug()<<"***********************";
}

AddBillwiseRecord::~AddBillwiseRecord()
{
    delete ui;
    //    delete allBills;
}

void AddBillwiseRecord::setLedger(LedgerMasterDataModel *value)
{
    qDebug()<<"Set ledger"<<value->LedgerID<<value->amount;
    ledger = value;
    if(ledger->amount>0)
    {
        ui->enteredAmountLineEdit->setText(QString::number(ledger->amount,'f',2) + " Cr");
        enteredAmount=ledger->amount;
        ui->CrRadioButton->setChecked(true);
    }
    else{
        ui->enteredAmountLineEdit->setText(QString::number(-1*ledger->amount,'f',2) + " Dr");
        enteredAmount=ledger->amount;
        ui->DrRadioButton->setChecked(true);
    }
}

void AddBillwiseRecord::setOnAccountData(float balance)
{
    qDebug()<<"SET ON ACCOUNT"<<ledger->LedgerID<<balance;
    refObj = new BillwiseRecordDataModel();
    //    refObj->RefVoucherNo = "ON ACC";
    refObj->MethodOfAdjustment = "ON ACC";
    refObj->Amount = balance;
    if(balance>0){
        refObj->CreditAmount = balance;
        refObj->DebitAmount = 0;
    }
    else {
        refObj->DebitAmount = -1*balance;
        refObj->CreditAmount = 0;
    }
    refObj->LedgerID = ledger->LedgerID;
    refObj->VoucherDate = ui->VoucherDateEdit->date();
    refObj->DueDate = ui->DueDateDateEdit->date();
    if(!transFlag)
        refObj->VoucherType = "OPENING";
    refObj->VoucherPrefix = "A";
    refObj->selectedAmount = balance;
    refObj->RefType = "ON ACC";
    ledger->mapList.append(refObj);
    qDebug()<<"appended to list"<<ledger->mapList.size();

    //    setTable();

}

void AddBillwiseRecord::setVoucherData(LedgerMasterDataModel *ledger, QDate voucherDate)
{
    setLedger(ledger);
    setDefaultMappings(voucherDate, true);
}


void AddBillwiseRecord::setBillwiseMapping(QDate date, bool flag)
{
    qDebug()<<"Set billwise mapping"<<flag;
    //    on_OkPushButton_clicked();

    allBills = billHelper->getPendingBillsByLedger(ledger->LedgerID,date);
    qDebug()<<"No of pending bills"<<allBills.size();
    qDebug()<<"mapping size"<<ledger->mapList.size();

    int cnt =0;
    for(BillwiseRecordDataModel* obj:allBills){
        obj->selectedAmount=0;
        for(BillwiseRecordDataModel* map:ledger->mapList){
            if(obj->VoucherNo == map->RefVoucherNo){
                obj->selectedAmount+=map->Amount;
                //                obj->Amount-=map->Amount;
                map->pos=cnt;
                qDebug()<<"MAP: "<<map;
            }
        }
        cnt++;
    }

    //    balance=enteredValue;
    qDebug()<<"Entered Amount:"<<ledger->amount;
    transFlag = flag;
    qDebug()<<"Flag :"<<flag;
    //    if(ledger->crAmount>0)
    //    {
    //        ui->enteredAmountLineEdit->setText(QString::number(ledger->crAmount,'f',2) + " Cr");
    //        enteredAmount=ledger->crAmount;
    //        ui->CrRadioButton->setChecked(true);
    //    }
    //    else{
    //        ui->enteredAmountLineEdit->setText(QString::number(ledger->drAmount,'f',2) + " Dr");
    //        enteredAmount=-1*ledger->drAmount;
    //        ui->DrRadioButton->setChecked(true);
    //    }
    qDebug()<<"Number of rows:"<<ledger->mapList.size();

    setTableHeader();
    setTable();
}

void AddBillwiseRecord::setDefaultMappings(QDate date, bool flag)
{
    transFlag = flag;
    allBills = billHelper->getPendingBillsByLedger(ledger->LedgerID,date);
    qDebug()<<"Pending bills"<<allBills.size();

    //    if(ledger->crAmount>0)
    //    {
    //        enteredAmount=ledger->crAmount;
    //    }
    //    else{
    //        enteredAmount=-1*ledger->drAmount;
    //    }
    qDebug()<<"Entered amount"<<enteredAmount;
    int x=0;
    for(BillwiseRecordDataModel* obj:allBills)        {
        qDebug()<<"Value:"<<enteredAmount<<obj->Amount<<ledger->amount;
        if(enteredAmount!=0){
            if((obj->Amount>0 && ledger->amount <0) || (obj->Amount<0 && ledger->amount >0)){

                // use this amount
                if(qFabs(obj->Amount)>=qFabs(enteredAmount)){
                    obj->selectedAmount = enteredAmount;
                }
                else{
                    obj->selectedAmount = -1*obj->Amount;
                }
                enteredAmount-=obj->selectedAmount;
                qDebug()<<"Selected "<<obj->selectedAmount<<enteredAmount;
                setMapData(x);

            }
            else {
                obj->selectedAmount=0;
            }
        }
        x++;
    }
    qDebug()<<"Pending amount on account:"<<enteredAmount;
    if(enteredAmount!=0){
        qDebug()<<"ON ACC"<<enteredAmount;
        setOnAccountData(enteredAmount);
    }


    setTableHeader();
    setTable();
}

GMSQLQUeryModel *AddBillwiseRecord::setPendingBillsView(QDate date, bool flag)
{
//    return billHelper->getPendingBillsModelByLedger(ledger->LedgerID,date);
}

void AddBillwiseRecord::showOpeningBalanceList(bool flag)
{
//    qDebug()<<"Set billwise mapping"<<flag;

    ui->DrRadioButton->hide();
    ui->CrRadioButton->hide();
    LedgerMasterDatabaseHelper* ledHelper = new LedgerMasterDatabaseHelper();
    //    allBills = billHelper->getOpeningBalanceMapping(ledger->LedgerID);
    allBills = ledger->mapList;
    qDebug()<<"mapping size"<<ledger->mapList.size();
    ledger->amount = ledger->openingBalance;
    qDebug()<<"Entered Opening balance:"<<ledger->openingBalance;
    transFlag = flag;
    qDebug()<<"Flag :"<<flag<<ledger->crAmount<<ledger->drAmount;
    if(ledger->amount>0)
    {
        qDebug()<<"CR";
        ui->enteredAmountLineEdit->setText(QString::number(ledger->amount,'f',2) + " Cr");
        enteredAmount=ledger->amount;
        ui->CrRadioButton->setChecked(true);
    }
    else{
        qDebug()<<"DR";
        ui->enteredAmountLineEdit->setText(QString::number(ledger->amount,'f',2) + " Dr");
        enteredAmount=-1*ledger->amount;
        ui->DrRadioButton->setChecked(true);
    }
    qDebug()<<"Number of rows:"<<ledger->mapList.size();

    setTableHeader();
    setTable();
    }

void AddBillwiseRecord::showPendingBills()
{
    qDebug()<<"Show pending";
    ui->RefLay_2->setVisible(false);
    ui->enteredAmountLabel->hide();
    ui->enteredAmountLineEdit->hide();
    ui->BalanceLabel->hide();
    ui->BalanceLineEdit->hide();
    ui->OkPushButton->hide();
    ui->CancelPushButton->hide();
    ui->clearDuesPushButton->show();
    ui->TotalLineEdit->clear();
    setBillwiseMapping(QDate::currentDate(), true);
}

void AddBillwiseRecord::on_OkPushButton_clicked()
{
    int cnt=ui->tableWidget->rowCount();
    qDebug()<<"Num of rows in mapping:"<<ledger->mapList.size();
    qDebug()<<"Balance ON account: "<< ui->BalanceLineEdit->text().toFloat();
    qDebug()<<"Entered amount"<<ledger->amount;
    float balance ;
    QString balStr = ui->BalanceLineEdit->text();
    int strlen = balStr.length();
    if(balStr.contains("Cr"))
        balance = balStr.remove(" Cr").toFloat();
    else
        balance = -1* balStr.remove(" Dr").toFloat();

    if(balance != 0){
        setOnAccountData(balance);
    }
    qDebug()<<"Final count:"<<ledger->mapList.size();
    this->close();
    emit addReference(ledger->mapList);
}

void AddBillwiseRecord::delButtonClicked(bool)
{
    qDebug()<<"Delete button clicked";
    int x= sender()->property("row").toInt();

    ui->tableWidget->removeRow(x);
    ledger->mapList.removeAt(x);
    //    allBills.removeAt(x);
    setTable();
}

void AddBillwiseRecord::on_CancelPushButton_clicked()
{
    this->close();
}

void AddBillwiseRecord::setTable()
{
    int cnt =0;
    int i;

    ui->tableWidget->setRowCount(0);
    float total=0;
    float balance=ledger->amount;
    qDebug()<<"Initial balance"<<balance;
    //        float billAmount;
    for(BillwiseRecordDataModel* obj:allBills){
        ui->tableWidget->insertRow(cnt);
        i=0;

        QTableWidgetItem* BillNo = new QTableWidgetItem;
        BillNo->setText(obj->VoucherNo);
        qDebug()<<"Mapping voucher:"<<obj->VoucherNo;
        BillNo->setFlags(BillNo->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,BillNo);

        QTableWidgetItem* voucherType = new QTableWidgetItem;
        voucherType->setText(obj->VoucherType);
        voucherType->setFlags(voucherType->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,voucherType);


        QTableWidgetItem* VoucherDate = new QTableWidgetItem;
        VoucherDate->setText(obj->VoucherDate.toString("yyyy-MM-dd"));
        VoucherDate->setFlags(VoucherDate->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,VoucherDate);

        QTableWidgetItem* DueDate = new QTableWidgetItem;
        DueDate->setText(obj->DueDate.toString("yyyy-MM-dd"));
        DueDate->setFlags(DueDate->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,DueDate);


        QTableWidgetItem* PendingAmount = new QTableWidgetItem;
        if(obj->Amount > 0){
            PendingAmount->setText(QString::number(obj->Amount,'f',2) + " Cr");
        }
        else{
            PendingAmount->setText(QString::number(-1*obj->Amount,'f',2) + " Dr");
        }
        PendingAmount->setFlags(PendingAmount->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,PendingAmount);

        //        ui->tableWidget->setItem(cnt,i++,AmtType);

        QTableWidgetItem* MethodOfAdj = new QTableWidgetItem;
        MethodOfAdj->setText(obj->MethodOfAdjustment);
        MethodOfAdj->setFlags(MethodOfAdj->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,MethodOfAdj);


        QLineEdit *amountEdit = new QLineEdit;
        amountEdit ->setValidator(new QDoubleValidator(0.0000,999999,2,this));
        amountEdit->setFrame(QFrame::NoFrame);
        amountEdit->setProperty("row",cnt);
        qDebug()<<"selected amt:"<<obj->selectedAmount;
        if(obj->selectedAmount>0)
            amountEdit->setText(QString::number(obj->selectedAmount,'f',2) + " Cr");
        else
            amountEdit->setText(QString::number(-1*obj->selectedAmount,'f',2) + " Dr");
        ui->tableWidget->setCellWidget(cnt,i++,amountEdit);
        amountEdit->setAlignment(Qt::AlignRight);
        QObject::connect(amountEdit, SIGNAL(returnPressed()), this, SLOT(setSelectedAmount()));

        QCheckBox *selectItem = new QCheckBox;
        selectItem->setProperty("row",cnt);
        if(obj->Amount == -1*obj->selectedAmount)
            selectItem->setChecked(true);
        else
            selectItem->setChecked(false);
        ui->tableWidget->setCellWidget(cnt,i++,selectItem);
        QObject::connect(selectItem, SIGNAL(stateChanged(int)), this, SLOT(setBillAmount(int)));

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",cnt);
        delButton->setAutoRaise(true);
        ui->tableWidget->setCellWidget(cnt,i++,delButton);
        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        qDebug()<<obj->selectedAmount;
        total+=obj->selectedAmount;
        balance-=obj->selectedAmount;
        if(transFlag)
            ui->tableWidget->hideColumn(8);
        else{
            ui->tableWidget->hideColumn(6);
            ui->tableWidget->hideColumn(7);
        }

        qDebug()<<"hide row &&&&&&&&&&&&"<<ledger->crAmount<<ledger->drAmount<<obj->Amount;
        if(transFlag && ((ledger->crAmount > 0 && obj->Amount > 0) || (ledger->drAmount >0 && obj->Amount <0) ))
        {
            ui->tableWidget->hideRow(cnt);

        }
        cnt++;
    }

    qDebug()<<"Total is "<<total;
    qDebug()<<"Balance :"<<balance;
    if(total>0)
        ui->TotalLineEdit->setText(QString::number(total,'f',2) + " Cr");
    else
        ui->TotalLineEdit->setText(QString::number(-1*total,'f',2) + " Dr");

    if(balance>0)
        ui->BalanceLineEdit->setText(QString::number(balance,'f',2) + " Cr");
    else
        ui->BalanceLineEdit->setText(QString::number(-1*balance,'f',2) + " Dr");

}

void AddBillwiseRecord::setTableHeader()
{
    ui->tableWidget->setColumnCount(9);

    int i=0;
    QTableWidgetItem* BillNo = new QTableWidgetItem;
    BillNo->setText("Bill No.");
    ui->tableWidget->setHorizontalHeaderItem(i++,BillNo);


    QTableWidgetItem* voucherType = new QTableWidgetItem;
    voucherType->setText("Voucher Type");
    ui->tableWidget->setHorizontalHeaderItem(i++,voucherType);


    QTableWidgetItem* VoucherDate = new QTableWidgetItem;
    VoucherDate->setText("Voucher Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,VoucherDate);

    QTableWidgetItem* DueDate = new QTableWidgetItem;
    DueDate->setText("Due Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,DueDate);

    QTableWidgetItem* Amount = new QTableWidgetItem;
    Amount->setText("Amount");
    ui->tableWidget->setHorizontalHeaderItem(i++, Amount);

    QTableWidgetItem* MethodOfAdj = new QTableWidgetItem;
    MethodOfAdj->setText("Method Of Adjustment");
    ui->tableWidget->setHorizontalHeaderItem(i++,MethodOfAdj);

    QTableWidgetItem* SelectedAmount = new QTableWidgetItem;
    SelectedAmount->setText("Selected Amount");
    ui->tableWidget->setHorizontalHeaderItem(i++, SelectedAmount);

    QTableWidgetItem* Select = new QTableWidgetItem;
    Select->setText("Select");
    ui->tableWidget->setHorizontalHeaderItem(i++, Select);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

    qDebug()<<"column count = "<<i;


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    ui->tableWidget->setColumnWidth(8,25);/*
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(0,100);*/

    if(transFlag)
        ui->tableWidget->hideColumn(8);
    else{
        ui->tableWidget->hideColumn(6);
        ui->tableWidget->hideColumn(7);
    }
}



void AddBillwiseRecord::on_AddPushButton_clicked()
{
    refObj = new BillwiseRecordDataModel();
    refObj->VoucherNo = ui->BillNoLineEdit->text();
    qDebug()<<"Old voucher no:"<<refObj->VoucherNo;
    if(ui->NewRefRadioButton->isChecked()==true)
        refObj->MethodOfAdjustment = "NEW REF";
    else if(ui->AdvanceRadioButton->isChecked()==true)
        refObj->MethodOfAdjustment = "ADV";
    else
        refObj->MethodOfAdjustment = "AGNST REF";
    if(refObj->VoucherNo == ""){
        refObj->MethodOfAdjustment = "ON ACC";
        refObj->VoucherNo = "ON ACC";
    }

    if(enteredAmount>0){
        qDebug()<<"Credit amount";
        refObj->Amount = ui->AmountLineEdit->text().toFloat();
        refObj->CreditAmount = ui->AmountLineEdit->text().toFloat();
        refObj->DebitAmount = 0;
    }
    if(enteredAmount<=0){
        refObj->Amount = -1* ui->AmountLineEdit->text().toFloat();
        refObj->CreditAmount = 0;
        refObj->DebitAmount = ui->AmountLineEdit->text().toFloat();
    }
    qDebug()<<refObj->Amount;
    refObj->selectedAmount = refObj->Amount;
    refObj->LedgerID = ledger->LedgerID;
    refObj->VoucherDate = ui->VoucherDateEdit->date();
    refObj->DueDate = ui->DueDateDateEdit->date();
    if(!transFlag)
        refObj->VoucherType = "OPENING";
    ledger->mapList.append(refObj);
    allBills.append(refObj);
    qDebug()<<"appended to list"<<ledger->mapList.size();
    setTable();

    ui->BillNoLineEdit->clear();
    ui->AmountLineEdit->clear();
    ui->DueDateDateEdit->setDate(QDate::currentDate().addDays(120));
    ui->VoucherDateEdit->setDate(QDate::currentDate());
    ui->CrRadioButton->setChecked(false);
    ui->DrRadioButton->setChecked(false);

}

void AddBillwiseRecord::setBillAmount(int state)
{
    int x= sender()->property("row").toInt();
    qDebug()<<"Check box state:"<<state;

    if(state == 2){
        qDebug()<<"Box checked. Selected bill amount is"<<allBills[x]->Amount;
        allBills[x]->selectedAmount = -1*allBills[x]->Amount;
    }
    else
        allBills[x]->selectedAmount = 0;
    setMapData(x);
    setTable();
}

void AddBillwiseRecord::setSelectedAmount()
{
    qDebug()<<"Line edit changed.Set selected Amount";
    int x= sender()->property("row").toInt();
    qDebug()<<"Previous amount"<<allBills[x]->selectedAmount;

    float selectedAmount = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(x,6))->text().toFloat();
    if(allBills[x]->Amount>0)
        selectedAmount*=-1;
    qDebug()<<"Selected Amount"<<selectedAmount;
    allBills[x]->selectedAmount=selectedAmount;
    setMapData(x);
    setTable();
}

void AddBillwiseRecord::on_clearDuesPushButton_clicked()
{
    voucher = new GeneralVoucherDataObject();
    GeneralVoucherDatabaseHelper vHelper;
    float total = 0 ;
    QString totStr = ui->TotalLineEdit->text();
    if(totStr.contains("Cr"))
        total = totStr.remove(" Cr").toFloat();
    else
        total = -1* totStr.remove(" Dr").toFloat();

    qDebug()<<"Clear dues"<<total;
    //    voucher.ledgersList.append(ledHelper->getLedgerObjectByID(ledger->LedgerID));
//    voucher->ledgersList.clear();
    voucher->ledgersList.append(*ledger);
    voucher->VoucherDate = QDate::currentDate();
    voucher->VoucherPrefix = LoginValues::voucherPrefix;
    voucher->status = QuotationStatus::Started;
    //    voucher.mapList=mapList;
    qDebug()<<"Voucher X";



    VoucherEditorSelector *vEditor = new VoucherEditorSelector;
    connect(vEditor, &VoucherEditorSelector::refresh, this, &AddBillwiseRecord::refreshLedgerReport);

    if(total == 0){
        qDebug()<<"No dues";
    }
    else if(total > 0){
        qDebug()<<"Show receipt voucher"<<total;
        voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher);
        voucher->voucherNumber = vHelper.getNextVoucherNoByType(voucher->voucherType,
                                                                voucher->VoucherPrefix,
                                                                GMVoucherTypes::getVoucherMainTablename(GMVoucherTypes::ReceiptVoucher));
        voucher->ledgersList[0].crAmount =  total;
        voucher->ledgersList[0].amount =  total;
        //        voucher.ledgersList[0] = ledger;
//        ReceiptVoucherEditor *addReceiptEntryWidget = new ReceiptVoucherEditor(new ReceiptVoucherDatabaseHelper);
//        addReceiptEntryWidget->setWindowFlags(Qt::Window);
//        addReceiptEntryWidget->setVoucher(voucher);
//        addReceiptEntryWidget->showVoucher();
//        addReceiptEntryWidget->showMaximized();
//        QObject::connect(addReceiptEntryWidget,SIGNAL(closing()),this,SLOT(refreshLedgerReport()));

    }
    else{
        qDebug()<<"Show payment voucher"<<total;
        voucher->voucherType = GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);
        voucher->voucherNumber = vHelper.getNextVoucherNoByType(voucher->voucherType,
                                                                voucher->VoucherPrefix,
                                                                GMVoucherTypes::getVoucherMainTablename(GMVoucherTypes::PaymentVoucher));

        voucher->ledgersList[0].drAmount = -1 * total;
        voucher->ledgersList[0].amount = -1 * total;
//        PaymentVoucherEditor *AddPaymentEntryWidget = new PaymentVoucherEditor(new PaymentVoucherDatabaseHelper);
//        AddPaymentEntryWidget->setWindowFlags(Qt::Window);
//        AddPaymentEntryWidget->setVoucher(voucher);
//        AddPaymentEntryWidget->showVoucher();
//        AddPaymentEntryWidget->showMaximized();
//        QObject::connect(AddPaymentEntryWidget,SIGNAL(closing()),this,SLOT(refreshLedgerReport()));

    }

    vEditor->openExternalVoucher(voucher);

}

void AddBillwiseRecord::setMapData(int x)
{
    refObj = new BillwiseRecordDataModel();

    refObj->Amount=allBills[x]->selectedAmount;
    if(allBills[x]->VoucherType == "ON ACCOUNT")
        refObj->MethodOfAdjustment = "ON ACC";
    else
        refObj->MethodOfAdjustment = "AGAINST REFERENCE";
    refObj->LedgerID = ledger->LedgerID;
    refObj->RefVoucherNo = allBills[x]->VoucherNo;
    refObj->RefPrefix = allBills[x]->VoucherPrefix;
    refObj->RefType = allBills[x]->VoucherType;
    if(refObj->Amount>0){
        refObj->CreditAmount=refObj->Amount;
        refObj->DebitAmount=0;
    }
    else{
        refObj->DebitAmount=-1*refObj->Amount;
        refObj->CreditAmount=0;
    }
    //    refObj->selectedAmount = selectedAmount;
    refObj->pos = x;
    removeObjFromMapList(x);
    ledger->mapList.append(refObj);
    //    qDebug()<<"appended to list"<<ledger->mapList.size()<<refObj->RefVoucherNo;
    //    setTable();
}


void AddBillwiseRecord::removeObjFromMapList(int pos)
{
    int i=0;
    for(BillwiseRecordDataModel* obj:ledger->mapList){

        if(obj->pos == pos){
            qDebug()<<"Removing obj from pos"<<pos;
            ledger->mapList.removeAt(i);
        }
        i++;
    }

}

void AddBillwiseRecord::refreshLedgerReport()
{
    ledger->mapList.clear();
    showPendingBills();
    emit refresh(*ledger);
    emit closing();
}
