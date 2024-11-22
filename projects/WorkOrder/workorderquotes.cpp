#include "workorderquotes.h"
#include "ui_workorderquotes.h"
#include <QMessageBox>
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QFormLayout>
#include <QMapIterator>
#include <QStandardItemModel>
#include <QCompleter>

WorkOrderQuotes::WorkOrderQuotes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkOrderQuotes)
{
    ui->setupUi(this);
    ui->voucherNumber->setReadOnly(true);
    ui->revWidget->hide();

    ui->label_6->hide();
    ui->customerNameLineEdit->hide();
    dbHelper = new SalesOrderDatabaseHelper();
    ui->tableWidget->setColumnCount(9);
    int i = 0;
    CompletedButton = new QToolButton(this);
    CompletedButton->setAutoRaise(true);
    CompletedButton->setText("Mark Completed");
    CompletedButton->setIcon(QIcon(":/icons/check.ico"));
    CompletedButton->setIconSize(QSize(32,32));
    CompletedButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    printButton = new QToolButton(this);
    printButton ->setText("Print");
    printButton ->setIcon(QIcon(":/images/printer1.ico"));
    printButton ->setIconSize(QSize(32,32));
    printButton ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    printButton ->setAutoRaise(true);

    ui->extrasLayout->addWidget(printButton);
    QObject::connect(printButton,SIGNAL(clicked(bool)),this,SLOT(printWorkOrder()));

    ui->horizontalLayout->addWidget(CompletedButton);
    QObject::connect(CompletedButton,SIGNAL(clicked(bool)),this,SLOT(completedClicked(bool)));

    CompletedButton->setHidden(true);
    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Item Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,ItemName);

    QTableWidgetItem* Model = new QTableWidgetItem;
    Model->setText("Model");
    ui->tableWidget->setHorizontalHeaderItem(i++,Model);

    QTableWidgetItem* Brand = new QTableWidgetItem;
    Brand->setText("Brand");
    ui->tableWidget->setHorizontalHeaderItem(i++,Brand);

    QTableWidgetItem* Serial = new QTableWidgetItem;
    Serial->setText("Serial");
    ui->tableWidget->setHorizontalHeaderItem(i++,Serial);


    QTableWidgetItem* Qty = new QTableWidgetItem;
    Qty->setText("Quantity");
    ui->tableWidget->setHorizontalHeaderItem(i++,Qty);

    QTableWidgetItem* Assign = new QTableWidgetItem;
    Assign->setText("Assign");
    ui->tableWidget->setHorizontalHeaderItem(i++,Assign);

    QTableWidgetItem* tentativeCompletion = new QTableWidgetItem;
    tentativeCompletion->setText("Expected Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,tentativeCompletion);


    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setText("Status");
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);



    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);

    ui->ReadyMadeTableWidget->setColumnCount(9);

    i=0;
    QTableWidgetItem* rSLNO = new QTableWidgetItem;
    rSLNO->setText("#");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rSLNO);

    QTableWidgetItem* rItemName = new QTableWidgetItem;
    rItemName->setText("Item Name");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rItemName);

    QTableWidgetItem* rModel = new QTableWidgetItem;
    rModel->setText("Model");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rModel);

    QTableWidgetItem* rBrand = new QTableWidgetItem;
    rBrand->setText("Brand");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rBrand);

    QTableWidgetItem* rSerial = new QTableWidgetItem;
    rSerial->setText("Serial");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rSerial);

    QTableWidgetItem* rStk = new QTableWidgetItem;
    rStk->setText("Curr Stock");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rStk);

    QTableWidgetItem* rreserv = new QTableWidgetItem;
    rreserv->setText("Reserved");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rreserv);

    QTableWidgetItem* ravlDate = new QTableWidgetItem;
    ravlDate->setText("Exp. Date");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,ravlDate);


    QTableWidgetItem* rQty = new QTableWidgetItem;
    rQty->setText("Quantity");
    ui->ReadyMadeTableWidget->setHorizontalHeaderItem(i++,rQty);

    ui->ReadyMadeTableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);


    ui->ReadyMadeTableWidget->setColumnWidth(0,50);


    user = new UserProfileDatabaseHelper;
    people =  user->getAllUsers();

    settable();

}

WorkOrderQuotes::~WorkOrderQuotes()
{
    delete ui;
}
void WorkOrderQuotes::on_tableWidget_clicked(const QModelIndex &index)
{

}

