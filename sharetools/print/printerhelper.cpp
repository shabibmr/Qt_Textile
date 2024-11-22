#include "printerhelper.h"
#include <QProgressDialog>
#include <QFontDatabase>
#include <QFileDialog>
#include <QStandardPaths>
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"


#define CODE128_B_START 104
#define CODE128_STOP 106

//void PrinterHelper::testPrint(GeneralVoucherDataObject* voucher)
//{
//    QtRPT *report = new QtRPT(this);

//    //Make a page of report
//    RptPageObject *page = new RptPageObject();
//    page->pageNo=0;
//    report->pageList.append(page); //Append page to the report

//    //Make a ReportTitleBand
//    RptBandObject *band1 = new RptBandObject();
//    band1->name = "ReportTitle";
//    band1->height = 80;
//    band1->type = ReportTitle;
//    page->addBand(band1); //Append band to the page

//    //Make a field
//    RptFieldObject *field1 = new RptFieldObject();
//    field1->name = "field1";
//    field1->fieldType = Text;
//    field1->rect.setTop(0);
//    field1->rect.setLeft(0);
//    field1->rect.setHeight(60);
//    field1->rect.setWidth(700);
//    field1->font.setBold(true);
//    field1->font.setPointSize(24);
//    field1->aligment = Qt::AlignCenter;
//    field1->textWrap = 1;
//    field1->value = "Creation of the report from user application without XML file";
//    field1->borderLeft = Qt::white;
//    field1->borderRight = Qt::white;
//    field1->borderBottom = Qt::white;
//    field1->borderTop = Qt::white;
//    band1->addField(field1);  //Append field to the ReportTitleBand

//    //Make a MasterHeaderBand
//    RptBandObject *band2 = new RptBandObject();
//    band2->name = "MasterHeaderBand";
//    band2->height = 30;
//    band2->type = MasterHeader;
//    page->addBand(band2); //Append band to the page

//    //Make a field
//    RptFieldObject *h1 = new RptFieldObject();
//    h1->name = "h1";
//    h1->fieldType = Text;
//    h1->rect.setTop(0);
//    h1->rect.setLeft(0);
//    h1->rect.setHeight(30);
//    h1->rect.setWidth(150);
//    h1->value = "Header 1";
//    h1->font.setBold(true);
//    h1->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
//    h1->aligment = Qt::AlignCenter;
//    band2->addField(h1);  //Append field to the MasterHeaderBand

//    RptFieldObject *h2 = new RptFieldObject();
//    h2->name = "h2";
//    h2->fieldType = Text;
//    h2->rect.setTop(0);
//    h2->rect.setLeft(149);
//    h2->rect.setHeight(30);
//    h2->rect.setWidth(150);
//    h2->value = "Header 2";
//    h2->font.setBold(true);
//    h2->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
//    h2->aligment = Qt::AlignCenter;
//    band2->addField(h2);  //Append field to the MasterHeaderBand

//    RptFieldObject *h3 = new RptFieldObject();
//    h3->name = "h3";
//    h3->fieldType = Text;
//    h3->rect.setTop(0);
//    h3->rect.setLeft(298);
//    h3->rect.setHeight(30);
//    h3->rect.setWidth(150);
//    h3->value = "Header 3";
//    h3->font.setBold(true);
//    h3->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
//    h3->aligment = Qt::AlignCenter;
//    band2->addField(h3);  //Append field to the MasterHeaderBand

//    RptFieldObject *h4 = new RptFieldObject();
//    h4->name = "h4";
//    h4->fieldType = Text;
//    h4->rect.setTop(0);
//    h4->rect.setLeft(447);
//    h4->rect.setHeight(30);
//    h4->rect.setWidth(150);
//    h4->value = "Header 4";
//    h4->font.setBold(true);
//    h4->setDefaultBackgroundColor(Qt::lightGray); //Set default background color
//    h4->aligment = Qt::AlignCenter;
//    band2->addField(h4);  //Append field to the MasterHeaderBand

//    //Make a MasterDataBand
//    RptBandObject *band3 = new RptBandObject();
//    band3->name = "MasterData";
//    band3->height = 30;
//    band3->type = MasterData;
//    page->addBand(band3); //Append band to the page

//    //Make a field
//    RptFieldObject *c1 = new RptFieldObject();
//    c1->name = "c1";
//    c1->fieldType = Text;
//    c1->rect.setTop(0);
//    c1->rect.setLeft(0);
//    c1->rect.setHeight(30);
//    c1->rect.setWidth(150);
//    band3->addField(c1);  //Append field to the MasterDataBand

//    RptFieldObject *c2 = new RptFieldObject();
//    c2->name = "c2";
//    c2->fieldType = Text;
//    c2->rect.setTop(0);
//    c2->rect.setLeft(149);
//    c2->rect.setHeight(30);
//    c2->rect.setWidth(150);
//    band3->addField(c2);  //Append field to the MasterDataBand

//    RptFieldObject *c3 = new RptFieldObject();
//    c3->name = "c3";
//    c3->fieldType = Text;
//    c3->rect.setTop(0);
//    c3->rect.setLeft(298);
//    c3->rect.setHeight(30);
//    c3->rect.setWidth(150);
//    band3->addField(c3);  //Append field to the MasterDataBand

//    RptFieldObject *c4 = new RptFieldObject();
//    c4->name = "c4";
//    c4->fieldType = Text;
//    c4->rect.setTop(0);
//    c4->rect.setLeft(447);
//    c4->rect.setHeight(30);
//    c4->rect.setWidth(150);
//    band3->addField(c4);  //Append field to the MasterDataBand

//    //Make a PageFooterBand
//    RptBandObject *band4 = new RptBandObject();
//    band4->name = "PageFooterBand";
//    band4->height = 50;
//    band4->type = PageFooter;
//    page->addBand(band4); //Append band to the page

//    RptFieldObject *pf = new RptFieldObject();
//    pf->name = "pf";
//    pf->fieldType = Text;
//    pf->borderLeft = Qt::white;
//    pf->borderRight = Qt::white;
//    pf->borderBottom = Qt::white;
//    pf->borderTop = Qt::white;
//    pf->font.setBold(true);
//    pf->rect.setTop(0);
//    pf->rect.setLeft(330);
//    pf->rect.setHeight(30);
//    pf->rect.setWidth(150);
//    pf->value = "<Page> of <TotalPages>";
//    band4->addField(pf);  //Append field to the PageFooterBand

//    QObject::connect(report, SIGNAL(setDSInfo(DataSetInfo &)), this, SLOT(setDSInfo(DataSetInfo &)));

//    QObject::connect(report, SIGNAL(setField(RptFieldObject &)), this, SLOT(setField(RptFieldObject &)));


//    report->printExec(false,true);
//}

//void PrinterHelper::setDSInfo(DataSetInfo &dsInfo)
//{
//    qDebug()<<"setDsinfo";
//    dsInfo.recordCount = 4;
//}

//void PrinterHelper::setField(RptFieldObject &fieldObject)
//{
//    qDebug()<<"set Field with "<<fieldObject.name;
//    if (fieldObject.name == "c1") {
//        fieldObject.value = "Column 1 Row "+QString::number(fieldObject.recNo()+1);
//        if (fieldObject.recNo() == 0)
//            fieldObject.backgroundColor = Qt::yellow;
//        if (fieldObject.recNo() == 1)
//            fieldObject.backgroundColor = Qt::magenta;
//        if (fieldObject.recNo() == 2)
//            fieldObject.backgroundColor = Qt::cyan;
//        if (fieldObject.recNo() == 3)
//            fieldObject.backgroundColor = Qt::green;
//    }
//    if (fieldObject.name == "c2") {
//        fieldObject.value = "Column 2 Row "+QString::number(fieldObject.recNo()+1);
//        if (fieldObject.recNo() == 1)
//            fieldObject.backgroundColor = Qt::yellow;
//        if (fieldObject.recNo() == 2)
//            fieldObject.backgroundColor = Qt::magenta;
//        if (fieldObject.recNo() == 3)
//            fieldObject.backgroundColor = Qt::cyan;
//        if (fieldObject.recNo() == 0)
//            fieldObject.backgroundColor = Qt::green;
//    }
//    if (fieldObject.name == "c3") {
//        fieldObject.value = "Column 3 Row "+QString::number(fieldObject.recNo()+1);
//        if (fieldObject.recNo() == 2)
//            fieldObject.backgroundColor = Qt::yellow;
//        if (fieldObject.recNo() == 3)
//            fieldObject.backgroundColor = Qt::magenta;
//        if (fieldObject.recNo() == 0)
//            fieldObject.backgroundColor = Qt::cyan;
//        if (fieldObject.recNo() == 1)
//            fieldObject.backgroundColor = Qt::green;
//    }
//    if (fieldObject.name == "c4") {
//        fieldObject.value = "Column 4 Row "+QString::number(fieldObject.recNo()+1);
//        if (fieldObject.recNo() == 3)
//            fieldObject.backgroundColor = Qt::yellow;
//        if (fieldObject.recNo() == 0)
//            fieldObject.backgroundColor = Qt::magenta;
//        if (fieldObject.recNo() == 1)
//            fieldObject.backgroundColor = Qt::cyan;
//        if (fieldObject.recNo() == 2)
//            fieldObject.backgroundColor = Qt::green;
//    }
//}

PrinterHelper::PrinterHelper(QWidget *parent) : QWidget(parent)
{
    userHelper = new UserProfileDatabaseHelper;
    imageHelper = new InventoryItemImagesDatabaseHelper;
    this->setPrinter(new QPrinter(QPrinter::ScreenResolution));
}

int borderMargins = 50;
//Change to quotation

int PrinterHelper::PrintInventoryVoucher(GeneralVoucherDataObject* data)
{
    QString ReportName = data->voucherType;

    //    QMessageBox::StandardButton reply;

    //    reply = QMessageBox::question(this,"Print Description",
    //                                  "Would you like to Print Description with Item Names",
    //                                  QMessageBox::Yes|QMessageBox::No);

    bool printDescr= false;

    //    if(reply==QMessageBox::Yes){
    //        printDescr=true;
    //    }



    qDebug()<<"printing" << ReportName;
    QList<int> colSizes; //not so sure.


    //    QPrinter printer(QPrinter::ScreenResolution) ;
    printer->setPageSize(QPrinter::A4);
    //Fathima
    //    if(!printAsPdf){
    //        printAsPdfG=false;
    //        QPrintDialog pDial(printer,this);
    //        if(pDial.exec()==QDialog::Rejected) return 0 ;
    //    }
    //    else{
    //        printAsPdfG=true;

    //    }
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QString blankSpace="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString borderWidth = "border-width=\"1px\"";
    //  QString totals = QString::number(total, 'f', 2);
    //"<center>"+text2+"&trade;</center>"
    QString headhtml = "<HTML>"
                       "<HEAD>";
    QString cssTable;
    cssTable = "<style type=\"text/css\">";
    cssTable += "table.tb1 { border-color: }";

    cssTable+="</style></head>";

    QString companyName = "<font size =\"12\">Al Rumooz</font>";
    QString VoucherName = "<font size =\"8\" color=\"green\"><i><u>"+data->voucherType+"</u></i></font>";
    QString bodyTable ="";

    QString css;
    css = "<style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 0px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 0px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:4px;font-size:12px;}";
    css += "table.tbl th {background-color:#ed1a24; padding-bottom:3px;padding-right:4px;padding-left:0px; border-color:#ed1a24;font-color:#ffffff;}";


    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";

    css += "table.tblsign {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tblsign td {padding-right:0px;padding-top: 3px;font-size:12px;padding-bottom: 3px;padding-left: 4px;}";
    css += "table.tblsign th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Arial; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    QString listTableRowSpan = "colspan=\"2\"";
    //    bodyTable +=  "<table class=\"tblb\" width =\"100%\">";
    //    bodyTable +=  "<tr><td width=\"66%\"><img src=:/images/rmzlogo200w.png></td><td align=\"right\">"+
    //            VoucherName+"</td></tr>"
    //                        "";
    QString VoucherDetails = "";//<tr><td "+listTableRowSpan+">";
    VoucherDetails +="<table class=\"tbl\" width =\"100%\">";

    QString CustomerData = "<tr><td width = \"50%\"><font face=\"calibri\" size=\"3\">";
    //    <tr><td>Customer Details:<br>"
    //                           "<font size =\"5\"><b> "+data.ledgerObject.LedgerName+"</b></font>";

    CustomerData += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+data->ledgerObject.Address;
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ";
    CustomerData += "<br><br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contact Person :</b>";
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
            +data->ledgerObject.ContactPersonName;
    CustomerData += "</font></td>";


    QString QuoteDetails = "<td><font face=\"calibri\" size =\"3\">";
    QuoteDetails += "<table class=\"tbldet\" width =\"100%\">";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Prepared By</b></td><td>&nbsp;&nbsp;<b>Currency</b></td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ADMIN</td><td>&nbsp;&nbsp;AED</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Contact</b></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;Salesman</td><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Reference</b></td>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ORDER BY MAIL</td></tr>";

    //    QuoteDetails += "<tr><td>Validity</td><td>30 days";

    QuoteDetails += ""
                    "</td></tr></table></font></td></tr>";
    VoucherDetails +=CustomerData+QuoteDetails + "</table>";

    QString SubjectRow ="";

    QString itemFontSize ="3";
    QString itemHeadFontSize ="4";
    QString CoverLetter ="<tr><td "+listTableRowSpan+">";
    CoverLetter += "<pre class =\"cusfont\">Hello Sir,\n"
                   "    Please find following quotaion";
    CoverLetter += "</pre></td></tr>";

    QString html = "<tr><td "+
            listTableRowSpan
            +">";
    //    html = "<table width=\"100%\" <tr><td>hello</td><tr></table>";
    html =
            "<br><font face =\"calibri\"><table class=\"tbl\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">"
            "<tr>"
            "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Sl No</font></th>"
                              "<th width=\"45%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Item</font></th>"
                              "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Uom</font></th>"
            //                              "<th width=\"10%\" align=\"left\"><font color=\"#ffffff\" size =\""
            //            +itemHeadFontSize+"\">Model</font></th>"
            //                              "<th width=\"10%\" align=\"left\"><font color=\"#ffffff\" size =\""
            //            +itemHeadFontSize+"\">Brand</font></th>"
            "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Qty</font></th>"
                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Price</font></th>"
                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Vat</font></th>"
                              "<th width=\"15%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Total</h2></th>"
                              "</tr>";

    int numlines = 0;
    for(int i=0;i<data->InventoryItems.size();i++){
        html += "\n<tr>";
        html += "<td align =\"left\"> <font >";
        html += QString::number(i+1);
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data->InventoryItems[i].BaseItem.ItemName.toUpper();
        if(printDescr){
            numlines+=data->InventoryItems[i].BaseItem.ItemDescription.split("\n").count();
            html += "<br>&nbsp;"+data->InventoryItems[i].BaseItem.ItemDescription.replace("\n","<br>&nbsp;").simplified();
        }

        html += "</td>";

        html += "<td align =\"left\"> <font >";
        UomDataBaseHelper *uomHelper = new UomDataBaseHelper();
        QString uomName = uomHelper->getUOMNameById(data->InventoryItems[i].BaseItem.uomObject.UomName);

        html += uomName;//.toUpper();
        html += "</td>";

        //        html += "<td align =\"left\"> <font >";
        //        html += data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        //        html += "</td>";

        //        html += "<td align =\"left\"> <font >";
        //        html += data.InventoryItems[i].BaseItem.brandName.toUpper();
        //        html += "</font></td>";

        html += "<td align =\"right\"> <font >";
        html += QString::number(data->InventoryItems[i].BaseItem.quantity/data->InventoryItems[i].BaseItem.uomObject.convRate);
        html += "</td>";

        html += "<td align =\"right\"> <font >";
        html += formatNumber(data->InventoryItems[i].BaseItem.price*data->InventoryItems[i].BaseItem.uomObject.convRate);
        html += "</td>";

        html += "<td align =\"right\"> <font >";
        html += formatNumber(data->InventoryItems[i].BaseItem.taxAmount);
        html += "</td>";


        html += "<td align =\"right\"> <font >";
        html += formatNumber(data->InventoryItems[i].BaseItem.subTotal + data->InventoryItems[i].BaseItem.discountinAmount);
        html += "</td>";

        html+= "</tr>";
        numlines++;
    }
    int listSize = 4;

    if(data->InventoryItems.size()<20)
        for(int i=numlines;i<20;i++){
            html+="<tr><td></td></tr>";
            //html+="<tr><td ><font size =\""+itemFontSize+"\">"+QString::number(i)+"</font></td></tr>";
        }
    //    qDebug()<<"659";

    QString TotalString = "";
    bool subtotalPrinted =false;

    if(data->discountinAmount > 0){
        subtotalPrinted = true;
        TotalString += "<tr><td colspan=\"7\"><hr color=\"red\"></td>";
        TotalString += "<tr><td></td><td></td><td></td>";
        TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Gross Total</font></td>";
        TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data->subTotal+data->discountinAmount)+"</font></td>";

        TotalString += "<tr><td></td><td></td><td></td>";
        TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Discount Amount</font></td>";
        TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data->discountinAmount)+"</font></td>";
    }

    TotalString += "<tr><td></td><td></td><td></td>";
    TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Sub Total</font></td>";
    TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data->subTotal)+"</font></td>";



    if(data->taxTotalAmount>0){
        if(subtotalPrinted==false){
            TotalString += "<tr><td colspan=\"7\"><hr color=\"red\"></td>";
            TotalString += "<tr><td></td><td></td><td></td>";
            TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Gross Total</font></td>";
            TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data->subTotal)+"</font></td>";
        }
        TotalString += "<tr><td></td><td></td><td></td>";
        TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Vat @ 5%</font></td>";
        TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data->taxTotalAmount)+"</font></td>";

    }
    TotalString += "<tr><td></td><td></td><td></td>";
    TotalString += "<td align=\"left\" colspan=\"3\"><font color=\"#000000\" face=\"calibri\" size =\"5\">Grand Total</font></td>";
    TotalString += "<td align=\"right\"><font color=\"#000000\" size =\"6\">"+formatNumber(data->grandTotal)+"</font></td>";

    //    TotalString += "<td>";
    //    TotalString += "<table class=\"tbl\">";
    //    TotalString += "<td colspan=\"2\" align=\"left\">  <font size =\"5\">Grand Total</font></td>";
    //    TotalString += "<td align=\"right\"> <font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></td>";
    //    TotalString += "</table>";

    //    TotalString += "</td>";

    //    TotalString += "<th align=\"left\" colspan=\"4\"><font face=\"calibri\" size =\"5\">";
    //    TotalString += "<p align=\"left\">Grand Total";
    //    TotalString += "<span align=\"right\>"+formatNumber(data.grandTotal-100)+"</span</p></th>";
    //    TotalString += "<table class=\"tbl\" width =\"100%\"><th align=\"left\">";
    //    TotalString += "Grand Total</th>";
    //    TotalString += "<th align=\"right\"><font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></th>";
    //    TotalString += "</table></th>";


    TotalString += "</tr></table>";

    QString amountInWords = "<table width =\"100%\"> ";

    amountInWords += "<tr><td "+listTableRowSpan+"> <font face=\"calibri\" size =\"4\">";
    amountInWords += "Amount in Words : <i>Aed. "+numberToText(data->grandTotal)+ " only</i></font>";
    amountInWords += "</td></tr>";

    QString narrationText = "<table width =\"100%\"> ";

    narrationText += "<tr><td "+listTableRowSpan+"> <font face=\"calibri\" size =\"4\">";
    narrationText += "<u>Narration : </u><br><i> "+data->narration.replace("\n","<br>").simplified()+"</i></font>";
    narrationText += "</td></tr>";


    QString lineBlock = "";
    lineBlock += "<tr><td "+listTableRowSpan+">";
    lineBlock += "<hr color=\"red\"></td></tr>";

    QString SignatureBlock;

    SignatureBlock += "</table>";
    SignatureBlock += "<table class=\"tblsign\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">";//"<table width=\"100%\">";
    SignatureBlock += "<tr>";
    SignatureBlock += "<td width=\"50%\">";
    SignatureBlock += "<table>";
    SignatureBlock += "<tr><td>Receiver Name</td><td>:</td><tr>";
    SignatureBlock += "<tr><td>Phone </td><td>:</td><tr>";
    SignatureBlock += "<tr><td>Time</td><td>:</td><tr>";
    SignatureBlock += "<tr><td>Customer Acknowledgement<br><br><br><br><hr></td><td></td><tr>";
    SignatureBlock += "<tr><td></td><td>&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";

    SignatureBlock += "<tr><td></td><td></td></tr>";
    SignatureBlock += "</table>";
    SignatureBlock += "</td>";
    SignatureBlock += "<td width=\"30%\"></td>";
    SignatureBlock += "<td width=\"20%\">";
    SignatureBlock += "<table>";
    SignatureBlock += "<tr><td>Authorized Signatory<br><br><br><br><hr></td><td></td></tr>";
    SignatureBlock += "<tr><td></td><td></td></tr>";
    SignatureBlock += "<tr><td></td><td></td></tr>";
    SignatureBlock += "<tr><td></td><td></td></tr>";

    SignatureBlock += "</td>";

    SignatureBlock += "</tr></table>";


    //  html = headhtml+css+bodyTable+VoucherDetails + html+TotalString+amountInWords+lineBlock;
    html = headhtml+css+html+TotalString+amountInWords+lineBlock+narrationText+SignatureBlock;

    //    html = css+html;
    //    html = headhtml + html + TotalString+amountInWords+lineBlock+SignatureBlock;
    //    html+=  "</td></tr></table></body></html>";
    qDebug()<<"Tr count = "<<html.count("<tr");
    qDebug()<<"Tr +br count = "<<html.count("<tr")+html.count("<br>");

    qDebug()<<html;
    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);

    cursor.insertHtml(html);

    //    QPainter painter1( &printer );
    //    QPainter *painter = &painter1;
    //    const QSizeF pageSize = printer.paperRect().size();
    //    const double bm = mmToPixels(printer, borderMargins);
    //    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    //    painter->drawRect(borderRect);

    //    cursor.setPosition(1000,QTextCursor::MoveAnchor);
    //    cursor.insertText("Text Here");
    //    qDebug()<<cursor.position();
    //    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,5);
    //    qDebug()<<cursor.position();
    //    cursor.insertText("Text2 Here2");

    //    QTextBlockFormat blockFormat;
    //    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    //    cursor.insertBlock(blockFormat);
    //    //    cursor.setPosition(cursor.position()+100);
    //    cursor.insertHtml(getTermsinHtml(validity,subject,payment,warranty,extra,delivery,exclusion));

    QString a="Prepared By";
    QString b="";
    if(ReportName.contains("Sales Inv",Qt::CaseInsensitive)){
        b = "TRN.";
    }
    else if(ReportName.contains("Sales ord",Qt::CaseInsensitive)){
        b = "Quotation Ref.";
    }

    QString c="LPO Ref";
    QString v1;QString v2;QString v3;
    v1 = data->AddedBy;
    v2 = LoginValues::getCompany().trn;
    v3 = data->LPO;

    printDocument(*printer,&doc,0,*data,a,b,c,v1,v2,v3);

    return 0;
}


int PrinterHelper::printIssueRequest(GeneralVoucherDataObject *voucher)
{

    QPrinter printer(QPrinter::ScreenResolution) ;
    QPrintDialog pDial(&printer,this);

    if(pDial.exec()==QDialog::Rejected)
        return  0;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    //cursor.insertHtml(getIssueRequestVoucherinHTML(voucher));

    getIssueTable(&cursor,voucher);
    formatIssueRequest(printer,&doc,0,voucher);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);
    return 1;


    //    qDebug()<<"start print";
    //    int flag =1;
    //    QTextDocument doc ;
    //    //    getBillasTextDocument(doc,obj,printerName);

    //    QString html = getIssueRequestVoucherinHTML(voucher);
    //    qDebug()<<"got as HTML";
    //    doc.setHtml(html);
    //    QPrinter printer(QPrinter::ScreenResolution);
    //    qDebug()<<"Printer Setting name";

    //    //printer.setPrinterName(printerName);
    //    qDebug()<<"Printer Name set";

    //    QFont font;
    //    font.setFamily("Avant Garde");
    //    font.setPointSize(8);

    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);

    //    doc.print(&printer);

    //    qDebug()<<"deleting doc";


    //    qDebug()<<"Print  complete";
    //    return flag;

}

void  PrinterHelper::setPrinter(QPrinter *printer)
{
    this->printer = printer;
}


