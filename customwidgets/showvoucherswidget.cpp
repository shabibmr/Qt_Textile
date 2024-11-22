#include "showvoucherswidget.h"
#include "ui_showvoucherswidget.h"

ShowVouchersWidget::ShowVouchersWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowVouchersWidget)
{
    ui->setupUi(this);

    dbHelper = new GeneralVoucherDatabaseHelper();

    this->setMinimumSize(400,600);

    //    ui->verticalLayout->addLayout(vLayout);

    sectionLabel = new QLabel("Selected Section : ");
    ui->sectionLay->addWidget(sectionLabel);
//    sectionCombo = new QComboBox;
//    sectionCombo->setEditable(true);
//    sectionCombo->hide();
    //    ui->sectionLay->addWidget(sectionCombo);
    //    QObject::connect(sectionCombo,SIGNAL(currentTextChanged(QString)),
    //                     this,SLOT(sectionSelected(QString)));

    ui->tableWidget->setColumnCount(3);
    fromDate = QDateTime::fromString("2000-01-01","yyyy-MM-dd");
    toDate = QDateTime::fromString("2000-01-01","yyyy-MM-dd");
    int i=0;

    QTableWidgetItem* BillNo = new QTableWidgetItem;
    BillNo->setText("Bill No.");
    ui->tableWidget->setHorizontalHeaderItem(i++,BillNo);

    QTableWidgetItem* Ledger = new QTableWidgetItem;
    Ledger->setText("Ledger");
    ui->tableWidget->setHorizontalHeaderItem(i++,Ledger);

    QTableWidgetItem* combo = new QTableWidgetItem;
    combo->setText("");
    ui->tableWidget->setHorizontalHeaderItem(i++,combo);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(2,25);
}

void ShowVouchersWidget::setShowWidget(bool value)
{
    showWidget = value;
}

ShowVouchersWidget::~ShowVouchersWidget()
{
    delete ui;
}

void ShowVouchersWidget::setVoucherTypes(int srcVType, int tgtVType)
{
    sourceVoucherType = srcVType;
    targetVoucherType = tgtVType;

    //    qDebug()<<"1 : "<<QDateTime::currentDateTime();
    //    vouchersList = dbHelper->getVoucherListToexportByType(sourceVoucherType, targetVoucherType, filter );
    //    qDebug()<<"2 : "<<QDateTime::currentDateTime();
    //    sectionsList = dbHelper->getPendingSectionToExportByType(sourceVoucherType, targetVoucherType);
    //    qDebug()<<"3 : "<<QDateTime::currentDateTime();
    //    setVouchersList();
    //    qDebug()<<"4 : "<<QDateTime::currentDateTime();
    //    if(targetVoucherType != GMVoucherTypes::WorkOrder){
    //        sectionLabel->hide();
    //        sectionCombo->hide();
    //        //        setSectionsList();
    //    }
    //    else{
    //        setSectionsList();
    //    }
    //    qDebug()<<"5 : "<<QDateTime::currentDateTime();

}

void ShowVouchersWidget::setLedger(QString ledgerId)
{
    qDebug()<<Q_FUNC_INFO;
    if(ledgerId.length() != 0){
        LedgerID = ledgerId;
        filter.insert("ledger", LedgerID);
        // vouchersList = dbHelper->getVoucherListToexportByDate(fromDate, toDate, sourceVoucherType, targetVoucherType, filter);
        vouchersList = dbHelper->getVoucherListToexportByType( sourceVoucherType,targetVoucherType,filter);
        if(vouchersList.size() == 0){
            qDebug()<<"No voucher";
            emit noVoucherToExport();
        }
        else{
            setVouchersList();
            if(showWidget)
                this->show();
        }
    }
}

void ShowVouchersWidget::setSection(QString section1)
{
    qDebug()<<Q_FUNC_INFO;
    if(section1 != ""){
        section = section1;
        filter.insert("section", section);
        qDebug()<<fromDate<<toDate<<"date";
        if(fromDate ==  QDateTime::fromString("2000-01-01","yyyy-MM-dd") && toDate ==  QDateTime::fromString("2000-01-01","yyyy-MM-dd"))
        {
            qDebug()<<Q_FUNC_INFO<<"first";
            vouchersList = dbHelper->getVoucherListToexportByType( sourceVoucherType,targetVoucherType,filter);

        }
        else
        {
            qDebug()<<Q_FUNC_INFO<<"second";
            vouchersList = dbHelper->getVoucherListToexportByDate(fromDate, toDate,sourceVoucherType, targetVoucherType, filter);
        }
        qDebug()<<fromDate<<toDate<<"date";
        //    sectionsList = dbHelper->getPendingSectionToExportByType(sourceVoucherType, targetVoucherType);

        if(vouchersList.size() == 0){
            qDebug()<<"NO VOUCHER";
            emit noVoucherToExport();
            //            this->close();
        }
        else{
            setVouchersList();
            if(showWidget)
                this->show();
        }
    }
    //    setSectionsList();
}

