#include "shiftvalueseditor.h"
#include "ui_shiftvalueseditor.h"

#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QScreen>

ShiftValuesEditor::ShiftValuesEditor(ShiftDataModel *shift, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShiftValuesEditor)
{
    ui->setupUi(this);

    this->setObjectName("shiftClosingValues");
    this->setStyleSheet("QWidget#shiftClosingValues{background-color:#e0f7fa}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    dbHelper = new RestaurantShiftDatabaseHelper;
    this->shift = shift;

    setWidgets();
    //    initExtraActions();
    setTable();
    setExpenseTable();
}

ShiftValuesEditor::~ShiftValuesEditor()
{
    delete ui;
}

void ShiftValuesEditor::setShift(ShiftDataModel *value)
{
    shift = value;
}


void ShiftValuesEditor::setWidgets()
{
    vouchersView = new ReportsTableView;
    shiftTable = new QTableWidget;

    shiftTable->setFixedHeight(300);
    expTable = new QTableWidget;
    expTable->setFixedHeight(100);
    shiftTable->setStyleSheet(
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

    expTable->setStyleSheet(
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

    shiftTable->setColumnCount(4);
    shiftTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(shiftTable, &QTableWidget::doubleClicked, this, &ShiftValuesEditor::showVouchersByLedger );
    expTable->setColumnCount(4);

    int i=0;
    QTableWidgetItem *Ledger = new QTableWidgetItem;
    Ledger->setText("Ledger");
    shiftTable->setHorizontalHeaderItem(i++,Ledger);


    QTableWidgetItem* system = new QTableWidgetItem;
    system->setText("System");
    shiftTable->setHorizontalHeaderItem(i++,system);


    QTableWidgetItem* actual = new QTableWidgetItem;
    actual->setText("Actual");
    shiftTable->setHorizontalHeaderItem(i++,actual);

    QTableWidgetItem* diff = new QTableWidgetItem;
    diff->setText("Difference");
    shiftTable->setHorizontalHeaderItem(i++,diff);

    shiftTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    shiftTable->verticalHeader()->hide();

    expTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    expTable->verticalHeader()->hide();
    expTable->horizontalHeader()->hide();

    QLabel* l1 = new QLabel();
    l1->setText("Sales By Ledger");
    l1->setFont(QFont("",20));
    QLabel* l2 = new QLabel();
    l2->setText("Expenses");
    l2->setFont(QFont("",20));

    l1->setMaximumHeight(30);
    l2->setMaximumHeight(30);

    ui->verticalLayout->addWidget(l1,1);
    ui->verticalLayout->addWidget(shiftTable,5);
    ui->verticalLayout->addWidget(l2,1);
    ui->verticalLayout->addWidget(expTable,3);



//        ui->verticalLayout->addStretch(10);

}


void ShiftValuesEditor::setTable()
{
    qDebug()<<Q_FUNC_INFO;

//    allLedgersModel = dbHelper->AllLedgerSummary(shift, false);
    shift->ledgers = dbHelper->getSalesByLedgersList(shift);
    qDebug()<<shift->ledgers.size();

    int i, row;

    shiftTable->setRowCount(0);


    for( row=0; row<shift->ledgers.size(); row++){

        ShiftLedgersDataModel *led = shift->ledgers[row];
        shiftTable->insertRow(row);
        i=0;

        qDebug()<<"adding : "<<led->ledgerName<<led->systemAmount;

        QTableWidgetItem* Ledger = new QTableWidgetItem;
        Ledger->setText(led->ledgerName);
        Ledger->setFlags(Ledger->flags() ^ Qt::ItemIsEditable);
        shiftTable->setItem(row,i++,Ledger);

        QTableWidgetItem* system = new QTableWidgetItem;
        float systemVal = led->systemAmount;
        system->setText(QString::number(systemVal, 'f',2));
        system->setFlags(system->flags() ^ Qt::ItemIsEditable);
        system->setTextAlignment(Qt::AlignRight);
        shiftTable->setItem(row,i++,system);

        QLineEdit *actualValue = new QLineEdit;
        actualValue->setValidator(new QDoubleValidator(0.0000,999999,2,this));
        actualValue->setFrame(QFrame::NoFrame);
        actualValue->setProperty("row",row);
        actualValue->setAlignment(Qt::AlignRight);
        shiftTable->setCellWidget(row,i++,actualValue);
        connect(actualValue, &QLineEdit::returnPressed, this, [=](){
            int row1= sender()->property("row").toInt();
            shift->ledgers[row1]->actualAmount =  qobject_cast<QLineEdit*>(shiftTable->cellWidget(row1,2))->text().toFloat();
            float diff = shift->ledgers[row1]->systemAmount  -
                   shift->ledgers[row1]->actualAmount;

            QTableWidgetItem *diffValue = new QTableWidgetItem;
            diffValue->setText(QString::number(diff, 'f',2));
            diffValue->setFlags(diffValue->flags() ^ Qt::ItemIsEditable);
            diffValue->setTextAlignment(Qt::AlignRight);
            shiftTable->setItem(row1,3,diffValue);
            calculateTotals();

        });

        QTableWidgetItem *diffValue = new QTableWidgetItem;
        shiftTable->setItem(row,i++,diffValue);

    }

    // Shift total row
    row = shiftTable->rowCount();
    i=0;
    qDebug()<<"tab wid row"<<shiftTable->rowCount();
    qDebug()<<"row"<<row;
    shiftTable->insertRow(row);
    qDebug()<<"inserted row"<<shiftTable->rowCount();
    QTableWidgetItem* Ledger = new QTableWidgetItem;
    Ledger->setText("Total");
    Ledger->setFlags(Ledger->flags() ^ Qt::ItemIsEditable);
    shiftTable->setItem(row,i++,Ledger);

    QTableWidgetItem* system = new QTableWidgetItem;
    system->setText(QString::number(systemTotal, 'f',2));
    system->setFlags(system->flags() ^ Qt::ItemIsEditable);
    system->setTextAlignment(Qt::AlignRight);
    shiftTable->setItem(row,i++,system);

    QTableWidgetItem* actual = new QTableWidgetItem;
    actual->setText(QString::number(actualTotal, 'f',2));
    actual->setFlags(actual->flags() ^ Qt::ItemIsEditable);
    actual->setTextAlignment(Qt::AlignRight);
    shiftTable->setItem(row,i++,actual);

    QTableWidgetItem* diff = new QTableWidgetItem;
    diff->setTextAlignment(Qt::AlignRight);
    diff->setText(QString::number(diffTotal, 'f',2));
    diff->setFlags(diff->flags() ^ Qt::ItemIsEditable);
    shiftTable->setItem(row,i++,diff);

//    shiftTable->setFixedHeight(30 * (shiftTable->rowCount() + 1));

    calculateTotals();

}

void ShiftValuesEditor::setExpenseTable()
{
    expTable->setRowCount(0);
    int row = 0;
    expTable->insertRow(row);
    qDebug()<<"tab wid row"<<expTable->rowCount();
    int i=0;
    QTableWidgetItem* Ledger = new QTableWidgetItem;
    Ledger->setText("Cash Payment");
    Ledger->setFlags(Ledger->flags() ^ Qt::ItemIsEditable);
    expTable->setItem(row,i++,Ledger);

    QTableWidgetItem* system = new QTableWidgetItem;
    float systemVal = dbHelper->getPaymentByCash(shift) ;
    system->setText(QString::number(systemVal, 'f',2));
    system->setFlags(system->flags() ^ Qt::ItemIsEditable);
    system->setTextAlignment(Qt::AlignRight);
    expTable->setItem(row,i++,system);

    QLineEdit *actualValue = new QLineEdit;
    actualValue->setValidator(new QDoubleValidator(0.0000,999999,2,this));
    actualValue->setFrame(QFrame::NoFrame);
    actualValue->setProperty("row",row);
    actualValue->setAlignment(Qt::AlignRight);
    expTable->setCellWidget(row,i++,actualValue);

    connect(actualValue, &QLineEdit::returnPressed, this, [=](){
        int row= sender()->property("row").toInt();
        float diff = systemVal  -
                qobject_cast<QLineEdit*>(expTable->cellWidget(row,2))->text().toFloat();

        QTableWidgetItem *diffValue = new QTableWidgetItem;
        diffValue->setText(QString::number(diff, 'f',2));
        diffValue->setFlags(diffValue->flags() ^ Qt::ItemIsEditable);
        diffValue->setTextAlignment(Qt::AlignRight);
        expTable->setItem(row,3,diffValue);

    });

    row ++;
    expTable->insertRow(row);
    qDebug()<<"tab wid row"<<expTable->rowCount();
    i=0;
    QTableWidgetItem* Ledger1 = new QTableWidgetItem;
    Ledger1->setText("Cash Purchase");
    Ledger1->setFlags(Ledger1->flags() ^ Qt::ItemIsEditable);
    expTable->setItem(row,i++,Ledger1);

    QTableWidgetItem* System1 = new QTableWidgetItem;
    float System1Val = dbHelper->getPurchaseByCash(shift) ;
    System1->setText(QString::number(System1Val, 'f',2));
    System1->setFlags(System1->flags() ^ Qt::ItemIsEditable);
    System1->setTextAlignment(Qt::AlignRight);
    expTable->setItem(row,i++,System1);

    QLineEdit *actual1Value = new QLineEdit;
    actual1Value->setValidator(new QDoubleValidator(0.0000,999999,2,this));
    actual1Value->setFrame(QFrame::NoFrame);
    actual1Value->setProperty("row",row);
    actual1Value->setAlignment(Qt::AlignRight);
    expTable->setCellWidget(row,i++,actual1Value);

    connect(actual1Value, &QLineEdit::returnPressed, this, [=](){
        int row= sender()->property("row").toInt();
        float diff = System1Val  -
                qobject_cast<QLineEdit*>(expTable->cellWidget(row,2))->text().toFloat();

        QTableWidgetItem *diffValue = new QTableWidgetItem;
        diffValue->setText(QString::number(diff, 'f',2));
        diffValue->setFlags(diffValue->flags() ^ Qt::ItemIsEditable);
        diffValue->setTextAlignment(Qt::AlignRight);
        expTable->setItem(row,3,diffValue);

    });

    row++;

    expTable->insertRow(row);
    qDebug()<<"tab wid row"<<expTable->rowCount();
    i=0;
    QTableWidgetItem* ledger2 = new QTableWidgetItem;
    ledger2->setText("Cash Receipts");
    ledger2->setFlags(ledger2->flags() ^ Qt::ItemIsEditable);
    expTable->setItem(row,i++,ledger2);

    QTableWidgetItem* system2 = new QTableWidgetItem;
    float system2Val = dbHelper->getReceiptByCash(shift) ;
    system2->setText(QString::number(system2Val, 'f',2));
    system2->setFlags(system2->flags() ^ Qt::ItemIsEditable);
    system2->setTextAlignment(Qt::AlignRight);
    expTable->setItem(row,i++,system2);

    QLineEdit *actual2Value = new QLineEdit;
    actual2Value->setValidator(new QDoubleValidator(0.0000,999999,2,this));
    actual2Value->setFrame(QFrame::NoFrame);
    actual2Value->setProperty("row",row);
    actual2Value->setAlignment(Qt::AlignRight);
    expTable->setCellWidget(row,i++,actual2Value);

    connect(actual2Value, &QLineEdit::returnPressed, this, [=](){
        int row= sender()->property("row").toInt();
        float diff = system2Val  -
                qobject_cast<QLineEdit*>(expTable->cellWidget(row,2))->text().toFloat();

        QTableWidgetItem *diffValue = new QTableWidgetItem;
        diffValue->setText(QString::number(diff, 'f',2));
        diffValue->setFlags(diffValue->flags() ^ Qt::ItemIsEditable);
        diffValue->setTextAlignment(Qt::AlignRight);
        expTable->setItem(row,3,diffValue);

    });

}

void ShiftValuesEditor::calculateTotals()
{
    int row = shiftTable->rowCount()-1;

    diffTotal=0;
    actualTotal =0;
    systemTotal = 0;

    qDebug()<<Q_FUNC_INFO<<row;
    for( int j=0; j<shift->ledgers.size(); j++){
        qDebug()<<"in loop"<<j;

        systemTotal += shift->ledgers[j]->systemAmount;
        qDebug()<<systemTotal;
        actualTotal += shift->ledgers[j]->actualAmount;
        qDebug()<<actualTotal;
        //       diffTotal += qobject_cast<QLineEdit*>(shiftTable->cellWidget(j,3))->text().toFloat();
        diffTotal = systemTotal - actualTotal;
        qDebug()<<diffTotal;

    }

    qDebug()<<"calculated values"<<systemTotal<<actualTotal<<diffTotal;

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(QString::number(systemTotal, 'f', 2));
    item->setTextAlignment(Qt::AlignRight);
    shiftTable->setItem(row, 1, item);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QString::number(actualTotal, 'f', 2));
    item1->setTextAlignment(Qt::AlignRight);
    shiftTable->setItem(row, 2, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(QString::number(diffTotal, 'f', 2));
    item2->setTextAlignment(Qt::AlignRight);
    shiftTable->setItem(row, 3, item2);


    //    shiftTable->removeRow(shiftTable->rowAt(row--));

    qDebug()<<"end of func";

}

void ShiftValuesEditor::showVouchersByLedger(QModelIndex index)
{
    int row =     index.row();
    if(row == shiftTable->rowCount()-1)
        return;
    QString ledgerID = shift->ledgers[row]->ledgerID;;
    qDebug()<<"ledger selected"<<ledgerID;

    if(vouchersView != nullptr)
    {
        delete vouchersView;
    }

    QWidget *widget = new QWidget;
    QVBoxLayout *Layout = new QVBoxLayout;
    widget->setLayout(Layout);

    QLabel* l1 = new QLabel();
    l1->setText("Vouchers By Ledger");
    l1->setFont(QFont("Roboto",20));

    vouchersView = new ReportsTableView;
    Layout->addWidget(l1);
    Layout->addWidget(vouchersView);
    //    vouchersView->hide();

    model = dbHelper->getVouchersByLedger(shift, ledgerID);
    vouchersView->setAvlcols(QList<int> {1});
    vouchersView->setModel(model);
    vouchersView->setModelQry(model);
    qDebug()<<"tabe size"<<vouchersView->size()<<model->rowCount();
    //    vouchersView->lastRow->hide();
    vouchersView->setSortingEnabled(true);
    vouchersView->setColumns();

    vouchersView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    vouchersView->show();
    vouchersView->setFixedHeight((model->rowCount()+2)*30);

    connect(vouchersView, &QTableView::doubleClicked, this, [=](QModelIndex index){
        int row = index.row();
        showModeOfPay(model->record(row).value(0).toString(), shift->voucherPrefix);
    });

    widget->setFixedHeight(vouchersView->height() + 100);

    widget->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, widget->size(),
                                          qApp->primaryScreen()->geometry()));
    widget->setWindowFlag(Qt::Popup);
    widget->show();

}

void ShiftValuesEditor::showModeOfPay(QString vNo , QString vPrefix)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Switch Mode Of Pay","Are you sure you want to switch Mode Of Pay for Voucher : " + vNo +"?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        salesVoucherDatabaseHelper *voucherHelper = new salesVoucherDatabaseHelper;

        GeneralVoucherDataObject *voucher = voucherHelper->getVoucherByVoucherNoPtr(vNo, vPrefix);

        ModeOfPay* ModeOfPayWidget = new ModeOfPay(voucher,new LedgerMasterDatabaseHelper(),new SalesOrderDatabaseHelper(),true);

        //ModeOfPayWidget->hideDiscounts();

        ModeOfPayWidget->show();
        QObject::connect(ModeOfPayWidget,&ModeOfPay::modeOfPaySignal,this,[=](){
            TransactionCalculator::VoucherCalculateSales(voucher);
            voucherHelper->updateVoucher(voucher,voucher->voucherNumber);
            CounterPrinterHelper pHelper;
            pHelper.printSalesVoucher(voucher);
            vouchersView->hide();
            setTable();
        });
    }


}