int PrinterHelper::PrintInventorySalesVoucher(GeneralVoucherDataObject *voucher)
{

    qDebug()<<Q_FUNC_INFO;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    doc.setDefaultFont(font);
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;

    if(printer->pageSize() == QPrinter::A5)
        widthlist << 5 << 35.27 + 5 << 9.67 << 9.67<< 9.67<< 9.67 <<12.5;
    else
        widthlist << 5 << 35.27 << 9.67 << 9.67 << 9.67<< 9.67<< 9.67 <<12.5;

    //    widthlist << 5 << 47.27 << 9.67+5 << 9.67<< 9.67 <<12.5 + 4.67; -- del note

    getInventoryVoucherTableSales(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //  getHSNCodeTable(&cursor,voucher);
    //    printer->setFullPage(true);// -- FR
    qDebug()<<"FR";
    formatInventoryVoucherPages(*printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //        paperSize.setWidth(printer->width());
    //        paperSize.setHeight(printer->height());
    //    doc.setPageSize(paperSize);
    //        doc.print(printer);

    qDebug()<<"Print Completed";

    return 1;

}


QString PrinterHelper::getPayVHTML(GeneralVoucherDataObject *obj)
{
    QString html ="";
    QString CompanyName = LoginValues::getCompany().CompanyName;
    QByteArray companyLogo = LoginValues::getCompany().logo;
    QString imageType;
    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
//    QString POSNumber=CounterSettingsDataModel::POSNumber;

    QString vType ="Payment Voucher";

    html += "<html><body>";

    html += "<table width =\"100%\"><tr>";
    if(companyLogo.size()>0){
        QPixmap pixmap(companyLogo);
        QBuffer buffer(&companyLogo);
        pixmap.save(&buffer, "PNG");
        QString url ="";

        url = QString("<img src=\"data:image/png;base64,") + companyLogo.toBase64() +">";

        //                    + "\"  width=\""+QString::number(item.imageWidth)
        //                    +"\" height =\""+QString::number(item.imageHeight)+"\">";

        html += "<tr><td><center>"+url+"<br></center></td</tr><tr><td></td></tr>";
        html += "<tr><td><center><font size =\"4\"> </font></center>";
        html += "<tr><td><center><font size =\"4\">"+CompanyName+"</font></center></td></tr>";
    }
    else{
        html += "<tr><td><center><font size =\"6\">"+CompanyName+"</font></center></td></tr>";
    }

    html += "<tr><td><center><font size =\"3\">"+branch+"</font></center>";
    html += "<tr><td><center><font size =\"3\">"+addressLine+"</font></center>";
    html += "<tr><td><center><font size =\"3\">Phone : "+phoneNumber+"</font></center>";
    if(trnNo.length()>0)
        html += "<tr><td><center><font size =\"4\">GST : "+trnNo+"</font></center>";


    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";


    html += "<table width =\"100%\">";

    html += "<tr><td width=\"25%\">"+vType+" No :</td>";
    html += "<td width=\"25%\"><b>"+obj->voucherNumber+"</td>";

    html += "<tr><td> Date :</td>";
    html += "<td>"+obj->VoucherDate.toString("dd-MM-yyyy")+"</td>";

    html += "<td>Time :</td>";
    if(obj->timestamp.toString("hh:mm:ss").length()>0)
        html += "<td>"+obj->timestamp.toString("hh:mm:ss")+"</td></tr>";
    else
        html += "<td>"+QDateTime::currentDateTime().toString("hh:mm:ss")+"</td></tr>";

    html += "</table>";
    html += "<table width=\"100%\">";
    //     qDebug()<<"Start html 99";



    int colCount = 3;

    html += "<tr><td colspan =\""+QString::number(colCount)+"\">";
    html += "Received As : "+obj->ledgerObject.LedgerName;
    html += "</td></tr>";

    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    html += "<tr>";
    html += "<th align=\"left\" width =\"5%\">#<\th>";
    html += "<th align=\"left\" width =\"50%\">Paid By<\th>";

    html += "<th align=\"right\" width =\"10%\">Amount<\th>";
    html += "</tr>";

    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";


    for(int i=0;i<obj->ledgersList.size();i++){
        html += "<tr>";
        html += "<td>";
        html += QString::number(i+1);
        html += "</td>";
        html += "<td>";
        html += obj->ledgersList[i].LedgerName;

        html += "</td>";
        float amt = obj->ledgersList[i].drAmount;
        html += "<td align=\"right\">";
        html += QString::number(amt,'f',2);
        html += "</td>";

        html += "</tr>";
    }


    html += "<tr></tr><tr></tr><tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";



    html += "<tr>";
    html += "<td colspan =\""+QString::number(colCount)+"\">";
    html += "<font size =\"4\"><b>";
    html += "Grand Total ("+LoginValues::company.Currency+") ";
    html += "</td><td align=\"right\">";
    html += "<font size =\"4\"><b>";
    html += QString::number(obj->grandTotal,'f',2);
    html += "</td></tr>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center>THANK YOU!!!</td></tr>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";


    html += "</table>";
    return html;
}



int PrinterHelper::PrintInventoryPurchaseVoucher(GeneralVoucherDataObject *voucher)
{
    QPrinter printer(QPrinter::ScreenResolution) ;

    //    QPrintDialog pDial(&printer,this);
    //    printer.setPageSize(QPrinter::A4);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  0;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;
    widthlist << 5 << 35.27 << 9.67 << 9.67 << 9.67<< 9.67<< 9.67 <<12.5;

    getInventoryVoucherTablePurchase(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //    getHSNCodeTable(&cursor,voucher);
    printer.setFullPage(true);
    formatInventoryVoucherPages(printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);
    //    doc.print(&printer);
    return 1;
}

int PrinterHelper::PrintWorkOrderVoucher(GeneralVoucherDataObject *voucher)
{
    QPrinter printer(QPrinter::ScreenResolution) ;

    //    QPrintDialog pDial(&printer,this);
    //    printer.setPageSize(QPrinter::A4);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  0;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;
    widthlist << 5 << 80 << 10 ;

    getInventoryVoucherTableWorkOrder(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //    getHSNCodeTable(&cursor,voucher);

    printer.setFullPage(true);
    formatWorkOrderPages(printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);
    //    doc.print(&printer);
    return 1;
}

int PrinterHelper::PrintIssueRequestVoucher(GeneralVoucherDataObject *voucher)
{
    QPrinter printer(QPrinter::ScreenResolution) ;

    //    QPrintDialog pDial(&printer,this);
    //    printer.setPageSize(QPrinter::A4);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  0;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;
    widthlist << 5 << 35.27 << 9.67 << 9.67 << 9.67 << 9.67 ;

    getInventoryVoucherTableIssueReq(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //    getHSNCodeTable(&cursor,voucher);
    printer.setFullPage(true);
    formatIssueVoucherPages(printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);
    //    doc.print(&printer);
    return 1;
}

int PrinterHelper::PrintInventoryDeliveryNote(GeneralVoucherDataObject *voucher)
{
    QPrinter printer(QPrinter::ScreenResolution) ;

    //    QPrintDialog pDial(&printer,this);
    //    printer.setPageSize(QPrinter::A4);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  0;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;
    widthlist << 5 << 47.27 << 9.67+5 << 9.67<< 9.67 <<12.5 + 4.67;

    getInventoryVoucherTableDeliveryNote(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //    getHSNCodeTable(&cursor,voucher);
    printer.setFullPage(true);
    formatDelNotePages(printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);
    //    doc.print(&printer);
    return 1;
}

int PrinterHelper::PrintDispatchSlip(GeneralVoucherDataObject *voucher)
{
    QPrinter printer(QPrinter::ScreenResolution) ;

    //    QPrintDialog pDial(&printer,this);
    //    printer.setPageSize(QPrinter::A4);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  0;
    printer.setFullPage(true);

    qDebug()<<"Q-Debug";
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(10);
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QList<qreal> widthlist;
    widthlist << 1 << 65 << 15 << 15;

    QString html = getDispatchHTML(voucher,widthlist);
    //    cursor.insertHtml(html);
    qDebug()<<" FIRST>>>>>>>>>>>>> Doc Height: "<<doc.size().height()<<"lc : "<<doc.lineCount();
    qreal nh,mh;
    nh = doc.size().height();
    int n = doc.lineCount();

    qDebug()<<"HTML IS : "<<html;
    //    cursor.insertHtml(html);
    qDebug()<<"html at 916";
    qDebug()<<html;
    qDebug()<<" SECOND>>>>>>>>>>>>> Doc Height: "<<doc.size().height()<<"lc : "<<doc.lineCount()<<html;
    int m = doc.lineCount();
    mh = doc.size().height();
    qDebug()<<"Lines added"<<m-n;
    qDebug()<<"Height Diff"<<mh-nh;
    qDebug()<<"Line Inc = "<<(mh-nh)/(m-n);
    html += "</tbody>";
    html += "</table></body></html>";

    //    cursor.insertHtml(html);
    qDebug()<<" THIRD>>>>>>>>>>>>> Doc Height: "<<doc.size().height()<<"lc : "<<doc.lineCount();

    qDebug()<<html;
    qDebug()<<"Doc Height: "<<doc.size().height();
    qDebug()<<"Paper Size : "<<printer.pageRect().size();
    qDebug()<<"Line Count : "<<doc.lineCount();
    cursor.insertHtml(html);

    //    getDispatchTextTable(&cursor,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>>DOC POSITION "<<doc.lineCount();
    //    getHSNCodeTable(&cursor,voucher);

    qDebug()<<" >>>>>>>>>>>>>  Height: "<<doc.size().height();
    formatDispatchPages(printer,&doc,voucher,widthlist);
    qDebug()<<" >>>>>>>>>>>>> DOC-2 POSITION : "<<doc.lineCount();
    qDebug()<<" >>>>>>>>>>>>> Doc Height: "<<doc.size().height();

    //    addTable(cursor);
    //    printDocumentExample(printer,&doc,0);


    //        doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);
    //        doc.print(&printer);
    return 1;
}

QString PrinterHelper::getDispatchHTML(GeneralVoucherDataObject *obj,QList<qreal> widthList){

    UserProfileDatabaseHelper userHelper;
    QString sman = userHelper.getEmployeeNameFromID(obj->SalesmanID);
    QString addedby = userHelper.getEmployeeNameFromID(obj->AddedById);
    int colCount = widthList.length();
    QString colCountStr = QString::number(colCount);

    QString companyName = LoginValues::getCompany().CompanyName;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString branch = LoginValues::getCompany().branch;

    addressLine = branch+", "+addressLine;

    QString dateLine = QString("Date : " +obj->VoucherDate.toString("dd-MM-yyyy"));
    QString ReportLine = "Dispatch Slip";


    QString html ="";

    html += "<html>";
    html += "<head>";

    html += "<style type=\"text/css\">";
    html += "table "
            "{"
            "border-style: none;"
            "border-top: none;"
            "border-style: solid;"
            "border-width: 1px;"
            "border-color: #F3a28a;"
            "border-bottom-color: #ffffff;"
            "border-top-color: #ffffff;"
            "border-right-color: #F3a28a;"
            "border-collapse: collapse;"
            "}";

    html += "th "
            "{background-color:grey;"
            "font-weight: bold ;"
            "border: 1px solid blue;"
            "}";

    html += "td"
            "{"
            "border-top: none;"
            "border-width: 1px;"
            "border-color: #f302fa;"
            ""
            "}";

    html += "table.tblb {border-collapse: collapse; border-width: 1px;border-color: #430ffa;color: green;border-bottom-color:#ffffff;border-top-color:#ffffff;}";
    html += "table.tblb td {padding: 1px;border-width: 0px; border-bottom-color:#ffffff;border-top-color:#ffffff;}";
    html += "table.tblb th {padding: 1px;}";
    //    html += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    //    html += "table.tblb th {background-color:#0000f1; padding-left:5px}";


    html += "</style>";

    html += "</head>";
    html += "<body>";

    html += "<center><font size =\"5\">"+ReportLine+"</center>";

    html += "<table  width=\"100%\" border=\"1\" cellspacing=\"0\" border-top-color=\"#ffffff\">";      //table-1Main
    html += "<thead>";
    html += "<tr>";
    html += "<td colspan=\""+colCountStr+"\">";

    html += "<table width=\"100%\" border=\"1\" cellspacing=\"0\" border-top-color=\"#ffffff\">";       //Table-Head1
    html += "<tr><td width=\"50%\">";

    html += "<table width=\"100%\" border=\"1\" cellspacing=\"0\" style=\"border-width: 1px; border-top-style : none; border-bottom-style:none;color:#000000; border-bottom-color: #ffffff; border-top-color: #ffffff; \">";       //Table-Head2.1

    html += "<tr><td colspan=\"2\"><font size =\"6\">";
    html += companyName ;
    html += "</td></tr>";

    html += "<tr><td colspan=\"2\"><font size =\"5\">";
    html += addressLine ;
    html += "</td></tr>";

    html += "<tr></tr>";
    html += "<tr><td colspan=\"2\">";

    html += "<table width =\"100%\" class=\"tblb\" cellspacing=\"0\" >";
    html += "<tr>";
    html += "<td width=\"25%\"><font size =\"4\">";
    html += "Customer : " ;
    html += "<td><font size =\"4\">";
    html += obj->ledgerObject.LedgerName;
    html += "<br>";
    html += obj->ledgerObject.Address.replace("\n","<br>");
    html += "</td></tr></table>";

    html += "</td></tr>";

    html += "</table></td>";

    html += "<td width=\"50%\">";

    html += "<table width=\"100%\" border=\"1\" cellspacing=\"0\" class =\"tblb\">";       //2.2

    html += "<tr><td><font size =\"4\">Date </td><td>";
    html += obj->VoucherDate.toString("dd-MM-yyyy");
    html += "</td></tr>";

    html += "<tr><td><font size =\"4\"> Voucher No.</td><td>";
    html += obj->voucherNumber;
    html += "</td></tr>";

    html += "<tr><td><font size =\"4\">Salesman</td><td><font size =\"4\">";
    html += sman;
    html += "</td></tr>";

    html += "<tr><td><font size =\"4\">Prepared By</td><td><font size =\"4\">";
    html += addedby;
    html += "</td></tr>";

    html += "</table></td>";
    html += "</table>";


    html += "</td>";
    html += "<tr>";
    html += "<tr>";

    html += "<th align=\"left\" width =\""+QString::number(widthList[0])+"%\"><font size =\"4\">#</th>";
    html += "<th align=\"left\" width =\""+QString::number(widthList[1])+"%\"><font size =\"4\">Items</th>";
    html += "<th align=\"left\" width =\""+QString::number(widthList[2])+"%\"><font size =\"4\">MRP</th>";
    html += "<th align=\"left\" width =\""+QString::number(widthList[3])+"%\"><font size =\"4\">Quantity</th>";

    html += "</tr>";


    html += "</thead>";

    html += "<tbody>";

    for(int p=0;p<1;p++){
        for(int i=0;i<obj->InventoryItems.length();i++){
            html += "<tr>";

            html += "<td width=\""+QString::number(widthList[0])+"%\"><font size =\"4\">"+QString::number(i+1) +"</td>";

            html += "<td width=\""+QString::number(widthList[1])+"%\"><font size =\"4\">"+obj->InventoryItems[i].BaseItem.ItemName+"</td>";
            html += "<td align =\"right\" width=\""+QString::number(widthList[2])+"%\"><font size =\"4\">"+QString::number(obj->InventoryItems[i].BaseItem.price_2,'f',2)+"</td>";
            html += "<td align =\"right\" width=\""+QString::number(widthList[3])+"%\"><font size =\"4\">"+QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints)+"</td>";
            html +="</tr>";
        }
    }

    qDebug()<<"HTML "<<html;

    return html;
}

void PrinterHelper::printVoucherHeaderFooter(QPrinter& printer,
                                             int pageNumber,
                                             int pageCount,
                                             QPainter* painter,
                                             QTextDocument* doc,
                                             const QRectF& textRect,
                                             qreal footerHeight,
                                             GeneralVoucherDataObject *voucher,
                                             QList<qreal> widthList)
{
    qDebug()<<Q_FUNC_INFO;
    const QSizeF pageSize = printer.pageRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins;
    int endx =  pageSize.width()-borderMargins;
    int _3quatx = (midx+endx)/2;
    int quatx = midx+borderMargins/2;

    int localMargins=5;

    qDebug()<<"PRINTER PAPER  SIZE : "<<pageSize;
    qDebug()<<"PRINTER PAGE(MM)SIZE : "<<printer.pageRect(QPrinter::Millimeter).size();
    qDebug()<<"PRINTER PAGE(Pxl)IZE : "<<printer.pageRect().size();

    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    //    painter->setBrush(Qt::green);

    //    painter->drawRect(textRect);


    //Righ Ribbon;

    QRect myQRect(pageSize.width()-15,0,10,pageSize.height());
    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    gradient.setColorAt(0, "#ff0000");
    gradient.setColorAt(0.5, "#ffcc00");
    gradient.setColorAt(1, "#ff0000");
    painter->fillRect(myQRect, gradient);

    const double bm = borderMargins;// mmToPixels(printer, borderMargins);
    //    const double bm = /*borderMargins;//*/ mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);

    QString l1= LoginValues::getCompany().POBox.length()>0?"PO. Box: "+LoginValues::getCompany().POBox:"";
    QString l2= LoginValues::getCompany().branch;
    QString l22=LoginValues::getCompany().addressLine;
    QString l3= LoginValues::getCompany().PhoneNumber.length()>0?"T: "+LoginValues::getCompany().PhoneNumber:"";
    QString l4= LoginValues::getCompany().Fax.length()>0?"F: "+LoginValues::getCompany().Fax:"";
    QString l5= LoginValues::getCompany().email.length()>0?"E: "+LoginValues::getCompany().email:"";

    QString bankInfo = LoginValues::getCompany().bankDetails;

    int lf=14;


    QByteArray companyLogo = LoginValues::getCompany().logo;
    QImage img = QImage::fromData(companyLogo);
//    img.scaled(40,40);
    //    img.scaledToWidth(tLength,Qt::SmoothTransformation);
    QFont CompanyFont("Arial",11);CompanyFont.setBold(true); CompanyFont.setItalic(true);
    qreal companyFontHeight = painter->fontMetrics().height();

    painter->setFont(CompanyFont);
    int x = midx+tLength/4;
    int y = borderMargins-companyFontHeight;
    int lineSpacing14 = lf * 1.5;
    int lineSpacing15 = lf * 1.5;
    int lineSpacing13 = lf * 1.3;

    if(companyLogo.size()>0)
        painter->drawImage(QPoint(borderMargins,borderMargins),img/*.scaledToWidth(tLength/2-10,Qt::SmoothTransformation)*/);

    else{
        painter->drawText(borderMargins,borderMargins,LoginValues::getCompany().CompanyName);


        painter->setFont(normalFont);
        painter->setBrush(normalBrush);
        painter->setPen(normalPen);


        painter->drawText(borderMargins,borderMargins+lineSpacing13,l2);
        painter->drawText(borderMargins,borderMargins+2*lineSpacing13,l22);
        if(printer.pageSize() == QPrinter::A5){
            painter->drawText(borderMargins,borderMargins+3*lineSpacing13,l3/*+" "+l5*/);
            painter->drawText(borderMargins,borderMargins+4*lineSpacing13,l5);
        }
        else
            painter->drawText(borderMargins,borderMargins+3*lineSpacing13,l3+" "+l5);
    }

    //Translate shabib


    //    painter->translate(borderMargins,borderMargins+2*lf);
    QFont ReportNameFont("Verdana",lf*0.8);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));
    if(printer.pageSize() == QPrinter::A5)
        painter->fillRect(borderMargins,borderMargins+4*lineSpacing13+2,10,painter->fontMetrics().height(),QColor("#ed1a24"));
    else
        painter->fillRect(borderMargins,borderMargins+3*lineSpacing13+2,10,painter->fontMetrics().height(),QColor("#ed1a24"));



    painter->setPen(penHText);

    if(printer.pageSize() == QPrinter::A5){
        painter->drawText(borderMargins+20,borderMargins+lineSpacing13*5+2,voucher->voucherType.toUpper());
    }
    else
        painter->drawText(borderMargins+20,borderMargins+lineSpacing13*4+2,voucher->voucherType.toUpper());


    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);
    painter->save();

    //    painter->resetTransform();


    // Footer: page number or "end"

    QString PhoneSymbol = "\u260F";
    QString emailSymbol = "\u2709";
    QString truckSymbol = "\u26DF";

    int yy=0;

    //left side banner

    if(printer.pageSize() == QPrinter::A5){
        painter->translate(borderMargins,borderMargins+7*lf);
    }
    else
        painter->translate(borderMargins,borderMargins+6*lf);

    QPen greyPen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));
    QPen redPen(QColor("#ff0000"));
    QPen yellowPen(QColor("#ffff00"));


    painter->setPen(greyPen);



    painter->setPen(normalPen);

    painter->fillRect(0,0,tLength/2,lineSpacing14,QColor("#ed1a24"));

    painter->drawRect(0,0,tLength/2,lf*10+4);

    lineSpacing14 = lf * 1.4;
    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(localMargins,lineSpacing14,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    painter->setFont(LedgerNameFont);


    yy=40;

    QString ledName = voucher->ledgerObject.LedgerName.toUpper();
    QString address = voucher->ledgerObject.Address.toUpper();
    QString pobox= voucher->ledgerObject.PoBox.toUpper();
    QString city = voucher->ledgerObject.City.toUpper();
    QString country = voucher->ledgerObject.Country.toUpper();
    QString phoneNumber = voucher->ledgerObject.phoneNumber.toUpper();
    QString email = voucher->ledgerObject.emailAddress.toLower();
    QString custContact = voucher->ledgerObject.ContactPersonName.toUpper();
    QString cusPhone = voucher->ledgerObject.ContactPersonNumber;
    QString location = voucher->Location;
    QString Trn = voucher->ledgerObject.TRN;


    if(voucher->voucherType.contains("Quotation")){
        //        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<obj.Contact.CompanyName;
        ledName = voucher->Contact.CompanyName.toUpper();
        address = "";
        if(voucher->Contact.Building.length()>0)
            address += voucher->Contact.Building.toUpper() + ", ";
        if(voucher->Contact.Street.length()>0)
            address += voucher->Contact.Street.toUpper()+ ", ";
        address +=voucher->Contact.address.toUpper();
        pobox = voucher->Contact.POBox.toUpper();
        city= voucher->Contact.city.toUpper();
        country = voucher->Contact.country.toUpper();
        phoneNumber = voucher->Contact.PhoneNumber;
        email=voucher->Contact.email;
        custContact = voucher->Contact.ContactName.toUpper();
        cusPhone = voucher->Contact.mobileNumber;
    }
    int lcnt =2;
//    if(printer.pageSize() == QPrinter::A5)
//        lcnt = 1;
    painter->drawText(localMargins,lineSpacing14*lcnt,ledName);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,address);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,city);
    lcnt++;
    if(Trn.length()>0)
        painter->drawText(localMargins,lineSpacing14*lcnt,"GSTRN : "+Trn);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,PhoneSymbol+"  "+phoneNumber);
    lcnt++;
    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(localMargins-2,lineSpacing14*lcnt,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(localMargins+10,lineSpacing14*lcnt,email.toLower());



    //RIGHT SIDE BANNER

    painter->resetTransform();

    painter->translate(midx+localMargins,borderMargins);

    int halfLength = tLength/2;
    int _3partLength = halfLength/3;


    int endx2 = halfLength - localMargins;

    painter->setPen(normalPen);

    //RIGHT FULL OUTER BOX
    painter->drawRect(0, 3 ,endx2, lf*10+4 + borderMargins+10);

    // horizontal Lines;

    yy  = 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);
    yy += 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);

    //Vertical Lines


    painter->drawLine(_3partLength  ,3,_3partLength  ,yy);
    painter->drawLine(_3partLength*2,3,_3partLength*2,yy);

    yy += 3*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);


    // Paint Contents


    // First Line
    yy = lineSpacing14;
    painter->setPen(normalPen);
    painter->setFont(HeadingFont);

    QRect curr = QRect(localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);

    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Date");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->VoucherDate.toString("dd-MM-yyyy"));

    curr = QRect(_3partLength+localMargins,localMargins,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Invoice No.");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->voucherNumber);

    curr = QRect(_3partLength*2+localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Page");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));



    // Second Lines


    curr = QRect(localMargins,localMargins+ 2*lineSpacing13 ,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"GST No");
    QFont TrnFont("Calibri", 8);
    painter->setFont(TrnFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,LoginValues::company.trn.toUpper());

    curr = QRect(_3partLength+localMargins,localMargins + 2*lineSpacing13,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Printed at");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss"));

    curr = QRect(_3partLength*2+localMargins,localMargins + 2*lineSpacing13,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, " ");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,"");



    painter->setFont(LedgerNameFont);
    painter->setPen(normalPen);


    UserProfileDataModel salesman = userHelper->getEmployeeByID(voucher->SalesmanID);
    //    yy+=20;

    painter ->setFont(HeadingFont);

    curr = QRect(localMargins,7*lineSpacing14,halfLength,2*lineSpacing15);
    if(bankInfo.length() > 0){
        painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Bank A/C");
        painter->setFont(LedgerNameFont);
        painter ->setPen(normalPen);
        curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
        yy+=16;
        painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,bankInfo);

    }
    else {

        painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Contact");
        painter->setFont(LedgerNameFont);
        painter ->setPen(normalPen);
        curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
        yy+=16;
        painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Name.toUpper());
        curr = QRect(localMargins,9*lineSpacing14,halfLength,2*lineSpacing15);
        yy+=17;
        painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,PhoneSymbol+"  "+salesman.Phone);
    }
    curr = QRect(localMargins,10*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=17;
    painter->setFont(drawfont);
    //    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Email);
    painter->setPen(normalPen);
    yy+=18;
    curr = QRect(localMargins,4*lineSpacing14,halfLength,2*lineSpacing15);
    painter ->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Customer Contact");
    //    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    curr = QRect(localMargins,5*lineSpacing14,halfLength,2*lineSpacing15);
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,custContact.toUpper());
    //    painter->drawText(5,yy,custContact.toUpper());
    yy+=17;
    painter->setFont(LedgerNameFont);
    //    painter->drawText(5,yy,PhoneSymbol+"   "+cusPhone);
    QFont fnt12; fnt12.setPointSize(15);
    painter->setFont(fnt12);
    //    painter->drawText(midx/2,yy,truckSymbol+"  "); NO nEED
    painter->setFont(LedgerNameFont);
    //    painter->drawText(midx/2+15,yy,"Loc: " + location);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());

    painter->setPen(normalPen);

    painter->resetTransform();

    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0,
                              pageNumber * doc->pageSize().height(),
                              doc->pageSize().width() ,
                              doc->pageSize().height()
                              );




    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->drawRect(textRect);
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());

    doc->drawContents(painter);
    painter->restore();

    painter->resetTransform();

    painter->drawRect(textRect.x(),textRect.y(),tLength,2*lf);
    int xcord = borderMargins ;
    qDebug()<<"width sizes <....913....> "<<widthList.size();



    if(pageNumber+1 == pageCount){

        // LAST PAGE


        painter->drawRect(textRect.x(),textRect.y()+textRect.height(),tLength,2*lf);

        QStringList gTotalLine;
        gTotalLine<< " ";
        gTotalLine<< "Grand total";
        if(printer.pageSize()!= QPrinter::A5)
            gTotalLine<< " ";
        gTotalLine<< QString::number(voucher->quantityTotal,'f',2);
        gTotalLine<< " ";
        gTotalLine<< " ";
        gTotalLine<< " ";


        gTotalLine<< QString::number(voucher->grandTotal,'f',2);

        QFont totalLineFont("Calibri",14);
        if(voucher->quantityTotal > 100000 || voucher->grandTotal > 100000 ){
            totalLineFont.setPointSize(10);
            totalLineFont.setBold(true);
        }
        if(printer.pageSize() == QPrinter::A5){
            totalLineFont.setPointSize(12);
            totalLineFont.setBold(true);

        }

        int prevXcord=borderMargins;
        painter->setFont(totalLineFont);


        for(int i =0;i<widthList.size();i++){

            //            qDebug()<<" I is "<<i<<widthList[i];

            xcord += textRect.width()*widthList[i]/100;

            int boxwidth = tLength*widthList[i]/100-3;


            if(i < widthList.size()-1)
                painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height()+2*lf);

            if(i==1)
                painter->drawText(QRect(prevXcord+2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignLeft|Qt::AlignVCenter,gTotalLine[i]);
            else
                painter->drawText(QRect(prevXcord-2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignRight|Qt::AlignVCenter,gTotalLine[i]);

            prevXcord = xcord;

            qDebug()<<"Prev :"<<prevXcord;
        }

        yy=textRect.bottom()+2*lf;
        painter->setFont(HeadingFont);
        QRect amtInWordsrect = QRect(borderMargins,yy,tLength,8*lf);
        painter->drawRect(amtInWordsrect);
        amtInWordsrect.setX(borderMargins+3);
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,tr("Amount In Words : "));
        amtInWordsrect.setY(textRect.bottom()+3.5*lf);

        float amount = voucher->grandTotal;

        int integerpart = amount;
        int decimalP = amount*100;
        decimalP = decimalP % 100;

        QString amtinWords = "Rupees "+numberToText( integerpart);
        if(decimalP>0)
            amtinWords += " and "+ numberToText(decimalP)+" Paisa";
        amtinWords += " only.";
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,"   "+amtinWords);




    }
    else{
        for(int i =0;i<widthList.size()-1;i++){
            xcord += tLength*widthList[i]/100;
            painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height());
        }
    }

    painter->setFont(normalFont);
    painter->setPen(bluePen);



    painter->setPen(normalPen);
    QRectF footerRect = textRect;
    footerRect.setTop(pageSize.height()-borderMargins);
    footerRect.setHeight(footerHeight);

    painter->setPen(normalPen);

    QString footString = "This is a computer generated Invoice";

    QString authString = "AUTHORIZED SIGNATORY";

    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignRight, authString);


    painter->drawText(borderMargins,footerRect.y(),pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y(),textRect.width()+borderMargins,footerRect.y());

    //    painter->drawEllipse(borderMargins,footerRect.y(),5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);
    qDebug()<<Q_FUNC_INFO<<__LINE__;


}

