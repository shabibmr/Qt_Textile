#include "restaurantshiftentry.h"
#include "ui_restaurantshiftentry.h"
#include "database/Reports/closingstockhelper.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

RestaurantShiftEntry::RestaurantShiftEntry(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantShiftEntry)
{
    ui->setupUi(this);

    this->setObjectName("report");
    this->setStyleSheet("QWidget#report{background-color:white}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("white"));
    setAutoFillBackground(true);
    setPalette(pal);

    shift = new ShiftDataModel;
    dbHelper = new RestaurantShiftDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    invHelper = new InventoryHelper();
    ledHelper  = new LedgerHelper();

    shiftOpening = ConfigurationSettingsDatabaseHelper::getValue(shiftOpeningCash, -1).toFloat();


    salesTable = new ReportsTableView("Total");
    purchaseTable = new ReportsTableView("Total");
    receiptsTable = new ReportsTableView("Total");
    paymentsTable = new ReportsTableView("Total");
    debitNoteTable = new ReportsTableView("Total");
    creditNoteTable = new ReportsTableView("Total");

    table1 = new QTableView;
    table1->setFixedHeight(250);
    table2 = new QTableView;
    table2->setFixedHeight(520);

    QString stylesheet= "QTableView::item{"
                        "gridline-color : #ffffff;"
                        "border : 0px none #ffffff;"
                        "border-bottom: 1px solid #ffffff;"
                        "}"
                        "QTableView::item:selected{"
                        "color:#00acc1;"
                        "background-color:#e0f7fa;"
                        "}"
                        "QTableView{"
                        "background-color: #ffffff;"
                        "border-right: 1px solid #ddd;"
                        "border-left: 1px solid #ddd;"
                        "gridline-color : #ffffff;"
                        "color: #006064;"
                        "border : 1px solid #ddd;"
                        "/*selection-background-color: #999*/}"
                        "QHeaderView::section:horizontal"
                        "{"
                        "background-color: #00bcd4;"
                        "color: white;"
                        "border: 1px solid #ffffff;"
                        "}";
    table1->setStyleSheet(stylesheet);
    table2->setStyleSheet(stylesheet);

    ui->tableLay1->addWidget(table1);
    ui->tableLay2->addWidget(table2);

    ui->salesTableLay->addWidget(salesTable);
    ui->purchaseTableLay->addWidget(purchaseTable);
    ui->receiptTableLay->addWidget(receiptsTable);
    ui->paymentTableLay->addWidget(paymentsTable);
    ui->dnTableLay->addWidget(debitNoteTable);
    ui->cnTableLay->addWidget(creditNoteTable);

    ui->clearButton->hide();

    //    ui->outputVatAmountLabel->hide();
    //    ui->outputVatAmountLineEdit->hide();
    //    ui->taxableTotalLabel->hide();
    //    ui->taxableTotalLineEdit->hide();


    ui->CalculateButton->hide();
    ui->CalculateButton->setText("Edit");

    ui->printButton->setDisabled(true);
    ui->pdfButton->setDisabled(true);

    ui->CloseButton->setDisabled(true);



    shift->cashID = CounterSettingsDataModel::DefaultCash;
    shift->voucherPrefix = LoginValues::voucherPrefix;

    //    ui->zomatoCODLabel_2->hide();
    //    ui->zomatoCashLabel_2->hide();
    //    ui->zomatosalesLabel_2->hide();
    //    ui->label->hide();
    //    ui->label_2->hide();

    //    ui->talabatSalesLineEdit->hide();
    //    ui->talabatSalesLineEdit->hide();

    //    setAllNonEditable();

}

RestaurantShiftEntry::~RestaurantShiftEntry()
{
    delete ui;
}

void RestaurantShiftEntry::setShift(ShiftDataModel *value)
{
    shift = value;
//    qDebug()<<shift->ledgers.size();

    shift->CashInCounter = shift->denominations->total;
    CalculateValues();
    setValues();
    shift->shiftID=dbHelper->insertShiftData(shift);

}

//void RestaurantShiftEntry::setAllNonEditable()
//{

//    ui->cashWithdrawnLineEdit->setReadOnly(false);
//    ui->narration->setReadOnly(false);
//    ui->openingCashLineEdit->setReadOnly(false);

//}

void RestaurantShiftEntry::on_printButton_clicked()
{
    // Abridge
    CounterPrinterHelper pHelper;
    pHelper.printShiftReport(shift);
    pHelper.printExpenses(PaymentDetails);
    ui->CloseButton->setEnabled(true);

}

void RestaurantShiftEntry::on_saveButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Close Shift","Are you sure you want to close Shift",
                                  QMessageBox::Yes|QMessageBox::No);
    shift->Narration = ui->narration->toPlainText();
    if(reply==QMessageBox::Yes){

        dbHelper->updateShiftData(shift,shift->shiftID);
        dbHelper->truncateKotNumberTable();

        ui->saveButton->setDisabled(true);
        ui->narration->setDisabled(true);
        ui->printButton->setEnabled(true);
        ui->pdfButton->setEnabled(true);
        sendMail();
    }
}