void WorkOrderQuotes::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    WorkOrderItemDetailWidget = new WorkOrderItemDetail(this);
    WorkOrderItemDetailWidget->setWindowFlags(Qt::Window);
    WorkOrderItemDetailWidget->setQuoteParams(Voucher.voucherNumber,Voucher.ProjectId);
    WorkOrderItemDetailWidget->editItem(Voucher.InventoryItems[r]);
    WorkOrderItemDetailWidget->showMaximized();
    QObject::connect(WorkOrderItemDetailWidget,SIGNAL(closing()),this,SLOT(settable()));
}

void WorkOrderQuotes::settable()
{

    ui->tableWidget->setRowCount(0);
    ui->ReadyMadeTableWidget->setRowCount(0);

    int i=0;
    int cnt =0;
    for(CompoundItemDataObject obj:Voucher.InventoryItems){
        ui->tableWidget->insertRow(cnt);

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        ui->tableWidget->setItem(cnt,0,SLNO);

        QTableWidgetItem* name = new QTableWidgetItem;
        name->setText(obj.BaseItem.ItemName);
        ui->tableWidget->setItem(cnt,1,name);

        QTableWidgetItem* model = new QTableWidgetItem;
        model->setText(obj.BaseItem.ItemCode);
        model->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(cnt,2,model);

        QTableWidgetItem* brand = new QTableWidgetItem;
        brand->setText(obj.BaseItem.brandName);
        brand->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(cnt,3,brand);

        QTableWidgetItem* qty = new QTableWidgetItem;
        qty->setText("1");
        qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->tableWidget->setItem(cnt,5,qty);




        QTableWidgetItem* serial = new QTableWidgetItem;
        serial->setText(obj.BaseItem.SerailNumber);

        ui->tableWidget->setItem(cnt,4,serial);

        QComboBox *technicianCombo   = new QComboBox;
        QDateEdit *tentatveDate = new QDateEdit;

        technicianCombo->setEditable(true);
        technicianCombo->addItem("",0);
        technicianCombo->lineEdit()->setPlaceholderText("Assign");

        tentatveDate->setCalendarPopup(true);

        QMapIterator<int,QString> ix(people);
        while (ix.hasNext()) {
            ix.next();
            int id = ix.key();
            QString name = ix.value();
            technicianCombo->addItem(name,id);
        }

        QCompleter *completer1 = new QCompleter(this);

        technicianCombo->setCompleter(completer1);

        technicianCombo->setProperty("id",i);
        tentatveDate->setProperty("id",i);
        tentatveDate->setDate(QDate::currentDate());

        int currTechID =0;// dbHelper->getTechnician(obj.BaseItem.requirementItemIdOld,obj.BaseItem.quantity,Voucher.voucherNumber);


        technicianCombo->setCurrentText(user->getEmployeeNameFromID(currTechID));


        tentatveDate->setDate(dbHelper->getExpectedDate(obj.BaseItem.requirementItemIdOld,obj.BaseItem.quantity,Voucher.voucherNumber));
        ui->tableWidget->setCellWidget(cnt,6,technicianCombo);
        ui->tableWidget->setCellWidget(cnt,7,tentatveDate);
        tentatveDate->setEnabled(false);

        QObject::connect(technicianCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(technicianAssigned(int)));

        QObject::connect(tentatveDate,SIGNAL(dateChanged(QDate)),this,SLOT(ExpectedDateChenged(QDate)));


        ui->tableWidget->setCellWidget(cnt,6,technicianCombo);
        ui->tableWidget->setCellWidget(cnt,7,tentatveDate);


        QToolButton* check = new QToolButton;

        check->setCheckable(true);
        check->setText("Mark");
        check->setProperty("check",0);
        check->setProperty("id",i);

//        if(dbHelper->getSerailNumber(obj.BaseItem.requirementItemIdOld,obj.BaseItem.quantity,Voucher.voucherNumber).length()==0){
//            if(technicianCombo->currentText().length()){
//                check->setText("Assigned");
//            }
//            else{
//                check->setText("Unassigned");

//            }


//        }
//        else{
//            check->setChecked(true);
//            check->setIcon(QIcon(":/icons/check.ico"));
//            check->setIconSize(QSize(16,16));
//            check->setToolButtonStyle(Qt::ToolButtonIconOnly);
//            check->setAutoRaise(true);
//            check->setProperty("check",1);
//        }


        if(obj.BaseItem.isCustomItem){
            QObject::connect(check,SIGNAL(toggled(bool)),this,SLOT(checkToggle(bool)));
            ui->tableWidget->setCellWidget(cnt,8,check);

        }
        else{
            QTableWidgetItem* qtc = new QTableWidgetItem;
            qtc->setText("Ready");
            ui->tableWidget->setItem(cnt,8,qtc);
        }
        cnt++;
        i++;
    }

    cnt =0;
    i=0;
    for(CompoundItemDataObject obj:ReadyMadeVoucher.InventoryItems){
        ui->ReadyMadeTableWidget->insertRow(cnt);

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        ui->ReadyMadeTableWidget->setItem(cnt,0,SLNO);

        QTableWidgetItem* name = new QTableWidgetItem;
        name->setText(obj.BaseItem.ItemName);
        ui->ReadyMadeTableWidget->setItem(cnt,1,name);

        QTableWidgetItem* model = new QTableWidgetItem;
        model->setText(obj.BaseItem.ItemCode);
        model->setTextAlignment(Qt::AlignCenter);
        ui->ReadyMadeTableWidget->setItem(cnt,2,model);

        QTableWidgetItem* brand = new QTableWidgetItem;
        brand->setText(obj.BaseItem.brandName);
        brand->setTextAlignment(Qt::AlignCenter);
        ui->ReadyMadeTableWidget->setItem(cnt,3,brand);

        QTableWidgetItem* qty = new QTableWidgetItem;
        qty->setText(QString::number(obj.BaseItem.quantity));
        qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->ReadyMadeTableWidget->setItem(cnt,5,qty);

        QTableWidgetItem* stk = new QTableWidgetItem;
        stk->setText("0");
        stk->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->ReadyMadeTableWidget->setItem(cnt,6,stk);

        QTableWidgetItem* res = new QTableWidgetItem;
        res->setText("0");
        res->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->ReadyMadeTableWidget->setItem(cnt,7,res);

//        QTableWidgetItem* rdate = new QTableWidgetItem;
        QDateEdit* rdate = new QDateEdit;
        rdate->setReadOnly(true);
        ui->ReadyMadeTableWidget->setCellWidget(cnt,8,rdate);


        QTableWidgetItem* serial = new QTableWidgetItem;
        serial->setText(obj.BaseItem.SerailNumber);

        ui->ReadyMadeTableWidget->setItem(cnt,4,serial);

        cnt++;
        i++;
    }




}