void PrinterHelper::printDelHeaderFooter(QPrinter& printer,
                                         int pageNumber,
                                         int pageCount,
                                         QPainter* painter,
                                         QTextDocument* doc,
                                         const QRectF& textRect,
                                         qreal footerHeight,
                                         GeneralVoucherDataObject *voucher,
                                         QList<qreal> widthList)
{
    qDebug()<<Q_FUNC_INFO;
    const QSizeF pageSize = printer.pageRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins;
    int endx =  pageSize.width()-borderMargins;
    int _3quatx = (midx+endx)/2;
    int quatx = midx+borderMargins/2;

    int localMargins=5;

    qDebug()<<"PRINTER PAPER  SIZE : "<<pageSize;
    qDebug()<<"PRINTER PAGE(MM)SIZE : "<<printer.pageRect(QPrinter::Millimeter).size();
    qDebug()<<"PRINTER PAGE(Pxl)IZE : "<<printer.pageRect().size();

    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    //    painter->setBrush(Qt::green);

    //    painter->drawRect(textRect);


    //Righ Ribbon;

    QRect myQRect(pageSize.width()-15,0,10,pageSize.height());
    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    gradient.setColorAt(0, "#ff0000");
    gradient.setColorAt(0.5, "#ffcc00");
    gradient.setColorAt(1, "#ff0000");
    painter->fillRect(myQRect, gradient);

    const double bm = borderMargins;// mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);

    QString l1= LoginValues::getCompany().POBox.length()>0?"PO. Box: "+LoginValues::getCompany().POBox:"";
    QString l2= LoginValues::getCompany().branch;
    QString l22=LoginValues::getCompany().addressLine;
    QString l3= LoginValues::getCompany().PhoneNumber.length()>0?"T: "+LoginValues::getCompany().PhoneNumber:"";
    QString l4= LoginValues::getCompany().Fax.length()>0?"F: "+LoginValues::getCompany().Fax:"";
    QString l5= LoginValues::getCompany().email.length()>0?"E: "+LoginValues::getCompany().email:"";

    int lf=14;
    QFont CompanyFont("Calibri",16);CompanyFont.setBold(true);
    painter->setFont(CompanyFont);
    painter->drawText(borderMargins,borderMargins,LoginValues::getCompany().CompanyName);



    qreal companyFontHeight = painter->fontMetrics().height();

    int x = midx+tLength/4;
    int y = borderMargins-companyFontHeight;
    int lineSpacing14 = lf * 1.5;
    int lineSpacing15 = lf * 1.5;
    int lineSpacing13 = lf * 1.3;

    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);


    painter->drawText(borderMargins,borderMargins+lineSpacing13,l2);
    painter->drawText(borderMargins,borderMargins+2*lineSpacing13,l22);
    painter->drawText(borderMargins,borderMargins+3*lineSpacing13,l3+" "+l5);


    //Translate shabib


    //    painter->translate(borderMargins,borderMargins+2*lf);
    QFont ReportNameFont("Verdana",lf*0.8);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));
    painter->fillRect(borderMargins,borderMargins+3*lineSpacing13+2,10,painter->fontMetrics().height(),QColor("#ed1a24"));


    painter->setPen(penHText);

    painter->drawText(borderMargins+20,borderMargins+lineSpacing13*4+2,voucher->voucherType.toUpper());


    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);
    painter->save();

    //    painter->resetTransform();


    // Footer: page number or "end"

    QString PhoneSymbol = "\u260F";
    QString emailSymbol = "\u2709";
    QString truckSymbol = "\u26DF";

    int yy=0;

    //left side banner

    painter->translate(borderMargins,borderMargins+6*lf);

    QPen greyPen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));
    QPen redPen(QColor("#ff0000"));
    QPen yellowPen(QColor("#ffff00"));


    painter->setPen(greyPen);



    painter->setPen(normalPen);

    painter->fillRect(0,0,tLength/2,lineSpacing14,QColor("#ed1a24"));

    painter->drawRect(0,0,tLength/2,lf*10+4);

    lineSpacing14 = lf * 1.4;
    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(localMargins,lineSpacing14,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    painter->setFont(LedgerNameFont);


    yy=40;

    QString ledName = voucher->ledgerObject.LedgerName.toUpper();
    QString address = voucher->ledgerObject.Address.toUpper();
    QString pobox= voucher->ledgerObject.PoBox.toUpper();
    QString city = voucher->ledgerObject.City.toUpper();
    QString country = voucher->ledgerObject.Country.toUpper();
    QString phoneNumber = voucher->ledgerObject.phoneNumber.toUpper();
    QString email = voucher->ledgerObject.emailAddress.toLower();
    QString custContact = voucher->ledgerObject.ContactPersonName.toUpper();
    QString cusPhone = voucher->ledgerObject.ContactPersonNumber;
    QString location = voucher->Location;
    QString Trn = voucher->ledgerObject.TRN;

    if(voucher->voucherType.contains("Quotation")){
        //        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<obj.Contact.CompanyName;
        ledName = voucher->Contact.CompanyName.toUpper();
        address = "";
        if(voucher->Contact.Building.length()>0)
            address += voucher->Contact.Building.toUpper() + ", ";
        if(voucher->Contact.Street.length()>0)
            address += voucher->Contact.Street.toUpper()+ ", ";
        address +=voucher->Contact.address.toUpper();
        pobox = voucher->Contact.POBox.toUpper();
        city= voucher->Contact.city.toUpper();
        country = voucher->Contact.country.toUpper();
        phoneNumber = voucher->Contact.PhoneNumber;
        email=voucher->Contact.email;
        custContact = voucher->Contact.ContactName.toUpper();
        cusPhone = voucher->Contact.mobileNumber;
    }

    int lcnt =2;
    painter->drawText(localMargins,lineSpacing14*lcnt,ledName);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,address);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,city);
    lcnt++;
    if(Trn.length()>0)
        painter->drawText(localMargins,lineSpacing14*lcnt,"GSTRN : "+Trn);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,PhoneSymbol+"  "+phoneNumber);
    lcnt++;
    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(localMargins-2,lineSpacing14*lcnt,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(localMargins+10,lineSpacing14*lcnt,email.toLower());



    //RIGHT SIDE BANNER

    painter->resetTransform();

    painter->translate(midx+localMargins,borderMargins);

    int halfLength = tLength/2;
    int _3partLength = halfLength/3;


    int endx2 = halfLength - localMargins;

    painter->setPen(normalPen);

    //RIGHT FULL OUTER BOX
    painter->drawRect(0, 3 ,endx2, lf*10+4 + borderMargins+10);

    // horizontal Lines;

    yy  = 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);
    yy += 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);

    //Vertical Lines


    painter->drawLine(_3partLength  ,3,_3partLength  ,yy);
    painter->drawLine(_3partLength*2,3,_3partLength*2,yy);

    yy += 3*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);


    // Paint Contents


    // First Line
    yy = lineSpacing14;
    painter->setPen(normalPen);
    painter->setFont(HeadingFont);

    QRect curr = QRect(localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);

    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Date");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->VoucherDate.toString("dd-MM-yyyy"));

    curr = QRect(_3partLength+localMargins,localMargins,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Document No.");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->voucherNumber);

    curr = QRect(_3partLength*2+localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Page");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));



    // Second Lines


    curr = QRect(localMargins,localMargins+ 2*lineSpacing13 ,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"GST No");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,LoginValues::company.trn.toUpper());

    curr = QRect(_3partLength+localMargins,localMargins + 2*lineSpacing13,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Printed at");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss"));

    curr = QRect(_3partLength*2+localMargins,localMargins + 2*lineSpacing13,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, " ");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,"");



    painter->setFont(LedgerNameFont);
    painter->setPen(normalPen);


    UserProfileDataModel salesman = userHelper->getEmployeeByID(voucher->SalesmanID);
    //    yy+=20;

    painter ->setFont(HeadingFont);

    curr = QRect(localMargins,7*lineSpacing14,halfLength,2*lineSpacing15);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Contact");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);
    curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=16;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Name.toUpper());
    curr = QRect(localMargins,9*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=17;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,PhoneSymbol+"  "+salesman.Phone);
    curr = QRect(localMargins,10*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=17;
    painter->setFont(drawfont);
    //    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Email);
    painter->setPen(normalPen);
    yy+=18;
    curr = QRect(localMargins,4*lineSpacing14,halfLength,2*lineSpacing15);
    painter ->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Customer Contact");
    //    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    curr = QRect(localMargins,5*lineSpacing14,halfLength,2*lineSpacing15);
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,custContact.toUpper());
    //    painter->drawText(5,yy,custContact.toUpper());
    yy+=17;
    painter->setFont(LedgerNameFont);
    //    painter->drawText(5,yy,PhoneSymbol+"   "+cusPhone);
    QFont fnt12; fnt12.setPointSize(15);
    painter->setFont(fnt12);
    //    painter->drawText(midx/2,yy,truckSymbol+"  "); NO nEED
    painter->setFont(LedgerNameFont);
    //    painter->drawText(midx/2+15,yy,"Loc: " + location);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());

    painter->setPen(normalPen);

    painter->resetTransform();

    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0,
                              pageNumber * doc->pageSize().height(),
                              doc->pageSize().width() ,
                              doc->pageSize().height()
                              );




    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->drawRect(textRect);
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    painter->resetTransform();

    painter->drawRect(textRect.x(),textRect.y(),tLength,2*lf);
    int xcord = borderMargins ;
    qDebug()<<"width sizes <....913....> "<<widthList.size();



    if(pageNumber+1 == pageCount){

        // LAST PAGE


        painter->drawRect(textRect.x(),textRect.y()+textRect.height(),tLength,2*lf);

        QStringList gTotalLine;
        gTotalLine<< " ";
        gTotalLine<< "Grand total";
        gTotalLine<< " ";
        gTotalLine<< QString::number(voucher->quantityTotal,'f',2);
        gTotalLine<< " ";
        gTotalLine<< " ";



        gTotalLine<< QString::number(voucher->grandTotal,'f',2);

        QFont totalLineFont("Calibri",10);

        int prevXcord=borderMargins;
        painter->setFont(totalLineFont);

        for(int i =0;i<widthList.size();i++){

            //            qDebug()<<" I is "<<i<<widthList[i];

            xcord += textRect.width()*widthList[i]/100;

            int boxwidth = tLength*widthList[i]/100-3;


            if(i < widthList.size()-1){
                painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height()+2*lf);
            }
            if(i==1)
                painter->drawText(QRect(prevXcord+2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignLeft|Qt::AlignVCenter,gTotalLine[i]);
            else
                painter->drawText(QRect(prevXcord-2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignRight|Qt::AlignVCenter,gTotalLine[i]);

            prevXcord = xcord;

            qDebug()<<"Prev :"<<prevXcord;
        }

        yy=textRect.bottom()+2*lf;
        painter->setFont(HeadingFont);
        QRect amtInWordsrect = QRect(borderMargins,yy,tLength,4*lf);
        //        painter->drawRect(amtInWordsrect);
        amtInWordsrect.setX(borderMargins+3);
        //        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,tr("Amount In Words : "));
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,tr("Narration : "));
        //        amtInWordsrect.setY(textRect.bottom()+3.5*lf);

        float amount = voucher->grandTotal;

        int integerpart = amount;
        int decimalP = amount*100;
        decimalP = decimalP % 100;

        QString amtinWords = "Rupees "+numberToText( integerpart);
        if(decimalP>0)
            amtinWords += " and "+ numberToText(decimalP)+" Paise";
        amtinWords += " only.";
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,"   "+voucher->narration.replace("\n","<br>"));



    }
    else{
        for(int i =0;i<widthList.size()-1;i++){
            xcord += tLength*widthList[i]/100;
            //            painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height());
        }
    }

    painter->setFont(normalFont);
    painter->setPen(bluePen);



    painter->setPen(normalPen);
    QRectF footerRect = textRect;
    footerRect.setTop(pageSize.height()-borderMargins);
    footerRect.setHeight(footerHeight);

    painter->setPen(normalPen);

    QString footString = "This is a computer generated Invoice";

    QString authString = "AUTHORIZED SIGNATORY";

    //    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignLeft, authString);

    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignLeft, "Packed By:");
    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignHCenter, "Delivery Via:");

    //    painter->drawText(borderMargins,footerRect.y(),pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y(),textRect.width()+borderMargins,footerRect.y());

    //    painter->drawEllipse(borderMargins,footerRect.y(),5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);

}

void PrinterHelper::printDispatchFooter(QPrinter& printer,
                                        int pageNumber,
                                        int pageCount,
                                        QPainter* painter,
                                        QTextDocument* doc,
                                        QRectF textRect,
                                        qreal footerHeight,
                                        GeneralVoucherDataObject *voucher,
                                        QList<qreal> widthList)
{
    qDebug()<<Q_FUNC_INFO;
    const QSizeF pageSize = printer.pageRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins;
    int endx =  pageSize.width()-borderMargins;
    int _3quatx = (midx+endx)/2;
    int quatx = midx+borderMargins/2;
    int lf=14;
    int localMargins=5;

    qDebug()<<"PRINTER PAPER  SIZE : "<<pageSize;
    qDebug()<<"PRINTER PAGE(MM)SIZE : "<<printer.pageRect(QPrinter::Millimeter).size();
    qDebug()<<"PRINTER PAGE(Pxl)IZE : "<<printer.pageRect().size();

    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();

    painter->save();

    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0,
                              pageNumber * doc->pageSize().height(),
                              doc->pageSize().width() ,
                              doc->pageSize().height()
                              );




    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    if(pageNumber+1==pageCount){
        textRect = QRectF(textRect.left(),textRect.top(),textRect.width(),textRect.height()-100);
    }
    painter->drawRect(textRect);
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    int xcord = borderMargins ;

    QPen bluePen(QColor("#0000ff"));
    painter->setFont(normalFont);
    painter->setPen(bluePen);

    int x = borderMargins;
    int y = textRect.bottom()+lf;
    qDebug()<<x<<y;
    painter->setPen(QColor(0,0,0));
    painter->drawText(QPoint(x,y),"Narration : \n"+voucher->narration);



    painter->drawText(QPoint(x,y+2*lf),"Packed By : \n"+voucher->narration);
    painter->drawText(QPoint(x,y+4*lf),"Delivered Via : \n"+voucher->narration);

    painter->setPen(normalPen);
    QRectF footerRect = textRect;
    footerRect.setTop(pageSize.height()-borderMargins);
    footerRect.setHeight(footerHeight);
    painter->drawRect(footerRect);
    //    painter->setPen(normalPen);

    QString footString = "This is a computer generated Invoice";

    QString authString = "AUTHORIZED SIGNATORY";

    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignRight, authString);


    //    painter->drawText(borderMargins,footerRect.y(),pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y(),textRect.width()+borderMargins,footerRect.y());

    //    painter->drawEllipse(borderMargins,footerRect.y(),5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);

}


void PrinterHelper::printWorkOrderHeaderFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter, QTextDocument *doc, const QRectF &textRect, qreal footerHeight, GeneralVoucherDataObject *voucher, QList<qreal> widthList)
{
    const QSizeF pageSize = printer.paperRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins;
    int endx =  pageSize.width()-borderMargins;
    int _3quatx = (midx+endx)/2;
    int quatx = midx+borderMargins/2;

    int localMargins=5;


    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    //    painter->setBrush(Qt::green);

    //    painter->drawRect(textRect);


    //Righ Ribbon;

    QRect myQRect(pageSize.width()-15,0,10,pageSize.height());
    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    gradient.setColorAt(0, "#ff0000");
    gradient.setColorAt(0.5, "#ffcc00");
    gradient.setColorAt(1, "#ff0000");
    painter->fillRect(myQRect, gradient);

    const double bm = borderMargins;// mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);

    //  painter->drawRect(borderRect);


    //  painter->drawImage(borderMargins,10,QImage(":/images/rrmzlogo.png"));


    //  painter->drawImage(borderMargins+100,10,QImage(":/images/thirty.png"));



    QString l1= LoginValues::getCompany().POBox.length()>0?"PO. Box: "+LoginValues::getCompany().POBox:"";
    QString l2= LoginValues::getCompany().branch+", "+LoginValues::getCompany().addressLine;
    QString l3= LoginValues::getCompany().PhoneNumber.length()>0?"T: "+LoginValues::getCompany().PhoneNumber:"";
    QString l4= LoginValues::getCompany().Fax.length()>0?"F: "+LoginValues::getCompany().Fax:"";
    QString l5= LoginValues::getCompany().email.length()>0?"E: "+LoginValues::getCompany().email:"";

    int lf=16;
    QFont CompanyFont("Calibri",lf);CompanyFont.setBold(true);
    painter->setFont(CompanyFont);
    painter->drawText(borderMargins,borderMargins,LoginValues::getCompany().CompanyName);



    qreal companyFontHeight = painter->fontMetrics().height();

    int x = midx+tLength/4;
    int y = borderMargins-companyFontHeight;
    int lineSpacing14 = lf*1.5;
    int lineSpacing15 = lf * 1.5;
    int lineSpacing13 = lf * 1.3;

    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);


    painter->drawText(borderMargins,borderMargins+lineSpacing14,l2);

    //    painter->drawText(borderMargins+625,25,l1);
    //    painter->drawText(borderMargins+625,40,l2);





    painter->drawText(x,y,l3);



    painter->drawText(x,y+lf,l4);
    painter->drawText(x,y+2*lf,l5);



    //Translate shabib

    painter->translate(borderMargins,borderMargins+2*lf);
    QFont ReportNameFont("Verdana",16);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));

    painter->fillRect(1,lf,10,painter->fontMetrics().height(),QColor("#ed1a24"));


    painter->setPen(penHText);

    painter->drawText(20,40,voucher->voucherType.toUpper());


    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);
    painter->save();

    painter->resetTransform();


    // Footer: page number or "end"

    QString PhoneSymbol = "\u260F";
    QString emailSymbol = "\u2709";
    QString truckSymbol = "\u26DF";

    int yy=0;

    //left side banner

    painter->translate(borderMargins,borderMargins+6*lf);

    QPen greyPen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));
    QPen redPen(QColor("#ff0000"));
    QPen yellowPen(QColor("#ffff00"));


    painter->setPen(greyPen);



    painter->setPen(normalPen);

    painter->fillRect(0,0,tLength/2,lineSpacing14,QColor("#ed1a24"));

    painter->drawRect(0,0,tLength/2,lf*10);

    lineSpacing14 = lf*1.4;
    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(localMargins,lineSpacing14,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    painter->setFont(LedgerNameFont);


    yy=40;

    QString ledName = voucher->ledgerObject.LedgerName.toUpper();
    QString address = voucher->ledgerObject.Address.toUpper();
    QString pobox= voucher->ledgerObject.PoBox.toUpper();
    QString city = voucher->ledgerObject.City.toUpper();
    QString country = voucher->ledgerObject.Country.toUpper();
    QString phoneNumber = voucher->ledgerObject.phoneNumber.toUpper();
    QString email = voucher->ledgerObject.emailAddress.toLower();
    QString custContact = voucher->ledgerObject.ContactPersonName.toUpper();
    QString cusPhone = voucher->ledgerObject.ContactPersonNumber;
    QString location = voucher->Location;
    QString Trn = voucher->ledgerObject.TRN;


    int lcnt =2;
    painter->drawText(localMargins,lineSpacing14*lcnt,ledName);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,address);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,city);
    lcnt++;
    //    if(Trn.length()>0)
    //        painter->drawText(localMargins,lineSpacing14*lcnt,"TRN : "+Trn);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,PhoneSymbol+"  "+phoneNumber);
    lcnt++;
    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(localMargins,lineSpacing14*lcnt,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(localMargins+10,lineSpacing14*lcnt,email.toLower());



    //RIGHT SIDE BANNER

    painter->resetTransform();

    painter->translate(midx+localMargins,borderMargins);

    int halfLength = tLength/2;
    int _3partLength = halfLength/3;


    int endx2 = halfLength - localMargins;

    painter->setPen(normalPen);

    painter->drawRect(0, 3 ,endx2,lf*16);

    // horizontal Lines;

    yy  = 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);
    yy += 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);

    //Vertical Lines


    painter->drawLine(_3partLength  ,3,_3partLength  ,yy);
    painter->drawLine(_3partLength*2,3,_3partLength*2,yy);

    yy += 3*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);





    // Paint Contents

    // First Line
    yy = lineSpacing14;
    painter->setPen(normalPen);
    painter->setFont(HeadingFont);

    QRect curr = QRect(localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);

    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Date");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->VoucherDate.toString("dd-MM-yyyy"));

    curr = QRect(_3partLength+localMargins,localMargins,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Document No.");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->voucherNumber);

    curr = QRect(_3partLength*2+localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Page");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));



    // Second Lines


    curr = QRect(localMargins,localMargins+ 2*lineSpacing13 ,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"GST No");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,LoginValues::company.trn.toUpper());

    curr = QRect(_3partLength+localMargins,localMargins + 2*lineSpacing13,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Printed at");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss"));

    curr = QRect(_3partLength*2+localMargins,localMargins + 2*lineSpacing13,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, " ");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,"");



    painter->setFont(LedgerNameFont);
    painter->setPen(normalPen);


    UserProfileDataModel salesman = userHelper->getEmployeeByID(voucher->SalesmanID);
    yy+=20;
    painter ->setFont(HeadingFont);

    curr = QRect(localMargins,7*lineSpacing14,halfLength,2*lineSpacing15);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Contact");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);
    curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=16;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Name.toUpper());
    curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=17;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,PhoneSymbol+"  "+salesman.Phone);

    yy+=17;
    painter->setFont(drawfont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Email);
    painter->setPen(normalPen);
    //    yy+=15;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,custContact.toUpper());
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,PhoneSymbol+"   "+cusPhone);
    QFont fnt12; fnt12.setPointSize(15);
    painter->setFont(fnt12);
    //    painter->drawText(midx/2,yy,truckSymbol+"  ");
    painter->setFont(LedgerNameFont);
    painter->drawText(midx/2+15,yy,"Loc: " + location);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());

    painter->setPen(normalPen);

    painter->resetTransform();

    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0,
                              pageNumber * doc->pageSize().height(),
                              doc->pageSize().width() ,
                              doc->pageSize().height()
                              );




    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->drawRect(textRect);
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    painter->resetTransform();

    painter->drawRect(textRect.x(),textRect.y(),tLength,2*lf);
    int xcord = borderMargins ;
    qDebug()<<"width sizes <....913....> "<<widthList.size();



    if(pageNumber+1 == pageCount && false){

        // LAST PAGE


        painter->drawRect(textRect.x(),textRect.y()+textRect.height(),tLength,2*lf);

        QStringList gTotalLine;
        gTotalLine<< " ";
        gTotalLine<< "Grand total";
        gTotalLine<< " ";
        gTotalLine<< QString::number(voucher->quantityTotal,'f',2);
        gTotalLine<< " ";
        gTotalLine<< " ";
        gTotalLine<< " ";


        gTotalLine<< QString::number(voucher->grandTotal,'f',2);

        QFont totalLineFont("Calibri",14);

        int prevXcord=borderMargins;
        painter->setFont(totalLineFont);

        for(int i =0;i<widthList.size();i++){

            qDebug()<<" I is "<<i<<widthList[i];

            xcord += textRect.width()*widthList[i]/100;

            int boxwidth = tLength*widthList[i]/100-3;


            if(i < widthList.size()-1)
                painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height()+2*lf);

            if(i==1)
                painter->drawText(QRect(prevXcord+2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignLeft|Qt::AlignVCenter,gTotalLine[i]);
            else
                painter->drawText(QRect(prevXcord-2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignRight|Qt::AlignVCenter,gTotalLine[i]);

            prevXcord = xcord;

            qDebug()<<"Prev :"<<prevXcord;
        }

        yy=textRect.bottom()+2*lf;
        painter->setFont(HeadingFont);
        QRect amtInWordsrect = QRect(borderMargins,yy,tLength,4*lf);
        painter->drawRect(amtInWordsrect);
        amtInWordsrect.setX(borderMargins+3);
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,tr("Amount In Words : "));
        amtInWordsrect.setY(textRect.bottom()+3.5*lf);

        float amount = voucher->grandTotal;

        int integerpart = amount;
        int decimalP = amount*100;
        decimalP = decimalP % 100;

        QString amtinWords = "Rupees "+numberToText( integerpart);
        if(decimalP>0)
            amtinWords += " and "+ numberToText(decimalP)+" Paisa";
        amtinWords += " only.";
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,"   "+amtinWords);

    }

    else{
        for(int i =0;i<widthList.size()-1;i++){
            xcord += tLength*widthList[i]/100;
            painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height());
        }
    }

    painter->setFont(normalFont);
    painter->setPen(bluePen);


    qDebug()<<"98822";
    painter->setPen(normalPen);
    QRectF footerRect = textRect;
    footerRect.setTop(pageSize.height()-borderMargins);
    footerRect.setHeight(footerHeight);

    painter->setPen(normalPen);


    QString authString = "AUTHORIZED SIGNATORY";

    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignRight, authString);


    //    painter->drawText(borderMargins,footerRect.y(),pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y(),textRect.width()+borderMargins,footerRect.y());

    //    painter->drawEllipse(borderMargins,footerRect.y(),5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);

}

void PrinterHelper::printIssueReqHeaderFooter(QPrinter &printer,
                                              int pageNumber,
                                              int pageCount,
                                              QPainter *painter,
                                              QTextDocument *doc,
                                              const QRectF &textRect,
                                              qreal footerHeight,
                                              GeneralVoucherDataObject *voucher,
                                              QList<qreal> widthList)
{
    const QSizeF pageSize = printer.paperRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins;
    int endx =  pageSize.width()-borderMargins;
    int _3quatx = (midx+endx)/2;
    int quatx = midx+borderMargins/2;

    int localMargins=5;


    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    //    painter->setBrush(Qt::green);

    //    painter->drawRect(textRect);


    //Righ Ribbon;

    QRect myQRect(pageSize.width()-15,0,10,pageSize.height());
    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    gradient.setColorAt(0, "#ff0000");
    gradient.setColorAt(0.5, "#ffcc00");
    gradient.setColorAt(1, "#ff0000");
    painter->fillRect(myQRect, gradient);

    const double bm = borderMargins;// mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);

    //  painter->drawRect(borderRect);


    //  painter->drawImage(borderMargins,10,QImage(":/images/rrmzlogo.png"));


    //  painter->drawImage(borderMargins+100,10,QImage(":/images/thirty.png"));



    QString l1= LoginValues::getCompany().POBox.length()>0?"PO. Box: "+LoginValues::getCompany().POBox:"";
    QString l2= LoginValues::getCompany().branch+", "+LoginValues::getCompany().addressLine;
    QString l3= LoginValues::getCompany().PhoneNumber.length()>0?"T: "+LoginValues::getCompany().PhoneNumber:"";
    QString l4= LoginValues::getCompany().Fax.length()>0?"F: "+LoginValues::getCompany().Fax:"";
    QString l5= LoginValues::getCompany().email.length()>0?"E: "+LoginValues::getCompany().email:"";

    int lf=16;
    QFont CompanyFont("Calibri",lf);CompanyFont.setBold(true);
    painter->setFont(CompanyFont);
    painter->drawText(borderMargins,borderMargins,LoginValues::getCompany().CompanyName);



    qreal companyFontHeight = painter->fontMetrics().height();

    int x = midx+tLength/4;
    int y = borderMargins-companyFontHeight;
    int lineSpacing14 = lf*1.5;
    int lineSpacing15 = lf * 1.5;
    int lineSpacing13 = lf * 1.3;

    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);


    painter->drawText(borderMargins,borderMargins+lineSpacing14,l2);

    //    painter->drawText(borderMargins+625,25,l1);
    //    painter->drawText(borderMargins+625,40,l2);





    painter->drawText(x,y,l3);



    painter->drawText(x,y+lf,l4);
    painter->drawText(x,y+2*lf,l5);



    //Translate shabib

    painter->translate(borderMargins,borderMargins+2*lf);
    QFont ReportNameFont("Verdana",16);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));

    painter->fillRect(1,lf,10,painter->fontMetrics().height(),QColor("#ed1a24"));


    painter->setPen(penHText);

    painter->drawText(20,40,voucher->voucherType.toUpper());


    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);
    painter->save();

    painter->resetTransform();


    // Footer: page number or "end"

    QString PhoneSymbol = "\u260F";
    QString emailSymbol = "\u2709";
    QString truckSymbol = "\u26DF";

    int yy=0;

    //left side banner

    painter->translate(borderMargins,borderMargins+6*lf);

    QPen greyPen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));
    QPen redPen(QColor("#ff0000"));
    QPen yellowPen(QColor("#ffff00"));


    painter->setPen(greyPen);



    painter->setPen(normalPen);

    painter->fillRect(0,0,tLength/2,lineSpacing14,QColor("#ed1a24"));

    painter->drawRect(0,0,tLength/2,lf*10);

    lineSpacing14 = lf*1.4;
    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(localMargins,lineSpacing14,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    painter->setFont(LedgerNameFont);


    yy=40;

    QString ledName = voucher->ledgerObject.LedgerName.toUpper();
    QString address = voucher->ledgerObject.Address.toUpper();
    QString pobox= voucher->ledgerObject.PoBox.toUpper();
    QString city = voucher->ledgerObject.City.toUpper();
    QString country = voucher->ledgerObject.Country.toUpper();
    QString phoneNumber = voucher->ledgerObject.phoneNumber.toUpper();
    QString email = voucher->ledgerObject.emailAddress.toLower();
    QString custContact = voucher->ledgerObject.ContactPersonName.toUpper();
    QString cusPhone = voucher->ledgerObject.ContactPersonNumber;
    QString location = voucher->Location;
    QString Trn = voucher->ledgerObject.TRN;


    int lcnt =2;
    painter->drawText(localMargins,lineSpacing14*lcnt,ledName);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,address);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,city);
    lcnt++;
    //    if(Trn.length()>0)
    //        painter->drawText(localMargins,lineSpacing14*lcnt,"TRN : "+Trn);
    lcnt++;
    painter->drawText(localMargins,lineSpacing14*lcnt,PhoneSymbol+"  "+phoneNumber);
    lcnt++;
    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(localMargins,lineSpacing14*lcnt,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(localMargins+10,lineSpacing14*lcnt,email.toLower());



    //RIGHT SIDE BANNER

    painter->resetTransform();

    painter->translate(midx+localMargins,borderMargins);

    int halfLength = tLength/2;
    int _3partLength = halfLength/3;


    int endx2 = halfLength - localMargins;

    painter->setPen(normalPen);

    painter->drawRect(0, 3 ,endx2,lf*16);

    // horizontal Lines;

    yy  = 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);
    yy += 2*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);

    //Vertical Lines


    painter->drawLine(_3partLength  ,3,_3partLength  ,yy);
    painter->drawLine(_3partLength*2,3,_3partLength*2,yy);

    yy += 3*lineSpacing14;
    painter->drawLine(0,yy,endx2,yy);





    // Paint Contents

    // First Line
    yy = lineSpacing14;
    painter->setPen(normalPen);
    painter->setFont(HeadingFont);

    QRect curr = QRect(localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);

    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Date");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->VoucherDate.toString("dd-MM-yyyy"));

    curr = QRect(_3partLength+localMargins,localMargins,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Document No.");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,voucher->voucherNumber);

    curr = QRect(_3partLength*2+localMargins,localMargins,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Page");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));



    // Second Lines


    curr = QRect(localMargins,localMargins+ 2*lineSpacing13 ,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"GST No");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,LoginValues::company.trn.toUpper());

    curr = QRect(_3partLength+localMargins,localMargins + 2*lineSpacing13,_3partLength-localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, "Printed at");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,QDateTime::currentDateTime().toString("dd-MMM-yy hh:mm:ss"));

    curr = QRect(_3partLength*2+localMargins,localMargins + 2*lineSpacing13,_3partLength-2*localMargins,2*lineSpacing14-localMargins);
    painter->setFont(HeadingFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop, " ");
    painter->setFont(LedgerNameFont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignBottom,"");



    painter->setFont(LedgerNameFont);
    painter->setPen(normalPen);


    UserProfileDataModel salesman = userHelper->getEmployeeByID(voucher->SalesmanID);
    yy+=20;
    painter ->setFont(HeadingFont);

    curr = QRect(localMargins,7*lineSpacing14,halfLength,2*lineSpacing15);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,"Contact");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);
    curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=16;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Name.toUpper());
    curr = QRect(localMargins,8*lineSpacing14,halfLength,2*lineSpacing15);
    yy+=17;
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,PhoneSymbol+"  "+salesman.Phone);

    yy+=17;
    painter->setFont(drawfont);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(curr,Qt::AlignLeft|Qt::AlignTop,salesman.Email);
    painter->setPen(normalPen);
    yy+=18;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,custContact.toUpper());
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,PhoneSymbol+"   "+cusPhone);
    QFont fnt12; fnt12.setPointSize(15);
    painter->setFont(fnt12);
    //    painter->drawText(midx/2,yy,truckSymbol+"  ");
    painter->setFont(LedgerNameFont);
    painter->drawText(midx/2+15,yy,"Loc: " + location);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());

    painter->setPen(normalPen);

    painter->resetTransform();

    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0,
                              pageNumber * doc->pageSize().height(),
                              doc->pageSize().width() ,
                              doc->pageSize().height()
                              );




    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->drawRect(textRect);
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    painter->resetTransform();

    painter->drawRect(textRect.x(),textRect.y(),tLength,2*lf);
    int xcord = borderMargins ;
    qDebug()<<"width sizes <....913....> "<<widthList.size();



    if(pageNumber+1 == pageCount && false){

        // LAST PAGE
        painter->drawRect(textRect.x(),textRect.y()+textRect.height(),tLength,2*lf);



        QStringList gTotalLine;
        gTotalLine<< " ";
        gTotalLine<< "Grand total";
        gTotalLine<< " ";
        gTotalLine<< QString::number(voucher->quantityTotal,'f',2);
        gTotalLine<< " ";
        gTotalLine<< " ";
        gTotalLine<< " ";


        gTotalLine<< QString::number(voucher->grandTotal,'f',2);

        QFont totalLineFont("Calibri",14);

        int prevXcord=borderMargins;
        painter->setFont(totalLineFont);

        for(int i =0;i<widthList.size();i++){

            qDebug()<<" I is "<<i<<widthList[i];

            xcord += textRect.width()*widthList[i]/100;

            int boxwidth = tLength*widthList[i]/100-3;


            if(i < widthList.size()-1)
                painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height()+2*lf);

            if(i==1)
                painter->drawText(QRect(prevXcord+2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignLeft|Qt::AlignVCenter,gTotalLine[i]);
            else
                painter->drawText(QRect(prevXcord-2,textRect.bottom()+2,boxwidth,2*lf),Qt::AlignRight|Qt::AlignVCenter,gTotalLine[i]);

            prevXcord = xcord;

            qDebug()<<"Prev :"<<prevXcord;
        }

        yy=textRect.bottom()+2*lf;
        painter->setFont(HeadingFont);
        QRect amtInWordsrect = QRect(borderMargins,yy,tLength,4*lf);
        painter->drawRect(amtInWordsrect);
        amtInWordsrect.setX(borderMargins+3);
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,tr("Amount In Words : "));
        amtInWordsrect.setY(textRect.bottom()+3.5*lf);

        float amount = voucher->grandTotal;

        int integerpart = amount;
        int decimalP = amount*100;
        decimalP = decimalP % 100;

        QString amtinWords = "Rupees "+numberToText( integerpart);
        if(decimalP>0)
            amtinWords += " and "+ numberToText(decimalP)+" Paisa";
        amtinWords += " only.";
        painter->drawText(amtInWordsrect,Qt::AlignLeft|Qt::AlignTop,"   "+amtinWords);

    }

    else{
        for(int i =0;i<widthList.size()-1;i++){
            xcord += tLength*widthList[i]/100;
            painter->drawLine(xcord,textRect.top(),xcord,textRect.top()+textRect.height());
        }
    }

    painter->setFont(normalFont);
    painter->setPen(bluePen);


    qDebug()<<"98822";
    painter->setPen(normalPen);
    QRectF footerRect = textRect;
    footerRect.setTop(pageSize.height()-borderMargins);
    footerRect.setHeight(footerHeight);

    painter->setPen(normalPen);


    QString authString = "AUTHORIZED SIGNATORY";

    painter->drawText(borderMargins,footerRect.y()-2*lf,pageSize.width()-borderMargins*2,20, Qt::AlignRight, authString);


    //    painter->drawText(borderMargins,footerRect.y(),pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y(),textRect.width()+borderMargins,footerRect.y());

    //    painter->drawEllipse(borderMargins,footerRect.y(),5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);

}

