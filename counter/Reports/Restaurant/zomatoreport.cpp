#include "zomatoreport.h"
#include "ui_zomatoreport.h"
#include <QDateTime>
#include <QDate>
//#include "excelreporthelper.h"
#include "QFileDialog"
#include <QSqlQuery>
#include <QSqlError>

ZomatoReport::ZomatoReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZomatoReport)
{
    ui->setupUi(this);
    ui->cashLine->setReadOnly(true);
    ui->creditLine->setReadOnly(true);
    ui->commAmount->setReadOnly(true);
    ui->TotallineEdit->setReadOnly(true);


    ui->cashLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->creditLine->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->commAmount->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->TotallineEdit->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    ui->commPercent->setAlignment(Qt::AlignRight|Qt::AlignVCenter);


    ui->commPercent->setValidator(new QDoubleValidator(0,999,2,this));
    ui->FromdateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->FromdateTimeEdit->setTime(QTime(0,0,0));

    ui->TodateTimeEdit_2->setDateTime(QDateTime::currentDateTime());
    ui->TodateTimeEdit_2->setTime( QTime(23,59,59));

    ui->tableWidget->setColumnCount(6);
    int i=0;

    QTableWidgetItem *Vno = new QTableWidgetItem;
    Vno->setText("Voucher No");
    ui->tableWidget->setHorizontalHeaderItem(i++,Vno);

    QTableWidgetItem *Date = new QTableWidgetItem;
    Date->setText("Date Time");
    ui->tableWidget->setHorizontalHeaderItem(i++,Date);

    QTableWidgetItem *Amout = new QTableWidgetItem;
    Amout->setText("Bill Amount");
    ui->tableWidget->setHorizontalHeaderItem(i++,Amout);

    QTableWidgetItem *Paidto = new QTableWidgetItem;
    Paidto->setText("Paid By");
    ui->tableWidget->setHorizontalHeaderItem(i++,Paidto);


    QTableWidgetItem *comm = new QTableWidgetItem;
    comm->setText("Commision");
    ui->tableWidget->setHorizontalHeaderItem(i++,comm);

    QTableWidgetItem *zom = new QTableWidgetItem;
    zom->setText("Potafo Payment");
    ui->tableWidget->setHorizontalHeaderItem(i++,zom);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->commPercent->setText("12.00");

    setTable();

}

ZomatoReport::~ZomatoReport()
{
    delete ui;
}

void ZomatoReport::on_OKpushButton_clicked()
{
    setTable();
}

void ZomatoReport::setTable()
{
    QDateTime fdate = ui->FromdateTimeEdit->dateTime();
    QDateTime tdate = ui->TodateTimeEdit_2->dateTime();

    int i;
    float perc = ui->commPercent->text().toFloat();
    float totalCommision = 0;
    float cashSale=0;
    float zomSales=0;
    salesVoucherDatabaseHelper sHelper;
    LedgerMasterDatabaseHelper led;

    QString qStr = "SELECT _id, Total,"
                   "(select Ledger_Name FROM " + led.Ledger_Master_TableName + " where ledger_id=tab.ledger_Id)"
                   ", date_created "
                   "FROM "+sHelper.Sales_Invoice_Main_TableName
            +" tab WHERE "+sHelper.Sales_Invoice_Main_Billing_Name
            +"= 'Potafo' AND " + sHelper.Sales_Invoice_Main_TimeStamp
            +">='"+fdate.toString("yyyy-MM-dd hh:mm:ss")+"' and "+
            sHelper.Sales_Invoice_Main_Voucher_Date +"<='"+
            tdate.toString("yyyy-MM-dd hh:mm:ss")+"'";

    qDebug()<<qStr;
    qDebug()<<fdate.toString("yyyy-MM-dd hh:mm:ss");
    qDebug()<<tdate.toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    query.prepare(qStr);

    query.bindValue(":dfrom",fdate.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":dto",tdate.toString("yyyy-MM-dd hh:mm:ss"));

    if(query.exec()){
        ui->tableWidget->setRowCount(0);

        qDebug()<<query.size();
        int cnt =0;
        while(query.next()){
            ui->tableWidget->insertRow(cnt);
            i=0;
            QString ledgerName = query.value(2).toString();

            QTableWidgetItem *Vno = new QTableWidgetItem;
            Vno->setText(query.value(0).toString());
            ui->tableWidget->setItem(cnt,i++,Vno);

            QTableWidgetItem *Date = new QTableWidgetItem;
            Date->setText(query.value(3).toDateTime().toString("yy-MM-dd hh:mm:ss"));
            ui->tableWidget->setItem(cnt,i++,Date);

            QTableWidgetItem *Amout = new QTableWidgetItem;
            Amout->setText(QString::number(query.value(1).toFloat(),'f',2));
            ui->tableWidget->setItem(cnt,i++,Amout);

            QTableWidgetItem *Paidto = new QTableWidgetItem;
            Paidto->setText(ledgerName);
            ui->tableWidget->setItem(cnt,i++,Paidto);

            float commAmt = query.value(1).toFloat() * perc /100;
            QTableWidgetItem *comm = new QTableWidgetItem;
            comm->setText(QString::number(commAmt,'f',2));
            ui->tableWidget->setItem(cnt,i++,comm);

            float amt = -commAmt;
            if(!ledgerName.contains("cash",Qt::CaseInsensitive)){
                amt +=query.value(1).toFloat();
                zomSales+=query.value(1).toFloat();

            }
            else{
                cashSale+=query.value(1).toFloat();
            }
            qDebug()<<amt;
            QTableWidgetItem *zom = new QTableWidgetItem;
            zom->setText(QString::number(amt,'f',2));
            ui->tableWidget->setItem(cnt,i++,zom);

            totalCommision +=amt;

            cnt++;
        }
    }
    else{
        qDebug()<<query.lastError();
    }
    ui->commAmount->setText(QString::number(totalCommision,'f',2));
    ui->cashLine->setText(QString::number(cashSale,'f',2));
    ui->creditLine->setText(QString::number(zomSales,'f',2));
    ui->TotallineEdit->setText(QString::number(zomSales+cashSale,'f',2));
}


void ZomatoReport::on_exportToExcelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->FromdateTimeEdit->date();
    QDate tDate = ui->TodateTimeEdit_2->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Potafo Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    //    ExcelReportHelper *ex = new ExcelReportHelper;

    //    QStringList cols; cols<<"20"<<"20";
    //    reportName = "Waiter Wise Sales Report "+wwaitername;
    //    ex->GenericReport(fileName,reportName,cols,ui->tableWidget,fromDate,toDate);

}


void ZomatoReport::on_commPercent_textChanged(const QString &arg1)
{

}

void ZomatoReport::on_calcButton_clicked()
{
    float percent = ui->commPercent->text().toFloat();
    float sales = ui->TotallineEdit->text().toFloat();
    float commision = sales*percent/100;
    ui-> commAmount ->setText(QString::number(commision,'f',2));

    setTable();
}

void ZomatoReport::on_excel_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->FromdateTimeEdit->date();
    QDate tDate = ui->TodateTimeEdit_2->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Potafo Sales Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20"<<"20"<<"20";
    reportName = "Potafo Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenericReportFromTableView(fileName,reportName,cols,ui->tableWidget,fromDate,toDate,avlCols);
}