void WorkOrderQuotes::technicianAssigned(int x)
{
    QComboBox* techcombo = qobject_cast <QComboBox*> (sender());

    int row = techcombo->property("id").toInt();

    int techid = techcombo->currentData().toInt();
    if(techid>0){
    QToolButton* butt = qobject_cast<QToolButton*> (ui->tableWidget->cellWidget(row,8));
    butt->setText("Assigned");
//    dbHelper->setTechnician(techid,Voucher.InventoryItems[row].BaseItem.requirementItemIdOld,
//                            Voucher.InventoryItems[row].BaseItem.quantity,Voucher.voucherNumber);

    QDateEdit* dateEDit = qobject_cast<QDateEdit*>( ui->tableWidget->cellWidget(row,7));
    dateEDit->setEnabled(true);
    }

}

void WorkOrderQuotes::ExpectedDateChenged(QDate date)
{
    QDateEdit* techcombo = qobject_cast <QDateEdit*> (sender());

    int row = techcombo->property("id").toInt();


    dbHelper->setExpectedDate(date,Voucher.InventoryItems[row].BaseItem.requirementItemIdOld,
                            Voucher.InventoryItems[row].BaseItem.quantity,Voucher.voucherNumber);


}

void WorkOrderQuotes::showVoucher(GeneralVoucherDataObject quote)
{

    Voucher = quote;
    ReadyMadeVoucher = quote;
    CustomItemsVoucher = quote;
    SalesOrderVoucher = quote;

    Voucher.InventoryItems.clear();
    ReadyMadeVoucher.InventoryItems.clear();
    CustomItemsVoucher.InventoryItems.clear();


    qDebug()<<"items List Count = "<<quote.InventoryItems.count();
    qDebug()<<"S.O number = "<<quote.voucherNumber;

    for(CompoundItemDataObject item:quote.InventoryItems){
        if(item.BaseItem.isCustomItem){
            CustomItemsVoucher.InventoryItems.append(item);
        }
        else{
            ReadyMadeVoucher.InventoryItems.append(item);
        }
    }


    for(CompoundItemDataObject item:CustomItemsVoucher.InventoryItems){
        qDebug()<<"Item count = "<<item.BaseItem.quantity;
        int qty = item.BaseItem.quantity;
        for(int i=0,j=0;i<qty;i++,j++){
            item.BaseItem.quantity=i;
//            item.BaseItem.SerailNumber=dbHelper->getSerailNumber(item.BaseItem.requirementItemIdOld,i,quote.voucherNumber);
            qDebug()<<"Serial Number = "<<item.BaseItem.SerailNumber;
//            item.BaseItem.TechnicianID=dbHelper->getTechnician(item.BaseItem.requirementItemIdOld,i,quote.voucherNumber);
//            item.BaseItem.itemProductionStatus = dbHelper->getProductionStatus(item.BaseItem.requirementItemIdOld,i,quote.voucherNumber);
            item.BaseItem.dateCreated=dbHelper->getCompletedDate(item.BaseItem.requirementItemIdOld,i,quote.voucherNumber);
            Voucher.InventoryItems.append(item);

        }
    }



    ui->voucherNumber->setText(dbHelper->getWorkOrderNumberbySo(Voucher.voucherNumber));
    ui->dateEdit->setDate(Voucher.VoucherDate);

    if(Voucher.RevisionNo>0){
        ui->revWidget->show();
        ui->revisionNumber->setText(QString::number(Voucher.RevisionNo));
    }




    ui->customerNameLineEdit->setText(Voucher.ledgerObject.LedgerName);
    CompletedButton->setHidden(false);
    //CompletedButton->setHidden(LoginValues::getModel().);
    ui->salesMan->setText(user->getEmployeeNameFromID(Voucher.SalesmanID));

    settable();

}

