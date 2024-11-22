#include "requirementslist.h"
#include "ui_requirementslist.h"

RequirementsList::RequirementsList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequirementsList)
{
    ui->setupUi(this);
    dbhelper = new RequirementsDatabaseHelper();
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
    ui->tableWidget->setColumnCount(6);

    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* dateItem = new QTableWidgetItem;
    dateItem->setText("Date");
    ui->tableWidget->setHorizontalHeaderItem(i++,dateItem);

    QTableWidgetItem* LedgerName = new QTableWidgetItem;
    LedgerName->setText("Ledger");
    ui->tableWidget->setHorizontalHeaderItem(i++,LedgerName);

    QTableWidgetItem* ReqNo = new QTableWidgetItem;
    ReqNo->setText("No.");
    ui->tableWidget->setHorizontalHeaderItem(i++,ReqNo);

    QTableWidgetItem* Amount = new QTableWidgetItem;
    Amount->setText("Amount");
    ui->tableWidget->setHorizontalHeaderItem(i++,Amount);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

    QTableWidgetItem* check = new QTableWidgetItem;
    check->setIcon(QIcon(":/icons/trash.ico"));
    ui->tableWidget->setHorizontalHeaderItem(i++,check);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(5,25);

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->setShowGrid(false);
    setTable();
}

RequirementsList::~RequirementsList()
{
    delete ui;
}

void RequirementsList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void RequirementsList::on_createNewPushButton_clicked()
{
//    PrinterHelper *pHelper = new PrinterHelper;
//    GeneralVoucherDataObject obj;
//    pHelper->PrintInventoryVoucher(obj);
    //pHelper->printDummy();
    RequirementWidget = new Requirement(this);
    RequirementWidget->setWindowFlags(Qt::Window);
    RequirementWidget->showMaximized();
    QObject::connect(RequirementWidget,SIGNAL(closing()),this,SLOT(setTable()));
}


void RequirementsList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = ui->tableWidget->item(index.row(),0)->text().toInt()-1;
    RequirementWidget = new Requirement(this);
    RequirementWidget->setWindowFlags(Qt::Window);
    RequirementWidget->showVoucher(voucherList[r]);
    RequirementWidget->show();
    QObject::connect(RequirementWidget,SIGNAL(closing()),this,SLOT(setTable()));

}

void RequirementsList::setTable()
{
    QDate dateFrom = ui->fromDateDateEdit->date();
    QDate dateTo = ui->toDateDateEdit->date();
    voucherList = dbhelper->getVoucherListByDate(dateFrom,dateTo);
    qDebug()<<"Size = "<<voucherList.size();
    int cnt =0;
    int i=0;
    ui->tableWidget->setRowCount(0);
    for(GeneralVoucherDataObject obj:voucherList){
        ui->tableWidget->insertRow(cnt);
        i=0;
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        SLNO->setFlags(SLNO->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,SLNO);


        QTableWidgetItem* dateItem = new QTableWidgetItem;
        dateItem->setText(obj.VoucherDate.toString("dd-MM-yyyy"));
        dateItem->setFlags(dateItem->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,dateItem);

        QTableWidgetItem* LedgerName = new QTableWidgetItem;
        LedgerName->setText(obj.ledgerObject.LedgerName);
        LedgerName->setFlags(LedgerName->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,LedgerName);

        QTableWidgetItem* ReqNo = new QTableWidgetItem;
        ReqNo->setText(obj.voucherNumber);
        ReqNo->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);

        ReqNo->setFlags(ReqNo->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,ReqNo);

        QTableWidgetItem* Amount = new QTableWidgetItem;
        Amount->setText(QString::number(obj.grandTotal,'f',2));
        Amount->setTextAlignment(Qt::AlignRight |Qt::AlignVCenter);
        Amount->setFlags(Amount->flags()^ Qt::ItemIsEditable);
        ui->tableWidget->setItem(cnt,i++,Amount);

        QToolButton* delButton = new QToolButton;
        delButton->setIcon(QIcon(":/icons/trash.ico"));
        delButton->setProperty("row",cnt);
        delButton->setAutoRaise(true);
        //deleteItem->setIcon(QIcon(":/icons/trash.ico"));
        ui->tableWidget->setCellWidget(cnt,i++,delButton);

        QCheckBox* chk = new QCheckBox;
        ui->tableWidget->setCellWidget(cnt,i++,chk);

        QObject::connect(delButton,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        cnt++;
    }
}

void RequirementsList::on_okPushButton_clicked()
{
    setTable();
}

void RequirementsList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void RequirementsList::on_importButton_clicked()
{
    QList<GeneralVoucherDataObject> vouchers;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        QCheckBox* chk =  qobject_cast<QCheckBox*>( ui->tableWidget->cellWidget(i,ui->tableWidget->columnCount()-1));
        if(chk->isChecked()){
            vouchers.append(dbhelper->getVoucherByVoucherNo(voucherList[ui->tableWidget->item(i,0)->text().toInt()-1].voucherNumber));
        }
    }
}
