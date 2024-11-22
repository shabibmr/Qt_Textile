#include "profitandlossreport.h"
#include "ui_profitandlossreport.h"

ProfitAndLossReport::ProfitAndLossReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProfitAndLossReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());


    ledHelper = new AccountingReportDatabaseHelper();
    grpHelper = new AccountGroupMasterDatabaseHelper();

    ui->rightTotallineEdit->setFrame(false);
    ui->rightTotallineEdit->setReadOnly(true);
    ui->rightTotallineEdit->setAlignment(Qt::AlignRight);
    ui->leftTotallineEdit->setAlignment(Qt::AlignRight);
    ui->leftTotallineEdit->setFrame(false);
    ui->leftTotallineEdit->setReadOnly(true);

    //    ui->rightTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->rightTableWidget->verticalHeader()->hide();
    ui->rightTableWidget->horizontalHeader()->hide();
    ui->rightTableWidget->setAlternatingRowColors(true);
    ui->rightTableWidget->setShowGrid(false);

    //    ui->leftTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->leftTableWidget->verticalHeader()->hide();
    ui->leftTableWidget->horizontalHeader()->hide();
    ui->leftTableWidget->setAlternatingRowColors(true);
    ui->leftTableWidget->setShowGrid(false);

    ui->rightTableWidget->setColumnCount(2);
    ui->leftTableWidget->setColumnCount(2);

    ui->rightTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->leftTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    //    setTableView();
    QDate toDate = ui->toDateDateEdit->date();
    QDate fromDate = ui->fromDateDateEdit->date();


    setTable();
}

ProfitAndLossReport::~ProfitAndLossReport()
{
    delete ui;
}

void ProfitAndLossReport::on_OKButton_clicked()
{
    setTable();
}

void ProfitAndLossReport::on_excelButton_clicked()
{

}

void ProfitAndLossReport::on_printButton_clicked()
{

}

void ProfitAndLossReport::on_backButton_clicked()
{

}


void ProfitAndLossReport::setTable()
{
    qDebug()<<"set Table";

    ui->leftTableWidget->setRowCount(0);
    ui->rightTableWidget->setRowCount(0);
    lrow=0;
    rrow=0;
    ltotal=0;
    rtotal=0;

    QDate toDate = ui->toDateDateEdit->date();
    QDate fromDate = ui->fromDateDateEdit->date();
    qDebug()<<toDate<<fromDate;


    QList<AccountGroupDataModel> incomeGrp = grpHelper->getPrimaryAccountsByGroupType("Income");
    QList<AccountGroupDataModel> expenseGrp = grpHelper->getPrimaryAccountsByGroupType("Expense");

    clsStkValue =  ledHelper->getRestaurantStockValueByDate(toDate);
    openStkValue = ledHelper->getRestaurantStockValueByDate(fromDate.addDays(-1));

    addLTableEntry("Opening Stock", QString::number(openStkValue,'f',2));
    for(AccountGroupDataModel acc:expenseGrp)
    {
        if(acc.groupID != "0x12"){
            float Value = -1 * ledHelper->getBalanceByGroup(acc.groupID, fromDate, toDate);
            if(Value != 0)
                addLTableEntry(acc.groupName, QString::number(Value,'f',2));
        }
    }

    for(AccountGroupDataModel acc:incomeGrp)
    {
        if(acc.groupID != "0x11"){
            float Value = ledHelper->getBalanceByGroup(acc.groupID, fromDate, toDate);
            if(Value != 0)
                addRTableEntry(acc.groupName, QString::number(Value,'f',2));
        }
    }

    addRTableEntry("Closing Stock", QString::number(clsStkValue,'f',2));

    gp = rtotal - ltotal;
    qDebug()<<"total"<<rtotal<<ltotal;
    if(gp >= 0){
        addLTableEntry("Gross Profit c/o", QString::number(gp,'f',2));
        while(rrow<lrow)
            addRTableEntry(" ","");
        addLTableEntry(" ", QString::number(ltotal,'f',2));
        addRTableEntry(" ", QString::number(rtotal,'f',2));
        addRTableEntry("Gross Profit b/f", QString::number(gp,'f',2));
    }
    else{
        addRTableEntry("Gross Loss c/o", QString::number(-1 *gp,'f',2));
        while(lrow<rrow)
            addLTableEntry(" ", "");
        addLTableEntry(" ", QString::number(ltotal,'f',2));
        addRTableEntry(" ", QString::number(rtotal,'f',2));
        addLTableEntry("Gross Loss b/f",QString::number(-1 * gp,'f',2));
    }

    float indIncome = ledHelper->getBalanceByGroup("0x11", fromDate, toDate);
    if(indIncome != 0)
        addRTableEntry("Indirect Incomes", QString::number(indIncome,'f',2));
    float indExpense = ledHelper->getBalanceByGroup("0x12", fromDate, toDate);
    if(indExpense != 0)
        addLTableEntry("Indirect Expenses", QString::number(-1 * indExpense,'f',2));

    qDebug()<<"row num"<<lrow<<rrow;
//    if(lrow < rrow)
//        while(lrow<rrow)
//            addLTableEntry(" ", "");
//    else if(rrow < lrow)
//        while(rrow<lrow)
//            addRTableEntry(" ", "");

    float diff = rtotal - ltotal;
    if(diff >=0)
        addLTableEntry("Nett Profit", QString::number(diff,'f',2));
    else
        addRTableEntry("Nett Loss", QString::number(-1 * diff,'f',2));



    ui->rightTotallineEdit->setText(QString::number(rtotal,'f',2));
    ui->leftTotallineEdit->setText(QString::number(ltotal,'f',2));

}

