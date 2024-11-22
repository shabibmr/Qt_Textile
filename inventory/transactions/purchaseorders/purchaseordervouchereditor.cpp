#include "purchaseordervouchereditor.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "database/Reports/inventoryhelper.h"

#include "voucherOperations/vouchereditorselector.h"
#include "inventory/transactions/receiptnotes/receiptnotevouchereditor.h"

PurchaseOrderVoucherEditor::PurchaseOrderVoucherEditor(PurchaseOrderDatabaseHelper *db, InventoryLedgerTransactionsEditor *parent) :
    InventoryLedgerTransactionsEditor(GMVoucherTypes::PurchaseOrder, db,parent)
{
    setDrawerLayout();

}

PurchaseOrderVoucherEditor::~PurchaseOrderVoucherEditor()
{
    //    delete ui;
}

void PurchaseOrderVoucherEditor::setDrawerLayout()
{
    QPushButton *button = new QPushButton(this);
    button->setText("Convert To Purchase Invoice");
    drawer->drawerLayout()->addWidget(button);

    QPushButton *DNbutton = new QPushButton(this);
    DNbutton->setText("Convert To Receipt Note");
    drawer->drawerLayout()->addWidget(DNbutton);

    QObject::connect(exportButton,&QPushButton::clicked,
                     drawer,&QtMaterialDrawer::openDrawer);

    QObject::connect(button,&QPushButton::clicked,
                     this,&PurchaseOrderVoucherEditor::exportToPurchase);
    QObject::connect(DNbutton,&QPushButton::clicked,
                     this,&PurchaseOrderVoucherEditor::exportToReceiptNote);


}

void PurchaseOrderVoucherEditor::resetWidgets()
{
    if(voucher->status != QuotationStatus::Started && !voucher->fromExternal)
        shareButton->show();

    InventoryLedgerTransactionsEditor::resetWidgets();
}

void PurchaseOrderVoucherEditor::saveVoucher()
{
    if(editorStatus == EditorStatus::NewVoucher)
        voucher->status = QuotationStatus::PurchaseOrderRaised;
    InventoryLedgerTransactionsEditor::saveVoucher();
//     VoucherEditorBase::closeEditor();

    //    emit closing();
    //    initVoucher();
    //    resetWidgets();
    //    mainLedgerWidget->setFocus();
}


void PurchaseOrderVoucherEditor::askForMail()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Mail Voucher","Would You Like to send Mail to : "+LoginValues::getCompany().toMailAddresses.split("|")[0],
            QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        sendEmail();
    }
}

//void PurchaseOrderVoucherEditor::printVoucher(QPrinter *printer)
//{

//    CounterPrinterHelper pp;
//    pp.setPrinter(printer);
//    pp.printPO(voucher);

//    return;
//    printHelper = new PrinterHelper();
//    printHelper->PrintInventoryPurchaseVoucher(voucher);



//}


void PurchaseOrderVoucherEditor::AddItemstoVoucher(QList<CompoundItemDataObject> items,QString ledID)
{
    if(ledID=="")
        voucher->InventoryItems.append(items);
    else{
        for(int i=0;i<items.length();i++){
            if(items[i].BaseItem.defaultLedgerId == ledID)
                voucher->InventoryItems.append(items[i]);
        }
    }

    TransactionCalculator::VoucherCalculatePurchase(voucher);
    resetWidgets();
}