void RestaurantShiftEntry::sendMail(){

    QString htmlList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qStr = "Select "
           "(SELECT `Item_Name` FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" WHERE ITEM_ID="
                                                                                                       "SA.Inventory_Item_ID"
                                                                                                       ")"
                                                                                                      ",sum(Quantity),round(sum(subTotal),2), monthly.qty from `sales_invoice_details` SA JOIN "
            " (SELECT inventory_item_id, SUM(Quantity) as qty from sales_invoice_Details det where Voucher_Date >= DATE_FORMAT(NOW() ,'%Y-%m-01')  AND Voucher_Date <= CURRENT_DATE group by inventory_item_id) monthly "
            " ON SA.Inventory_item_id  = monthly.inventory_item_id "
            " where "
                                                                                                       " price > 0 and "
                                                                                                       "(`Voucher_No` BETWEEN cast('"+shift->BillsFrom+"' as INTEGER) and cast('"+shift->BillsTo+
            "' as INTEGER)) group by SA.`Inventory_Item_ID`";

    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
        qDebug()<<"ERROR HERE!!!!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    }
    else{

        qDebug()<<qStr<<"itemwise";

        htmlList += "<u>Itemwise Sales </u><br>";
        htmlList += "<table width=\"50%\" class='tblf'>";
        htmlList += "<th align='left'>Item Name</th>";
        htmlList += "<th align='right'>Qty</th>";
        htmlList += "<th align='right'>Amount</th>";
        htmlList += "<th align='right'>This Month</th>";

        while (query.next()) {
            htmlList += "<tr>";

            htmlList += "<td align ='left'>";
            htmlList += query.value(0).toString();
            htmlList += "</td>";

            htmlList += "<td align='right'>";
            htmlList += QString::number(query.value(1).toFloat(),'f',2);
            htmlList += "</td>";

            htmlList += "<td align='right'>";
            htmlList += QString::number(query.value(2).toFloat(),'f',2);
            htmlList += "</td>";

            htmlList += "<td align='right'>";
            htmlList += QString::number(query.value(3).toFloat(),'f',2);
            htmlList += "</td>";

            htmlList += "</tr>";

        }
    }

    QString html = "";


    QString css ;

    css = "<style type=\"text/css\">";
    css += "table.tblf ";
    css +="body {";
    css +="  font-family: 'lato', sans-serif;";
    css +="}";
    css +=".container {";
    css +="  max-width: 1000px;";
    css +="  margin-left: auto;";
    css +="  margin-right: auto;";
    css +="  padding-left: 10px;";
    css +="  padding-right: 10px;";
    css +="}";
    css +="";
    css +="h2 {";
    css +="  font-size: 26px;";
    css +="  margin: 20px 0;";
    css +="  text-align: center;";
    css +="  small {";
    css +="    font-size: 0.5em;";
    css +="  }";
    css +="}";
    css +="";
    css +=".responsive-table {";
    css +="  li {";
    css +="    border-radius: 3px;";
    css +="    padding: 25px 30px;";
    css +="    display: flex;";
    css +="    justify-content: space-between;";
    css +="    margin-bottom: 25px;";
    css +="  }";
    css +="  .table-header {";
    css +="    background-color: #95A5A6;";
    css +="    font-size: 14px;";
    css +="    text-transform: uppercase;";
    css +="    letter-spacing: 0.03em;";
    css +="  }";
    css +="  .table-row {";
    css +="    background-color: #ffffff;";
    css +="    box-shadow: 0px 0px 9px 0px rgba(0,0,0,0.1);";
    css +="  }";
    css +="  .col-1 {";
    css +="    flex-basis: 10%;";
    css +="  }";
    css +="  .col-2 {";
    css +="    flex-basis: 40%;";
    css +="  }";
    css +="  .col-3 {";
    css +="    flex-basis: 25%;";
    css +="  }";
    css +="  .col-4 {";
    css +="    flex-basis: 25%;";
    css +="  }";
    css +="  ";
    css +="  @media all and (max-width: 767px) {";
    css +="    .table-header {";
    css +="      display: none;";
    css +="    }";
    css +="    .table-row{";
    css +="      ";
    css +="    }";
    css +="    li {";
    css +="      display: block;";
    css +="    }";
    css +="    .col {";
    css +="      ";
    css +="      flex-basis: 100%;";
    css +="      ";
    css +="    }";
    css +="    .col {";
    css +="      display: flex;";
    css +="      padding: 10px 0;";
    css +="      &:before {";
    css +="        color: #6C7A89;";
    css +="        padding-right: 10px;";
    css +="        content: attr(data-label);";
    css +="        flex-basis: 50%;";
    css +="        text-align: right;";
    css +="      }";
    css +="    }";
    css +="  }";
    css +="}";
    css += "</style>";



    css = "<head><style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 1px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 1px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:4px;font-size:12px;}";

    css += "table.tbl tr:nth-child(odd) td{background-color: #4C8BF5;color: #fff;}";

    css += "table.tbl tr:nth-child(even) td{background-color: #0C3B55;color: #afefff;}";


    css += "table.tbl th {background-color:#ed1a24; padding-bottom:3px;padding-right:4px;padding-left:0px; border-color:#ed1a24;font-color:#ffffff;}";


    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";

    css += "table.tblsign {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tblsign td {box-shadow: 0px 0px 9px 0px rgba(0,0,0,0.1); padding-right:0px;padding-top: 3px;font-size:12px;padding-bottom: 3px;padding-left: 4px;}";
    css += "table.tblsign th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Arial; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    html+= "<html>"+css+"<body>";

    html += "<table width=\"50%\" class ='tbl'>";

    html+= "<tr>";
    html += "<td align='center'>";

    html += "<font size ='4' color='#343674'>";
    html += LoginValues::getCompany().CompanyName+"</font>";
    html += "</td>";
    html+= "</tr>";

    html+= "<tr>";
    html += "<td align='center'>";
    html += "<font size ='3' color='#343674'>";
    html += LoginValues::getCompany().branch+"</font>";
    html += "</td>";
    html+= "</tr>";


    html+= "<tr>";
    html += "<td align='center'>";
    html += "<font size ='3' color='#343674'>";

    html += "<font size =\"4\" color='#F40634'><u>CLOSING REPORT</u></font><br>";

    html += "</table>";



    html += "<table width=\"50%\" class='tbl'>";
    html += "<tr> <td align ='left'>";
    html += "Cashier Name";
    html += "<td align ='right'>"+shift->cashierName+"\n";
    html += "<tr> <td align ='left'>";
    html += "Shift From :\t";
    html += "<td align ='right'>"+shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm")+"\n";
    html += "<tr> <td align ='left'>";
    html += "Shift To :\t";
    html += "<td align ='right'>"+shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm")+"\n";
    html += "<tr> <td align ='left'>";
    html += "Bill From :\t";
    html += "<td align ='right'>"+shift->BillsFrom+"\n";
    html += "<tr> <td align ='left'>";
    html += "Bill To :\t\t";
    html += "<td align ='right'>"+shift->BillsTo+"\n";
    html += "<tr> <td align ='left'>";
    html += "No. of Bills :\t";
    html += "<td align ='right'>"+QString::number(shift->billsCount)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Opening Cash :\t";
    html += "<td align ='right'>"+QString::number(shift->OpeningCash,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales Total :\t";
    html += "<td align ='right'>"+QString::number(shift->SalesTotal,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales By Cash :\t";
    html += "<td align ='right'>"+QString::number(shift->CashSales,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales By Card :\t";
    html += "<td align ='right'>"+QString::number(shift->CardSales,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Credit Sales :\t";
    html += "<td align ='right'>"+QString::number(shift->CreditSales,'f',2)+"\n\n";

    //    html += "<tr> <td align ='left'>";
    //    html += "Taxable Sales\t";
    //    html += "<td align ='right'>"+QString::number(shift->taxableTotal,'f',2)+"\n";
    //    html += "<tr> <td align ='left'>";
    //    html += "GST @ 5 %";
    //    html += "<td align ='right'>"+QString::number(shift->vatTotal,'f',2)+"\n\n";

    float total = 0;
    QSqlQueryModel *taxModel = dbHelper->getSalesSplitModel(shift);
    if(taxModel->rowCount()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Sales Account Split-up<u></td></tr>";
        for(int i=0;i<taxModel->rowCount();i++){
            QString name = taxModel->record(i).value(0).toString();
            QString amt  = QString::number(taxModel->record(i).value(1).toFloat(),'f',2);
            html += "<tr>";
            html += "<td align ='left'>";
            html += name;
            html += "</td>";

            html += "<td align ='right'>";
            html += amt;
            html += "</td>";

            html += "</tr>";
        }

    }

    if(shift->advanceRecieved>0){
        html += "<tr> <td align ='left'>";
        html += "Advance Received";
        html += "<td align ='right'>"+QString::number(shift->advanceRecieved,'f',2)+"\n\n";
    }

    //        if(shift->ZomatoSales>0){
    //            html += "<tr> <td align ='left'>";
    //            html += "Potafo Sales:\t";
    //            html += "<td align ='right'>"+QString::number(shift->ZomatoSales,'f',2)+"\n";
    //            html += "<tr> <td align ='left'>";
    //            html += "Potafo Cash:\t";
    //            html += "<td align ='right'>"+QString::number(shift->ZomatoCash,'f',2)+"\n";
    //            html += "<tr> <td align ='left'>";
    //            html += "Potafo Credit:\t";
    //            html += "<td align ='right'>"+QString::number(shift->ZomatoCredit,'f',2)+"\n\n";
    //        }

    GeneralVoucherDataObject creditSales = dbHelper->getCreditSalesDetails(shift);



    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"1\"><font size=\"4\"><u>Credit Sales<u></td>";
        html += "<table width=\"100%\"><tr> <td align = 'right'>System</td><td align = 'right'>Entered</td></tr></table></tr>";

        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
            html += "<tr>";
            html += "<td align ='left'>";
            html += name;
            html += "</td>";
            html += "<td align = 'right'>";
            html += "<table width=\"100%\"><tr>";
            html += "<td align ='right'>";
            html += amt;
            html += "</td>";

            html += "<td></td>";
            html += "<td align ='right'>";
            for(ShiftLedgersDataModel *led: shift->ledgers)
            {

                if(led->ledgerID == creditSales.ledgersList[i].LedgerID){
                    qDebug()<<"&&&&&&&&&&&&&&&&&&&"<<led->ledgerName<<led->actualAmount;
                    html += QString::number(led->actualAmount, 'f',2);
                }

            }
            html += "</td></tr></table>";

            html += "</td>";

            html += "</tr>";
        }

    }
    html += "<tr><td colspan =\"2\"><hr></td></tr>";
    creditSales = dbHelper->getBankAmountDetails(shift);

    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"1\"><font size=\"4\"><u>Bank xfer<u></td>";
        html += "<table width=\"100%\"><tr> <td align = 'right'>System</td><td align = 'right'>Entered</td></tr></table></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
            html += "<tr>";
            html += "<td align ='left'>";
            html += name;
            html += "</td>";
            html += "<td >";
            html += "<table width=\"100%\"><tr>";
            html += "<td align ='right'>";
            html += amt;
            html += "</td>";

            html += "<td align ='right'>";
            for(ShiftLedgersDataModel *led: shift->ledgers)
            {
                if(led->ledgerID == creditSales.ledgersList[i].LedgerID)
                    html += QString::number( led->actualAmount , 'f',2);

            }
            html += "</td></tr></table>";

            html += "</td>";

            html += "</tr>";
        }
    }

    html += "<tr><td colspan =\"2\"><hr></td></tr>";

    html += "<tr> <td align ='left'>";
    html += "Purchase Total:\t\t";
    html += "<td align ='right'>"+QString::number(shift->PurchaseTotal,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Purchase By Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashPurchase,'f',2)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Purchase By Credit:\t";
    html += "<td align ='right'>"+QString::number(shift->CreditPurchase,'f',2)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Payments Total:\t";
    html += "<td align ='right'>"+QString::number(shift->PaymentsTotal,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Payments in Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashPayments,'f',2)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Receipt Total:";
    html += "<td align ='right'>"+QString::number(shift->ReceiptTotal,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Receipt in Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashReceipts,'f',2)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Cash in Counter:\t";
    html += "<td align ='right'>"+QString::number(shift->CashInCounter,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Total Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->totalCash,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Till Difference:\t";


    QString red="";
    if(shift->TillDifference)
    {
        red = "<font color=#FF0000>";
    }
    html += "<td align ='right'>"+red+QString::number(shift->TillDifference,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Cash WithDrawn:\t";
    html += "<td align ='right'>"+QString::number(shift->CashWithDrawn,'f',2)+"\n\n";


    html += "<tr> <td align ='left'>";
    html += "Closing Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashBalance,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Narration:\t";
    html += "<td align ='left'>"+shift->Narration.replace("\n","<br>")+"\n";

    html += "</tr>    ";
    html += "<tr>";
    html += "<th align=\"left\" width =\"60%\">Denomination</th>";
    html += "<th align=\"right\" width =\"20%\">Count</th>";
    html += "<th align=\"right\" width =\"20%\">Amount</th>";

    html += "<tr><td align = 'left'>2000</td>";
    html += "<td align = 'right'>" + QString::number(shift->denominations->count2000) + "</td>";
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt2000, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>500</td>";
    html += "<td align = 'right'>" + QString::number(shift->denominations->count500)+ "</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt500, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>200</td>";
    html += "<td align = 'right'>" + QString::number(shift->denominations->count200)+ "</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt200, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>100</td>";
    html += "<td align = 'right'>" + QString::number(shift->denominations->count100)+"</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt100, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>50</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->count50)+"</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt50, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>20</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->count20)+"</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt20, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>10</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->count10)+"</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt10, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'>5</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->count5)+ "</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amt5, 'f', 2) + "<td>";
    html += "<tr><td align = 'left'>Coins</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->countCoins)+ "</td>" ;
    html += "<td align = 'right'>" + QString::number(shift->denominations->amtCoins, 'f', 2) + "</td>";
    html += "<tr><td align = 'left'><b>Total</td><td></td>" ;
    html += "<td align = 'right'><b>" + QString::number(shift->denominations->total, 'f', 2) + "</td>";

    html += "</tr>";
    html += " </table>";

    //fathima work start

    QSqlQueryModel *model;
    QString wastageData="";
    total = 0;

    model   = invHelper->getWastageQueryByDate(shift->ShiftStartDateTime,shift->ShiftEndDateTime);



    QString wastage="";

    wastage += "<table width=\"50%\" class='tblf'>";
    wastage += "<th align='left'>Item Name</th>";
    wastage += "<th align='right'>Qty</th>";
    wastage += "<th align='right'>Amount</th>";

    for(int i=0;i<model->rowCount();i++){
        wastage += "<tr>";

        wastage += "<td align ='left'>";
        wastage += model->record(i).value(0).toString();
        wastage += "</td>";
        wastage += "<td align='right'>";
        wastage += QString::number(model->record(i).value(1).toFloat(),'f',2);
        wastage += "</td>";

        wastage += "<td align='right'>";
        wastage += QString::number(model->record(i).value(2).toFloat(),'f',2);
        wastage += "</td>";

        wastage += "</tr>";
        total += model->record(i).value(2).toFloat();

    }

    wastage +=  "<tr>";
    wastage +=  "<td align ='left'>";
    wastage +=  "<b>Total Value  :  ";
    wastage +=  "</td><td></td>";

    wastage += "<td align='right'>";
    wastage +=  "<b>"+QString::number(total,'f',2);
    wastage +=  "</td>";

    wastage += "</tr>";
    wastage += "</table>";

    if(model->rowCount()==0){
        wastage="";
    }
    else{
        wastage = "Wastage Entries <br>"+wastage;
    }


    model = ledHelper->getDiscountedSalesForMail(shift->ShiftStartDateTime,shift->ShiftEndDateTime);

    QString discountSales="";

    discountSales += "<table width=\"50%\" class='tblf'>";
    discountSales += "<th align='left'>Voucher#</th>";
    discountSales += "<th align='right'>Discount</th>";
    discountSales += "<th align='right'>Referrence</th>";

    total =0;
    for(int i=0;i<model->rowCount();i++){

        discountSales += "<tr>";
        discountSales += "<td align ='left'>";
        discountSales += model->record(i).value(0).toString();
        discountSales += "</td>";

        discountSales += "<td align='right'>";
        discountSales += QString::number(model->record(i).value(3).toFloat(),'f',2);
        discountSales += "</td>";

        discountSales += "<td align='left'>";
        discountSales += model->record(i).value(2).toString();
        discountSales += "</td>";

        discountSales += "</tr>";
        total += model->record(i).value(3).toFloat();

    }
    discountSales +=  "<tr>";
    discountSales +=  "<td align ='left'>";
    discountSales +=  "<b>Total Value  :  ";
    discountSales +=  "</td>";

    discountSales += "<td align='right'>";
    discountSales +=  "<b>"+QString::number(total,'f',2);
    discountSales +=  "</td>";

    discountSales += "</tr>";
    discountSales += "</table>";

    if(model->rowCount()==0){
        discountSales="";
    }
    else{
        discountSales = "Discounted Sales <br>"+discountSales;
    }

    model = invHelper->getComplimentarySalesForMail(shift->ShiftStartDateTime,shift->ShiftEndDateTime);
    QString complimentData="";

    complimentData += "<table width=\"50%\" class='tblf'>";
    complimentData += "<th align='left'>Item</th>";
    complimentData += "<th align='right'>Qty</th>";
    //        complimentData += "<th align='right'>Amounting</th>";

    total =0;
    for(int i=0;i<model->rowCount();i++){

        complimentData += "<tr>";
        complimentData += "<td align ='left'>";
        complimentData += model->record(i).value(0).toString();
        complimentData += "</td>";



        complimentData += "<td align='right'>";
        complimentData += QString::number(model->record(i).value(1).toInt(),'f',2);
        complimentData += "</td>";

        //            complimentData += "<td align='left'>";
        //            complimentData += model->record(i).value(2).toString();
        //            complimentData += "</td>";

        complimentData += "</tr>";
        total += model->record(i).value(3).toFloat();
    }

    //        complimentData +=  "<tr>";

    //        complimentData +=  "<td align ='left'>";
    //        complimentData +=  "<b>Total Value  :  ";
    //        complimentData +=  "</td>";

    //        complimentData += "<td align='right'>";
    //        complimentData +=  "<b>"+QString::number(total,'f',2);
    //        complimentData +=  "</td>";

    //        complimentData += "</tr>";

    complimentData += "</table>";

    if(model->rowCount()==0){
        complimentData="";
    }
    else{
        complimentData = "Combo/Compliment Sales <br>"+complimentData;
    }

    qDebug()<<complimentData;

    qDebug()<<"*****************************";

    QString closingStock="";
    if(ConfigurationSettingsDatabaseHelper::getValue(showStockReportInShift, false).toBool()){
        ClosingStockHelper *stkHelper = new ClosingStockHelper;
        model = stkHelper->getItemwiseClosingStockOfGrpByGodownAndLPP(shift->ShiftStartDateTime.date(),
                                                                      shift->ShiftEndDateTime.date(),
                                                                      "%" ,"0", true);


        closingStock += "<table width=\"100%\" class='tblf'>";
        //    closingStock += "<tr>";
        closingStock += "<th align='left' width =\"20%\">Item</th>";
        closingStock += "<th align='right' width =\"20%\">Opening Stock</th>";
        closingStock += "<th align='right' width =\20%\">Inward</th>";
        closingStock += "<th align='right' width =\"20%\">Issued</th>";
        if(ConfigurationSettingsDatabaseHelper::getValue(showStockAdjInReport, false).toBool())
            closingStock += "<th align='right'>Stock Adjustment</th>";
        closingStock += "<th align='right'>Closing Stock</th>";
        closingStock += "<th align='right'>Stock Value</th>";
        //    closingStock += "</tr>";

        total =0;
        float totalStk =0;
        float totalStkVal = 0;
        float totalInward = 0;
        float totalIssued = 0;
        float totalStkAdj = 0;
        float totalOpening = 0;
        for(int i=0;i<model->rowCount();i++){
            closingStock += "<tr>";
            closingStock += "<td align ='left'>";
            closingStock += model->record(i).value(2).toString();
            closingStock += "</td>";
            totalOpening += model->record(i).value(3).toFloat();
            closingStock += "<td align='right'>";
            closingStock += QString::number(model->record(i).value(3).toFloat(),'f',2);
            closingStock += "</td>";
            totalInward += model->record(i).value(4).toFloat();
            closingStock += "<td align='right'>";
            closingStock += QString::number(model->record(i).value(4).toFloat(),'f',2);
            closingStock += "</td>";
            totalIssued += model->record(i).value(5).toFloat();
            closingStock += "<td align='right'>";
            closingStock += QString::number(model->record(i).value(5).toFloat(),'f',2);
            closingStock += "</td>";
            if(ConfigurationSettingsDatabaseHelper::getValue(showStockAdjInReport, false).toBool())
            {
                totalStkAdj += model->record(i).value(6).toFloat();
                closingStock += "<td align='right'>";
                closingStock += QString::number(model->record(i).value(6).toFloat(),'f',2);
                closingStock += "</td>";

                totalStk += model->record(i).value(7).toFloat();
                closingStock += "<td align='right'>";
                closingStock += QString::number(model->record(i).value(7).toFloat(),'f',2);
                closingStock += "</td>";

                totalStkVal += model->record(i).value(8).toFloat();
                closingStock += "<td align='right'>";
                closingStock += QString::number(model->record(i).value(8).toFloat(),'f',2);
                closingStock += "</td>";
            }

            else {

                totalStk += model->record(i).value(6).toFloat();
                closingStock += "<td align='right'>";
                closingStock += QString::number(model->record(i).value(6).toFloat(),'f',2);
                closingStock += "</td>";

                totalStkVal += model->record(i).value(7).toFloat();
                closingStock += "<td align='right'>";
                closingStock += QString::number(model->record(i).value(7).toFloat(),'f',2);
                closingStock += "</td>";
            }

            closingStock += "</tr>";

            //            total += model->record(i).value(4).toFloat();

        }

        closingStock += "<tr>";
        closingStock += "<td align ='left'>";
        closingStock += "Total";
        closingStock += "</td>";
        closingStock += "<td align='right'>";
        closingStock += QString::number(totalOpening,'f',2);
        closingStock += "</td>";
        closingStock += "<td align='right'>";
        closingStock += QString::number(totalInward,'f',2);
        closingStock += "</td>";
        closingStock += "<td align='right'>";
        closingStock += QString::number(totalIssued,'f',2);
        closingStock += "</td>";
        if(ConfigurationSettingsDatabaseHelper::getValue(showStockAdjInReport, false).toBool())
        {
            closingStock += "<td align='right'>";
            closingStock += QString::number(totalStkAdj,'f',2);
            closingStock += "</td>";

            closingStock += "<td align='right'>";
            closingStock += QString::number(totalStk,'f',2);
            closingStock += "</td>";

            closingStock += "<td align='right'>";
            closingStock += QString::number(totalStkVal,'f',2);
            closingStock += "</td>";
        }
        else {

            closingStock += "<td align='right'>";
            closingStock += QString::number(totalStk,'f',2);
            closingStock += "</td>";

            closingStock += "<td align='right'>";
            closingStock += QString::number(totalStkVal,'f',2);
            closingStock += "</td>";
        }
        closingStock += "</tr>";

        closingStock += "</table>";

        if(model->rowCount()==0){
            closingStock="";
        }
        else{
            closingStock = "Closing Stock <br>"+closingStock;
        }

    }

    // Ledgerwise-Billwise Report;


    //    model = ledHelper->getBillwiseSalesForMail(shift->BillsFrom,shift->BillsTo);

    salesVoucherDeletedDatabaseHelper shelper;
    model = shelper.getDeletedItemsListByDateTime(shift->ShiftStartDateTime,
                                                  shift->ShiftEndDateTime);

    QString deletedList="";



    if(model->rowCount()>0){
        deletedList = "Deleted Items List<br>";
        deletedList += "<table width=\"50%\" class='tblf'>";
        deletedList += "<th align='left'>Date</th>";
        deletedList += "<th align='right'>Item</th>";
        deletedList += "<th align='right'>Quantity</th>";
        deletedList += "<th align='right'>Narration</th>";
        for(int i=0;i<model->rowCount();i++){

            deletedList += "<tr>";

            deletedList += "<td align ='left'>";
            deletedList += model->record(i).value(0).toString();
            deletedList += "</td>";

            deletedList += "<td align='left'>";
            deletedList += model->record(i).value(1).toString();
            deletedList += "</td>";

            deletedList += "<td align='left'>";
            deletedList += QString::number(model->record(i).value(2).toDouble(),'f',2);
            deletedList += "</td>";

            deletedList += "<td align='left'>";
            deletedList += model->record(i).value(3).toString();
            deletedList += "</td>";


            deletedList += "</tr>";
            //            total += model->record(i).value(4).toFloat();
        }
        deletedList += "</table>";
    }


    htmlList+="</table>";
    html += htmlList;
    html += wastage;
    html += discountSales;

    html += complimentData;



    QString html2 = html;
    if(ConfigurationSettingsDatabaseHelper::getValue(showStockReportInShift, false).toBool()){
        html += closingStock;
    }

    html += deletedList;

    QString mail2,mailMessage;
    EmailThread mail;
    EmailThread mail2dd;
    EmailThread mail3;
    EmailThread mail4;
    EmailThread mail5;
    //        EmailThread mail6;

    QStringList mailAddresses = LoginValues::getCompany().toMailAddresses.split("|");


    mail.subject =  LoginValues::company.CompanyName +" - "+ LoginValues::company.branch + " - SHIFT REPORT";
    mailMessage = html+"\n"+"\n"+"</body></html>";
    mail2 = html2+"\n"+"\n"+"</body></html>";
    mail.Message=mail2;


    for(int i =0;i<mailAddresses.size();i++){
        EmailThread *mailThread = new EmailThread();
        mailThread->subject = LoginValues::company.CompanyName +" - "+ LoginValues::company.branch + " - SHIFT REPORT";
        mailThread->Message = mailMessage;
        mailThread->setToAddress(mailAddresses[i]);
        mailThread->run();
    }


}

void RestaurantShiftEntry::on_CloseButton_clicked()
{
    this->close();
    //    emit shiftClosed();
}

void RestaurantShiftEntry::on_CalculateButton_clicked()
{
    if(ui->CalculateButton->text()=="Edit"){
        ui->CalculateButton->setText("Calculate");
    }
    else{
        CalculateValues();
    }



}

void RestaurantShiftEntry::setDenominationValues(DenominationsDataModel *obj){
    shift->denominations = obj;
    shift->CashInCounter = obj->total;
    //    shift->ledgers = obj->ledgers;
    CalculateValues();
    setValues();
    shift->shiftID=dbHelper->insertShiftData(shift);
    qDebug()<<"11111";
}

void RestaurantShiftEntry::CalculateValues()
{
    DailyClosingDatabaseHelper closingHelper;
    //    shift->BillsFrom = QString::number(closingHelper.getlastClosingDetailByVoucherPrefix().BillsTo.toInt()+1);
    shift->BillsFrom =  dbHelper->getFirstBillinShift(shift);

    shift->BillsTo = dbHelper->getLastBill(shift);
    shift->billsCount = dbHelper->getBillsCount(shift);
    shift->cashierName = LoginValues::userName;
    shift->cashierID = LoginValues::userID;

    // shift from daily closing
    //    shift->ShiftStartDateTime = closingHelper.getlastClosingDetailByVoucherPrefix().ShiftEndDateTime;

    // shift from last shift
    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
                shift->voucherPrefix).ShiftEndDateTime;

    //    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
    //        QString nulldate = "2017-01-01 00:00:00";
    //        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    //    }

    shift->ShiftEndDateTime = QDateTime::currentDateTime();

    shift->Narration = ui->narration->toPlainText();
    shift->POSNumber = CounterSettingsDataModel::POSNumber;

    if(shiftOpening == -1){

        shift->OpeningCash= dbHelper->getlastClosingDetailByVoucherPrefix(shift->voucherPrefix).CashBalance;
        //0;dbHelper->getOpeningCash(shift);

    }
    else{
        shift->OpeningCash = shiftOpening;
    }

    shift->SalesTotal = dbHelper->getSalesTotal(shift);
    shift->CashSales = dbHelper->getSalesByCash(shift);
    shift->CardSales = dbHelper->getSalesByCard(shift);
    shift->CreditSales = dbHelper->getSalesByCredit(shift);
    qDebug()<<"11";

    shift->vatTotal = dbHelper->getVatTotal(shift);
    qDebug()<<"22";
    shift->taxableTotal = shift->SalesTotal - shift->vatTotal;

    shift->CashPurchase = dbHelper->getPurchaseByCash(shift);
    shift->CardPurchase = dbHelper->getPurchaseByCard(shift);
    shift->CreditPurchase = dbHelper->getPurchaseByCredit(shift);
    shift->PurchaseTotal = dbHelper->getPurchaseTotal(shift);

    shift->CashReceipts = dbHelper->getReceiptByCash(shift);
    shift->CardReceipts = dbHelper->getReceiptByCard(shift);
    shift->CashPayments = dbHelper->getPaymentByCash(shift);
    shift->CardPayments = dbHelper->getPaymentByCard(shift);

    shift->ReceiptTotal = dbHelper->getReceiptTotal(shift);
    shift->PaymentsTotal = dbHelper->getPaymentTotal(shift);

    shift->CashPurchaseReturn = dbHelper->getPurchaseReturnByCash(shift);
    shift->CashSalesReturn = dbHelper->getSalesReturnByCash(shift);

    shift->TotalPurchaseReturn = dbHelper->getTotalPurchaseReturn(shift);
    shift->TotalSalesReturn = dbHelper->getTotalSalesReturn(shift);

    shift->advanceRecieved =dbHelper->getAdvancePaymentFromSalesOrders(shift->ShiftStartDateTime,shift->ShiftEndDateTime
                                                                       ,shift->voucherPrefix);


    PaymentDetails = dbHelper->getPaymentDetails(shift);
    qDebug()<<"33";
    shift->ZomatoSales = dbHelper->getZomatoSalesTotal(shift);
    shift->ZomatoCash = dbHelper->getZomatoSalesByCash(shift);
    shift->ZomatoCredit = dbHelper->getZomatoSalesByCredit(shift);
    qDebug()<<"44";
    shift->TalabatSales =  dbHelper->getTalabatSalesTotal(shift);
    shift->TalabatCash =   dbHelper->getTalabatSalesByCash(shift);
    shift->TalabatCredit = dbHelper->getTalabatSalesByCredit(shift);
    qDebug()<<"55";
    shift->CarriageSales =  dbHelper->getCarriageSalesTotal(shift);
    qDebug()<<"66";
    shift->deliverooSales = dbHelper->getDeliverooSalesTotal(shift);


    shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
            -shift->CashPayments + shift->CashReceipts;
    qDebug()<<"77";
    shift->TillDifference   = shift->CashInCounter-shift->totalCash;

    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;

    //    ui->cashBalanceInCounterLineEdit->setText(QString::number(shift->CashBalance,'f',2));

    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
                shift->voucherPrefix).ShiftEndDateTime;

    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
        QString nulldate = "2017-01-01 00:00:00";
        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    }

    QSqlQueryModel *model = dbHelper->AllLedgerSummary(shift);
    //    salesTable->verticalHeader()->hide();
    salesTable->setModel(model);
    salesTable->setItemDelegate(new ReportCommonDelegate());
    salesTable->setAvlcols(QList<int> {1});
    salesTable->setModelQry(model);
    salesTable->setColumns();
    salesTable->hideColumn(2);
    salesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    model = dbHelper->getCreditSummaryByLedger(shift, "PURCHASEVOUCHER");
    //    purchaseTable->verticalHeader()->hide();
    purchaseTable->setItemDelegate(new ReportCommonDelegate());
    purchaseTable->setAvlcols(QList<int> {1});
    purchaseTable->setModel(model);
    purchaseTable->setModelQry(model);
    purchaseTable->setColumns();
    purchaseTable->hideColumn(2);
    //    purchaseTable->lastRow->show();
    purchaseTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    model = dbHelper->getCreditSummaryByLedger(shift, "PAYMENTVOUCHER");
    //    paymentsTable->verticalHeader()->hide();
    paymentsTable->setItemDelegate(new ReportCommonDelegate());
    paymentsTable->setAvlcols(QList<int> {1});
    paymentsTable->setModel(model);
    paymentsTable->setModelQry(model);
    paymentsTable->setColumns();
    paymentsTable->hideColumn(2);
    paymentsTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    model = dbHelper->getDebitSummaryByLedger(shift, "RECEIPTVOUCHER");
    //    receiptsTable->verticalHeader()->hide();
    receiptsTable->setItemDelegate(new ReportCommonDelegate());
    receiptsTable->setAvlcols(QList<int> {1});
    receiptsTable->setModel(model);
    receiptsTable->setModelQry(model);
    receiptsTable->setColumns();
    receiptsTable->hideColumn(2);
    receiptsTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    model = dbHelper->getCreditSummaryByLedger(shift, "CREDITNOTE");
    //    creditNoteTable->verticalHeader()->hide();
    creditNoteTable->setItemDelegate(new ReportCommonDelegate());
    creditNoteTable->setAvlcols(QList<int> {1});
    creditNoteTable->setModel(model);
    creditNoteTable->setModelQry(model);
    creditNoteTable->setColumns();
    creditNoteTable->hideColumn(2);
    creditNoteTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    model = dbHelper->getDebitSummaryByLedger(shift, "DEBITNOTE");
    //    debitNoteTable->verticalHeader()->hide();
    debitNoteTable->setItemDelegate(new ReportCommonDelegate());
    debitNoteTable->setAvlcols(QList<int> {1});
    debitNoteTable->setModel(model);
    debitNoteTable->setModelQry(model);
    debitNoteTable->setColumns();
    debitNoteTable->hideColumn(2);
    debitNoteTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);



}