void WorkOrderQuotes::completedClicked(bool t)
{
    //Mark Complted;
    QMessageBox::StandardButton repl;
    QMessageBox::StandardButton started;

    repl = QMessageBox::question(this,"Mark Completed","Are You Sure All Item Production Completed?",QMessageBox::Yes|QMessageBox::No|started);
    if(repl == QMessageBox::Yes){
        dbHelper->setSalesOrderStatus(QuotationStatus::WorkOrderCompleted,Voucher.voucherNumber,Voucher.VoucherPrefix);
    }
    else{
        dbHelper->setSalesOrderStatus(QuotationStatus::WorkOrderRaised,Voucher.voucherNumber,Voucher.VoucherPrefix);
    }

}


void WorkOrderQuotes::on_save_clicked()
{

    emit closing();
    this->close();
}

void WorkOrderQuotes::on_cancelButton_clicked()
{
    this->close();
}

void WorkOrderQuotes::delButtonClicked(bool)
{
    int row = sender()->property("row").toInt();
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete File","Are You Sure you want to Delete File?",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        //        dbHelper->deleteQImage(docObj[row].id);
        //        docObj.removeAt(row);

    }
}

void WorkOrderQuotes::downloadClicked(bool)
{
    //    int row = sender()->property("row").toInt();
    //    qDebug()<<docObj[row].id;
    //    QByteArray ba = dbHelper->getFilewithId(docObj[row].id);
    //    qDebug()<<"image size "<<ba.size();
    //    QString path = QFileDialog::getExistingDirectory(this,"Select folder",QDir::currentPath());
    //    QFile file(path+"/"+docObj[row].filename);
    //    file.open(QIODevice::WriteOnly);
    //    QDataStream out(&file);
    //    out.writeRawData(ba,ba.size());
    //    //file.write(ba);
    //    file.close();
}

void WorkOrderQuotes::printWorkOrder()
{
    //    QuotationDetailsWidget = new QuotationDetails(this);
    //    QuotationDetailsWidget ->setWindowFlags(Qt::Window);
    //    QuotationDetailsWidget ->showMaximized();
    //    QObject::connect(QuotationDetailsWidget,SIGNAL
    //    (quoteDetailsSaved(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)),
    //                     this,SLOT(
    //    getQuoteDetails(QString,QString,QString,QString,QString,QString,QString,bool,bool,bool,bool)));
    PrinterHelper *pHelpr = new PrinterHelper;
    Voucher.voucherType ="Work Order";
    Voucher.voucherNumber = ui->voucherNumber->text();
    pHelpr->printWorkOrder(Voucher);

}

