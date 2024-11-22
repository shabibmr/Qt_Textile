#include "trialbalance.h"
#include "ui_trialbalance.h"

TrialBalance::TrialBalance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrialBalance)
{
    ui->setupUi(this);
    ledhelp = new AccountingReportDatabaseHelper;
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    AccountGroupMasterDatabaseHelper* accHelper = new AccountGroupMasterDatabaseHelper;
    accGroups = accHelper->getPrimaryAccounts();

    ui->tableView->verticalHeader()->hide();
    ui->tableView->setShowGrid(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setItemDelegate(new ReportCommonDelegate);
    ui->creditTotalLineEdit->setFrame(QFrame::NoFrame);
    ui->creditTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->debitTotalLineEdit->setAlignment(Qt::AlignRight);
    ui->debitTotalLineEdit->setFrame(QFrame::NoFrame);
    setTable();
}

TrialBalance::~TrialBalance()
{
    delete ui;
}

void TrialBalance::on_OKButton_clicked()
{
    setTable();
}

void TrialBalance::on_excelButton_clicked()
{

}

void TrialBalance::on_printButton_clicked()
{

}

void TrialBalance::on_backButton_clicked()
{

}

void TrialBalance::setTable()
{
    tbModel = new QStandardItemModel();
    QDate toDate = ui->toDateDateEdit->date();
    QDate fromDate = ui->fromDateDateEdit->date();
    //    model = ledhelp->getTrialBalance(toDate);
    model = ledhelp->getBalanceForAllLedgers(fromDate,toDate);
    float crTotal=0;
    float drTotal=0;


    for(int i=0;i<model->rowCount();i++){

        drTotal += model->record(i).value(1).toFloat();
        crTotal += model->record(i).value(2).toFloat();

        QList<QStandardItem*> NewRow;
        QString Group =model->record(i).value(0).toString();
        QStandardItem *newColumn = new QStandardItem(Group);
        NewRow.append(newColumn);
        QString Debit =QString::number(model->record(i).value(1).toFloat(),'f',2);
        newColumn = new QStandardItem(Debit);
        NewRow.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        QString Credit =QString::number(model->record(i).value(2).toFloat(),'f',2);
        newColumn = new QStandardItem(Credit);
        NewRow.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        tbModel->appendRow(NewRow);
    }

    //    float pnl = ledhelp->getProfitAndLossValue(fromDate, toDate);

    //    if(pnl<0){
    //        QList<QStandardItem*> NewRow1;
    //        QString Group1 ="Profit and Loss Account";
    //        QStandardItem *newColumn = new QStandardItem(Group1);
    //        NewRow1.append(newColumn);
    //        QString Debit =QString::number(-1*pnl,'f',2);
    //        newColumn = new QStandardItem(Debit);
    //        NewRow1.append(newColumn);
    //        newColumn->setTextAlignment(Qt::AlignRight);
    //        QString Credit =QString::number(0,'f',2);
    //        newColumn = new QStandardItem(Credit);
    //        NewRow1.append(newColumn);
    //        newColumn->setTextAlignment(Qt::AlignRight);
    //        tbModel->appendRow(NewRow1);
    //        drTotal+= -1*pnl;
    //    }
    //    else{
    //        QList<QStandardItem*> NewRow1;
    //        QString Group1 ="Profit and Loss Account";
    //        QStandardItem *newColumn = new QStandardItem(Group1);
    //        NewRow1.append(newColumn);
    //        QString Debit =QString::number(0,'f',2);
    //        newColumn = new QStandardItem(Debit);
    //        NewRow1.append(newColumn);
    //        newColumn->setTextAlignment(Qt::AlignRight);
    //        QString Credit =QString::number(pnl,'f',2);
    //        newColumn = new QStandardItem(Credit);
    //        NewRow1.append(newColumn);
    //        newColumn->setTextAlignment(Qt::AlignRight);
    //        tbModel->appendRow(NewRow1);
    //        crTotal+= pnl;
    //    }

    QFont newFont("MS Shell Dlg 2", 10, QFont::Bold, true);

    float diff = drTotal - crTotal;
    diff = ledhelp->getOpeningBalanceDifference(fromDate,toDate);
    qDebug()<<"diff"<<diff;
    if(diff<0){
        QList<QStandardItem*> NewRow1;
        QString Group ="    Difference in Opening Balance";
        QStandardItem *newColumn = new QStandardItem(Group);
        newColumn->setFont(newFont);
        NewRow1.append(newColumn);
        QString Debit =QString::number(0,'f',2);
        newColumn = new QStandardItem(Debit);
        NewRow1.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        QString Credit =QString::number(-1 * diff,'f',2);
        newColumn = new QStandardItem(Credit);
        NewRow1.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        tbModel->appendRow(NewRow1);
        crTotal+= -1 * diff;
    }
    else if(diff>0){
        QList<QStandardItem*> NewRow1;
        QString Group ="    Difference in Opening Balance";
        QStandardItem *newColumn = new QStandardItem(Group);
        newColumn->setFont(newFont);
        NewRow1.append(newColumn);
        QString Debit =QString::number( diff,'f',2);
        newColumn = new QStandardItem(Debit);
        NewRow1.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        QString Credit =QString::number(0,'f',2);
        newColumn = new QStandardItem(Credit);
        NewRow1.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        tbModel->appendRow(NewRow1);
        drTotal += diff;
    }

    tbModel->setHeaderData(0,Qt::Horizontal,"Particulars");
    tbModel->setHeaderData(2,Qt::Horizontal,"Credit");
    tbModel->setHeaderData(1,Qt::Horizontal,"Debit");

    ui->tableView->setModel(tbModel);
    ui->creditTotalLineEdit->setText(QString::number(crTotal,'f',2));
    ui->debitTotalLineEdit->setText(QString::number(drTotal,'f',2));

}