void PrinterHelper::printIssueVoucher(QPrinter& printer, int pageNumber, int pageCount,
                                      QPainter* painter, QTextDocument* doc,
                                      const QRectF& textRect, qreal footerHeight,
                                      GeneralVoucherDataObject *v)
{
    QString bismi;
    QString headingIssue ;
    QString headingIssueArabic;

    QString vNoLableIssue;
    QString vNoLableReceipt;

    QString SectionIssue;
    QString SectionReceipt;

    QString CustomerIssue;
    QString CustomerReceipt;

    QString StoreManIssue;
    QString StoreManReceipt;

    QString footNoteEnglish;

    QString footNoteArabic;

    QString DateLable = "Date/التاريخ";

    QString officeSign = "Office Stamp & Signature/التوقيع والختم";

    QString AuthLable = "Authority for Issue/سلطة الصرف";

    QString AuthContent = "شعبة الإتصالات وتقنية العلميات ";

    QString AccLable = "المحاسبة/الليدجر";

    QString NameLable = ":الاسم والرتبة";
    //    QString RankLable = "Rank/الرتبة";
    QString SignatureLable = ":التوقيع";

    QString SectionLable="Section/القسم";

    bismi="بسم الله الرحمن الرحيم";
    headingIssue = "Issue and Receipt\nVoucher";
    headingIssueArabic="سند الصرف والإستلام";


    vNoLableIssue = "IV No./رقم سند الصرف";
    CustomerIssue = "صادر من";
    StoreManIssue = "خازن الصرف";



    vNoLableReceipt = "RV No./رقم سند الإستلام";
    CustomerReceipt = "المستلم";
    StoreManReceipt = "الخازن المستلم";

    footNoteArabic = "\nملاحظات: توقع الوحدة المستلمة على النسختين الولى والثانية "
                     "\n من المواد المرفقة وتقوم باءادة النسخة الثانية على"
                     " الوحدة التى تقوم بالصرف";
    footNoteEnglish= "Note: Copies No. 1 & 2 to accompany stores: receiving unit will\n sign and return copy No. 2 to the issuing unit";


    if(v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        SectionIssue = v->fromGodownID;
    }
    else{
        SectionIssue = v->toGodownID;
    }

    QFont fontHeading; fontHeading.setPointSize(16);
    QFont fontLabel; fontLabel.setPointSize(12);
    QFont fontLabelUnderLine; fontLabelUnderLine.setPointSize(12);fontLabelUnderLine.setUnderline(true);

    QFont fontSmall; fontSmall.setPointSize(9);

    const QSizeF pageSize = printer.paperRect().size();
    //qDebug() << "pageSize=" << pageSize;

    //const double bm = mmToPixels(printer, borderMargins);
    double bm = borderMargins;
    const QRectF borderRect(bm, bm, pageSize.width() - (2 * bm)-1, pageSize.height() - 3 * bm);

    painter->drawRect(borderRect);

    int tLength = pageSize.width()- (2*borderMargins)-1;
    int midx = tLength/2+borderMargins-20;
    int endx =  pageSize.width()-borderMargins;
    int quatx = (midx+endx)/2;
    int halfLength = tLength/4;


    int length3rd = tLength/3;

    int one3rdx = borderMargins+length3rd;
    int two3rdx  = one3rdx+length3rd;

    int borderLeft = borderMargins+10;

    int lineSpacing = 30;

    int headLineSpacing = 50;

    int y;
    y = borderMargins+lineSpacing *3;

    painter->drawRect(borderMargins,100,length3rd,headLineSpacing);

    painter->drawRect(two3rdx,100,length3rd,headLineSpacing);

    painter->drawRect(borderMargins,100+headLineSpacing,length3rd,headLineSpacing);
    painter->drawRect(two3rdx,100+headLineSpacing,length3rd,headLineSpacing);

    painter->drawRect(one3rdx,100+headLineSpacing,length3rd,200);

    //    painter->drawRect(one3rdx,100+headLineSpacing,length3rd,235);

    painter->drawRect(one3rdx,335-headLineSpacing,length3rd,headLineSpacing+3);

    //Voucher Heading

    QString formNameEnglish = tr("Armed Forces Form 44");
    QString formNameArabic = tr("القوات المسلحة نموذج  44");

    painter->setFont(fontHeading);
    // painter->drawRect(QRect(borderMargins,borderMargins,endx,lineSpacing));

    painter->setFont(fontSmall);

    painter->drawText(QRect(borderMargins,borderMargins,endx,lineSpacing),Qt::AlignCenter,bismi);

    painter->setFont(fontLabelUnderLine);

    painter->drawText(QRect(borderMargins,borderMargins,tLength,lineSpacing*3),
                      Qt::AlignCenter,headingIssueArabic);
    painter->setFont(fontHeading);

    painter->drawText(QRect(borderMargins,borderMargins+lineSpacing+10,tLength,lineSpacing*3),
                      Qt::AlignCenter,headingIssue);

    painter->setFont(fontSmall);
    painter->drawText(QRect(borderLeft,0,endx,lineSpacing+borderMargins),Qt::AlignLeft,formNameEnglish);
    painter->drawText(QRect(tLength-100,0,endx,lineSpacing+borderMargins),Qt::AlignLeft,formNameArabic);

    QImage img1;
    img1.load(":/images/img1.png");
    img1.scaled(50,50);
    painter->drawImage(endx-50,borderMargins,img1.scaled(50,50));


    //left

    int topy = y;
    painter->setFont(fontSmall);

    painter->drawText(QRect(borderLeft,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,vNoLableReceipt);
    if(!v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        painter->drawText(QRect(midx/2,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
                          v->voucherNumber);
    }
    y += lineSpacing*2;

    painter->drawText(QRect(borderLeft,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,DateLable);
    if(!v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        painter->drawText(QRect(midx/2,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
                          v->VoucherDate.toString("dd-MM-yyyy"));
    }
    y += lineSpacing;

    painter->drawText(QRect(borderLeft-150,y,midx,lineSpacing+borderMargins),Qt::AlignRight,CustomerReceipt);
    if(!v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        //        painter->drawText(QRect(midx/2,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
        //                          v->ledgerObject.LedgerName);
    }
    y += lineSpacing;

    //    painter->drawText(QRect(borderLeft,y,midx,
    //                            lineSpacing+borderMargins),Qt::AlignLeft,officeSign);
    painter->drawText(QRect(borderLeft-150,y,midx,
                            lineSpacing+borderMargins),Qt::AlignRight,NameLable);
    //    painter->drawText(QRect(borderLeft,y+lineSpacing,midx,
    //                            lineSpacing+borderMargins),Qt::AlignLeft,RankLable);
    painter->drawText(QRect(borderLeft-150,y+lineSpacing,midx,
                            lineSpacing+borderMargins),Qt::AlignRight,SignatureLable);
    //Right

    y=topy;

    painter->drawText(QRect(two3rdx+10,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,vNoLableIssue);
    if(v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        painter->drawText(QRect(two3rdx+10+halfLength,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
                          v->voucherNumber);
    }
    y += lineSpacing*2;

    painter->drawText(QRect(two3rdx+10,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,DateLable);
    if(v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        painter->drawText(QRect(two3rdx+100,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
                          v->VoucherDate.toString("dd-MM-yyyy"));
    }
    y += lineSpacing;

    painter->drawText(QRect(two3rdx-140,y,midx,lineSpacing+borderMargins),Qt::AlignRight,CustomerIssue);

    if(v->voucherType.contains("ISSUE",Qt::CaseInsensitive)){
        //        painter->drawText(QRect(two3rdx+10+halfLength,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
        //                          v->ledgerObject.LedgerName);
    }
    y += lineSpacing;

    //    painter->drawText(QRect(two3rdx+10,y,midx,
    //                            lineSpacing+borderMargins),Qt::AlignLeft,officeSign);
    painter->drawText(QRect(two3rdx-140,y,midx,
                            lineSpacing+borderMargins),Qt::AlignRight,NameLable);
    //    painter->drawText(QRect(two3rdx+10,y+lineSpacing,midx,
    //                            lineSpacing+borderMargins),Qt::AlignLeft,RankLable);
    painter->drawText(QRect(two3rdx-140,y+lineSpacing,midx,
                            lineSpacing+borderMargins),Qt::AlignRight,SignatureLable);
    //Middle Side

    y=topy;

    y += lineSpacing*2;

    painter->drawText(QRect(one3rdx+10,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,AuthLable);
    painter->drawText(QRect(one3rdx+10,y+lineSpacing,midx,lineSpacing+borderMargins),Qt::AlignLeft,AuthContent);

    //    painter->drawText(QRect(one3rdx+10,y,halfLength,lineSpacing+borderMargins),Qt::AlignLeft,tr("Sheet No."));
    //    painter->drawText(QRect(quatx,y,halfLength,lineSpacing+borderMargins),Qt::AlignLeft,
    //                      v->Sheet_No);
    y += lineSpacing*4;

    painter->drawText(QRect(one3rdx+10,y,halfLength,lineSpacing+borderMargins),Qt::AlignLeft,SectionLable);
    painter->drawText(QRect(one3rdx+20,y+15,halfLength,lineSpacing+borderMargins),Qt::AlignLeft,
                      SectionIssue);

    y += lineSpacing;

    y += lineSpacing*2;
    // painter->drawText(QRect(midx,y,halfLength,lineSpacing+borderMargins),Qt::AlignLeft,officeSign);


    //     painter->drawText(QRect(midx/2,y,midx,lineSpacing+borderMargins),Qt::AlignLeft,
    //                       Section);



    painter->save();


    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(),
                              doc->pageSize().width(), doc->pageSize().height());
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    // Footer: page number or "end"

    //Header Colmun borders Here
    //    painter->drawLine(borderMargins,336,endx,336);
    painter->drawLine(borderMargins,327+headLineSpacing,endx,327+headLineSpacing);

    QList<int> widthlist;widthlist << 20 << 10 << 35 <<25 << 10;


    for(int i=0,point=bm;i<widthlist.size()-1;i++){
        point += tLength*widthlist[i]/100 -i*3;
        painter->drawLine(point,337,point,textRect.bottomLeft().y()+13);

    }

    painter->drawRect(textRect.topLeft().x(),
                      textRect.topLeft().y()+3,
                      tLength,textRect.topRight().y()+155);

    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    y=footerRect.bottomLeft().y();
    painter->drawLine(borderMargins,y,endx,y);

    painter->drawRect(borderMargins,y,length3rd,headLineSpacing*2);

    painter->drawRect(one3rdx,y,length3rd,headLineSpacing*2);
    painter->drawRect(two3rdx,y,length3rd,headLineSpacing*2);

    painter->drawText(borderLeft-20,y-lineSpacing,length3rd,lineSpacing*2,Qt::AlignBottom|Qt::AlignRight,NameLable);
    //    painter->drawText(borderLeft+110,y-(lineSpacing*2.2),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,v->ledgerObject.LedgerName);

    //    painter->drawText(borderLeft,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,RankLable);
    //    painter->drawText(borderLeft+110,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,v->ledgerObject.ContactPersonName);

    painter->drawText(borderLeft-20,y-lineSpacing,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,SignatureLable);

    painter->drawText(borderLeft-20,y,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,AccLable);

    painter->drawText(one3rdx-10,y-(lineSpacing*2),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,NameLable);
    //    painter->drawText(one3rdx+110,y-(lineSpacing*2.2),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,v->ledgerObject.LedgerName);

    //    painter->drawText(one3rdx+10,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,RankLable);
    //    painter->drawText(one3rdx+110,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,v->ledgerObject.ContactPersonName);

    painter->drawText(one3rdx-10,y-lineSpacing,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,SignatureLable);

    painter->drawText(one3rdx-10,y,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,StoreManReceipt);

    painter->drawText(two3rdx-10,y-(lineSpacing*2),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,NameLable);
    //    UserProfileDataModel u = userHelper->getEmployeeByID(LoginValues::userID);
    //    painter->drawText(two3rdx+110,y-(lineSpacing*2.2),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,u.Name);

    //    painter->drawText(two3rdx+10,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,RankLable);
    //    painter->drawText(two3rdx+110,y-(lineSpacing*1.6),length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,u.Designation);

    painter->drawText(two3rdx-10,y-lineSpacing,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,SignatureLable);

    painter->drawText(two3rdx-10,y,length3rd,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,StoreManIssue);

    painter->drawText(borderLeft,y+headLineSpacing,halfLength*2,lineSpacing*3,Qt::AlignBottom|Qt::AlignLeft,footNoteEnglish);

    painter->drawText(midx,y+headLineSpacing,halfLength*2,lineSpacing*3,Qt::AlignBottom|Qt::AlignRight,footNoteArabic);


    painter->drawLine(borderMargins,y,endx,y);

    //        if (pageNumber == pageCount - 1)
    //            painter->drawText(footerRect, Qt::AlignCenter, QObject::tr("www.alrumooz.com"));
    //        else
    //            painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));

}

void PrinterHelper::formatIssueRequest(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget,
                                       GeneralVoucherDataObject *obj)
{
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qDebug()<<"height = "<<pageSize.height();
    qDebug()<<"Width = "<<pageSize.width();

    qDebug()<<"height(mm) = "<<PixelsToMM(printer, pageSize.height());
    qDebug()<<"Width(mm) = "<<PixelsToMM(printer, pageSize.width());

    const QRectF textRect(borderMargins, 335, pageSize.width() - 2* borderMargins ,
                          pageSize.height() -330 - (10 * borderMargins) - footerHeight);

    qDebug() << "textRect=" << textRect;
    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;
        if (!firstPage)
            printer.newPage();

        printIssueVoucher( printer, pageIndex, pageCount, &painter, doc,
                           textRect, footerHeight,obj );
        firstPage = false;
    }
}

void PrinterHelper::addTable(QTextCursor& cursor)
{
    const int columns = 4;
    const int rows = 1;
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount( 1 );
    QTextTable* textTable = cursor.insertTable( rows + 1,
                                                columns,
                                                tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#DADADA" ) );

    QStringList headers;
    headers << "Product" << "Reference" << "Price" << "Price with shipping";
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat( tableHeaderFormat );
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( headers[column] );
    }
    int row = 0;
    for( int column = 0; column < columns; column++ ) {
        QTextTableCell cell = textTable->cellAt( row + 1, column );
        Q_ASSERT( cell.isValid() );
        QTextCursor cellCursor = cell.firstCursorPosition();
        const QString cellText = QString( "A 220.00" );
        cellCursor.insertText( cellText );
    }
    cursor.movePosition( QTextCursor::End );
}

void PrinterHelper::printBarcode3Lines(int CopyCount, inventoryItemDataModel item, QDate Mfd, QDate Exp, bool printMFD)
{
    qDebug()<<"testing barcode";
    QPageSize pageSize(QSizeF(78.0,25.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);
    QPrinter printer(QPrinter::PrinterResolution);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return  ;


    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(pageSize);
    printer.setFullPage(true);
    printer.setCopyCount(CopyCount);
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPainter painter(&printer);
    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,40,QFont::Normal);
    QTextDocument doc;

    //    QFont barCodeFont("Code 128", 40, QFont::Normal);
    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.5);
    QString str = encodeBarcode(item.ItemCode);
    //        painter->drawText(300,600,str);
    int w = printer.pageRect().width();
    int h = printer.pageRect().height();


    double width  = w/2; mmToPixels(printer,w);
    double height = h; mmToPixels(printer,h);

    double borderMargin = mmToPixels(printer,1);
    double middleMargin = mmToPixels(printer,2);

    QFont normal("Times",8);
    QFont smallFont("Times",6);
    //    smallFont.setUnderline(true);


    double lineHeight10 = mmToPixels(printer,10);
    double lineHeight7 = mmToPixels(printer,7);
    double yLine1=borderMargin + lineHeight10 ;
    double yLine2=borderMargin + 2*lineHeight10 +5;

    double yLine3=yLine2+lineHeight7/2;

    double yLine4=yLine2+lineHeight7;

    double offset = width+middleMargin-5;

    QDate expiry = QDate::currentDate().addDays(item.shelfLife);

    QFont companyNameFont("Times",12); companyNameFont.setBold(true);
    QFont priceFont("Times",9); priceFont.setBold(true);
    QFont expFont("Times",7); priceFont.setBold(false);

    QRect ptr ;;
    QString rate = QString::number(item.price + item.price * item.taxRate / 100,'f',2);
    int ny=0;
    for(int i=0;i<2;i++){
        double addendeum = offset *i;
        ny = borderMargin+5;
        painter.setFont(expFont);
        painter.drawText(borderMargin+addendeum+5,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,item.ItemName ,&ptr);
        ny = ptr.bottom();
        painter.setFont(barCodeFont);
        painter.drawText(borderMargin+addendeum-30,ny,width-2*borderMargin+10,lineHeight10,Qt::AlignLeft,str,&ptr);
        painter.setFont(smallFont);
        ny = ptr.bottom()-lineHeight7/2;
        if(LoginValues::getCompany().fssaiLicenseNo.length()>1){
            painter.drawText(borderMargin+addendeum+5,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Lic:" +LoginValues::getCompany().fssaiLicenseNo,&ptr );
            ny= ptr.bottom();
        }
        painter.setFont(priceFont);
        QString ExpStr = "Mfd:"+Mfd.toString("dd-MM-yy")+"|Exp:"+Exp.toString("dd-MM-yy");
        //        painter.drawRect(width/2+addendeum,yLine3+5,width/2-borderMargin,lineHeight7);
        painter.drawText(borderMargin+addendeum+8,ny,width/2-borderMargin,lineHeight10,Qt::AlignLeft,"Rs. "+ rate ,&ptr);
        ny = ptr.bottom();
        painter.setFont(expFont);
        if(printMFD)
            painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,ExpStr );
    }
    doc.drawContents(&painter);
    doc.print(&printer);
    qDebug()<<"Printer";

}
//99995070267

void PrinterHelper::printBarcode(int CopyCount, inventoryItemDataModel item, QDate Mfd, QDate Exp)
{
    qDebug()<<"testing barcode";
    QPageSize pageSize(QSizeF(88.0,33.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    //    QPageSize pageSize(QSizeF(78.0,50.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    QPrinter printer(QPrinter::PrinterResolution);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return  ;

    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(pageSize);
    printer.setFullPage(true);
    printer.setCopyCount(CopyCount);
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPainter painter(&printer);
    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,40,QFont::Normal);
    QTextDocument doc;

    //    QFont barCodeFont("Code 128", 40, QFont::Normal);
    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.5);

    QString str = encodeBarcode(item.ItemCode);
    //        painter->drawText(300,600,str);


    int w = printer.pageRect().width();
    int h = printer.pageRect().height();

    qDebug()<<"width :"<<w<<"height"<<h;
    double width  = w/2; mmToPixels(printer,w);
    double height = h; mmToPixels(printer,h);

    double borderMargin = mmToPixels(printer,3);
    double middleMargin = 11;

    QFont normal("Times",8);
    QFont smallFont("Times",6);
    //    smallFont.setUnderline(true);


    double lineHeight10 = mmToPixels(printer,10);

    double lineHeight7 = mmToPixels(printer,7);

    double yLine1=borderMargin + lineHeight10 ;

    double yLine2=borderMargin + 2*lineHeight10 +5;

    double yLine3=yLine2+lineHeight7/2;

    double yLine4=yLine2+lineHeight7;

    double offset = width+middleMargin-5;

    QDate expiry = QDate::currentDate().addDays(item.shelfLife);

    QFont companyNameFont("Times",12); companyNameFont.setBold(true);
    QFont priceFont("Times",9); priceFont.setBold(true);

    QRect ptr ;;

    QString rate = QString::number(item.price + item.price * item.taxRate / 100,'f',2);

    int ny=0;
    for(int i=0;i<2;i++){

        double addendeum = offset *i;
        painter.setFont(companyNameFont);
        painter.drawText(borderMargin+addendeum+8,borderMargin+3,width-2*borderMargin,lineHeight7,Qt::AlignLeft,LoginValues::getCompany().CompanyName,&ptr );

        painter.setFont(normal);
        ny = ptr.bottom();
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,item.ItemName ,&ptr);
        ny = ptr.bottom();
        painter.setFont(barCodeFont);
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight10,Qt::AlignLeft,str,&ptr);
        painter.setFont(smallFont);
        ny = ptr.bottom()-lineHeight7/2;
        if(LoginValues::getCompany().fssaiLicenseNo.length()>1){
            painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Lic:" +LoginValues::getCompany().fssaiLicenseNo,&ptr );
            ny= ptr.bottom();
        }
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Pkd:" +Mfd.toString("dd-MM-yyyy"),&ptr );
        ny= ptr.bottom();
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Exp:" +Exp.toString("dd-MM-yyyy"),&ptr );
        ny= ptr.bottom();
        painter.setFont(priceFont);
        //        painter.drawRect(width/2+addendeum,yLine3+5,width/2-borderMargin,lineHeight7);
        painter.drawText(borderMargin+addendeum+8,ny,width/2-borderMargin,lineHeight10,Qt::AlignLeft,"Rs. "+ rate );

    }
    doc.drawContents(&painter);
    doc.print(&printer);
    qDebug()<<"Printer";
}

void PrinterHelper::printBarcodeAgro(int CopyCount, inventoryItemDataModel item, QDate Mfd, QDate Exp)
{
    qDebug()<<"testing barcode";
    QPageSize pageSize(QSizeF(98.0,33.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    //    QPageSize pageSize(QSizeF(78.0,50.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    QPrinter printer(QPrinter::PrinterResolution);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return  ;


    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(pageSize);
    printer.setFullPage(true);
    printer.setCopyCount(CopyCount);
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPainter painter(&printer);
    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,40,QFont::Normal);
    QTextDocument doc;

    //    QFont barCodeFont("Code 128", 40, QFont::Normal);
    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.5);

    QString str = encodeBarcode(item.ItemCode);
    //        painter->drawText(300,600,str);


    int w = printer.pageRect().width();
    int h = printer.pageRect().height();

    qDebug()<<"width :"<<w<<"height"<<h;
    double width  = w/2; mmToPixels(printer,w);
    double height = h; mmToPixels(printer,h);

    double borderMargin = mmToPixels(printer,3);
    double middleMargin = 11;

    QFont normal("Times",8);
    QFont smallFont("Times",6);
    //    smallFont.setUnderline(true);


    double lineHeight10 = mmToPixels(printer,10);

    double lineHeight7 = mmToPixels(printer,7);

    double yLine1=borderMargin + lineHeight10 ;

    double yLine2=borderMargin + 2*lineHeight10 +5;

    double yLine3=yLine2+lineHeight7/2;

    double yLine4=yLine2+lineHeight7;

    double offset = width+middleMargin-5;

    QDate expiry = QDate::currentDate().addDays(item.shelfLife);

    QFont companyNameFont("Times",12); companyNameFont.setBold(true);
    QFont priceFont("Times",9); priceFont.setBold(true);

    QRect ptr ;;

    QString rate = QString::number(item.price + item.price * item.taxRate / 100,'f',2);
    int ny=0;
    for(int i=0;i<2;i++){

        double addendeum = offset *i;
        painter.setFont(companyNameFont);
        painter.drawText(borderMargin+addendeum+8,borderMargin+3,width-2*borderMargin,lineHeight7,Qt::AlignLeft,LoginValues::getCompany().CompanyName,&ptr );

        painter.setFont(normal);
        ny = ptr.bottom();
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,item.ItemName ,&ptr);
        ny = ptr.bottom();
        painter.setFont(barCodeFont);
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight10,Qt::AlignLeft,str,&ptr);
        painter.setFont(smallFont);
        ny = ptr.bottom()-lineHeight7/2;
        if(LoginValues::getCompany().fssaiLicenseNo.length()>1){
            painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Lic:" +LoginValues::getCompany().fssaiLicenseNo,&ptr );
            ny= ptr.bottom();
        }
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"",&ptr );
        ny= ptr.bottom();
        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"",&ptr );
        ny= ptr.bottom();
        painter.setFont(priceFont);
        //        painter.drawRect(width/2+addendeum,yLine3+5,width/2-borderMargin,lineHeight7);
        painter.drawText(borderMargin+addendeum+8,ny,width/2-borderMargin,lineHeight10,Qt::AlignLeft,"Rs. "+ rate );

    }
    doc.drawContents(&painter);
    doc.print(&printer);
    qDebug()<<"Printer";
}