void WorkOrderQuotes::showSerialDialog(int index){

    SerialNumberDialog = new QDialog(this);
    SerialNumberDialog ->setWindowTitle("Serial Number");

    QVBoxLayout *vLayout =new QVBoxLayout(SerialNumberDialog);

    QHBoxLayout *boxgetSerialNumber = new QHBoxLayout(SerialNumberDialog);

    QHBoxLayout *boxlayokCancel = new QHBoxLayout(SerialNumberDialog);

    QFormLayout *flay = new QFormLayout(SerialNumberDialog);

    QLabel* labelCompltedBy = new QLabel();
    QLabel* labelCompletedDate = new QLabel();
    QLabel* labelSerialNumber = new QLabel();


    CompletedByCombo = new QComboBox();
    CompletedDate = new QDateEdit();
    SerialNumber = new QLineEdit();
    CompletedDate->setReadOnly(true);

    labelCompltedBy->setText("Completed By : ");
    labelCompletedDate->setText("Completion Date : ");
    labelSerialNumber->setText("Serial Number : ");

    CompletedDate->setDate(QDate::currentDate());

    QPushButton *getSerailButton = new QPushButton();

    QPushButton *OkButton = new QPushButton();
    QPushButton *CancelButton = new QPushButton();
    QPushButton *PrintButton = new QPushButton();

    getSerailButton->setText("Get Serail Number");
    OkButton->setText("OK");
    CancelButton->setText("Cancel");
    PrintButton->setText("Print Serial");

    getSerailButton->setProperty("index",index);
    OkButton->setProperty("index",index);
    CancelButton->setProperty("index",index);
    PrintButton->setProperty("index",index);

    CompletedByCombo->setEditable(true);

    QMapIterator<int,QString> i(people);
    while (i.hasNext()) {
        i.next();
        int id = i.key();
        QString name = i.value();
        CompletedByCombo->addItem(name,id);
    }

    QObject::connect(getSerailButton,SIGNAL(clicked()),this,SLOT(setSerialNumber()));
    QObject::connect(OkButton,SIGNAL(clicked()),this,SLOT(OkButtonClicked()));
    QObject::connect(PrintButton,SIGNAL(clicked()),this,SLOT(printSerialButtonClicked()));
    QObject::connect(CancelButton,SIGNAL(clicked()),this,SLOT(CancelButtonClicked()));



    boxgetSerialNumber->addWidget(getSerailButton);
    boxlayokCancel->addWidget(PrintButton);
    boxlayokCancel->addWidget(OkButton);
    boxlayokCancel->addWidget(CancelButton);

    flay->addRow(labelCompltedBy,CompletedByCombo);
    flay->addRow(labelCompletedDate,CompletedDate);

    vLayout->addLayout(flay);
    vLayout->addLayout(boxgetSerialNumber);
    vLayout->addWidget(SerialNumber);
    vLayout->addLayout(boxlayokCancel);

    SerialNumberDialog->show();

}

void WorkOrderQuotes::setSerialNumber(){

    int Empid = CompletedByCombo->currentData().toInt();
    QDate fabricationDate = CompletedDate->date();

    QString idString = QString("%1").arg(Empid, 3, 10, QChar('0'));
    QString fabDate = fabricationDate.toString("yy:MM").replace(":","") ;
    QString voucherNo = QString("%1").arg(QString::number(Voucher.InventoryItems[glindex].BaseItem.requirementItemIdOld),QChar('5'));

    QString finalString = idString+fabDate+voucherNo+QString::number(Voucher.InventoryItems[glindex].BaseItem.quantity);

    SerialNumber->setText(finalString);


}

void WorkOrderQuotes::printSerialButtonClicked()
{

}

