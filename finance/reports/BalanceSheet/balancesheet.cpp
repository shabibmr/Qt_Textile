#include "balancesheet.h"
#include "ui_balancesheet.h"

BalanceSheet::BalanceSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BalanceSheet)
{
    ui->setupUi(this);

    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());

    ledHelper = new AccountingReportDatabaseHelper();
    grpHelper = new AccountGroupMasterDatabaseHelper();


    ui->assetsTotalLineEdit->setFrame(false);
    ui->assetsTotalLineEdit->setReadOnly(true);
    ui->LiabilitiesTotlLineEdit->setFrame(false);
    ui->LiabilitiesTotlLineEdit->setReadOnly(true);
    ui->assetsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->assetsTableView->verticalHeader()->hide();
    ui->assetsTableView->horizontalHeader()->hide();
    //    ui->assetsTableView->setAlternatingRowColors(false);
    ui->assetsTableView->setShowGrid(false);
    ui->liabilitiesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->liabilitiesTableView->verticalHeader()->hide();
    ui->liabilitiesTableView->horizontalHeader()->hide();
    //    ui->liabilitiesTableView->setAlternatingRowColors(false);
    ui->liabilitiesTableView->setShowGrid(false);

    setTableView();

}

BalanceSheet::~BalanceSheet()
{
    delete ui;
}

void BalanceSheet::on_OKButton_clicked()
{
    setTableView();
}

void BalanceSheet::on_excelButton_clicked()
{

}

void BalanceSheet::on_printButton_clicked()
{

}

void BalanceSheet::on_backButton_clicked()
{

}

void BalanceSheet::setTableView()
{
    QDate toDate = ui->toDateDateEdit->date();
    QDate fromDate = ui->fromDateDateEdit->date();

    float diff=0;
    float assetTotal = 0;
    float liabilitiesTotal=0;

    QList<AccountGroupDataModel> assetGrp = grpHelper->getPrimaryAccountsByGroupType("Assets");
    QList<AccountGroupDataModel> liabilitiesGrp = grpHelper->getPrimaryAccountsByGroupType("Liabilities");
//    modelAssets = ledHelper->getBalanceByGroupType("Assets", fromDate, toDate);
//    modelLiabilities = ledHelper->getBalanceByGroupType("Liabilities", fromDate, toDate);

    QStandardItemModel *AssetModel = new QStandardItemModel();
    AssetModel->setHeaderData(0,Qt::Horizontal,tr("Assets"));

    QStandardItemModel *LiabilitiesModel = new QStandardItemModel();
    LiabilitiesModel->setHeaderData(0,Qt::Horizontal,tr("Liabilities"));

    for(AccountGroupDataModel acc:assetGrp){
        QList<QStandardItem*> NewRow;
        QString Group =acc.groupName;
        QStandardItem *newColumn = new QStandardItem(Group);
        NewRow.append(newColumn);
        float Value = -1 * ledHelper->getBalanceByGroup(acc.groupID, fromDate, toDate);
        newColumn = new QStandardItem(QString::number(qFabs(Value),'f',2));
        NewRow.append(newColumn);
        newColumn->setTextAlignment(Qt::AlignRight);
        if(Value >= 0){
            AssetModel->appendRow(NewRow);
            assetTotal += Value;
        }
        else{
            Value *= -1;
            LiabilitiesModel->appendRow(NewRow);
            liabilitiesTotal += Value;
        }
    }

    for(AccountGroupDataModel acc:liabilitiesGrp)
    {
        QList<QStandardItem*> NewRow;
        QString Group =acc.groupName;
        QStandardItem *newColumn = new QStandardItem(Group);
        NewRow.append(newColumn);
        float Value = ledHelper->getBalanceByGroup(acc.groupID, fromDate, toDate);
        newColumn = new QStandardItem(QString::number(qFabs(Value),'f',2) );
        newColumn->setTextAlignment(Qt::AlignRight);
        NewRow.append(newColumn);
        if(Value >= 0){
            LiabilitiesModel->appendRow(NewRow);
            liabilitiesTotal += Value;
        }
        else{
            Value *= -1;
            AssetModel->appendRow(NewRow);
            assetTotal += Value;
        }
    }

    float pnl = ledHelper->getProfitAndLossValue(fromDate, toDate);

    QList<QStandardItem*> NewRow1;
    QString Group1 ="Profit and Loss Account";
    QStandardItem *newColumn1 = new QStandardItem(Group1);
    NewRow1.append(newColumn1);
    QString Value1 = QString::number(qFabs(pnl),'f',2);
    newColumn1 = new QStandardItem(Value1);
    newColumn1->setTextAlignment(Qt::AlignRight);
    NewRow1.append(newColumn1);

    if(pnl>=0){
        liabilitiesTotal += pnl;
        LiabilitiesModel->appendRow(NewRow1);
    }
    else{
        assetTotal += -1* pnl;
        AssetModel->appendRow(NewRow1);
    }

     QFont newFont("MS Shell Dlg 2", 10, QFont::Bold, true);

//    diff = assetTotal - liabilitiesTotal;
//    qDebug()<<"Diff"<<diff;


    diff = ledHelper->getOpeningBalanceDifference(fromDate,toDate);
    QList<QStandardItem*> NewRow;
    QString Group ="         Difference in Opening Balance";
    QStandardItem *newColumn = new QStandardItem(Group);
    newColumn->setFont(newFont);
    NewRow.append(newColumn);
    QString Value;
    if (diff>0)
        Value =QString::number(diff,'f',2);
    else
        Value =QString::number(-1*diff,'f',2);
    newColumn = new QStandardItem(Value);
    newColumn->setTextAlignment(Qt::AlignRight);
    NewRow.append(newColumn);


    if(diff<0){
        liabilitiesTotal += -1 * diff;
        LiabilitiesModel->appendRow(NewRow);
    }
    else if (diff>0){
        assetTotal +=  diff;
        AssetModel->appendRow(NewRow);
    }

    ui->assetsTableView->setModel(AssetModel);
    ui->liabilitiesTableView->setModel(LiabilitiesModel);
    ui->assetsTotalLineEdit->setText(QString::number(assetTotal,'f',2));
    ui->LiabilitiesTotlLineEdit->setText(QString::number(liabilitiesTotal,'f',2));

}
