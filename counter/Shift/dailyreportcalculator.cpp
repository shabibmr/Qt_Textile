#include "dailyreportcalculator.h"
#include "database/Reports/closingstockhelper.h"

#include <QVBoxLayout>
void DailyReportCalculator::setShift(ShiftDataModel *value)
{
    shift = value;
}

DailyReportCalculator::DailyReportCalculator(QObject *parent)
    : QObject(parent)
{

    shift = new ShiftDataModel;
    shift->denominations = new DenominationsDataModel;
    dbHelper = new RestaurantShiftDatabaseHelper();
    salesHelper = new salesVoucherDatabaseHelper();
    invHelper = new InventoryHelper();
    ledHelper  = new LedgerHelper();
    shift->voucherPrefix = LoginValues::voucherPrefix;
    shift->cashID = CounterSettingsDataModel::DefaultCash;



}

DailyReportCalculator::~DailyReportCalculator()
{

}

void DailyReportCalculator::setDate(QDateTime fDate, QDateTime tDate)
{
    shift->ShiftStartDateTime = fDate;
    shift->ShiftEndDateTime = tDate;

    qDebug()<<"shift start"<<fDate<<tDate;


}

void DailyReportCalculator::CalculateValues()
{
    qDebug()<<Q_FUNC_INFO;
    DailyClosingDatabaseHelper closingHelper;


    //    shift->BillsFrom = QString::number(closingHelper.getlastClosingDetailByVoucherPrefix().BillsTo.toInt()+1);
    shift->BillsFrom =  dbHelper->getFirstBillByDate(shift);
    qDebug()<<"bils from"<<shift->BillsFrom;
    shift->BillsTo = dbHelper->getLastBill(shift,shift->ShiftEndDateTime);
    qDebug()<<"bils to"<<shift->BillsTo;
    shift->billsCount = dbHelper->getBillsCount(shift);
    qDebug()<<"bils count"<<shift->billsCount;
    shift->cashierName = LoginValues::userName;
    shift->cashierID = LoginValues::userID;

    // shift from daily closing
    //    shift->ShiftStartDateTime = closingHelper.getlastClosingDetailByVoucherPrefix().ShiftEndDateTime;

    // shift from last shift
    //    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
    //                shift->voucherPrefix).ShiftEndDateTime;

    //    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
    //        QString nulldate = "2017-01-01 00:00:00";
    //        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    //    }

    //    shift->ShiftEndDateTime = QDateTime::currentDateTime();


    shift->POSNumber = CounterSettingsDataModel::POSNumber;

    shift->OpeningCash= /*dbHelper->getlastClosingDetailByVoucherPrefix().CashBalance;;0;*/dbHelper->getOpeningCashByShift(shift);
    shift->SalesTotal = dbHelper->getSalesTotalByVoucherDate(shift);
    shift->CashSales = dbHelper->getSalesByCashByVoucherDate(shift);
    shift->CardSales = dbHelper->getSalesByCardByVoucherDate(shift);
    shift->CreditSales = dbHelper->getSalesByCreditByVoucherDate(shift);
    qDebug()<<"11";

    shift->vatTotal = dbHelper->getVatTotal(shift);
    qDebug()<<"22";
    shift->taxableTotal = shift->SalesTotal - shift->vatTotal;

    shift->CashPurchase = dbHelper->getPurchaseByCash(shift);
    shift->CreditPurchase = dbHelper->getPurchaseByCredit(shift);
    shift->PurchaseTotal = dbHelper->getPurchaseTotal(shift);

    shift->CashReceipts = dbHelper->getReceiptByCash(shift);
    shift->CashPayments = dbHelper->getPaymentByCash(shift);

    shift->ReceiptTotal = dbHelper->getReceiptTotal(shift);
    shift->PaymentsTotal = dbHelper->getPaymentTotal(shift);

    shift->advanceRecieved =dbHelper->getAdvancePaymentFromSalesOrders(shift->ShiftStartDateTime,shift->ShiftEndDateTime);

    PaymentDetails = dbHelper->getPaymentDetails(shift);
    qDebug()<<"33";
    //    shift->ZomatoSales = dbHelper->getZomatoSalesTotal(shift);
    //    shift->ZomatoCash = dbHelper->getZomatoSalesByCash(shift);
    //    shift->ZomatoCredit = dbHelper->getZomatoSalesByCredit(shift);
    //    qDebug()<<"44";
    //    shift->TalabatSales =  dbHelper->getTalabatSalesTotal(shift);
    //    shift->TalabatCash =   dbHelper->getTalabatSalesByCash(shift);
    //    shift->TalabatCredit = dbHelper->getTalabatSalesByCredit(shift);
    qDebug()<<"55";
    //    shift->CarriageSales =  dbHelper->getCarriageSalesTotal(shift);
    //    qDebug()<<"66";
    //    shift->deliverooSales = dbHelper->getDeliverooSalesTotal(shift);

    shift->CashInCounter = dbHelper->getTotalCashInCounter(shift);
    shift->CashWithDrawn = dbHelper->getTotalCashWithDrawn(shift);

    shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
            -shift->CashPayments + shift->CashReceipts;
    qDebug()<<"77";
    shift->TillDifference   = shift->CashInCounter-shift->totalCash;


    qDebug()<<"88";

    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;


    //    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
    //                shift->voucherPrefix).ShiftEndDateTime;

    //    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
    //        QString nulldate = "2017-01-01 00:00:00";
    //        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    //    }




    // OLD STUFF

    return;
    shift->voucherPrefix = LoginValues::voucherPrefix;
    shift->cashID = CounterSettingsDataModel::DefaultCash;

    qDebug()<<"Pref "<<shift->voucherPrefix <<shift->cashID;
    html = "";
    closingStock = "";

    shift->BillsFrom = dbHelper->getFirstBillByDate(shift);

    shift->BillsTo  = dbHelper->getLastBillByDate(shift);

    shift->ShiftStartDateTime = dbHelper->getVoucherDateTimebyBill(shift->BillsFrom);

    shift->ShiftEndDateTime = dbHelper->getVoucherDateTimebyBill(shift->BillsTo);

    shift->billsCount = dbHelper->getBillsCount(shift);

    shift->POSNumber = CounterSettingsDataModel::POSNumber;

    shift->OpeningCash= /*dbHelper->getlastClosingDetailByVoucherPrefix().CashBalance;;0;*/dbHelper->getOpeningCashByShift(shift);

    shift->SalesTotal = dbHelper->getSalesTotalByVoucherDate(shift);
    shift->CashSales = dbHelper->getSalesByCashByVoucherDate(shift);
    shift->CardSales = dbHelper->getSalesByCardByVoucherDate(shift);
    shift->CreditSales = dbHelper->getSalesByCreditByVoucherDate(shift);

    shift->vatTotal = dbHelper->getVatTotal(shift);
    shift->taxableTotal = shift->SalesTotal - shift->vatTotal;

    shift->CashPurchase = dbHelper->getPurchaseByCash(shift);
    shift->CreditPurchase = dbHelper->getPurchaseByCredit(shift);
    shift->PurchaseTotal = dbHelper->getPurchaseTotal(shift);

    shift->CashReceipts = dbHelper->getReceiptByCash(shift);
    shift->CashPayments = dbHelper->getPaymentByCash(shift);

    PaymentDetails = dbHelper->getPaymentDetails(shift);

    shift->ZomatoSales = dbHelper->getZomatoSalesTotal(shift);
    shift->ZomatoCash = dbHelper->getZomatoSalesByCash(shift);
    shift->ZomatoCredit = dbHelper->getZomatoSalesByCredit(shift);




    shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
            -shift->CashPayments + shift->CashReceipts;

    shift->TillDifference   = shift->CashInCounter-shift->totalCash;

    shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;


    //    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
    //                shift->voucherPrefix).ShiftEndDateTime;

}