void PrinterHelper::printBarcodeOld(int CopyCount, inventoryItemDataModel item, QDate Mfd, QDate Exp)
{

    qDebug()<<"testing barcode";
    QPageSize pageSize(QSizeF(88.0,33.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    //    QPageSize pageSize(QSizeF(78.0,50.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);

    QPrinter printer(QPrinter::PrinterResolution);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return  ;


    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(pageSize);
    printer.setFullPage(true);
    printer.setCopyCount(CopyCount);
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPainter painter(&printer);
    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,40,QFont::Normal);
    QTextDocument doc;

    //    QFont barCodeFont("Code 128", 40, QFont::Normal);
    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.5);

    QString str = encodeBarcode(item.ItemCode);
    //        painter->drawText(300,600,str);


    int w = printer.pageRect().width();
    int h = printer.pageRect().height();

    qDebug()<<"width :"<<w<<"height"<<h;
    double width  = w/2; mmToPixels(printer,w);
    double height = h; mmToPixels(printer,h);

    double borderMargin = mmToPixels(printer,3);
    double middleMargin = 11;

    QFont normal("Times",8);
    QFont smallFont("Times",6);
    //    smallFont.setUnderline(true);


    double lineHeight10 = mmToPixels(printer,10);

    double lineHeight7 = mmToPixels(printer,7);

    double yLine1=borderMargin + lineHeight10 ;

    double yLine2=borderMargin + 2*lineHeight10 +5;

    double yLine3=yLine2+lineHeight7/2;

    double yLine4=yLine2+lineHeight7;

    double offset = width+middleMargin-5;

    QDate expiry = QDate::currentDate().addDays(item.shelfLife);

    QFont companyNameFont("Times",12); companyNameFont.setBold(true);
    QFont priceFont("Times",9); priceFont.setBold(true);

    QRect ptr ;;

    QString rate = QString::number(item.price + item.price * item.taxRate / 100,'f',2);
    int ny=0;
    for(int i=0;i<2;i++){


        double addendeum = offset *i;

        //        painter.setFont(companyNameFont);
        //        painter.drawText(borderMargin+addendeum+8,borderMargin+3,width-2*borderMargin,lineHeight7,Qt::AlignLeft,LoginValues::getCompany().CompanyName,&ptr );

        //        painter.setFont(normal);
        //        ny = ptr.bottom();

        ny = borderMargin+5;
        painter.drawText(borderMargin+addendeum+5,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,item.ItemName ,&ptr);

        ny = ptr.bottom();
        painter.setFont(barCodeFont);
        painter.drawText(borderMargin+addendeum-10,ny,width-2*borderMargin+10,lineHeight10,Qt::AlignLeft,str,&ptr);


        painter.setFont(smallFont);
        ny = ptr.bottom()-lineHeight7/2;
        if(LoginValues::getCompany().fssaiLicenseNo.length()>1){
            painter.drawText(borderMargin+addendeum+5,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Lic:" +LoginValues::getCompany().fssaiLicenseNo,&ptr );
            ny= ptr.bottom();
        }

        //        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Pkd:" +Mfd.toString("dd-MM-yyyy"),&ptr );

        //        ny= ptr.bottom();
        //        painter.drawText(borderMargin+addendeum+8,ny,width-2*borderMargin,lineHeight7,Qt::AlignLeft,"Exp:" +Exp.toString("dd-MM-yyyy"),&ptr );


        //        ny= ptr.bottom();

        painter.setFont(priceFont);
        //        painter.drawRect(width/2+addendeum,yLine3+5,width/2-borderMargin,lineHeight7);
        painter.drawText(borderMargin+addendeum+8,ny,width/2-borderMargin,lineHeight10,Qt::AlignLeft,"Rs. "+ rate );

    }
    doc.drawContents(&painter);
    doc.print(&printer);

}

void PrinterHelper::paintPageExample(QPrinter& printer, int pageNumber, int pageCount,
                                     QPainter* painter, QTextDocument* doc,
                                     const QRectF& textRect, qreal footerHeight)
{
    //qDebug() << "Printing page" << pageNumber;
    const QSizeF pageSize = printer.paperRect().size();
    //qDebug() << "pageSize=" << pageSize;

    const double bm = mmToPixels(printer, borderMarginsExample);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    painter->drawRect(borderRect);

    painter->save();
    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(), doc->pageSize().width(), doc->pageSize().height());
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textRect.left(), textRect.top());
    doc->drawContents(painter);
    painter->restore();

    // Footer: page number or "end"
    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    if (pageNumber == pageCount - 1)
        painter->drawText(footerRect, Qt::AlignCenter, QObject::tr("Fin du Bordereau de livraison"));
    else
        painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
}



void PrinterHelper::printDocumentExample(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget)
{
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMarginsExample);
    const qreal footerHeight = painter.fontMetrics().height();
    const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);
    //qDebug() << "textRect=" << textRect;
    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();
    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        paintPageExample(printer, pageIndex, pageCount, &painter, doc, textRect, footerHeight );
        firstPage = false;
    }
}

void PrinterHelper::formatDispatchPages(QPrinter& printer, QTextDocument* doc,
                                        GeneralVoucherDataObject *obj,QList<qreal> widthList,
                                        QWidget* parentWidget)
{
    qDebug()<<Q_FUNC_INFO;
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qreal height = pageSize.height();
    borderMargins = pageSize.height() * 0.05;

    qreal section1 = height * 0.0;
    qreal section2 = height * 0.80;
    qreal section3 = height * 0.20;

    int tmargin = borderMargins+mmToPixels(printer,3);

    qreal tablestartx = borderMargins+ section1;
    qreal tablewidth = pageSize.width()-2*borderMargins;
    qreal tableheight = section2;

    QRectF textRect(borderMargins,
                    tablestartx,
                    tablewidth ,
                    tableheight);

    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {


        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        printDispatchFooter( printer, pageIndex,
                             pageCount, &painter, doc,
                             textRect, footerHeight,obj,widthList );
        firstPage = false;

    }
}

void PrinterHelper::formatInventoryVoucherPages(QPrinter& printer, QTextDocument* doc,
                                                GeneralVoucherDataObject *obj,QList<qreal> widthList, QWidget* parentWidget)
{
    qDebug()<<Q_FUNC_INFO;
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qreal height = pageSize.height();
    borderMargins = pageSize.height() * 0.05;

    qreal section1 = height * 0.20;
    qreal section2 = height * 0.48;
    qreal section3 = height * 0.20;

    if(printer.pageSize() == QPrinter::A5){
        section1 = height * 0.30;
        section2 = height * 0.45;
        section3 = height * 0.20;
    }

    int tmargin = borderMargins+mmToPixels(printer,3);

    qreal tablestartx = borderMargins+ section1;
    qreal tablewidth = pageSize.width()-2*borderMargins;
    qreal tableheight = section2;

    const QRectF textRect(borderMargins,
                          tablestartx,
                          tablewidth ,
                          tableheight);

    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {


        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        printVoucherHeaderFooter( printer, pageIndex,
                                  pageCount, &painter, doc,
                                  textRect, footerHeight,obj,widthList );
        firstPage = false;

    }
}


void PrinterHelper::formatDelNotePages(QPrinter& printer, QTextDocument* doc,
                                       GeneralVoucherDataObject *obj,QList<qreal> widthList, QWidget* parentWidget)
{
    qDebug()<<Q_FUNC_INFO;
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    //    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qreal height = pageSize.height();
    borderMargins = pageSize.height() * 0.05;

    qreal section1 = height * 0.25;
    qreal section2 = height * 0.55;
    qreal section3 = height * 0.20;

    int tmargin = borderMargins + mmToPixels(printer,3);

    qreal tablestartx = borderMargins+ section1;
    qreal tablewidth = pageSize.width()-2*borderMargins;
    qreal tableheight = section2;

    const QRectF textRect(borderMargins,
                          tablestartx,
                          tablewidth ,
                          tableheight);

    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {


        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        printDelHeaderFooter( printer, pageIndex,
                              pageCount, &painter, doc,
                              textRect, footerHeight,obj,widthList );
        firstPage = false;

    }
}

void PrinterHelper::formatIssueVoucherPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget)
{
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qreal height = pageSize.height();
    borderMargins = pageSize.height() * 0.05;

    qreal section1 = height * 0.20;
    qreal section2 = height * 0.50;
    qreal section3 = height * 0.20;

    int tmargin = borderMargins+mmToPixels(printer,3);

    qreal tablestartx = borderMargins+ section1;
    qreal tablewidth = pageSize.width()-2*borderMargins;
    qreal tableheight = section2;

    const QRectF textRect(borderMargins,
                          tablestartx,
                          tablewidth ,
                          tableheight);

    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {


        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        printIssueReqHeaderFooter( printer, pageIndex,
                                   pageCount, &painter, doc,
                                   textRect, footerHeight,obj,widthList );
        firstPage = false;

    }
}

void PrinterHelper::formatWorkOrderPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget)
{
    QPainter painter( &printer );
    QSizeF pageSize = printer.pageRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    const qreal footerHeight = painter.fontMetrics().height();
    //const QRectF textRect(tm, tm, pageSize.width() - 2 * tm, pageSize.height() - 2 * tm - footerHeight);

    qreal height = pageSize.height();
    borderMargins = pageSize.height() * 0.05;

    qreal section1 = height * 0.20;
    qreal section2 = height * 0.50;
    qreal section3 = height * 0.20;

    int tmargin = borderMargins+mmToPixels(printer,3);

    qreal tablestartx = borderMargins+ section1;
    qreal tablewidth = pageSize.width()-2*borderMargins;
    qreal tableheight = section2;

    const QRectF textRect(borderMargins,
                          tablestartx,
                          tablewidth ,
                          tableheight);

    doc->setPageSize(textRect.size());

    const int pageCount = doc->pageCount();

    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {


        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        printWorkOrderHeaderFooter( printer, pageIndex,
                                    pageCount, &painter, doc,
                                    textRect, footerHeight,obj,widthList );
        firstPage = false;

    }
}



void PrinterHelper::getIssueTable(QTextCursor *cursor, GeneralVoucherDataObject *obj)
{

    QList<int> widthlist;
    widthlist << 20 << 10 << 35 <<25 << 10;

    QList<int> Allignment;
    Allignment << 1 << 1 << 0 <<0 << 1;

    QStringList headingText;

    headingText << "SL.NO"
                << "Particulars"
                << "Uom"
                << "Qty"
                << "Price";

    const int columns = 5;

    const int rows = obj->InventoryItems.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);

    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);




    QVector<QTextLength> constraints;

    for(int i=0;i<widthlist.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthlist[i]);
    }

    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormat.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable =  cursor->insertTable( maxrows,
                                                  columns,
                                                  tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;

    cellFormat.setBackground( QColor( "#ffffff" ) );

    cellFormat.setFontPointSize(12);




    QTextFrameFormat frameFormat;
    frameFormat.setHeight(15);


    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;


    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    //    int itemsSize = obj->InventoryItems.size();

    //    CompoundItemDataObject temp;
    //    int itemSize = obj->InventoryItems.size();
    //    for(int i=0;i<itemSize;i++){
    //        for(int j=0;j<itemSize;j++){
    //            if(obj->InventoryItems[i].BaseItem.ItemID.compare(
    //                        obj->InventoryItems[j].BaseItem.ItemID)){
    //                temp = obj->InventoryItems[i];
    //                obj->InventoryItems[i] = obj->InventoryItems[j];
    //                obj->InventoryItems[j] = temp;
    //            }
    //        }
    //    }
    //    for(int i=0;i<itemSize;i++){
    //        qDebug()<<obj->InventoryItems[i].BaseItem.ItemID
    //               <<obj->InventoryItems[i].BaseItem.ItemName;

    //    }
    //    qDebug()<<"sort complete";


    //    QString prevID="";
    //    QString currID;
    //    float currqty =0,cqty=0;
    //    int posCnt=0;

    //    for(int i =0;i<itemsSize;i++){
    //        currID = obj->InventoryItems[i].BaseItem.ItemID;

    //        if(prevID!=currID){

    //            if(i==0){
    //                currqty = obj->InventoryItems[i].BaseItem.quantity;
    //            }

    //            obj->InventoryItems[i-posCnt].BaseItem.quantity=currqty;
    //            currqty=obj->InventoryItems[i].BaseItem.quantity;

    //            posCnt = 1;
    //        }
    //        else{
    //            qDebug()<<"repeating";
    //            currqty += obj->InventoryItems[i].BaseItem.quantity;
    //            obj->InventoryItems[i].BaseItem.Remarks="";
    //            obj->InventoryItems[i].BaseItem.quantity=0;
    //            obj->InventoryItems[i].BaseItem.ItemName="";

    //            qDebug()<<i<<currqty;
    //            posCnt++;
    //        }
    //        prevID = currID;
    //    }

    //    obj->InventoryItems[itemsSize-posCnt].BaseItem.quantity=currqty;
    //    qDebug()<<obj->InventoryItems[itemsSize-posCnt].BaseItem.quantity;

    //    qDebug()<<"psCnt = "<<posCnt<<"crr QTY"<<currqty;

    int slNocnt=1;

    for(int row =0;row<rows;row++){
        int column =0;

        data = obj->InventoryItems[row].BaseItem.narration;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(centerAlignment);
        const QString cellText5 = QString(data);
        cellCursor.insertText( cellText5 );

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity);
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        data = obj->InventoryItems[row].BaseItem.ItemCode;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(centerAlignment);
        const QString cellText2 = QString( data );
        cellCursor.insertText( cellText2 );

        if(obj->InventoryItems[row].BaseItem.ItemName.length()>0){
            data = QString::number(slNocnt++);
            cell = textTable->cellAt( row + 1, column++ );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setBlockFormat(centerAlignment);
            cellCursor.setCharFormat(cellFormat);
            const QString cellText1 = QString( data );
            cellCursor.insertText( cellText1 );
        }
        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }
    cursor->movePosition( QTextCursor::End );
}