void WorkOrderQuotes::OkButtonClicked()
{
    if(SerialNumber->text().length()>3){
        SerialNumberDialog->close();
        int r = glindex;
        qDebug()<<r;
        Voucher.InventoryItems[r].BaseItem.itemProductionStatus = 1;
        Voucher.InventoryItems[r].BaseItem.SerailNumber = SerialNumber->text();

//        dbHelper->setProductionStatus(1,Voucher.InventoryItems[r].BaseItem.requirementItemIdOld,
//                                      Voucher.InventoryItems[r].BaseItem.quantity,Voucher.voucherNumber);

//        dbHelper->setSerialNumber(Voucher.InventoryItems[r].BaseItem.SerailNumber,
//                                  Voucher.InventoryItems[r].BaseItem.requirementItemIdOld,
//                                  Voucher.InventoryItems[r].BaseItem.quantity,Voucher.voucherNumber);


        bool f=true;
        int cnt =0;
        int socnt = 0;

        for(CompoundItemDataObject item:SalesOrderVoucher.InventoryItems){
            if(item.BaseItem.requirementItemIdOld == Voucher.InventoryItems[r].BaseItem.requirementItemIdOld){
                socnt = cnt;
                int qty = item.BaseItem.quantity;
                for(int k =0;k<qty;k++){
                    int statuss =0;
//                    dbHelper->getProductionStatus(Voucher.InventoryItems[r].BaseItem.requirementItemIdOld,
//                                                                Voucher.InventoryItems[r].BaseItem.quantity,Voucher.voucherNumber);
                    if(statuss == 0){
                        f = false;
                    }
                }
            }
            cnt++;
        }

        if(f){
            dbHelper->setItemProductionStatus
                    (1,QString::number(SalesOrderVoucher.InventoryItems[socnt].BaseItem.requirementItemIdOld));
        }
        else{
            dbHelper->setItemProductionStatus
                    (0,QString::number(SalesOrderVoucher.InventoryItems[socnt].BaseItem.requirementItemIdOld));

        }


        settable();
    }

}

void WorkOrderQuotes::CancelButtonClicked()
{
    SerialNumberDialog->close();
    int r = glindex;
    Voucher.InventoryItems[r].BaseItem.itemProductionStatus = 0;


//    dbHelper->setProductionStatus(0,Voucher.InventoryItems[r].BaseItem.requirementItemIdOld,
//                                  Voucher.InventoryItems[r].BaseItem.quantity,Voucher.voucherNumber);


//    dbHelper->setItemProductionStatus
//            (0,QString::number(Voucher.InventoryItems[r].BaseItem.requirementItemIdOld));


    settable();
}

void WorkOrderQuotes::checkToggle(bool val)
{
    QToolButton* butt = qobject_cast <QToolButton*> (sender());
    qDebug()<<"Value = "<<val;

    int r = butt->property("id").toInt();
    QComboBox* com =qobject_cast<QComboBox*> (ui->tableWidget->cellWidget(r,6));

    if(val && com->currentText().length()>0)
    {
        if(QMessageBox::Yes == QMessageBox::question(this,"QA Complted",
                                                     "Are you Sure Q&A is completed for\nthe Selected Item",
                                                     QMessageBox::Yes|QMessageBox::No))
        {
            showSerialDialog(r);

            glindex = r;
            Voucher.InventoryItems[r].BaseItem.itemProductionStatus = 1;
            butt->setText("");
            //        butt->setStyleSheet("background-color:#05ff04");
            qDebug()<<"Value = "<<butt->isChecked();
            butt->setIcon(QIcon(":/icons/check.ico"));
            butt ->setIconSize(QSize(16,16));
            butt ->setToolButtonStyle(Qt::ToolButtonIconOnly);
            butt ->setAutoRaise(true);
            butt ->setProperty("check",1);
            qDebug()<<"Chenged check to 1";
            qDebug()<<"New Value"<<butt->property("check").toInt();
            dbHelper->setItemProductionStatus
                    (1,QString::number(Voucher.InventoryItems[r].BaseItem.requirementItemIdOld));
        }
    }
    else{
        Voucher.InventoryItems[r].BaseItem.itemProductionStatus = 0;
        butt->setStyleSheet("background-color:#ececec");
        if(com->currentText().length()>0){
            butt->setText("Assigned");
        }

        else{
            butt->setText("Unassigned");
            QMessageBox box;box.setText("Assign First!!!"); box.exec();
        }
        butt->blockSignals(true);
        butt->setChecked(false);
        butt->blockSignals(false);
        butt ->setToolButtonStyle(Qt::ToolButtonTextOnly);
        butt->setIcon(QIcon(":/iconxs/"));
        butt->setProperty("check",0);
        dbHelper->setItemProductionStatus
                (0,QString::number(Voucher.InventoryItems[r].BaseItem.requirementItemIdOld));
        qDebug()<<"Chenged check to 0";
    }

}