void PurchaseOrderVoucherEditor::sendEmail()
{

    if(voucher->InventoryItems.size()==0){
        snackBar->addMessage("No Items Found!!!");
        return;
    }

    else{

        qDebug()<<"Sendig EMAIL";
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

        html += "<font size =\"4\" color='#F40634'><u>Purchase Order </u></font><br>";

        html += "</td></tr>";

        html+= "<tr>";
        html += "<td align='left'>";
        html += "<font size ='3'>";
        html += "Voucher No : "+voucher->voucherNumber;
        html += "Voucher Date : "+voucher->VoucherDate.toString("dd-MM-yyyy");
        html += "</td></tr>";


        html += "</table>";


        QString htmlDataTable="";

        htmlDataTable += "<table width=\"50%\" class='tblf'>";
        htmlDataTable += "<th align='left'>Item Name</th>";
        htmlDataTable += "<th align='right'>Qty</th>";

        for(int i=0;i<voucher->InventoryItems.size();i++){
            htmlDataTable += "<tr>";

            htmlDataTable += "<td align ='left'>";
            htmlDataTable += voucher->InventoryItems[i].BaseItem.ItemName;
            htmlDataTable += "</td>";
            htmlDataTable += "<td align='right'>";
            htmlDataTable += QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',voucher->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            htmlDataTable += "</td>";
            htmlDataTable += "</tr>";

        }



        htmlDataTable += "</tr>";
        htmlDataTable += "</table>";


        htmlDataTable = " Purchase Order "+htmlDataTable;


        html += htmlDataTable;

        EmailThread email;

        email.setToAddress(LoginValues::getCompany().toMailAddresses.split("|")[0]);

        email.subject = LoginValues::getCompany().CompanyName+ " PO#"+ voucher->voucherNumber;

        email.Message = html;

        email.run();

    }

}

void PurchaseOrderVoucherEditor::createExcel()
{
    ExcelReportHelper ex1;

    //    QString path = QStandardPaths::locate(QStandardPaths::DesktopLocation,voucher->voucherType+"_"+voucher->voucherNumber+".xlsx");

    QString path =  QString("%1/"+voucher->voucherType+"_"+voucher->voucherNumber).arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    qDebug()<<"path = "<<path;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),path,tr("Excel Files (*.xlsx)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ex1.InventoryVoucherSimple(fileName,voucher);

}

void PurchaseOrderVoucherEditor::exportToPurchase()
{
    drawer->closeDrawer();
//    if(voucher->status != QuotationStatus::PurchaseInvoiceRaised){
        qDebug()<<"exporting to voucher";

        VoucherEditorSelector *vEditor = new VoucherEditorSelector;
        vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher));

        QObject::connect(vEditor,&VoucherEditorSelector::saveCompleted,
                         this,&PurchaseOrderVoucherEditor::PurchaseVoucherSaved);
        this->hide();
//    }
//    else{
//        snackBar->addMessage("Purchase Invoice is generated Already");
//    }
}

void PurchaseOrderVoucherEditor::PurchaseVoucherSaved()
{
    // Update SalesOrders

    voucher->status = QuotationStatus::SalesInvoiceRaised;

    qDebug()<<"Updating SO "<<voucher->voucherNumber;
    qDebug()<<"Updating SO "<<voucher->VoucherPrefix;
    qDebug()<<"Updating SO "<<voucher->VoucherPrefix;


    dbHelper->updateVoucherStatus(voucher);

    QMessageBox box; box.setText("Saved Order"); box.exec();
    this->close();

}


void PurchaseOrderVoucherEditor::exportToReceiptNote()
{
//    if(voucher->status < QuotationStatus::ReceiptReady){

    drawer->closeDrawer();
        this->hide();
        VoucherEditorSelector *vEditor = new VoucherEditorSelector;
        QObject::connect(vEditor,&VoucherEditorSelector::saveCompleted,
                         this,&PurchaseOrderVoucherEditor::ReceiptNoteSaved);
        vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote));


//    }
//    else{
//        snackBar->addMessage("Purchase Order is Completed Already");
//    }
}

void PurchaseOrderVoucherEditor::ReceiptNoteSaved()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    voucher->status = QuotationStatus::ReceiptReady;

    // Update SalesOrders
    dbHelper->updateVoucherStatus(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    //    dbHelper->setAllItemVoucherStatusAsQStatus(voucher,voucher->status);
    QMessageBox box; box.setText("Saved Receipt Note"); box.exec();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->show();

    saveVoucher();

}