void PrinterHelper::getInventoryVoucherTableSales(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList)
{
    qDebug()<<Q_FUNC_INFO;

    QStringList headingText;
    headingText << "#"
                << "Particulars";
    if(printer->pageSize() != QPrinter::A5)
        headingText<< "HSN";
    headingText  << "Qty"
                 << "Rate"
                 << "Disc%"
                 << "Tax%"
                 << "Total";

    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();

    int ledgerCount = obj->ledgersList.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 1 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormatNoBorder.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( maxrows+2,
                                                 columns,
                                                 tableFormatNoBorder );

    cursor->movePosition( QTextCursor::End );
    tableFormatNoBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    if(printer->pageSize() == QPrinter::A5)
    {
        headerCellFormat.setFontPointSize(9);
        cellFormat.setFontPointSize(9);
    }


    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int row =0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        if(printer->pageSize() == QPrinter::A5)
            data = obj->InventoryItems[row].BaseItem.ItemName + " - "  + obj->InventoryItems[row].BaseItem.hsnCode;
        else
            data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        if(printer->pageSize() != QPrinter::A5){
            data = obj->InventoryItems[row].BaseItem.hsnCode;
            cell = textTable->cellAt( row + 1, column++ );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(cellFormat);
            cellCursor.setBlockFormat(centerAlignment);
            const QString cellText5 = QString(data);
            cellCursor.insertText( cellText5 );
        }

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        data = QString::number(obj->InventoryItems[row].BaseItem.price,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText2 = QString( data );
        cellCursor.insertText( cellText2 );

        data = QString::number(obj->InventoryItems[row].BaseItem.discountPercentage,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText6 = QString( data );
        cellCursor.insertText( cellText6 );

        data = QString::number(obj->InventoryItems[row].BaseItem.taxRate,'f',2) +"%";
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.insertText(  QString( data ) );


        data = QString::number(obj->InventoryItems[row].BaseItem.grandTotal,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText7 = QString( data );
        cellCursor.insertText( cellText7 );



        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }

    data = "_________";
    cell = textTable->cellAt( rows+1, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    QTextBlockFormat rightAlignment2;
    rightAlignment2.setAlignment(Qt::AlignRight);
    rightAlignment2.setRightMargin(0);
    cellCursor.setBlockFormat(rightAlignment2);

    cellCursor.insertText( data );

    data = QString::number(obj->subTotal,'f',2);
    cell = textTable->cellAt( rows+2, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.insertText( data );

    // Insert Ledgers Here

    float cgstTotal = 0;
    float sgstTotal = 0;
    float igstTotal = 0;


    for(int ledCnt=0;ledCnt<obj->ledgersList.size();ledCnt++){

        //        int j = i-rows-3;

        //        if(1){
        //            cgstTotal += obj->ledgersList[j].amount;
        //        }
        //        else if(1){
        //            sgstTotal += obj->ledgersList[j].amount;
        //        }
        //        else if(1){
        //            igstTotal += obj->ledgersList[j].amount;
        //        }

        qDebug()<<"Ledger ["<<ledCnt<<"]";
        qDebug()<<"table Row Count : "<<textTable->rows();

        if(! obj->ledgersList[ledCnt].LedgerID.contains("0x7x")){

            qDebug()<<"Creating new Row ";
            textTable->insertRows(textTable->rows(),1);
            int rowCnt = textTable->rows()-1;

            data = obj->ledgersList[ledCnt].LedgerName;
            cell = textTable->cellAt( rowCnt, 1 );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(cellFormat);
            cellCursor.setBlockFormat(leftAlignment);

            QString cellText = QString( data );
            cellCursor.insertText( cellText );

            if(obj->ledgersList[ledCnt].crAmount>0)
                data = QString::number(obj->ledgersList[ledCnt].crAmount,'f',2);
            else
                data = "(-)"+QString::number(obj->ledgersList[ledCnt].drAmount,'f',2);

            cell = textTable->cellAt( rowCnt, columns-1 );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(cellFormat);
            cellCursor.setBlockFormat(rightAlignment);

            cellText = QString( data );
            cellCursor.insertText( cellText );
        }

    }

    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}


void PrinterHelper::getInventoryVoucherTablePurchase(QTextCursor *cursor,
                                                     GeneralVoucherDataObject *obj, QList<qreal> widthList)
{


    QStringList headingText;

    headingText << "#"
                << "Particulars"
                << "HSN"
                << "Qty"
                << "Rate"
                << "Disc%"
                << "Tax%"
                << "Total";

    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();

    int ledgerCount = obj->ledgersList.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 1 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormatNoBorder.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( maxrows+2,
                                                 columns,
                                                 tableFormatNoBorder );

    cursor->movePosition( QTextCursor::End );
    tableFormatNoBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;
    qDebug()<<"items Count : "<<rows;

    for(int row =0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        data = obj->InventoryItems[row].BaseItem.hsnCode;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(centerAlignment);
        const QString cellText5 = QString(data);
        cellCursor.insertText( cellText5 );

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        data = QString::number(obj->InventoryItems[row].BaseItem.price,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText2 = QString( data );
        cellCursor.insertText( cellText2 );

        data = QString::number(obj->InventoryItems[row].BaseItem.discountPercentage,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText6 = QString( data );
        cellCursor.insertText( cellText6 );

        data = QString::number(obj->InventoryItems[row].BaseItem.taxRate,'f',2) +"%";
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText7 = QString( data );
        cellCursor.insertText( cellText7 );


        data = QString::number(obj->InventoryItems[row].BaseItem.subTotal,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);


        cellCursor.insertText( QString( data ) );



        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }

    data = "_________";
    cell = textTable->cellAt( rows+1, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    QTextBlockFormat rightAlignment2;
    rightAlignment2.setAlignment(Qt::AlignRight);
    rightAlignment2.setRightMargin(0);
    cellCursor.setBlockFormat(rightAlignment2);

    cellCursor.insertText( data );

    data = QString::number(obj->subTotal,'f',2);
    cell = textTable->cellAt( rows+2, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.insertText( data );

    // Insert Ledgers Here

    float cgstTotal = 0;
    float sgstTotal = 0;
    float igstTotal = 0;



    for(int ledCnt=0;ledCnt<obj->ledgersList.size();ledCnt++){

        //        int j = i-rows-3;

        //        if(1){
        //            cgstTotal += obj->ledgersList[j].amount;
        //        }
        //        else if(1){
        //            sgstTotal += obj->ledgersList[j].amount;
        //        }
        //        else if(1){
        //            igstTotal += obj->ledgersList[j].amount;
        //        }

        qDebug()<<"Ledger ["<<ledCnt<<"]";
        qDebug()<<"table Row Count : "<<textTable->rows();

        if(! obj->ledgersList[ledCnt].LedgerID.contains("0x8x")){

            qDebug()<<"Creating new Row ";
            textTable->insertRows(textTable->rows(),1);
            int rowCnt = textTable->rows()-1;

            data = obj->ledgersList[ledCnt].LedgerName;
            cell = textTable->cellAt( rowCnt, 1 );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(cellFormat);
            cellCursor.setBlockFormat(leftAlignment);

            QString cellText = QString( data );
            cellCursor.insertText( cellText );

            if(obj->ledgersList[ledCnt].drAmount>0)
                data = QString::number(obj->ledgersList[ledCnt].drAmount,'f',2);
            else
                data = "(-)"+QString::number(obj->ledgersList[ledCnt].crAmount,'f',2);

            cell = textTable->cellAt( rowCnt, columns-1 );
            Q_ASSERT( cell.isValid() );
            cellCursor = cell.firstCursorPosition();
            cellCursor.setCharFormat(cellFormat);
            cellCursor.setBlockFormat(rightAlignment);

            cellText = QString( data );
            cellCursor.insertText( cellText );
        }

    }

    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}

void PrinterHelper::getInventoryVoucherTableDeliveryNote(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList)
{
    QStringList headingText;

    headingText << "#"
                << "Particulars"
                   //                << "HSN"
                << "Qty"
                << "Price"
                << "Tax %"
                << "Total";

    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();

    int ledgerCount = obj->ledgersList.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 1 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }

    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormatNoBorder.setColumnWidthConstraints(constraints);

    //    tableFormat.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( maxrows+2,
                                                 columns,
                                                 tableFormatNoBorder );

    cursor->movePosition( QTextCursor::End );
    tableFormatNoBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(9);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int row =0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        //        data = obj->InventoryItems[row].BaseItem.hsnCode;
        //        cell = textTable->cellAt( row + 1, column++ );
        //        Q_ASSERT( cell.isValid() );
        //        cellCursor = cell.firstCursorPosition();
        //        cellCursor.setCharFormat(cellFormat);
        //        cellCursor.setBlockFormat(centerAlignment);
        //        const QString cellText5 = QString(data);
        //        cellCursor.insertText( cellText5 );

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        data = QString::number(obj->InventoryItems[row].BaseItem.price,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText2 = QString( data );
        cellCursor.insertText( cellText2 );

        data = QString::number(obj->InventoryItems[row].BaseItem.taxRate,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText6 = QString( data );
        cellCursor.insertText( cellText6 );

        data = QString::number(obj->InventoryItems[row].BaseItem.grandTotal,'f',2);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText7 = QString( data );
        cellCursor.insertText( cellText7 );



        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }

    qDebug()<<"4392";
    data = "_________";
    cell = textTable->cellAt( rows+1, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    QTextBlockFormat rightAlignment2;
    rightAlignment2.setAlignment(Qt::AlignRight);
    rightAlignment2.setRightMargin(0);
    cellCursor.setBlockFormat(rightAlignment2);

    cellCursor.insertText( data );

    data = QString::number(obj->grandTotal,'f',2);
    cell = textTable->cellAt( rows+2, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.insertText( data );





    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}

void PrinterHelper::getDispatchTextTable(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList)
{
    QStringList headingText;

    headingText << "#"
                << "Particulars"
                << "MRP"
                << "Qty";

    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();


    QString companyName = LoginValues::getCompany().CompanyName;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString branch = LoginValues::getCompany().branch;

    addressLine = branch+", "+addressLine;

    QString dateLine = QString("Date : " +obj->VoucherDate.toString("dd-MM-yyyy"));
    QString ReportLine = "Dispatch Slip";


    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 5 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 5 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }


    tableFormat.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( rows+5,
                                                 columns,
                                                 tableFormat );

    cursor->movePosition( QTextCursor::End );
    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;


    QTextCharFormat tableHeaderAddressFormat;
    tableHeaderAddressFormat.setFontPointSize(11);




    QTextCharFormat tableCompanyFormat;
    tableCompanyFormat.setFontPointSize(16);



    textTable->mergeCells(0, 0, 1, columns);
    textTable->mergeCells(1, 0, 1, columns);
    textTable->mergeCells(2, 0, 1, columns);
    textTable->mergeCells(3, 0, 1, columns);

    QTextTableCell cell1 = textTable->cellAt( 0, 0 );
    Q_ASSERT( cell1.isValid() );
    cell1.setFormat( tableCompanyFormat );
    QTextCursor cellCursor1 = cell1.firstCursorPosition();
    cellCursor1.setBlockFormat(centerAlignment);

    cellCursor1.insertText( companyName);

    cell1 = textTable->cellAt (1, 0 );
    Q_ASSERT( cell1.isValid() );
    cell1.setFormat( tableHeaderAddressFormat );
    cellCursor1 = cell1.firstCursorPosition();
    cellCursor1.insertText( addressLine);
    cellCursor1.setBlockFormat(centerAlignment);


    cell1 = textTable->cellAt (2, 0 );
    Q_ASSERT( cell1.isValid() );
    cell1.setFormat( tableHeaderAddressFormat );
    cellCursor1 = cell1.firstCursorPosition();
    cellCursor1.insertText(ReportLine );
    cellCursor1.setBlockFormat(centerAlignment);

    cell1 = textTable->cellAt (3, 0 );
    Q_ASSERT( cell1.isValid() );
    cell1.setFormat( tableHeaderAddressFormat );
    cellCursor1 = cell1.firstCursorPosition();
    cellCursor1.insertText( dateLine);
    cellCursor1.setBlockFormat(rightAlignment);


    QTextCharFormat right;


    //Insert Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 4, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int row = 0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);
        cell = textTable->cellAt( row + 5, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 5, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        data = QString::number(obj->InventoryItems[row].BaseItem.price_2,'f',2);
        cell = textTable->cellAt( row + 5, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText5 = QString(data);
        cellCursor.insertText( cellText5 );

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 5, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );


        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }


    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}

void PrinterHelper::getInventoryVoucherTableWorkOrder(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList)
{
    QStringList headingText;

    headingText << "#"
                << "Particulars"
                << "Qty";

    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();

    int ledgerCount = obj->ledgersList.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 1 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormatNoBorder.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( maxrows+2,
                                                 columns,
                                                 tableFormatNoBorder );

    cursor->movePosition( QTextCursor::End );
    tableFormatNoBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int row =0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );



        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );



        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }









    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}

void PrinterHelper::getInventoryVoucherTableIssueReq(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList)
{
    QStringList headingText;

    headingText << "#"
                << "Particulars"
                << "UOM"
                << "Calc Qty"
                << "Requested"
                << "Issued";


    const int columns = headingText.length();

    const int rows = obj->InventoryItems.count();

    int ledgerCount = obj->ledgersList.count();

    int maxrows = obj->InventoryItems.count()+1;

    int rowsCnt = maxrows-rows;

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormatNoBorder;
    tableFormatNoBorder.setBorder(1);
    tableFormatNoBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormatNoBorder.setHeaderRowCount( 1 );
    tableFormatNoBorder.setCellSpacing(0);
    tableFormatNoBorder.setCellPadding(3);
    tableFormatNoBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);

    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormatNoBorder.setColumnWidthConstraints(constraints);

    //tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextTable* textTable = cursor->insertTable( maxrows+2,
                                                 columns,
                                                 tableFormatNoBorder );

    cursor->movePosition( QTextCursor::End );
    tableFormatNoBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(8);

    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }

    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int row =0;row<rows;row++){

        int column =0;
        data = QString::number(slNocnt++);
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText1 = QString( data );
        //      cellCursor.insertFrame(frameFormat);
        cellCursor.insertText( cellText1 );

        qDebug()<<"row "<<row <<" : "<<cellCursor.position();

        data = obj->InventoryItems[row].BaseItem.ItemName;
        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        const QString cellText4 = QString( data );
        cellCursor.insertText( cellText4 );

        if(obj->InventoryItems[row].BaseItem.calculatedQty>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.calculatedQty) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        QString cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        if(obj->InventoryItems[row].BaseItem.requestQty>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.requestQty) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";


        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellText3 = QString( data );
        cellCursor.insertText( cellText3 );

        if(obj->InventoryItems[row].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[row].BaseItem.quantity) +" "+ obj->InventoryItems[row].BaseItem.uomObject.UomSymbol;
        else
            data ="";


        cell = textTable->cellAt( row + 1, column++ );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellText3 = QString( data );
        cellCursor.insertText( cellText3 );


        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }



    cursor->movePosition( QTextCursor::End );


    qDebug()<<"Text Table added";
}

void PrinterHelper::getInventoryVoucherTableNew(QTextTable *textTable, GeneralVoucherDataObject *obj,QList<float> widthList)
{
    QStringList headingText;

    int columns = textTable->columns();


    headingText << "#"
                << "Particulars"
                << "HSN"
                << "Qty"
                << "Price"
                << "Disc%"
                << "Total";

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( 1 );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);

    QTextTableFormat tableFormat_noBorder;
    tableFormat_noBorder.setBorder(0);
    tableFormat_noBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat_noBorder.setCellSpacing(0);
    tableFormat_noBorder.setCellPadding(3);
    tableFormat_noBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    tableFormat_noBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);


    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);


    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);




    QVector<QTextLength> constraints;

    for(int i=0;i<widthList.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthList[i]);

    }

    tableFormat.setColumnWidthConstraints(constraints);

    tableFormat_noBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));


    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(10);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);

    QTextFrameFormat frameFormat;
    frameFormat.setHeight(15);


    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    //Inset Header Text
    for( int column = 0; column < columns; column++ ) {

        QString data = headingText[column];
        QTextTableCell cell = textTable->cellAt( 0, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat (tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(headerCellFormat);
        cellCursor.insertText( data);
    }




    QString data;
    QTextTableCell cell;
    QTextCursor cellCursor;

    QList<QTextTable*> colTables;

    for(int i =0;i<textTable->columns();i++){
        cell = textTable->cellAt( 2, i );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);

        QTextTable* colTable = cellCursor.insertTable(1,1,tableFormat_noBorder);
        colTables.append(colTable);

    }

    //colTables[column]

    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    int slNocnt=1;

    for(int invCnt =0;invCnt<obj->InventoryItems.size();invCnt++){

        int column =0;

        if (invCnt>0)
            colTables[column]->appendRows(1);


        data = QString::number(slNocnt++);
        cell = colTables[column]->cellAt( invCnt, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );


        data = obj->InventoryItems[invCnt].BaseItem.ItemName;
        cell = colTables[column]->cellAt( invCnt, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = obj->InventoryItems[invCnt].BaseItem.hsnCode;
        cell = colTables[column]->cellAt( invCnt , 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(centerAlignment);
        cellCursor.insertText( data );

        if(obj->InventoryItems[invCnt].BaseItem.quantity>0)
            data = QString::number(obj->InventoryItems[invCnt].BaseItem.quantity) +" "+ obj->InventoryItems[invCnt].BaseItem.uomObject.UomSymbol;
        else
            data ="";

        cell = colTables[column]->cellAt( invCnt, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(obj->InventoryItems[invCnt].BaseItem.price,'f',2);
        cell = colTables[column]->cellAt( invCnt, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.insertText( data );

        data = QString::number(obj->InventoryItems[invCnt].BaseItem.discountPercentage,'f',2);
        cell = textTable->cellAt( invCnt + 1, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.insertText( data );

        data = QString::number(obj->InventoryItems[invCnt].BaseItem.subTotal,'f',2);
        cell = colTables[column]->cellAt( invCnt , 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);
        const QString cellText7 = QString( data );
        cellCursor.insertText( cellText7 );



        //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);
    }
    /*
    data = "_________";
    cell = textTable->cellAt( rows+1, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    QTextBlockFormat rightAlignment2;
    rightAlignment2.setAlignment(Qt::AlignRight);
    rightAlignment2.setRightMargin(0);
    cellCursor.setBlockFormat(rightAlignment2);

    cellCursor.insertText( data );

    data = QString::number(obj->subTotal,'f',2);
    cell = textTable->cellAt( rows+2, columns-1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setCharFormat(cellFormat);
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.insertText( data );

    // Insert Ledgers Here

    for(int i=rows+3;i<obj->ledgersList.size()+rows+3;i++){
        int j = i-rows-3;

        /*
        data = obj->ledgersList[j].LedgerName;
        cell = textTable->cellAt( i, 1 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(leftAlignment);

        QString cellText = QString( data );
        cellCursor.insertText( cellText );

        data = QString::number(obj->ledgersList[j].amount,'f',2);
        cell = textTable->cellAt( i, columns-1 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setCharFormat(cellFormat);
        cellCursor.setBlockFormat(rightAlignment);

        cellText = QString( data );
        cellCursor.insertText( cellText );

    }
*/

    textTable->insertRows(4,8);

    qDebug()<<"Text Table added";


}

void PrinterHelper::getHSNCodeTable(QTextCursor *cursor, GeneralVoucherDataObject *obj)
{

    // Calculate HSNCODES;


    /*
     * 1. HSN CODE
     * 2. Taxable Amount
     * 3. Rate of Tax
     */
    QMap<QString,std::pair<float,float>> hsnList;



    for(int i =0;i<obj->InventoryItems.size();i++){
        if(hsnList.contains(obj->InventoryItems[i].BaseItem.hsnCode)){
            hsnList[obj->InventoryItems[i].BaseItem.hsnCode].second +=
                    obj->InventoryItems[i].BaseItem.subTotal-obj->InventoryItems[i].BaseItem.discountinAmount;
        }
        else{
            std::pair<float,float> newPair;
            newPair.first = obj->InventoryItems[i].BaseItem.taxRate;
            newPair.second = obj->InventoryItems[i].BaseItem.subTotal-obj->InventoryItems[i].BaseItem.discountinAmount;
            hsnList[obj->InventoryItems[i].BaseItem.hsnCode] = newPair;
        }

    }

    int nHsn = hsnList.size();
    bool hasIGST=false;

    int nCols = 7;
    if(hasIGST)
        nCols = 9;



    QList<qreal> widthlistHSNTable;
    widthlistHSNTable << 45 << 10 <<7.5<< 10<<7.5<< 10<<10;

    QList<int> Allignment;
    Allignment << 1   <<  0    << 0    <<0    <<   0 <<   0    << 0 ;

    QStringList headingText;

    headingText << "HSN Code"
                << "Taxable Value"
                << "Rate"
                << "Amount"
                << "Rate"
                << "Amount"
                << "Total Tax Amount";

    if(hasIGST){
        headingText.insert(6,"Rate");
        headingText.insert(7,"Amounr");

        widthlistHSNTable.insert(6,0);
        widthlistHSNTable.insert(7,0);
    }

    const int columns = headingText.length();

    const int rows = nHsn;

    const int headerRowCount = 2;



    int maxrows = headerRowCount + 1 +1;


    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.setHeaderRowCount( headerRowCount );
    tableFormat.setCellSpacing(0);
    tableFormat.setCellPadding(3);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Inset);

    QTextTableFormat tableFormat_noBorder;
    tableFormat_noBorder.setBorder(0);
    tableFormat_noBorder.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat_noBorder.setCellSpacing(0);
    tableFormat_noBorder.setCellPadding(3);
    tableFormat_noBorder.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    tableFormat_noBorder.setBorderStyle(QTextFrameFormat::BorderStyle_None);



    //    tableFormat.setBorderBrush(QBrush("#000000"));

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);

    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);




    QVector<QTextLength> constraints;

    for(int i=0;i<widthlistHSNTable.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthlistHSNTable[i]);

    }

    tableFormat.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    tableFormat.setColumnWidthConstraints(constraints);

    QList<float> widthlist;
    widthlist << 5 << 44.94 << 9.67 << 9.67<< 9.67<< 9.67 <<12.5;


    QTextTable* MainTable = cursor->insertTable( 3,
                                                 widthlist.size(),
                                                 tableFormat );
    getInventoryVoucherTableNew(MainTable,obj,widthlist);

    cursor->movePosition( QTextCursor::End );



    QTextTable* amountWordsTable = cursor->insertTable( 2,
                                                        1,
                                                        tableFormat_noBorder );
    cursor->movePosition( QTextCursor::End );

    QTextTable* hsnTable = cursor->insertTable( 4,
                                                columns,
                                                tableFormat );

    cursor->movePosition( QTextCursor::End );
    QTextTable* taxWordsTable = cursor->insertTable( 2,
                                                     1,
                                                     tableFormat_noBorder );



    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#ffffff" ) );
    tableHeaderFormat.setFontPointSize(11);

    tableHeaderFormat.merge(tableFormat);

    QTextCharFormat cellFormat;
    cellFormat.setBackground( QColor( "#ffffff" ) );
    cellFormat.setFontPointSize(11);


    QFont font;
    font.setBold(true);
    font.setPointSize(11);

    QTextCharFormat headerCellFormat;
    headerCellFormat.setBackground( QColor( "#ffffff" ) );
    headerCellFormat.setFontPointSize(11);
    headerCellFormat.setFont(font);



    QTextFrameFormat frameFormat;
    frameFormat.setHeight(15);


    QTextBlockFormat format = centerAlignment;
    QTextCharFormat centre;

    // Format Header Row

    hsnTable->mergeCells(0,0,2,1);
    hsnTable->mergeCells(0,1,2,1);
    hsnTable->mergeCells(0,2,1,2);

    hsnTable->mergeCells(0,4,1,2);
    hsnTable->mergeCells(0,6,2,1);



    //Insert Header Text

    QString data = headingText[0];
    QTextTableCell cell = hsnTable->cellAt( 0, 0 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    QTextCursor cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);


    data = headingText[1];
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 0, 1 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);

    data = "Central tax";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 0, 2 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);



    data = "Rate";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 1, 2 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);

    data = "Amount";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 1, 3 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);

    data = "State tax";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 0, 4 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);


    data = "Rate";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 1, 4 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);

    data = "Amount";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 1, 5 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);

    data = "Total Tax Aount";
    cellCursor = cell.firstCursorPosition();
    cell = hsnTable->cellAt( 0, 6 );
    Q_ASSERT( cell.isValid() );
    cell.setFormat (tableHeaderFormat );

    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(centerAlignment);
    cellCursor.setCharFormat(headerCellFormat);
    cellCursor.insertText( data);



    //total Printing Area


    //    data = "Total";
    //    cell = textTable->cellAt( maxrows-1, 0 );
    //    Q_ASSERT( cell.isValid() );
    //    cellCursor = cell.firstCursorPosition();
    //    cellCursor.setBlockFormat(rightAlignment);
    //    cellCursor.setCharFormat(cellFormat);
    //    cellCursor.insertText( data );

    //    data = QString::number(obj->subTotal,'f',2);
    //    cell = textTable->cellAt( maxrows-1, 1 );
    //    Q_ASSERT( cell.isValid() );
    //    cellCursor = cell.firstCursorPosition();
    //    cellCursor.setBlockFormat(rightAlignment);
    //    cellCursor.setCharFormat(cellFormat);
    //    cellCursor.insertText( data );

    //    data = QString::number(0,'f',2);
    //    cell = textTable->cellAt( maxrows-1, 3 );
    //    Q_ASSERT( cell.isValid() );
    //    cellCursor = cell.firstCursorPosition();
    //    cellCursor.setBlockFormat(rightAlignment);
    //    cellCursor.setCharFormat(cellFormat);
    //    cellCursor.insertText( data );

    //    data = QString::number(0,'f',2);
    //    cell = textTable->cellAt( maxrows-1, 5 );
    //    Q_ASSERT( cell.isValid() );
    //    cellCursor = cell.firstCursorPosition();
    //    cellCursor.setBlockFormat(rightAlignment);
    //    cellCursor.setCharFormat(cellFormat);
    //    cellCursor.insertText( data );

    //    hsnTable->mergeCells(maxrows-1,0,1,columns);




    QTextTableFormat tableFormat2;

    //    tableFormat2.setBorder(1);
    //    tableFormat2.setBorderBrush(QBrush(Qt::SolidPattern));
    //    tableFormat2.setHeaderRowCount( headerRowCount );
    //    tableFormat2.setCellSpacing(0);
    //    tableFormat2.setCellPadding(3);
    //    tableFormat2.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
    //    tableFormat2.setWidth(QTextLength(QTextLength::PercentageLength, 100));
    //    tableFormat2.setColumnWidthConstraints(constraints);

    data = "Total";
    cell = hsnTable->cellAt( maxrows-1, 0 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.setCharFormat(cellFormat);
    cellCursor.insertText( data );

    data = QString::number(obj->subTotal,'f',2);
    cell = hsnTable->cellAt( maxrows-1, 1 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.setCharFormat(cellFormat);
    cellCursor.insertText( data );



    cellCursor.setCharFormat(cellFormat);

    format = leftAlignment;

    QList<QTextTable*> colTables;

    for(int i =0;i<hsnTable->columns();i++){
        cell = hsnTable->cellAt( 2, i );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);

        QTextTable* colTable = cellCursor.insertTable(hsnList.size(),1,tableFormat_noBorder);
        colTables.append(colTable);

    }

    QMapIterator<QString, std::pair<float,float>> ix(hsnList);

    float stateTax=0;
    float centralTax=0;
    float integrartedTax=0;
    int i=0;

    while (ix.hasNext()) {
        ix.next();
        int j=0;
        QString hsncode = ix.key();
        float taxable = ix.value().second;
        float taxRate = ix.value().first;

        data = hsncode;
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(leftAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxable,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxRate/2,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxable * taxRate/200,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxRate/2,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxable * taxRate/200,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        data = QString::number(taxable * taxRate/100,'f',2);
        cell = colTables[j++]->cellAt( i, 0 );
        Q_ASSERT( cell.isValid() );
        cellCursor = cell.firstCursorPosition();
        cellCursor.setBlockFormat(rightAlignment);
        cellCursor.setCharFormat(cellFormat);
        cellCursor.insertText( data );

        stateTax   += taxable * taxRate/200;
        centralTax += taxable * taxRate/200;

        i++;


    }

    data = QString::number(centralTax,'f',2);
    cell = hsnTable->cellAt( maxrows-1, 3 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.setCharFormat(cellFormat);
    cellCursor.insertText( data );

    data = QString::number(stateTax,'f',2);
    cell = hsnTable->cellAt(  maxrows-1, 5 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.setCharFormat(cellFormat);
    cellCursor.insertText( data );

    data = QString::number(obj->taxTotalAmount,'f',2);
    cell = hsnTable->cellAt( maxrows-1, 6 );
    Q_ASSERT( cell.isValid() );
    cellCursor = cell.firstCursorPosition();
    cellCursor.setBlockFormat(rightAlignment);
    cellCursor.setCharFormat(cellFormat);
    cellCursor.insertText( data );

    cursor->movePosition( QTextCursor::End );


}




int PrinterHelper::PrintDeliveryNoteVoucher(GeneralVoucherDataObject data)
{

    QString ReportName = data.voucherType;

    qDebug()<<"printing" << ReportName;
    QList<int> colSizes; //not so sure.

    //    QMessageBox::StandardButton reply;

    //    reply = QMessageBox::question(this,"Print Description",
    //                                  "Would you like to Print Description with Item Names",
    //                                  QMessageBox::Yes|QMessageBox::No);

    bool printDescr= false;

    //    if(reply==QMessageBox::Yes){
    //        printDescr=true;
    //    }


    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPageSize(QPrinter::A4);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return 0 ;

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QString blankSpace="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString borderWidth = "border-width=\"1px\"";
    //  QString totals = QString::number(total, 'f', 2);
    //"<center>"+text2+"&trade;</center>"
    QString headhtml = "<HTML>"
                       "<HEAD>";
    QString cssTable;
    cssTable = "<style type=\"text/css\">";
    cssTable += "table.tb1 { border-color: }";

    cssTable+="</style></head>";

    QString companyName = "<font size =\"12\">Al Rumooz</font>";
    QString VoucherName = "<font size =\"8\" color=\"green\"><i><u>"+data.voucherType+"</u></i></font>";
    QString bodyTable ="";

    QString css;
    css = "<style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 0px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 0px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:0px; font-size:12px;padding-top:2px;padding-right:5px}";
    css += "table.tbl th {background-color:#ed1a24;padding-boottom:4px; padding-left:0px; padding-right:5px;border-color:#ed1a24;font-color:#ffffff;}";

    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px; font-size:12px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";


    css += "table.tblsign {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tblsign td {padding-right:0px;padding-top: 3px;font-size:12px;padding-bottom: 3px;padding-left: 4px;}";
    css += "table.tblsign th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Arial; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    QString listTableRowSpan = "colspan=\"2\"";
    //    bodyTable +=  "<table class=\"tblb\" width =\"100%\">";
    //    bodyTable +=  "<tr><td width=\"66%\"><img src=:/images/rmzlogo200w.png></td><td align=\"right\">"+
    //            VoucherName+"</td></tr>"
    //                        "";
    QString VoucherDetails = "";//<tr><td "+listTableRowSpan+">";
    VoucherDetails +="<table class=\"tbl\" width =\"100%\">";

    QString CustomerData = "<tr><td width = \"50%\"><font face=\"calibri\" size=\"3\">";
    //    <tr><td>Customer Details:<br>"
    //                           "<font size =\"5\"><b> "+data.ledgerObject.LedgerName+"</b></font>";

    CustomerData += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+data.ledgerObject.Address;
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ";
    CustomerData += "<br><br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contact Person :</b>";
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
            +data.ledgerObject.ContactPersonName;
    CustomerData += "</font></td>";


    QString QuoteDetails = "<td><font face=\"calibri\" size =\"3\">";
    QuoteDetails += "<table class=\"tbldet\" width =\"100%\">";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Prepared By</b></td><td>&nbsp;&nbsp;<b>Currency</b></td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ADMIN</td><td>&nbsp;&nbsp;AED</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Rumooz Contact</b></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;Salesman</td><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Reference</b></td>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ORDER BY MAIL</td></tr>";

    //    QuoteDetails += "<tr><td>Validity</td><td>30 days";

    QuoteDetails += ""
                    "</td></tr></table></font></td></tr>";
    VoucherDetails +=CustomerData+QuoteDetails + "</table>";

    QString SubjectRow ="";

    QString itemFontSize ="4";
    QString itemHeadFontSize ="4";
    QString CoverLetter ="<tr><td "+listTableRowSpan+">";
    CoverLetter += "<pre class =\"cusfont\">Hello Sir,\n"
                   "    Please find following quotaion";
    CoverLetter += "</pre></td></tr>";

    QString html = "<tr><td "+
            listTableRowSpan
            +">";
    //    html = "<table width=\"100%\" <tr><td>hello</td><tr></table>";
    html =
            "<br><font face =\"calibri\"><table class=\"tbl\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">"
            "<tr>"
            "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Sl No</font></th>"
                              "<th width=\"35%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Item</font></th>"
                              "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Uom</font></th>"
                              "<th width=\"12%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Model</font></th>"
                              "<th width=\"15%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Brand</font></th>"
                              "<th width=\"15%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Serial No.</font></th>"
                              "<th width=\"5%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Qty</font></th>"
                              "<th width=\"13%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Remarks</font></th>";
    //                              "<th width=\"16%\" align=\"right\"><font color=\"#ffffff\" size =\""
    //            +itemHeadFontSize+"\">Total</h2></th>"
    html+= "</tr>";

    int numlines = 0;
    for(int i=0;i<data.InventoryItems.size();i++){
        html += "\n<tr>";
        html += "<td align =\"left\"> <font >";
        html += QString::number(i+1);
        html += "</td>";

        //html += "<td align =\"left\"> <font >";

        html += "<td align =\"left\">";
        html += data.InventoryItems[i].BaseItem.ItemName.toUpper();
        if(printDescr){
            numlines+=data.InventoryItems[i].BaseItem.ItemDescription.split("\n").count();
            html += "<br>&nbsp;"+data.InventoryItems[i].BaseItem.ItemDescription.replace("\n","<br>&nbsp;").simplified();
        }
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.uomObject.UomName.toUpper();
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.brandName.toUpper();
        html += "</font></td>";

        html += "<td align =\"left\"> <font >";
        QStringList sllist = salesOrderDatabaseHelper.getSerialNumbersbyRequirement(data.InventoryItems[i].BaseItem.requirementItemIdOld);
        qDebug()<<sllist;
        QString ssl = sllist.join("<br>");
        html += ssl;
        numlines++;

        //html += data.InventoryItems[i].BaseItem.SerailNumber.toUpper();
        html += "</font></td>";


        html += "<td align =\"right\"> <font >";
        html += QString::number(data.InventoryItems[i].BaseItem.quantity);
        html += "</td>";

        html += "<td align =\"left\"> <font >";

        html += "</td>";

        //        html += "<td align =\"right\"> <font size =\""+itemFontSize+"\">";
        //        html += formatNumber(data.InventoryItems[i].BaseItem.price);
        //        html += "</td>";

        //        html += "<td align =\"right\"> <font size =\""+itemFontSize+"\">";
        //        html += formatNumber(data.InventoryItems[i].BaseItem.subTotal);
        //        html += "</td>";

        html+= "</tr>";


    }

    int listSize = 4;

    //    qDebug()<<"Num Lines = "<<numlines;
    if(numlines<20)
        for(int i=numlines;i<20;i++){
            html+="<tr><td></td></tr>";
            //html+="<tr><td ><font size =\""+itemFontSize+"\">"+QString::number(i)+"</font></td></tr>";
        }
    //    qDebug()<<"659";

    QString TotalString = "";
    TotalString += "<tr><td colspan=\"8\"><hr color=\"red\"></td>";
    TotalString += "<tr><td></td><td></td><td></td><td></td>";
    TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Gross Total</font></td>";
    TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data.grandTotal)+"</font></td>";

    TotalString += "<tr><td></td><td></td><td></td><td></td>";
    TotalString += "<td align=\"left\" colspan=\"3\"><font face=\"calibri\" size =\"4\">Discount</font></td>";
    TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(100)+"</font></td>";

    TotalString += "<tr><td></td><td></td><td></td><td></td>";
    TotalString += "<th align=\"left\" colspan=\"3\"><font color=\"#ffffff\" face=\"calibri\" size =\"5\">Grand Total</font></th>";
    TotalString += "<th align=\"right\"><font color=\"#ffffff\" size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></th>";

    //    TotalString += "<td>";
    //    TotalString += "<table class=\"tbl\">";
    //    TotalString += "<td colspan=\"2\" align=\"left\">  <font size =\"5\">Grand Total</font></td>";
    //    TotalString += "<td align=\"right\"> <font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></td>";
    //    TotalString += "</table>";

    //    TotalString += "</td>";

    //    TotalString += "<th align=\"left\" colspan=\"4\"><font face=\"calibri\" size =\"5\">";
    //    TotalString += "<p align=\"left\">Grand Total";
    //    TotalString += "<span align=\"right\>"+formatNumber(data.grandTotal-100)+"</span</p></th>";
    //    TotalString += "<table class=\"tbl\" width =\"100%\"><th align=\"left\">";
    //    TotalString += "Grand Total</th>";
    //    TotalString += "<th align=\"right\"><font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></th>";
    //    TotalString += "</table></th>";


    TotalString += "</tr></table>";

    QString amountInWords = "<table width =\"100%\"> ";

    amountInWords += "<tr><td "+listTableRowSpan+"> <font face=\"calibri\" size =\"4\">";
    amountInWords += "Amount in Words : <i>Aed. "+numberToText(data.grandTotal-100)+ " only</i></font>";
    amountInWords += "</td></tr>";


    QString lineBlock = "";
    lineBlock += "<tr><td colspan=\"8\">";
    lineBlock += "<hr color=\"red\"></td></tr>";

    QString SignatureBlock;

    SignatureBlock += "</table>";
    SignatureBlock += "<table class=\"tblsign\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">";//"<table width=\"100%\">";
    SignatureBlock += "<tr>";
    SignatureBlock += "<td></td><td></td><td>For AL RUMOOZ<br><br><br><br><HR></td>";
    SignatureBlock += "<tr>";

    SignatureBlock += "<td width=\"50%\">";
    SignatureBlock += "<table>";
    SignatureBlock += "<tr><td>Reciever Name</td><td>:</td></tr>";
    SignatureBlock += "<tr><td>Phone Number</td><td>:</td></tr>";
    SignatureBlock += "<tr><td>Date & Time </td><td>:</td></tr>";
    SignatureBlock += "<tr><td>Reciever Signature </td><td>:</td></tr>";

    SignatureBlock += "</td>";
    SignatureBlock += "<td width=\"20%\"></td>";

    SignatureBlock += "<td width=\"30%\">";
    SignatureBlock += "<table>";
    SignatureBlock += "<tr><td>Location </td><td>:</td></tr>";
    SignatureBlock += "<tr><td>Delivered By </td><td>:</td></tr>";
    SignatureBlock += "<tr><td>Time        </td><td>:&nbsp;&nbsp;&nbsp;&nbsp;"
            +QDateTime::currentDateTime().toString("dd-MM-yyyy  hh:mm")
            +"</td></tr>";
    SignatureBlock += "<tr><td>Store Keeper Signature</td><td>:</td></tr>";
    SignatureBlock += "</table>";
    SignatureBlock += "</td>";


    SignatureBlock += "</tr></table>";






    //  html = headhtml+css+bodyTable+VoucherDetails + html+TotalString+amountInWords+lineBlock;
    html = headhtml+css+html+lineBlock+SignatureBlock;
    //+SignatureBlock;

    //    html = css+html;
    //    html = headhtml + html + TotalString+amountInWords+lineBlock+SignatureBlock;
    //    html+=  "</td></tr></table></body></html>";
    qDebug()<<"Tr count = "<<html.count("<tr");
    qDebug()<<"Tr +br count = "<<html.count("<tr")+html.count("<br>");

    qDebug()<<html;
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    cursor.insertHtml(html);

    //    QPainter painter1( &printer );
    //    QPainter *painter = &painter1;
    //    const QSizeF pageSize = printer.paperRect().size();
    //    const double bm = mmToPixels(printer, borderMargins);
    //    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    //    painter->drawRect(borderRect);

    //    cursor.setPosition(1000,QTextCursor::MoveAnchor);
    //    cursor.insertText("Text Here");
    //    qDebug()<<cursor.position();
    //    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,5);
    //    qDebug()<<cursor.position();
    //    cursor.insertText("Text2 Here2");

    //    QTextBlockFormat blockFormat;
    //    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    //    cursor.insertBlock(blockFormat);
    //    //    cursor.setPosition(cursor.position()+100);
    //    cursor.insertHtml(getTermsinHtml(validity,subject,payment,warranty,extra,delivery,exclusion));
    QString a="Prepared By";
    QString b="Sales Order Ref";
    QString c="LPO Ref";
    QString v1;QString v2;QString v3;
    v1 = data.AddedBy;
    v2 = data.ConvertedToSalesOrder;
    v3 = data.LPO;

    printDocument(printer,&doc,0,data,a,b,c,v1,v2,v3);

    return 0;
}

int PrinterHelper::PrintAccountVoucher(GeneralVoucherDataObject data)
{

}

int PrinterHelper::PrintLedgerList(QList<LedgerMasterDataModel> data)
{

}

int PrinterHelper::PrintInventoryList(QList<inventoryItemDataModel> data)
{

}

int PrinterHelper::PrintGeneralReport(QTableWidget *table, QList<int> colSizes, QString ReportName, QDate dateFrom, QDate dateTo)
{

}

int PrinterHelper::printQuotation(QString validity, QString subject, QString payment,
                                  QString warranty, QString extra, QString delivery,
                                  QString exclusion, bool printExtra, bool printNow,
                                  GeneralVoucherDataObject data,bool printAsPdf)
{
    QString ReportName = data.voucherType;
    QuoteRef = subject;

    if( data.RevisionNo>0){
        data.voucherNumber += " - r"+QString::number(data.RevisionNo);
    }

    qDebug()<<"printing" << ReportName;
    QList<int> colSizes; //not so sure.


    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPageSize(QPrinter::A4);
    if(printAsPdf!=true){
        printAsPdfG=false;
        QPrintDialog pDial(&printer,this);
        if(pDial.exec()==QDialog::Rejected)
            return 0 ;
    }
    else{
        printAsPdfG=true;
        QString dir = QDir::home().path();
        QString fileName = dir+"/QN_"+data.voucherNumber+" "+data.Contact.CompanyName+".pdf";
        printer.setOutputFormat(QPrinter::PdfFormat);

        fileName = QFileDialog::getSaveFileName(this,"File Name",fileName,tr("pdf"));
        printer.setOutputFileName(fileName);
    }
    printer.setPageSize(QPrinter::A4);
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QString blankSpace="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString borderWidth = "border-width=\"1px\"";
    //  QString totals = QString::number(total, 'f', 2);
    //"<center>"+text2+"&trade;</center>"
    QString headhtml = "<HTML>"
                       "<HEAD>";
    QString cssTable;
    cssTable = "<style type=\"text/css\">";
    cssTable += "table.tb1 { border-color: }";

    cssTable+="</style></head>";

    QString companyName = "<font size =\"12\">Al Rumooz</font>";
    QString VoucherName = "<font size =\"8\" color=\"green\"><i><u>"+data.voucherType+"</u></i></font>";
    QString bodyTable ="";

    QString css;
    css = "<style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 0px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 0px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:3px; font-size:12px;padding-top:2px;}";
    css += "table.tbl th {background-color:#ed1a24;padding-bottom:4px; padding-left:0px;padding-right:3px; border-color:#ed1a24;font-color:#ffffff;}";

    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;font-size:12px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Arial; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    QString listTableRowSpan = "colspan=\"2\"";
    //    bodyTable +=  "<table class=\"tblb\" width =\"100%\">";
    //    bodyTable +=  "<tr><td width=\"66%\"><img src=:/images/rmzlogo200w.png></td><td align=\"right\">"+
    //            VoucherName+"</td></tr>"
    //                        "";
    QString VoucherDetails = "";//<tr><td "+listTableRowSpan+">";
    VoucherDetails +="<table class=\"tbl\" width =\"100%\">";

    QString CustomerData = "<tr><td width = \"50%\"><font face=\"calibri\" size=\"3\">";
    //    <tr><td>Customer Details:<br>"
    //                           "<font size =\"5\"><b> "+data.ledgerObject.LedgerName+"</b></font>";

    CustomerData += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+data.ledgerObject.Address;
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ";
    CustomerData += "<br><br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contact Person :</b>";
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
            +data.ledgerObject.ContactPersonName;
    CustomerData += "</font></td>";


    QString QuoteDetails = "<td><font face=\"calibri\" size =\"3\">";
    QuoteDetails += "<table class=\"tbldet\" width =\"100%\">";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Prepared By</b></td><td>&nbsp;&nbsp;<b>Currency</b></td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ADMIN</td><td>&nbsp;&nbsp;AED</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Rumooz Contact</b></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;Salesman</td><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Reference</b></td>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ORDER BY MAIL</td></tr>";

    //    QuoteDetails += "<tr><td>Validity</td><td>30 days";

    QuoteDetails += ""
                    "</td></tr></table></font></td></tr>";
    VoucherDetails +=CustomerData+QuoteDetails + "</table>";

    QString SubjectRow ="";

    QString itemFontSize ="3";
    QString itemHeadFontSize ="4";
    QString CoverLetter ="<tr><td "+listTableRowSpan+">";
    CoverLetter += "<pre class =\"cusfont\">Hello Sir,\n"
                   "    Please find following quotaion";
    CoverLetter += "</pre></td></tr>";

    QString html = "<tr><td "+
            listTableRowSpan
            +">";
    //    html = "<table width=\"100%\" <tr><td>hello</td><tr></table>";
    html =
            "<br><font face =\"calibri\"><table class=\"tbl\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">"
            "<tr>"
            "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Sl</font></th>"
                              "<th width=\"35%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Item</font></th>";
    //    html+="<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
    //            +itemHeadFontSize+"\">Uom</font></th>";
    html+="<th width=\"16%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Model</font></th>"
                              "<th width=\"16%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Brand</font></th>"
                              "<th width=\"8%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Qty</font></th>"
                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Price</font></th>"
                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Total</h2></th>"
                              "</tr>";
    html +="<tr></tr>";
    for(int i=0;i<data.InventoryItems.size();i++){
        html += "\n<tr>";
        html += "<td align =\"left\"> <font> ";
        html += QString::number(i+1);
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.ItemName.toUpper();
        html += "</td>";

        //        html += "<td align =\"left\"> <font >";
        //        html += data.InventoryItems[i].BaseItem.UOMName.toUpper();
        //        qDebug()<<"UOM Name is "<<data.InventoryItems[i].BaseItem.UOMName.toUpper();
        //        qDebug()<<"item Codeis "<<data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        //        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.brandName.toUpper();
        html += "</font></td>";

        html += "<td align =\"right\"> <font >";
        html += QString::number(data.InventoryItems[i].BaseItem.quantity);
        html += "</td>";

        html += "<td align =\"right\"> <font >";

        if(data.InventoryItems[i].BaseItem.subTotal==0 && data.InventoryItems[i].BaseItem.price !=0){
            html +=  formatNumber(data.InventoryItems[i].BaseItem.price*data.InventoryItems[i].BaseItem.length/1000);
        }
        else{
            html += formatNumber((data.InventoryItems[i].BaseItem.subTotal+data.InventoryItems[i].BaseItem.discountinAmount)/
                                 data.InventoryItems[i].BaseItem.quantity);
        }
        html += "</td>";

        html += "<td align =\"right\"> <font >";
        if(data.InventoryItems[i].BaseItem.subTotal==0 && data.InventoryItems[i].BaseItem.price !=0){
            html += "(Optional)";
        }
        else{
            html += formatNumber(data.InventoryItems[i].BaseItem.subTotal + data.InventoryItems[i].BaseItem.discountinAmount);
        }
        html += "</td>";

        html+= "</tr>";
    }
    int listSize = 4;

    if(data.InventoryItems.size()<20)
        for(int i=data.InventoryItems.size();i<20;i++){
            html+="<tr><td></td></tr>";
            //html+="<tr><td ><font size =\""+itemFontSize+"\">"+QString::number(i)+"</font></td></tr>";
        }
    qDebug()<<"659"<<data.discount;


    QString TotalString = "";
    TotalString += "<tr><td colspan=\"7\"><hr color=\"#000000\"></td>";
    qDebug()<<"$$$$$$$$$$$$$ Discount "<<data.discount;
    if(data.discountinAmount>(float) 0){

        TotalString += "<tr><td></td><td></td><td></td><td></td>";
        TotalString += "<td align=\"left\" colspan=\"2\"><font face=\"calibri\" size =\"4\">Gross Total</font></td>";
        TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data.subTotal)+"</font></td>";

        TotalString += "<tr><td></td><td></td><td></td><td></td>";
        TotalString += "<td align=\"left\" colspan=\"2\"><font face=\"calibri\" size =\"4\">Discount</font></td>";
        TotalString += "<td align=\"right\"><font size =\"4\">"+formatNumber(data.discountinAmount)+"</font></td>";
    }
    TotalString += "<tr><td colspan=\"4\"><table width=\"75%\"><tr><td width =\"20%\"><font face=\"calibri\" size =\"4\">Amount in Words : </td><td><font face=\"calibri\" size =\"4\"><b><i>";
    TotalString += numberToText(data.grandTotal)+" only</b></i></font></td></tr></table></td>";
    TotalString += "<th align=\"left\" colspan=\"2\"><font color=\"#ffffff\" face=\"calibri\" size =\"5\">Grand Total</font></th>";
    TotalString += "<th align=\"right\"><font color=\"#ffffff\" size =\"5\">"+formatNumber(data.grandTotal)+"</font></th>";

    //    TotalString += "<td>";
    //    TotalString += "<table class=\"tbl\">";
    //    TotalString += "<td colspan=\"2\" align=\"left\">  <font size =\"5\">Grand Total</font></td>";
    //    TotalString += "<td align=\"right\"> <font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></td>";
    //    TotalString += "</table>";

    //    TotalString += "</td>";

    //    TotalString += "<th align=\"left\" colspan=\"4\"><font face=\"calibri\" size =\"5\">";
    //    TotalString += "<p align=\"left\">Grand Total";
    //    TotalString += "<span align=\"right\>"+formatNumber(data.grandTotal-100)+"</span</p></th>";
    //    TotalString += "<table class=\"tbl\" width =\"100%\"><th align=\"left\">";
    //    TotalString += "Grand Total</th>";
    //    TotalString += "<th align=\"right\"><font size =\"5\">"+formatNumber(data.grandTotal-100)+"</font></th>";
    //    TotalString += "</table></th>";


    TotalString += "</tr></table>";

    QString amountInWords = "<table width =\"100%\"> ";

    amountInWords += "<tr><td "+listTableRowSpan+"> <font face=\"calibri\" size =\"4\">";
    // amountInWords += "Amount in Words : <i><b>Aed. "+numberToText(data.grandTotal)+ " only</b></i></font>";
    amountInWords += "</td></tr>";



    QString lineBlock = "";
    lineBlock += "<tr><td "+listTableRowSpan+">";
    lineBlock += "<hr color=\"#000000\"></td></tr>";


    //  html = headhtml+css+bodyTable+VoucherDetails + html+TotalString+amountInWords+lineBlock;
    html = headhtml+css+html+TotalString+amountInWords+lineBlock;
    //+SignatureBlock;

    //    html = css+html;
    //    html = headhtml + html + TotalString+amountInWords+lineBlock+SignatureBlock;
    //    html+=  "</td></tr></table></body></html>";
    //    qDebug()<<"Tr count = "<<html.count("<tr");
    //    qDebug()<<"Tr +br count = "<<html.count("<tr")+html.count("<br>");

    qDebug()<<html;
    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //   doc.setPageSize(paperSize);
    //   doc.setPageSize(QPrinter::A4);
    cursor.insertHtml(html);

    //    QPainter painter1( &printer );
    //    QPainter *painter = &painter1;
    //    const QSizeF pageSize = printer.paperRect().size();
    //    const double bm = mmToPixels(printer, borderMargins);
    //    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    //    painter->drawRect(borderRect);

    //    cursor.setPosition(1000,QTextCursor::MoveAnchor);
    //    cursor.insertText("Text Here");
    //    qDebug()<<cursor.position();
    //    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,5);
    //    qDebug()<<cursor.position();
    //    cursor.insertText("Text2 Here2");

    QTextBlockFormat blockFormat;
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    //    cursor.setPosition(cursor.position()+100);

    if(printExtra){
        cursor.insertBlock(blockFormat);

        completedDescription = 0;
        while(completedDescription>-1){
            cursor.insertHtml(getDescription(data,0));
            cursor.insertBlock(blockFormat);

        }
    }
    else{
        cursor.insertBlock(blockFormat);
    }
    cursor.insertHtml(getTermsinHtml(validity,subject,payment,warranty,extra,delivery,exclusion));

    QString a ="Prepared By";
    QString b = "Currency";
    QString c = "Reference";
    QString v1;QString v2;QString v3;
    v1 = data.AddedBy;
    v2 = "Aed";
    v3 = subject;

    printDocument(printer,&doc,0,data,a,b,c,v1,v2,v3);


    return 0;
}

int PrinterHelper::printDummy()
{
    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPrinterName("POS-80");
    printer.setPageSize(QPrinter::A4);

    QPainter painter(&printer); // create a painter which will paint 'on printer'.

    painter.drawImage(100,100,QImage(":/images/rumoozlogo1.png"));

    painter.fillRect(300,300,100,50,Qt::red);
    painter.setBrush(Qt::white);
    painter.drawEllipse(250,275,75,75);
    painter.setBrush(Qt::black  );

    painter.drawText(100,100,"Test");
    painter.setFont(QFont("Tahoma",16));
    painter.drawText(200,200,"Test");
    painter.drawText(310,331,"Test");
    painter.end();
}

QString PrinterHelper::getTermsinHtml(QString validity, QString subject, QString payment,
                                      QString warranty, QString extra, QString delivery, QString exclusion)
{
    QString html = "<br><br><font face=\"calibri\" size =\"3\">";
    QString br ="<br><br><br><br><br><br><br><br><br><br><br>";

    html+= "<table width =\"100%\"><tr><td align =\"left\"><h3><u>TERMS & CONDITIONS</u></h3></td></tr></table>";
    QString fontsize ="4";
    QString fontsizeHead ="4";


    QString tabspace = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    //    for(int i=0;i<10;i++){tabspace+=tabspace;}

    //    tabspace="";

    if(validity.length()>0){
        html+= "<b>QUOTATION VALIDITY</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= validity.toUpper();
        html +="<br>";
    }
    if(payment.length()>0){
        html+= "<b>PAYMENT</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= payment.toUpper();
        html +="<br>";
    }
    if(warranty.length()>0){
        html+= "<b>WARRANTY</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= warranty.toUpper();
        html +="<br>";
    }
    if(delivery.length()>0){
        html+= "<b>DELIVERY</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= delivery.toUpper();
        html +="<br>";
    }
    if(exclusion.length()>0){
        html+= "<b>EXCLUSION</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= exclusion.toUpper();
        html +="<br>";
    }
    if(extra.length()>0){
        html+= "<b>NOTE</b>";
        html+= "<br>";
        html+= ""+tabspace;
        html+= extra.toUpper();
        html+="<br>";
    }

    QString SignatureBlock = "<br>"+br;
    //SignatureBlock += "<tr><td "+listTableRowSpan+"><font size =\"4\">";
    SignatureBlock += "<table class=\"tbl\" width =\"100%\"><tr>";
    SignatureBlock += "<td colspan=\"4\" align=\"center\">We trust the above will meet your requirements. With your signature and Stamp, this quotation deems approved.<br><br><br><br></td><tr>";
    SignatureBlock += "<td width =\"25%\"><hr></td><td width=\"45%\"></td><td width =\"25%\"><hr><td width=\"5%\"></td><tr><td align=\"center\">";
    SignatureBlock += "ACCEPTANCE SIGNATURE";
    SignatureBlock += "</td>";
    SignatureBlock += "<td></td><td align=\"center\">For AL RUMOOZ";
    SignatureBlock += "</td><td></td>";
    SignatureBlock += "</tr></table>";
    html+=SignatureBlock;


    //    qDebug()<<html;

    return html;


}

QString PrinterHelper::getDescription(GeneralVoucherDataObject voucher,int i)
{
    QString css;
    css = "<html><head><style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 0px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 0px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:0px; font-size:12px;padding-top:2px;padding-right:5px}";
    css += "table.tbl th {background-color:#ed1a24;padding-boottom:4px; padding-left:0px; padding-right:5px;border-color:#ed1a24;font-color:#ffffff;}";

    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px; font-size:12px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";


    css += "table.tblsign {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tblsign td {padding-right:0px;padding-top: 3px;font-size:12px;padding-bottom: 3px;padding-left: 4px;}";
    css += "table.tblsign th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Calibri; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    QString html =css;
    html += " <table class=\"tbl\" width =\"100%\">";
    html +=  "<tr><td></td></tr><tr><td><u>Item Description</u></td></tr>";
    bool printedImageFlag = false;
    int numLines = 0;
    int numItems = 4;
    int startCount = 1;
    for(int i=completedDescription;i<voucher.InventoryItems.size();i++){

        html += "<tr><td width =\"100%\"><br>"+QString::number(i+1)+". "+voucher.InventoryItems[i].BaseItem.ItemName+"</td></tr>";
        //    html += "<tr><td width =\"100%\"><pre class=\"cusfont\">"+voucher.InventoryItems[i].BaseItem.ItemDescription+"</pre></td></tr>";
        html += "<tr><td width =\"50%\"><ul>";
        QStringList dest = voucher.InventoryItems[i].BaseItem.ItemDescription.split('\n');

        for(QString a:dest){
            numLines++;
            if(a.simplified().length()>0)
                html+="<li>"+toCapsFirstCase(a.simplified())+"</li>";
        }

        if(voucher.InventoryItems[i].BaseItem.isCustomItem && voucher.InventoryItems[i].BaseItem.Dimension.length()>0){
            html+="<li>Dimension(mm) : "+QString::number(voucher.InventoryItems[i].BaseItem.length,'f',0)+"x"+voucher.InventoryItems[i].BaseItem.Dimension.simplified();
            html+="</li";
            numLines++;
        }

        if(voucher.InventoryItems[i].BaseItem.narration.length()>0){
            html+="<li>"+toCapsFirstCase(voucher.InventoryItems[i].BaseItem.narration.replace("\n","<br>").simplified());
            html+="</li";
            numLines+=voucher.InventoryItems[i].BaseItem.narration.split("\n").count();
        }

        ItemImageDatamodel item ;
        item.itemID = voucher.InventoryItems[i].BaseItem.ItemID;
        item= imageHelper->getItemImageAsQimage(item);
        QPixmap pixmap(item.img);
        QBuffer buffer(&item.img);
        pixmap.save(&buffer, "JPG");
        QString url ="";
        if(item.img.size()>0)
        {
            url = QString("<img src=\"data:image/png;base64,") + item.img.toBase64() + "\"  width=\""+QString::number(item.imageWidth)
                    +"\" height =\""+QString::number(item.imageHeight)+"\">";
            printedImageFlag = true;
        }
        html+="</td>";
        //        html +=  "<td width=\"50%\"><img src=:/images/test"+QString::number(i+1)+".jpg width=\"180\" height =\"120\"></td>";

        html +=  "<td width=\"50%\">"+url+"</td>";

        html += "<tr><td colspan =\"2\"><hr></td></tr>";

        completedDescription++;
        startCount++;

        if((startCount)%numItems == 0 ||numLines >20)
            break;

    }
    if(completedDescription >= voucher.InventoryItems.size())
        completedDescription=-1;
    if(printedImageFlag)
        html += "<font size=\"3\"><i>Product images are for illustrative purposes only</i></font>";

    qDebug()<<html;

    return html;

}


QString PrinterHelper::numberToText(uint number)
{
    static QMap<uint, QString> numbers;
    //Only initialize once
    if (numbers.isEmpty())
    {
        numbers[0] = "zero";
        numbers[1] = "One";
        numbers[2] = "Two";
        numbers[3] = "Three";
        numbers[4] = "Four";
        numbers[5] = "Five";
        numbers[6] = "Six";
        numbers[7] = "Seven";
        numbers[8] = "Eight";
        numbers[9] = "Nine";
        numbers[10] = "Ten";
        numbers[11] = "Eleven";
        numbers[12] = "Twelve";
        numbers[13] = "Thirteen";
        numbers[14] = "Fourteen";
        numbers[15] = "Fifteen";
        numbers[16] = "Sixteen";
        numbers[17] = "Seventeen";
        numbers[18] = "Eighteen";
        numbers[19] = "Nineteen";
        numbers[20] = "Twenty";
        numbers[30] = "Thirty";
        numbers[40] = "Forty";
        numbers[50] = "Fifty";
        numbers[60] = "Sixty";
        numbers[70] = "Seventy";
        numbers[80] = "Eighty";
        numbers[90] = "Ninety";
    }

    static QMap<uint, QString> powers;

    //Only initialize once
    if (powers.isEmpty())
    {
        powers[2] = "Hundred";
        powers[3] = "Thousand";
        powers[6] = "Million";
        powers[9] = "Billion";
    }

    QString output;

    if (number < 21)
    {
        output = numbers[number];
    }
    else if (number < 100)
    {
        output = numbers[10 * qFloor(number / 10)];
        uint remainder = number % 10;

        if (remainder > 0)
            output += "-" + numberToText(remainder);
    }
    else
    {
        uint power = 2;
        uint place = 0;
        QString powerString;

        //QMap::keys is ordered
        foreach (uint pow, powers.keys())
        {
            uint place_value = qPow(10, pow);
            uint tmp_place = qFloor(number / place_value);
            if (tmp_place < 1)
                break;

            place = tmp_place;
            power = pow;
            powerString = powers[pow];
        }

        if (power > 0)
        {
            output = numberToText(place) + " " + powerString;
            uint remainder = number % uint(qPow(10, power));

            if (remainder > 0)
                output += " " + numberToText(remainder);
        }
    }

    return output;
}

void PrinterHelper::printDocument(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget,
                                  GeneralVoucherDataObject obj,QString a,QString b,QString c,
                                  QString v1,QString v2,QString v3)
{
    QPainter painter( &printer );

    QSizeF pageSize = printer.paperRect().size(); // page size in pixels
    // Calculate the rectangle where to lay out the text
    const double tm = mmToPixels(printer, textMargins);
    //    qDebug()<<"TM= "<<tm;
    const qreal footerHeight = painter.fontMetrics().height();
    const QRectF textRect(0, 235, pageSize.width() - 2* borderMargins +10,
                          pageSize.height() -230 - (6 * borderMargins) - footerHeight);
    //    qDebug() << "textRect=" << textRect;
    //    qDebug()<< "page size  "<<pageSize;

    doc->setPageSize(textRect.size());


    const int pageCount = doc->pageCount();
    QProgressDialog dialog( QObject::tr( "Printing" ), QObject::tr( "Cancel" ), 0, pageCount, parentWidget );
    dialog.setWindowModality( Qt::ApplicationModal );

    bool firstPage = true;
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        dialog.setValue( pageIndex );
        if (dialog.wasCanceled())
            break;

        if (!firstPage)
            printer.newPage();

        //        if(pageIndex>0){
        //            qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Fahim";
        //            doc->setPageSize(secondSize.size());
        //        }
        paintPage( printer, pageIndex, pageCount, &painter, doc, textRect, footerHeight,obj ,a,b,c,v1,v2,v3);
        firstPage = false;
    }
}