void RestaurantShiftEntry::setValues()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    model1 = new GMShiftModel(shift, 1);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    model2 = new GMShiftModel(shift, 2);

    table1->setItemDelegate(new ReportCommonDelegate);
    table2->setItemDelegate(new ReportCommonDelegate);

    table1->setModel(model1);
    table2->setModel(model2);

    table1->horizontalHeader()->hide();
    table1->verticalHeader()->hide();
    table1->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    table2->horizontalHeader()->hide();
    table2->verticalHeader()->hide();
    table2->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

}


void RestaurantShiftEntry::on_clearButton_clicked()
{

}

void RestaurantShiftEntry::showShareOptions()
{
    shareOptionsWidget = new ShareTools("Shift entry", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);
    //    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);

    shareOptionsWidget->excelButton->hide();
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&RestaurantShiftEntry::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&RestaurantShiftEntry::printReport);
    //    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
    //                     this,&RestaurantShiftEntry::on_excelButton_clicked);
}

void RestaurantShiftEntry::printReport(QPrinter *printer)
{
    // List
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);

    printerHelper->printShiftReport(shift);
    printerHelper->printExpenses(PaymentDetails);
    ui->CloseButton->setEnabled(true);

}

void RestaurantShiftEntry::on_pdfButton_clicked()
{
    // PDF
    CounterPrinterHelper pHelper;

    pHelper.printShiftReport(shift, true);
    pHelper.printExpenses(PaymentDetails, true);
    //    ui->CloseButton->setEnabled(true);
}