void DailyReportCalculator::sendMail()
{
    QString mailMessage,mail1;

    QString html2 = html;//+closingStock;

    EmailThread mail;

    EmailThread mail2dd;
    EmailThread mail3;
    EmailThread mail4;
    EmailThread mail5;


    mail.subject =  LoginValues::company.CompanyName +" - "+ LoginValues::company.branch + " - SHIFT REPORT";
    mail1 = html+"\n"+"\n"+"</body></html>";
    mailMessage = html2+"\n"+"\n"+"</body></html>";

    QStringList mailAddresses = LoginValues::getCompany().toMailAddresses.split("|");

    QWidget *widget = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(widget);
    QHBoxLayout *hlay = new QHBoxLayout(widget);

    QList<QtMaterialCheckBox*> list;
    for(int i=0;i<mailAddresses.length();i++){
        QtMaterialCheckBox *check = new QtMaterialCheckBox(widget);
        list.append(check);
        check->setText(mailAddresses[i]);
        lay->addWidget(check);
    }

    QtMaterialFlatButton *ok,*cancel;

    ok = new QtMaterialFlatButton(widget);
    ok->setText("OK");

    cancel = new QtMaterialFlatButton(widget);
    cancel->setText("Cancel");
    connect(cancel,&QtMaterialFlatButton::clicked,
            [=](){
        widget->close();
    });

    connect(ok,&QtMaterialFlatButton::clicked,
            [=](){
        QStringList mailList;
        for(int i=0;i<mailAddresses.length();i++){
            if(list[i]->isChecked() && mailAddresses[i].length()>0){
                mailList.append(mailAddresses[i]);
            }
        }
        widget->close();
        for(int i =0;i<mailList.size();i++){
        EmailThread *mailThread = new EmailThread();
        mailThread->subject = LoginValues::company.CompanyName +" - "+ LoginValues::company.branch + " - SHIFT REPORT";
        mailThread->Message = mailMessage;
            mailThread->setToAddress(mailList[i]);
        mailThread->run();
    }

    });
    hlay->addWidget(cancel);
    hlay->addWidget(ok);
    lay->addLayout(hlay);
    widget->setLayout(lay);
    widget->show();


}