void PrinterHelper::paintPage(QPrinter& printer, int pageNumber, int pageCount,
                              QPainter* painter, QTextDocument* doc,
                              const QRectF& textRect, qreal footerHeight,GeneralVoucherDataObject obj,
                              QString first,QString second,QString third,
                              QString v1,QString v2,QString v3)
{
    //   qDebug() << "Printing page" << pageNumber;
    const QSizeF pageSize = printer.paperRect().size();
    //    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins-20;
    int endx =  pageSize.width()-borderMargins;
    int quatx = (midx+endx)/2;

    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    painter->setBrush(Qt::green);

    //    QRect myQRect(pageSize.width()-10,0,10,pageSize.height());
    //    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    //    gradient.setColorAt(0, "#ff0000");
    //    gradient.setColorAt(0.5, "#ffcc00");
    //    gradient.setColorAt(1, "#ff0000");
    //    painter->fillRect(myQRect, gradient);

    const double bm = mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    // painter->drawRect(borderRect);

    //painter->drawImage(borderMargins,10,QImage(":/images/rrmzlogo.png"));




    //    painter->drawImage(borderMargins+100,10,QImage(":/images/thirty.png"));



    QString l1= LoginValues::getCompany().POBox.length()>0?"PO. Box: "+LoginValues::getCompany().POBox:"";
    QString l2= LoginValues::getCompany().branch+", "+LoginValues::getCompany().addressLine;
    QString l3= LoginValues::getCompany().PhoneNumber.length()>0?"T: "+LoginValues::getCompany().PhoneNumber:"";
    QString l4= LoginValues::getCompany().Fax.length()>0?"F: "+LoginValues::getCompany().Fax:"";
    QString l5= LoginValues::getCompany().email.length()>0?"E: "+LoginValues::getCompany().email:"";


    QFont CompanyFont("Calibri",16);CompanyFont.setBold(true);
    painter->setFont(CompanyFont);
    painter->drawText(borderMargins,70,LoginValues::getCompany().CompanyName);


    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);
    painter->drawText(borderMargins,90,l2);

    //    painter->drawText(borderMargins+625,25,l1);
    //    painter->drawText(borderMargins+625,40,l2);
    painter->drawText(borderMargins+625,55,l3);
    painter->drawText(borderMargins+625,70,l4);
    painter->drawText(borderMargins+625,85,l5);

    //    painter->fillRect(borderMargins+1,105,tLength,3,QColor("#ff0000"));

    painter->translate(borderMargins,120);
    QFont ReportNameFont("Verdana",16);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));

    painter->fillRect(1,20,10,25,QColor("#ed1a24"));

    painter->setPen(penHText);
    painter->drawText(20,40,obj.voucherType.toUpper());
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);

    QRect *trect = new QRect;

    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);


    painter->save();

    painter->resetTransform();
    painter->translate(borderMargins-5,90);



    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(), doc->pageSize().width()+10,
                              doc->pageSize().height());

    qDebug()<<"textPageRect"<<textPageRect;
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textPageRect.left(), textRect.top());
    //painter->drawRect(textPageRect);
    doc->drawContents(painter);
    painter->restore();
    painter->resetTransform();
    // Footer: page number or "end"

    QString PhoneSymbol ="\u260F";
    QString emailSymbol = "\u2709";
    QString truckSymbol = "\u26DF";
    int yy=0;
    //left side banner


    painter->translate(borderMargins,190);
    QPen linePen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));

    painter->setPen(linePen);

    painter->drawRect(0,0,midx-10,140);
    painter->setPen(normalPen);
    painter->fillRect(1,1,midx-11,21,QColor("#ed1a24"));

    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(5,17,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    // LedgerNameFont.setBold(true);
    painter->setFont(LedgerNameFont);
    yy=40;

    QString ledName = obj.ledgerObject.LedgerName.toUpper();
    QString address = obj.ledgerObject.Address.toUpper();
    QString pobox= obj.ledgerObject.PoBox.toUpper();
    QString city = obj.ledgerObject.City.toUpper();
    QString country = obj.ledgerObject.Country.toUpper();
    QString phoneNumber = obj.ledgerObject.phoneNumber.toUpper();
    QString email = obj.ledgerObject.emailAddress.toLower();
    QString custContact = obj.ledgerObject.ContactPersonName.toUpper();
    QString cusPhone = obj.ledgerObject.ContactPersonNumber;
    QString location = obj.Location;
    QString Trn = obj.ledgerObject.TRN;

    if(obj.voucherType.contains("Quotation")){
        //        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<obj.Contact.CompanyName;
        ledName = obj.Contact.CompanyName.toUpper();
        address = "";
        if(obj.Contact.Building.length()>0)
            address += obj.Contact.Building.toUpper() + ", ";
        if(obj.Contact.Street.length()>0)
            address += obj.Contact.Street.toUpper()+ ", ";
        address +=obj.Contact.address.toUpper();
        pobox = obj.Contact.POBox.toUpper();
        city= obj.Contact.city.toUpper();
        country = obj.Contact.country.toUpper();
        phoneNumber = obj.Contact.PhoneNumber;
        email=obj.Contact.email;
        custContact = obj.Contact.ContactName.toUpper();
        cusPhone = obj.Contact.mobileNumber;
    }

    painter->drawText(5,yy,"M/S "+ledName);
    yy+=15;
    painter->drawText(5,yy,address);
    yy+=15;
    painter->drawText(5,yy,pobox);
    yy+=15;
    painter->drawText(5,yy,city);
    yy+=15;
    painter->drawText(5,yy,"TRN : "+Trn);
    yy+=15;
    painter->drawText(5,yy,PhoneSymbol+"  "+phoneNumber);
    yy+=15;

    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(0,yy+5,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(25,yy,email.toLower());


    int widthHalf = pageSize.width()/2;

    //RIGHT SIDE BANNER

    painter->translate(midx,-75);
    painter->setPen(linePen);
    painter->drawRect(0,0,midx-10,215);

    yy=45;
    painter->drawLine(0,yy,midx-10,yy);
    yy+=45;
    painter->drawLine(0,yy,midx-10,yy);
    painter->drawLine(midx/3,0,midx/3,yy);
    painter->drawLine(midx*2/3,0,midx*2/3,yy);
    yy+=70;
    painter->drawLine(0,yy,midx-10,yy);


    yy=20;
    painter->setPen(normalPen);
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy-3,"Date");
    painter->drawText((midx/3)+5,yy-3,"Document No.");
    painter->drawText((midx*2/3)+5,yy-3,"Page");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);

    yy+=20;
    painter->drawText(5,yy,obj.VoucherDate.toString("dd-MM-yyyy"));
    painter->drawText((midx*1/3)+5,yy,obj.voucherNumber);
    painter->drawText((midx*2/3)+5,yy,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
    painter->setPen(normalPen);

    yy+=25;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy-3,first);
    painter->drawText((midx/3)+5,yy-3,second);
    painter->drawText((midx*2/3)+5,yy-3,third);
    painter ->setFont(normalFont);
    painter ->setPen(normalPen);

    yy+=20;
    painter->drawText(5,yy,v1);
    painter->drawText((midx*1/3)+5,yy,v2);
    painter->drawText((midx*2/3)+5,yy,v3);
    painter->setPen(normalPen);
    qDebug()<<"1555";
    UserProfileDataModel salesman = userHelper->getEmployeeByID(obj.SalesmanID);
    yy+=20;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Contact");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);
    yy+=16;
    painter->drawText(5,yy,salesman.Name.toUpper());
    // painter->drawText((midx*1/3)+5,yy,"TRN 123456");


    yy+=17;
    painter->drawText(5,yy,PhoneSymbol+"  "+salesman.Phone);

    yy+=17;
    painter->setFont(drawfont);
    painter->drawText(2,yy+4,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(27,yy,salesman.Email);
    painter->setPen(normalPen);
    yy+=18;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,custContact.toUpper());
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,PhoneSymbol+"   "+cusPhone);
    QFont fnt12; fnt12.setPointSize(15);
    painter->setFont(fnt12);
    //    painter->drawText(midx/2,yy,truckSymbol+"  ");
    painter->setFont(LedgerNameFont);
    painter->drawText(midx/2+15,yy,"Loc: " + location);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());




    painter->setPen(normalPen);
    painter->resetTransform();

    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    //    qDebug()<<"footerRect ="<<footerRect;
    //    if (pageNumber < pageCount - 1){
    //        painter->drawLine(borderMargins,textPageRect.bottomLeft().y(),pageSize.width()-borderMargins,textPageRect.bottomLeft().y());
    //    }
    //painter->drawText( pageSize.width() * 2/3,100,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
    //thirtysmall.png

    painter->setPen(normalPen);
    //  QString footString = "Po. Box : 9011, Abu Dhabi, UAE | Phone : +971 2 555 9619 | Fax : +971 2 554 1315 | info@alrumooz.com | www.alrumooz.com";
    QString footString = "This is a computer generated Invoice";
    painter->drawText(borderMargins,footerRect.y()+120,pageSize.width()-borderMargins*2,20, Qt::AlignCenter, footString.toUpper());
    //    QImage image(":/images/thirtymed.svg");

    int rat = 4.2;
    int mVal = -3;
    qDebug()<<"Save as PDF"<<printAsPdfG;
    if(printAsPdfG){
        rat = 15;
        mVal = 0;
    }
    //    image.setDevicePixelRatio(rat);
    //    painter->drawImage(tLength-60,footerRect.y()+120-31-mVal,image);

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y()+115,tLength-54,footerRect.y()+115);

    painter->drawEllipse(borderMargins,footerRect.y()+112,5,5);

    painter->setPen(normalPen);
    painter->setBrush(normalBrush);

    qDebug()<<"1618";

    //    painter->drawImage(tLength-10,footerRect.y()+100,QImage(":/images/thirtymed.png").scaledToWidth(80));

    //    if (pageNumber == pageCount - 1)
    //        painter->drawText(footerRect, Qt::AlignCenter, QObject::tr("Po. Box : 9011, Abu Dhabi, UAE|Phone : +971 2 555 9619|Fax : +971 2 554 1315|info@alrumooz.com|www.alrumooz.com"));
    //        //painter->drawText(footerRect, Qt::AlignCenter, QObject::tr("www.alrumooz.com"));
    //    else
    //        painter->drawText(footerRect, Qt::AlignVCenter | Qt::AlignRight, QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
}