void ShowVouchersWidget::setDate(QDateTime fromDate, QDateTime toDate)
{

    this->fromDate = fromDate;
    this->toDate = toDate;

}

void ShowVouchersWidget::setVouchersList()
{
    qDebug()<<"set widget for vouchers to export"<<vouchersList.size();
    int cnt=0;
    int i;

    ui->tableWidget->setRowCount(0);

    //    while (auto item = vLayout->takeAt(0)) {
    //        delete item->widget();
    //    }

    for(GeneralVoucherDataObject *v:vouchersList){

        qDebug()<<"Add voucher"<<v->voucherType<<v->voucherNumber<<v->VoucherPrefix;
        ui->tableWidget->insertRow(cnt);
        i=0;

        QTableWidgetItem* BillNo = new QTableWidgetItem;
        BillNo->setText(v->voucherNumber);
        BillNo->setFlags(BillNo->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,BillNo);

        QTableWidgetItem* Ledger = new QTableWidgetItem;
        Ledger->setText(v->ledgerObject.LedgerName);
        Ledger->setFlags(BillNo->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Ledger);

        QCheckBox *checkBox = new QCheckBox;
        checkBox->setProperty("row",cnt);
        //        checkBox->setText(v->voucherNumber);
        //        checkBox->show();
        //        vLayout->addWidget(checkBox);
        ui->tableWidget->setCellWidget(cnt,i++,checkBox);
        QObject::connect(checkBox, SIGNAL(stateChanged(int)), this,SLOT(selectVoucher(int)));
        cnt++;
    }

    //    ui->verticalLayout->addLayout(vLayout);

}

void ShowVouchersWidget::setSectionsList()
{
//    sectionCombo->clear();
//    sectionCombo->addItem("Select");
//    sectionCombo->addItems(sectionsList.values());
//    sectionCombo->show();
    //    QObject::connect(sectionCombo,SIGNAL(currentTextChanged(QString)),
    //                     this,SLOT(sectionSelected(QString)));
}

void ShowVouchersWidget::selectVoucher(int state)
{
    int pos = sender()->property("row").toInt();
    //    qDebug()<<"selected"<<pos;
    //    qDebug()<<state;

    if(state == 2){
        vouchersList[pos]->voucherToExport=true;
    }
    else{
        vouchersList[pos]->voucherToExport=false;
    }

}

void ShowVouchersWidget::on_ImportPushButton_clicked()
{
    qDebug()<<Q_FUNC_INFO;
    GeneralVoucherDataObject *importedVoucher = new GeneralVoucherDataObject;
    importedVoucher->toGodownID = section;
    for(GeneralVoucherDataObject *v:vouchersList){
        if(v->voucherToExport){
            qDebug()<<"Selected:*********"<<v->voucherNumber;
            importedVoucher->InventoryItems.append(v->InventoryItems);
        }
    }
    qDebug()<<Q_FUNC_INFO<<__LINE__<<importedVoucher->InventoryItems.size();

    if(importedVoucher->InventoryItems.size() == 0)
        emit noVoucherToExport();
    else
        emit importVoucher(importedVoucher);

    qDebug()<<Q_FUNC_INFO<<"emit";
    this->close();

}

void ShowVouchersWidget::on_CancelPushButton_clicked()
{
    this->close();
}

void ShowVouchersWidget::sectionSelected(QString text)
{
    qDebug()<<"selected section"<<text;
    //    section = text;
//    section = sectionsList.key(sectionCombo->currentText());
//    filter.insert("section", section);
//    vouchersList = dbHelper->getVoucherListToexportByType(sourceVoucherType, targetVoucherType, filter);
//    setVouchersList();
    //    setSectionsList();
}

void ShowVouchersWidget::selectAllVouchers()
{
    qDebug()<<Q_FUNC_INFO;
    for(int i=0;i<vouchersList.size(); i++)
    {
        vouchersList[i]->voucherToExport=true;

    }
}

void ShowVouchersWidget::import()
{
    qDebug()<<Q_FUNC_INFO;
    on_ImportPushButton_clicked();
}