QString DailyReportCalculator::getShiftasHTML()
{
    QString htmlList;
    QSqlQuery query(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = "";
    qStr = "Select "
           "(SELECT `Item_Name` FROM " + SalesInventoryItemDatabaseHelper::Sales_Inventory_Table_Name +" WHERE ITEM_ID="
                                                                                                       "SA.Inventory_Item_ID"
                                                                                                       ")"
                                                                                                       ",sum(Quantity),sum(subTotal) from `sales_invoice_details` SA where "
                                                                                                       " price > 0 and "
                                                                                                       "(`Voucher_No` BETWEEN cast('"+shift->BillsFrom+"' as INTEGER) and cast('"+shift->BillsTo+
            "' as INTEGER)) group by `Inventory_Item_ID`";

    if(!query.exec(qStr)){
        qDebug()<<query.lastError();
        qDebug()<<"ERROR HERE!!!!%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
    }
    else{

        //        qDebug()<<qStr;

        htmlList += "<u>Itemwise Sales </u><br>";
        htmlList += "<table width=\"50%\" class='tblf'>";
        htmlList += "<th align='left'>Item Name</th>";
        htmlList += "<th align='right'>Qty</th>";
        htmlList += "<th align='right'>Amount</th>";

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

            htmlList += "</tr>";

        }
    }

    html = "";


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
    html += "Shift From:\t";
    //    qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<shift->ShiftStartDateTime;
    html += "<td align ='right'>"+shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm")+"\n";
    html += "<tr> <td align ='left'>";
    html += "Shift To:\t";
    html += "<td align ='right'>"+shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm")+"\n";
    html += "<tr> <td align ='left'>";
    html += "Bill From:\t";
    html += "<td align ='right'>"+shift->BillsFrom+"\n";
    html += "<tr> <td align ='left'>";
    html += "Bill To:\t\t";
    html += "<td align ='right'>"+shift->BillsTo+"\n";
    html += "<tr> <td align ='left'>";
    html += "No. of Bills:\t";
    html += "<td align ='right'>"+QString::number(shift->billsCount)+"\n\n";
    html += "<tr> <td align ='left'>";
    html += "Opening Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->OpeningCash,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales Total :\t";
    html += "<td align ='right'>"+QString::number(shift->SalesTotal,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales By Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashSales,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Sales By Card:\t";
    html += "<td align ='right'>"+QString::number(shift->CardSales,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Credit Sales:\t";
    html += "<td align ='right'>"+QString::number(shift->CreditSales,'f',2)+"\n\n";

    //    html += "<tr> <td align ='left'>";
    //    html += "Taxable Sales\t";
    //    html += "<td align ='right'>"+QString::number(shift->taxableTotal,'f',2)+"\n";
    //    html += "<tr> <td align ='left'>";
    //    html += "GST @ 5 %";
    //    html += "<td align ='right'>"+QString::number(shift->vatTotal,'f',2)+"\n\n";

    //    html += "</table>";
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



    //    html += "<table width=\"50%\" class='tbl'>";

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
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Credit Sales<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
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
    html += "<tr><td colspan =\"2\"><hr></td></tr>";
    creditSales = dbHelper->getBankAmountDetails(shift);

    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Bank xfer<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
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

    html += "<tr><td colspan =\"2\"><hr></td></tr>";

    if(shift->TalabatSales>0){
        html += "<tr> <td align ='left'>";
        html += "Talabat Sales:\t"+QString::number(shift->TalabatSales,'f',2)+"\n";
        html += "<tr> <td align ='left'>";
        html += "Talabat Cash:\t";
        html += "<td align ='right'>"+QString::number(shift->TalabatCash,'f',2)+"\n";
        html += "<tr> <td align ='left'>";
        html += "Talabat Online:\t";
        html += "<td align ='right'>"+QString::number(shift->TalabatCredit,'f',2)+"\n\n";

    }
    if(shift->CarriageSales>0){
        html += "<tr> <td align ='left'>";
        html += "Carriage Sales:\t";
        html += "<td align ='right'>"+QString::number(shift->CarriageSales,'f',2)+"\n\n";
    }

    if(shift->deliverooSales>0){
        html += "<tr> <td align ='left'>";
        html += "Deliveroo Sales:\t";
        html += "<td align ='right'>"+QString::number(shift->deliverooSales,'f',2)+"\n\n";
    }

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
    html += "Calulated Cash : \t ";
    html += "<td align ='right'>"+QString::number(shift->totalCash,'f',2)+"\n";




    html += "<tr> <td align ='left'>";
    html += "Cash in Counter:\t";
    html += "<td align ='right'>"+QString::number(shift->CashInCounter,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Cash WithDrawn:\t";
    html += "<td align ='right'>"+QString::number(shift->CashWithDrawn,'f',2)+"\n\n";

/*
    html += "<tr> <td align ='left'>";
    html += "Till Difference:\t";
    QString red="";
    if(shift->TillDifference)
    {
        red = "<font color=#FF0000>";
    }

    html += "<td align ='right'>"+red+QString::number(shift->TillDifference,'f',2)+"\n";

    */


    html += "<tr> <td align ='left'>";
    html += "Closing Cash:\t";
    html += "<td align ='right'>"+QString::number(shift->CashBalance,'f',2)+"\n";
    html += "<tr> <td align ='left'>";
    html += "Narration:\t";
    html += "<td align ='left'>"+shift->Narration.replace("\n","<br>")+"\n";

    html += "</tr></table>";

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

    //    qDebug()<<complimentData;

    //    qDebug()<<"*****************************";

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
    //    qDebug()<<"&&&&&&&&&&&&&html: "<<html;

    if(ConfigurationSettingsDatabaseHelper::getValue(showStockReportInShift, false).toBool())
        html += closingStock;

    html += deletedList;

    //    html = salesSplitData;
    return html;
}

void DailyReportCalculator::printReport()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    CounterPrinterHelper *pHelper = new CounterPrinterHelper;
    pHelper->printShiftReport(shift, false, false);

    pHelper->printExpenses(PaymentDetails);

}