void PrinterHelper::paintPageDeliveryNote(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter, QTextDocument *doc, const QRectF &textRect, qreal footerHeight, GeneralVoucherDataObject obj)
{
    //   qDebug()<< "Printing page" << pageNumber;
    const QSizeF pageSize = printer.paperRect().size();
    qDebug() << "pageSize=" << pageSize;

    int tLength = pageSize.width()- (2*borderMargins);
    int midx = tLength/2+borderMargins-20;
    int endx =  pageSize.width()-borderMargins;
    int quatx = (midx+endx)/2;

    QFont normalFont = painter->font();
    QBrush normalBrush = painter->brush();
    QPen normalPen = painter->pen();
    painter->setBrush(Qt::green);

    //    QRect myQRect(pageSize.width()-10,0,10,pageSize.height());
    //    QLinearGradient gradient(myQRect.topLeft(), myQRect.bottomRight()); // diagonal gradient from top-left to bottom-right
    //    gradient.setColorAt(0, "#ff0000");
    //    gradient.setColorAt(0.5, "#ffcc00");
    //    gradient.setColorAt(1, "#ff0000");
    //    painter->fillRect(myQRect, gradient);

    const double bm = mmToPixels(printer, borderMargins);
    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    // painter->drawRect(borderRect);
    painter->drawImage(borderMargins,10,QImage(":/images/rrmzlogo.png"));
    //    painter->drawImage(borderMargins+100,10,QImage(":/images/thirty.png"));



    QString l1= "PO. Box: 9011";
    QString l2= "Abu Dhabi, UAE";
    QString l3= "T: +971 2 555 9619";
    QString l4= "F: +971 2 554 1315";
    QString l5= "E: info@alrumooz.com";
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    painter->drawText(borderMargins+625,25,l1);
    painter->drawText(borderMargins+625,40,l2);
    painter->drawText(borderMargins+625,55,l3);
    painter->drawText(borderMargins+625,70,l4);
    painter->drawText(borderMargins+625,85,l5);

    //    painter->fillRect(borderMargins+1,105,tLength,3,QColor("#ff0000"));

    painter->translate(borderMargins+1,120);
    QFont ReportNameFont("Verdana",16);
    ReportNameFont.setBold(true);
    painter->setFont(ReportNameFont);
    QPen penHText(QColor("#000000"));

    painter->fillRect(1,20,10,25,QColor("#ed1a24"));

    painter->setPen(penHText);
    painter->drawText(20,40,obj.voucherType.toUpper());
    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);

    //painter->drawRect((tLength/2)+borderMargins,105,(tLength/2),200);
    //painter->drawLine(0,5,tLength,5);

    QPen whitePen(QColor("#ffffff"));

    painter->setPen(normalPen);

    QRect *trect = new QRect;

    painter->setFont(normalFont);
    painter->setBrush(normalBrush);
    painter->setPen(normalPen);


    painter->save();

    painter->resetTransform();
    painter->translate(borderMargins,90);



    // textPageRect is the rectangle in the coordinate system of the QTextDocument, in pixels,
    // and starting at (0,0) for the first page. Second page is at y=doc->pageSize().height().
    const QRectF textPageRect(0, pageNumber * doc->pageSize().height(), doc->pageSize().width(),
                              doc->pageSize().height());

    qDebug()<<"textPageRect"<<textPageRect;
    // Clip the drawing so that the text of the other pages doesn't appear in the margins
    painter->setClipRect(textRect);
    // Translate so that 0,0 is now the page corner
    painter->translate(0, -textPageRect.top());
    // Translate so that 0,0 is the text rect corner
    painter->translate(textPageRect.left(), textRect.top());
    //painter->drawRect(textPageRect);
    doc->drawContents(painter);
    painter->restore();
    painter->resetTransform();
    // Footer: page number or "end"

    QString PhoneSymbol ="\u260F";
    QString emailSymbol = "\u2709";
    int yy=0;
    //left side banner


    painter->translate(borderMargins+1,190);
    QPen linePen(QColor("#bbbbbb"));
    QPen bluePen(QColor("#0000ff"));
    painter->setPen(linePen);

    painter->drawRect(0,0,midx-10,140);
    painter->setPen(normalPen);
    painter->fillRect(1,1,midx-11,21,QColor("#ed1a24"));

    QFont HeadingFont("Calibri",10);
    HeadingFont.setBold(true);
    painter ->setFont(HeadingFont);
    painter ->setPen(whitePen);
    painter->drawText(17,17,"Customer Details :");
    painter->setPen(normalPen);
    QFont LedgerNameFont("Calibri",9);
    // LedgerNameFont.setBold(true);
    painter->setFont(LedgerNameFont);
    yy=40;
    painter->drawText(20,yy,obj.ledgerObject.LedgerName);
    yy+=15;
    painter->drawText(20,yy,obj.ledgerObject.Address);
    yy+=15;
    painter->drawText(20,yy,obj.ledgerObject.PoBox);
    yy+=15;
    painter->drawText(20,yy,obj.ledgerObject.City);
    yy+=15;
    painter->drawText(20,yy,obj.ledgerObject.Country);
    yy+=15;
    painter->drawText(20,yy,PhoneSymbol+"  "+obj.ledgerObject.phoneNumber);
    yy+=15;
    QFont drawfont("Calibri",16);
    painter->setFont(drawfont);
    painter->drawText(15,yy+5,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(40,yy,obj.ledgerObject.emailAddress.toLower());


    int widthHalf = pageSize.width()/2;

    //RIGHT SIDE BANNER

    painter->translate(midx,-75);
    painter->setPen(linePen);
    painter->drawRect(0,0,midx-10,215);

    yy=45;
    painter->drawLine(0,yy,midx-10,yy);
    yy+=45;
    painter->drawLine(0,yy,midx-10,yy);
    painter->drawLine(midx/3,0,midx/3,yy);
    painter->drawLine(midx*2/3,0,midx*2/3,yy);
    yy+=70;
    painter->drawLine(0,yy,midx-10,yy);


    yy=20;
    painter->setPen(normalPen);
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy-3,"Date");
    painter->drawText((midx/3)+5,yy-3,"Document No.");
    painter->drawText((midx*2/3)+5,yy-3,"Page");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);

    yy+=20;
    painter->drawText(5,yy,obj.VoucherDate.toString("dd-MM-yyyy"));
    painter->drawText((midx*1/3)+5,yy,obj.voucherNumber);
    painter->drawText((midx*2/3)+5,yy,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));
    painter->setPen(normalPen);

    yy+=25;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy-3,"Prepared By");
    painter->drawText((midx/3)+5,yy-3,"Currency.");
    painter->drawText((midx*2/3)+5,yy-3,"Reference");
    painter ->setFont(normalFont);
    painter ->setPen(normalPen);

    yy+=20;
    painter->drawText(5,yy,toCamelCase( obj.AddedBy.toUpper()));
    painter->drawText((midx*1/3)+5,yy,"Aed");
    painter->drawText((midx*2/3)+5,yy,"By Mail");
    painter->setPen(normalPen);

    yy+=20;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Our Contact");
    painter->setFont(LedgerNameFont);
    painter ->setPen(normalPen);
    yy+=16;
    painter->drawText(5,yy,"Salesman");

    yy+=17;
    painter->drawText(5,yy,PhoneSymbol+"  "+"0555916906");

    yy+=17;
    painter->setFont(drawfont);
    painter->drawText(2,yy+4,emailSymbol);
    painter->setFont(LedgerNameFont);
    painter->setPen(bluePen);
    painter->drawText(27,yy,"shabib@gmail.com");
    painter->setPen(normalPen);
    yy+=18;
    painter ->setFont(HeadingFont);
    painter->drawText(5,yy,"Customer Contact");
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,obj.ledgerObject.ContactPersonName);
    yy+=17;
    painter->setFont(LedgerNameFont);
    painter->drawText(5,yy,PhoneSymbol+"   "+obj.ledgerObject.phoneNumber);

    //painter->drawText(5,yy,obj.ledgerObject.emailAddress.toLower());




    painter->setPen(normalPen);
    painter->resetTransform();

    QRectF footerRect = textRect;
    footerRect.setTop(textRect.bottom());
    footerRect.setHeight(footerHeight);
    qDebug()<<"footerRect ="<<footerRect;

    //    if (pageNumber < pageCount - 1){
    //        painter->drawLine(borderMargins,textPageRect.bottomLeft().y(),pageSize.width()-borderMargins,textPageRect.bottomLeft().y());
    //    }
    //painter->drawText( pageSize.width() * 2/3,100,QObject::tr("Page %1/%2").arg(pageNumber+1).arg(pageCount));

    painter->setPen(QPen("#ed1a24"));
    //    painter->drawLine(borderMargins,footerRect.y()+120,tLength-10,footerRect.y()+120);
    //thirtysmall.png

    painter->setPen(normalPen);
    QString footString = "Po. Box : 9011, Abu Dhabi, UAE | Phone : +971 2 555 9619 | Fax : +971 2 554 1315 | info@alrumooz.com | www.alrumooz.com";
    //    painter->drawText(borderMargins,footerRect.y()+120,pageSize.width()-borderMargins*2,20, Qt::AlignLeft, footString.toUpper());
    // QImage image(":/images/thirtymed.svg");

    int rat = 4.2;
    int mVal = -3;

    qDebug()<<"Save as PDF"<<printAsPdfG;
    if(printAsPdfG){
        rat = 15;
        mVal = 0;
    }
    // image.setDevicePixelRatio(rat);
    //painter->drawImage(tLength-60,footerRect.y()+120-31-mVal,image);

    painter->setPen(QPen("#ed1a24"));
    painter->setBrush(QBrush("#ed1a24"));

    painter->drawLine(borderMargins,footerRect.y()+115,tLength-54,footerRect.y()+115);

    painter->drawEllipse(borderMargins,footerRect.y()+112,5,5);

}

void PrinterHelper::printWorkOrderDrafts(CompoundItemDataObject item,DocumentsDataModel imageData)
{

    //    QPrinter printer(QPrinter::HighResolution) ;
    //    printer.setPageSize(QPrinter::A4);


    //    QPrintDialog pDial(&printer,this);
    //    if(pDial.exec()==QDialog::Rejected) return  ;


    //    Poppler::Document *doc = Poppler::Document::loadFromData(imageData.fileObj);
    //    int numpage = doc->numPages();

    //    qDebug()<<numpage;

    //    int scaleFactor = 6;

    //    QImage image = doc->page(0)->renderToImage(
    //                scaleFactor * physicalDpiX(),
    //                scaleFactor * physicalDpiY());


    //    QPainter painter( &printer );
    //    painter.drawImage(-30,0,image);

    //    painter.end();



}

void PrinterHelper::testBarcode()
{
    qDebug()<<"testing barcode";;
    QPageSize pageSize(QSizeF(60.0,40.0),QPageSize::Millimeter,"",QPageSize::ExactMatch);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(pageSize);
    printer.setFullPage(true);
    printer.setNumCopies(3);
    printer.setOutputFormat(QPrinter::NativeFormat);

    QPainter painter(&printer);
    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,40,QFont::Normal);
    QTextDocument doc;

    //    QFont barCodeFont("Code 128", 40, QFont::Normal);
    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.5);

    QString str = encodeBarcode("123456789");
    //        painter->drawText(300,600,str);


    int w = printer.pageRect().width();
    int h = printer.pageRect().height();

    qDebug()<<"width :"<<w<<"height"<<h;
    double width = w; mmToPixels(printer,w);
    double height = h;mmToPixels(printer,h);;

    double borderMargin = 3;

    double lineLength = mmToPixels(printer,10);

    double lineLength7 = mmToPixels(printer,7);
    qDebug()<<"Line lengths : "<<lineLength<<lineLength7;
    double yLine1=borderMargin +lineLength;
    double yLine2=borderMargin +2*lineLength;

    double yLine3=yLine2+lineLength7;

    double yLine4=yLine2+lineLength7;


    painter.drawText(borderMargin,yLine3-10,width-2*borderMargin,lineLength7,Qt::AlignLeft,"Boy's T-Shirt" );
    painter.drawText(width/2,yLine3+5,width/2-borderMargin,lineLength7,Qt::AlignCenter,"Rs. 1088.00" );

    QFont companyNameFont("Helvetica [Cronyx]",12);

    painter.setFont(companyNameFont);
    painter.drawText(borderMargin,borderMargin+30,width-2*borderMargin,lineLength,Qt::AlignHCenter,"KIDDO" );

    painter.setFont(barCodeFont);

    painter.drawText(borderMargin,yLine1,width-2*borderMargin,lineLength,Qt::AlignHCenter,str);

    doc.drawContents(&painter);

    doc.print(&printer);

    qDebug()<<"Printer";
}

QString PrinterHelper::formatNumber(float numberf)
{
    QString number = QString::number(numberf,'f',2);
    int i = number.lastIndexOf('.');
    if(i >0)
    {
        i -= 3;
        while(i > 0)
        {
            number.insert(i, ',');
            i -= 3;
        }
    }
    return number;
}

QString PrinterHelper::encodeBarcode(QString code)
{
    QString encoded;

    encoded.prepend(QChar(codeToChar(CODE128_B_START))); //Start set with B Code 104
    encoded.append(code);
    encoded.append(QChar(calculateCheckCharacter(code)));
    encoded.append(QChar(codeToChar(CODE128_STOP))); //End set with Stop Code 106

    return encoded;
}

int PrinterHelper::codeToChar(int code)
{
    return code + 105;
}

int PrinterHelper::calculateCheckCharacter(QString code)
{
    QByteArray encapBarcode(code.toUtf8()); //Convert code to utf8

    //Calculate check character
    long long sum = CODE128_B_START; //The sum starts with the B Code start character value
    int weight = 1; //Initial weight is 1

    foreach(char ch, encapBarcode) {
        int code_char = charToCode((int)ch); //Calculate character code
        sum += code_char*weight; //add weighted code to sum
        weight++; //increment weight
    }

    int remain = sum%103; //The check character is the modulo 103 of the sum

    //Calculate the font integer from the code integer
    if(remain >= 95)
        remain += 105;
    else
        remain += 32;

    return remain;
}

int PrinterHelper::charToCode(int ch)
{
    return ch - 32;
}

int PrinterHelper::printWorkOrder(GeneralVoucherDataObject data)
{


    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPageSize(QPrinter::A4);

    QPrintDialog pDial(&printer,this);
    if(pDial.exec()==QDialog::Rejected)
        return 0 ;

    //    if(printAsPdf!=true){
    //        QPrintDialog pDial(&printer,this);
    //        if(pDial.exec()==QDialog::Rejected)
    //            return 0 ;
    //    }
    //    else{
    //        QString dir = QDir::home().path();
    //        QString fileName = dir+"/QN_"+data.voucherNumber+" "+data.Contact.CompanyName+".pdf";
    //        printer.setOutputFormat(QPrinter::PdfFormat);

    //        fileName = QFileDialog::getSaveFileName(this,"File Name",fileName,tr("pdf"));
    //        printer.setOutputFileName(fileName);
    //    }
    printer.setPageSize(QPrinter::A4);
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QString blankSpace="&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    QTextDocument doc;
    QTextCursor cursor(&doc);

    QString borderWidth = "border-width=\"1px\"";
    //  QString totals = QString::number(total, 'f', 2);
    //"<center>"+text2+"&trade;</center>"
    QString headhtml = "<HTML>"
                       "<HEAD>";
    QString cssTable;
    cssTable = "<style type=\"text/css\">";
    cssTable += "table.tb1 { border-color: }";

    cssTable+="</style></head>";

    QString companyName = "<font size =\"12\">Al Rumooz</font>";
    QString VoucherName = "<font size =\"8\" color=\"green\"><i><u>"+data.voucherType+"</u></i></font>";
    QString bodyTable ="";

    QString css;
    css = "<style type=\"text/css\">";
    css += "table.tblb {border-width: 0px;border-style: solid;border-color: gray;margin-top: 20px;margin-bottom: 0px;color: black;}";
    css += "table.tblb td {padding: 0px;padding-left:5px;padding-right:15px}";
    css += "table.tblb th {background-color:#0000f1;padding-left:5px}";

    css += "table.tblf {border-width: 0px;border-style: solid;border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;border-radius: 4px;}";
    css += "table.tblf td {padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";
    css += "table.tblf th {background-color:#ff0000;padding-left:5px;padding-top:5px;padding-right:5px;padding-bottom:5px}";

    css += "table.tbl {border-width: 0px;border-style: solid;border-color:#ed1a24;border-collapse: collapse;"
           "margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbl td {padding-right:3px; font-size:12px;padding-top:2px;}";
    css += "table.tbl th {background-color:#ed1a24;padding-bottom:4px; padding-left:0px;padding-right:3px; border-color:#ed1a24;font-color:#ffffff;}";

    css += "table.tbldet {background-color:#fefefe; border-width: 0px; border-style: solid; "
           "border-color: gray;margin-top: 0px;margin-bottom: 0px;color: black;}";
    css += "table.tbldet td {padding-right:0px;padding-top: 3px;font-size:12px;padding-left: 4px;}";
    css += "table.tbldet th {background-color:#ececec;padding-left:5px}";

    css += "pre.cusfont {font-family: Arial; font-size:12px}";
    css += "span.right {text-align:right;}";
    css += "span.left {text-align:left;}";
    css+="</style></head>";

    QString listTableRowSpan = "colspan=\"5\"";
    //    bodyTable +=  "<table class=\"tblb\" width =\"100%\">";
    //    bodyTable +=  "<tr><td width=\"66%\"><img src=:/images/rmzlogo200w.png></td><td align=\"right\">"+
    //            VoucherName+"</td></tr>"
    //                        "";
    QString VoucherDetails = "";//<tr><td "+listTableRowSpan+">";
    VoucherDetails +="<table class=\"tbl\" width =\"100%\">";

    QString CustomerData = "<tr><td width = \"50%\"><font face=\"calibri\" size=\"3\">";
    //    <tr><td>Customer Details:<br>"
    //                           "<font size =\"5\"><b> "+data.ledgerObject.LedgerName+"</b></font>";

    CustomerData += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+data.ledgerObject.Address;
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ";
    CustomerData += "<br><br><br><b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Contact Person :</b>";
    CustomerData += "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
            +data.ledgerObject.ContactPersonName;
    CustomerData += "</font></td>";


    QString QuoteDetails = "<td><font face=\"calibri\" size =\"3\">";
    QuoteDetails += "<table class=\"tbldet\" width =\"100%\">";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Prepared By</b></td><td>&nbsp;&nbsp;<b>Currency</b></td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ADMIN</td><td>&nbsp;&nbsp;AED</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Rumooz Contact</b></td><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;Salesman</td><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;</td></tr>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;<b>Reference</b></td>";
    QuoteDetails += "<tr><td>&nbsp;&nbsp;ORDER BY MAIL</td></tr>";

    //    QuoteDetails += "<tr><td>Validity</td><td>30 days";

    QuoteDetails += ""
                    "</td></tr></table></font></td></tr>";
    VoucherDetails +=CustomerData+QuoteDetails + "</table>";

    QString SubjectRow ="";

    QString itemFontSize ="3";
    QString itemHeadFontSize ="4";
    QString CoverLetter ="<tr><td "+listTableRowSpan+">";
    CoverLetter += "<pre class =\"cusfont\">Hello Sir,\n"
                   "    Please find following quotaion";
    CoverLetter += "</pre></td></tr>";

    QString html = "<tr><td "+
            listTableRowSpan
            +">";
    //    html = "<table width=\"100%\" <tr><td>hello</td><tr></table>";
    html =
            "<br><font face =\"calibri\"><table class=\"tbl\" width =\"100%\" cellspacing=\"0\" cellpadding=\"0\">"
            "<tr>"
            "<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Sl</font></th>"
                              "<th width=\"45%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Item</font></th>";
    //    html+="<th width=\"5%\" align=\"left\"><font color=\"#ffffff\" size =\""
    //            +itemHeadFontSize+"\">Uom</font></th>";
    html+="<th width=\"20%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Model</font></th>"
                              "<th width=\"20%\" align=\"left\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Brand</font></th>"
                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            +itemHeadFontSize+"\">Qty</font></th>"
            //                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            //            +itemHeadFontSize+"\">Price</font></th>"
            //                              "<th width=\"10%\" align=\"right\"><font color=\"#ffffff\" size =\""
            //            +itemHeadFontSize+"\">Total</h2></th>"
            "</tr>";
    html +="<tr></tr>";
    int lineCount =0;
    for(int i=0;i<data.InventoryItems.size();i++){
        html += "\n<tr>";
        html += "<td align =\"left\"> <font> ";
        html += QString::number(i+1);
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.ItemName.toUpper();
        html += "<pre>";
        html += data.InventoryItems[i].BaseItem.ItemDescription;
        html += "</pre>";
        if(data.InventoryItems[i].BaseItem.isCustomItem && data.InventoryItems[i].BaseItem.Dimension.length()>0)
            html += "Dimension :"+QString::number(data.InventoryItems[i].BaseItem.length) + "x"+ data.InventoryItems[i].BaseItem.Dimension;
        html += "</td>";

        lineCount += data.InventoryItems[i].BaseItem.ItemDescription.split("\n").length();

        //        html += "<td align =\"left\"> <font >";
        //        html += data.InventoryItems[i].BaseItem.UOMName.toUpper();
        //        qDebug()<<"UOM Name is "<<data.InventoryItems[i].BaseItem.UOMName.toUpper();
        //        qDebug()<<"item Codeis "<<data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        //        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.ItemCode.toUpper();
        html += "</td>";

        html += "<td align =\"left\"> <font >";
        html += data.InventoryItems[i].BaseItem.brandName.toUpper();
        html += "</font></td>";

        html += "<td align =\"right\"> <font >";
        html += QString::number(data.InventoryItems[i].BaseItem.quantity);
        html += "</td>";

        //        html += "<td align =\"right\"> <font >";
        //        html += formatNumber(data.InventoryItems[i].BaseItem.price);
        //        html += "</td>";

        //        html += "<td align =\"right\"> <font >";
        //        if(data.InventoryItems[i].BaseItem.subTotal==0 && data.InventoryItems[i].BaseItem.price !=0){
        //            html += "(Optional)";
        //        }
        //        else{
        //            html += formatNumber(data.InventoryItems[i].BaseItem.subTotal);
        //        }
        //        html += "</td>";

        html+= "</tr>";
    }
    int listSize = 4;

    if(data.InventoryItems.size()+lineCount<20)
        for(int i=data.InventoryItems.size()+lineCount;i<20;i++){
            html+="<tr><td></td></tr>";
            //html+="<tr><td ><font size =\""+itemFontSize+"\">"+QString::number(i)+"</font></td></tr>";
        }
    qDebug()<<"659"<<data.discount;



    QString lineBlock = "";
    lineBlock += "<tr><td "+listTableRowSpan+">";
    lineBlock += "<hr color=\"#000000\"></td></tr>";


    //  html = headhtml+css+bodyTable+VoucherDetails + html+TotalString+amountInWords+lineBlock;
    html = headhtml+css+html+lineBlock;
    //+SignatureBlock;

    //    html = css+html;
    //    html = headhtml + html + TotalString+amountInWords+lineBlock+SignatureBlock;
    //    html+=  "</td></tr></table></body></html>";
    //    qDebug()<<"Tr count = "<<html.count("<tr");
    //    qDebug()<<"Tr +br count = "<<html.count("<tr")+html.count("<br>");

    qDebug()<<html;
    doc.setDefaultFont(font);
    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //   doc.setPageSize(paperSize);
    //   doc.setPageSize(QPrinter::A4);
    cursor.insertHtml(html);

    //    QPainter painter1( &printer );
    //    QPainter *painter = &painter1;
    //    const QSizeF pageSize = printer.paperRect().size();
    //    const double bm = mmToPixels(printer, borderMargins);
    //    const QRectF borderRect(bm, bm, pageSize.width() - 2 * bm, pageSize.height() - 2 * bm);
    //    painter->drawRect(borderRect);

    //    cursor.setPosition(1000,QTextCursor::MoveAnchor);
    //    cursor.insertText("Text Here");
    //    qDebug()<<cursor.position();
    //    cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,5);
    //    qDebug()<<cursor.position();
    //    cursor.insertText("Text2 Here2");

    QTextBlockFormat blockFormat;
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);
    //    cursor.setPosition(cursor.position()+100);
    //    if(printExtra){
    //        completedDescription = 0;
    //        while(completedDescription>-1){
    //            cursor.insertBlock(blockFormat);
    //            cursor.insertHtml(getDescription(data,0));}

    //    }
    // cursor.insertHtml(getTermsinHtml(validity,subject,payment,warranty,extra,delivery,exclusion));

    QString a ="Prepared By";
    QString b = "Currency";
    QString c = "Reference";
    QString v1;QString v2;QString v3;
    v1 = data.AddedBy;
    v2 = "Aed";
    //    v3 = subject;

    printDocument(printer,&doc,0,data,a,b,c,v1,v2,v3);


    return 0;

}