void ProfitAndLossReport::addLTableEntry(QString item, QString val)
{
    qDebug()<<"Add row left";
    ui->leftTableWidget->insertRow(lrow);
    QTableWidgetItem* group = new QTableWidgetItem;
    group->setText(item);
    group->setFlags(group->flags() ^ Qt::ItemIsEditable);
    ui->leftTableWidget->setItem(lrow,0,group);

    QTableWidgetItem* value = new QTableWidgetItem;
    value->setText(val);
    value->setTextAlignment(Qt::AlignRight);
    value->setFlags(value->flags() ^ Qt::ItemIsEditable);
    ui->leftTableWidget->setItem(lrow,1,value);

    ltotal += val.toFloat();
    lrow++;

}

void ProfitAndLossReport::addRTableEntry(QString item, QString val)
{
    ui->rightTableWidget->insertRow(rrow);
    QTableWidgetItem* group = new QTableWidgetItem;
    group->setText(item);
    group->setFlags(group->flags() ^ Qt::ItemIsEditable);
    ui->rightTableWidget->setItem(rrow,0,group);

    QTableWidgetItem* value = new QTableWidgetItem;
    value->setText(val);
    value->setTextAlignment(Qt::AlignRight);
    value->setFlags(value->flags() ^ Qt::ItemIsEditable);
    ui->rightTableWidget->setItem(rrow,1,value);

    rtotal += val.toFloat();
    rrow++;

}

void ProfitAndLossReport::on_leftTableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString group = ui->leftTableWidget->item(index.row(),0)->text();
    float oldValue = ui->leftTableWidget->item(index.row(),1)->text().toFloat();
    if(group == "Opening Stock"){
        bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Opening Stock:"), QLineEdit::Normal,
                                             "0", &ok);
        if (ok && !text.isEmpty())
        {
            openStkValue =text.toFloat();
            setTable();
        }
    }
}

void ProfitAndLossReport::on_rightTableWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString group = ui->rightTableWidget->item(index.row(),0)->text();
    float oldValue = ui->rightTableWidget->item(index.row(),1)->text().toFloat();
    if(group == "Closing Stock"){
        bool ok;
        QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Closing Stock:"), QLineEdit::Normal,
                                             "0", &ok);
        if (ok && !text.isEmpty())
        {
            clsStkValue =text.toFloat();
            setTable();
        }
    }
}
