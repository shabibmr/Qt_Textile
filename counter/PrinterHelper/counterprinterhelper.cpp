#include "counterprinterhelper.h"

#include <QPainter>
#include <QFileDialog>
#include <database/finance/Closing/restaurantshiftdatabasehelper.h>
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include <QSqlRecord>
#include <QTableView>
CounterPrinterHelper::CounterPrinterHelper(QWidget *parent) : QWidget(parent)
{
    userHelper = new UserProfileDatabaseHelper();
    configHelper = new ConfigurationSettingsDatabaseHelper();
    this->setPrinter(new QPrinter(QPrinter::ScreenResolution));
    multiVoucher = ConfigurationSettingsDatabaseHelper::getValue(multiVoucherFlag, true).toBool();
    printAddressInKotFlag = ConfigurationSettingsDatabaseHelper::getValue(printAddressInKot,1).toBool();
    add1Rout2 = ConfigurationSettingsDatabaseHelper::getValue(address1Route2,true).toBool();
    noDefaultPrint = ConfigurationSettingsDatabaseHelper::getValue(noDefaultKotPrint,false).toBool();
    kotAddrSize = ConfigurationSettingsDatabaseHelper::getValue(kotAddrPrintSize,"5").toString();
    billAddrSize = ConfigurationSettingsDatabaseHelper::getValue(billAddrPrintSize,"6").toString();
    kotNarration = ConfigurationSettingsDatabaseHelper::getValue(kotNarrationTitle,"Narration").toString();
    taxNameStr = ConfigurationSettingsDatabaseHelper::getValue(taxName,"GST IN").toString();
    printVPref = ConfigurationSettingsDatabaseHelper::getValue(printVoucherPref,false).toBool();
    printBarcodeInBillFlag =  ConfigurationSettingsDatabaseHelper::getValue(printBarcodeInBill,false).toBool();
}

QString CounterPrinterHelper::getHTMLMeasurement(GeneralVoucherDataObject *voucher)
{
    ContactMeasurement *measure = voucher->measurement;
    QString html = "<html><body>";

    html += "<br>";
    html += "<br>";

    html += "<font size =\"5\">";
    html += "<table width = \"100%\">";

    html += "<tr>";
    html += "<td width = \"50%\">Bill Number : </td>";
    html += "<td width = \"50%\">"+voucher->voucherNumber+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Bill Date : </td>";
    html += "<td width = \"50%\">"+voucher->VoucherDate.toString("dd-MM-yyyy")+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Contact : </td>";
    html += "<td width = \"50%\">"+voucher->Contact.ContactName+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Phone : </td>";
    html += "<td width = \"50%\">"+voucher->Contact.PhoneNumber+"</td>";
    html += "</tr>";


    html += "<tr>";
    html += "<td width = \"50%\">Reference : </td>";
    html += "<td width = \"50%\">"+voucher->measurement->reference+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Measurement Of</td>";
    html += "<td width = \"50%\">"+voucher->measurement->measurement->measurementName+"</td>";
    html += "</tr>";

    html += "</table>";
    html += "<br>";
    html += "<hr>";
    html += "<br>";
    html += "<table width = \"100%\">";
    html += "<tr>";
    html += "<th width = \"50%\">Label</th>";
    html += "<th width = \"50%\">Value</th>";
    html += "</tr>";

    html += "</table>";
    html += "<br>";
    html += "<hr>";
    html += "<br>";
    html += "<table width = \"100%\">";


    for(int i=0;i<measure->lables.length();i++){
        html += "<tr>";
        html += "<td>"+measure->lables[i].lable.labelName+"</td>";
        html += "<td>"+measure->lables[i].value+"</td>";
        html += "</tr>";
    }

    html += "</table>";
    html += "<br>";
    html += "<br>";
    html += "<br>";
    html += "<br>";

    html += "</body></html>";

    qDebug()<<"_____";
    qDebug()<<html;

    qDebug()<<"_____";
    return html;
}

QString CounterPrinterHelper::getHTMLMeasurementContact(ContactMeasurement *measurement,AddressBookDataModel contact)
{
    QString html = "<html><body>";

    html += "<br>";
    html += "<br>";

    html += "<font size =\"5\">";
    html += "<table width = \"100%\">";



    html += "<tr>";
    html += "<td width = \"50%\">Contact : </td>";
    html += "<td width = \"50%\">"+contact.ContactName+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Phone : </td>";
    html += "<td width = \"50%\">"+contact.PhoneNumber+"</td>";
    html += "</tr>";


    html += "<tr>";
    html += "<td width = \"50%\">Reference : </td>";
    html += "<td width = \"50%\">"+measurement->reference+"</td>";
    html += "</tr>";

    html += "<tr>";
    html += "<td width = \"50%\">Measurement Of</td>";
    html += "<td width = \"50%\">"+measurement->measurement->measurementName+"</td>";
    html += "</tr>";

    html += "</table>";
    html += "<br>";
    html += "<hr>";
    html += "<br>";
    html += "<table width = \"100%\">";
    html += "<tr>";
    html += "<th width = \"50%\">Label</th>";
    html += "<th width = \"50%\">Value</th>";
    html += "</tr>";

    html += "</table>";
    html += "<br>";
    html += "<hr>";
    html += "<br>";
    html += "<table width = \"100%\">";


    for(int i=0;i<measurement->lables.length();i++){
        html += "<tr>";
        html += "<td>"+measurement->lables[i].lable.labelName+"</td>";
        html += "<td>"+measurement->lables[i].value+"</td>";
        html += "</tr>";
    }

    html += "</table>";
    html += "<br>";
    html += "<br>";
    html += "<br>";
    html += "<br>";

    html += "</body></html>";

    qDebug()<<"_____";
    qDebug()<<html;

    qDebug()<<"_____";
    return html;
}

int CounterPrinterHelper::printMeasurement(GeneralVoucherDataObject *voucher)
{
    if(voucher->measurement == nullptr){
        qDebug()<<"No Measurement to PRONT";
        return -1;
    }
    int flag =1;
    QTextDocument doc ;

    QString html = getHTMLMeasurement(voucher);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution);

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    //    printer.setPageSize(QPrinter::A4);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    doc.print(&printer);

    return flag;

}

int CounterPrinterHelper::printMeasurementContact(ContactMeasurement *measurement,AddressBookDataModel contact)
{
    int flag =1;
    QTextDocument doc ;

    QString html = getHTMLMeasurementContact(measurement,contact);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution);

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    //    printer.setPageSize(QPrinter::A4);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    doc.print(&printer);

    return flag;
}


void CounterPrinterHelper::setPrinter(QPrinter *value)
{
    printer = value;
}

void CounterPrinterHelper::getBillasTextDocument(QTextDocument *doc,GeneralVoucherDataObject obj,QString printerName)
{
    //     qDebug()<<" doc 10";
    //     qDebug()<<" doc 12";
    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPrinterName(printerName);
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    //     qDebug()<<" doc 17";
    QTextCursor cursor(doc);

    QString CompanyName;
    QByteArray companyLogo;
    QString imageType;
    QString branch;
    QString addressLine;
    QString trnNo;
    QString phoneNumber;
    QString cashierName;
    QString POSNumber;
    QImage imageLogo;
    QPixmap imLogo;
    //     qDebug()<<" doc 31";
    //    if(companyLogo.size()>0){
    //        imageLogo = QImage::fromData(companyLogo);
    //        imLogo = QPixmap(companyLogo);

    //    }
    //    QTextImageFormat imageFormat;


    //    cursor.insertImage(imageLogo);
    QString html = "<html><body>";
    html+="test";

    cursor.insertHtml(html);

    doc->setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc->setPageSize(paperSize);

    //     qDebug()<<"Returning doc";

}

QString CounterPrinterHelper::getHtml(GeneralVoucherDataObject *obj, QString printerName)
{
    QString html;
    //     qDebug()<<"Start html ";
    QString CompanyName = LoginValues::getCompany().CompanyName;
    QByteArray companyLogo = LoginValues::getCompany().logo;
    QString imageType;
    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
    QString POSNumber=CounterSettingsDataModel::POSNumber;
    QString fssai = LoginValues::getCompany().fssaiLicenseNo;

    QString vType = "";
    if(obj->voucherType.contains("SalesOrder",Qt::CaseInsensitive)){
        vType = "Ord ";
    }
    else{
        vType = "Bill";
    }

    html += "<html><body>";


    html += "<table width =\"100%\"><tr>";
    if(companyLogo.size()>0){
        // QPixmap pixmap(companyLogo);
        // QBuffer buffer(&companyLogo);
        // pixmap.save(&buffer, "PNG");
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
        html += "<tr><td><center><font size =\"4\">TRN : "+trnNo+"</font></center>";

    if(fssai.length()>0)
        html += "<tr><td><center><font size =\"4\"><i>fssai : "+fssai+"</font></center>";

    if(trnNo.length()>0)
        html += "<tr><td><center><font size =\"4\"><u>TAX INVOICE/الفاتورة الضريبية</u></font></center>";
    else
        html += "<tr><td><center><font size =\"4\"><u>INVOICE/الفاتورة</u></font></center>";

    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";

    html += "<table width =\"100%\">";
    html += "<tr><td width=\"15%\">"+vType+" : </td>";
    html += "<td width=\"35%\"><b>"+(multiVoucher==true?obj->VoucherPrefix+" - ":"")+obj->voucherNumber+"</td>";

    html += "<td width=\"15%\">POS # :</td>";
    html += "<td width=\"35%\">"+POSNumber+"</td></tr>";

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

    bool showArabicTitle=true;

    int colCount;
    if(ConfigurationSettingsDatabaseHelper::getValue(printFormat, "1").toInt() == 2)
        colCount = 5;
    else
        colCount = 5;
    //    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";
    if(obj->ledgerObject.LedgerGroupId == "0x5x19" && obj->ledgerObject.LedgerID!="0x5x19A135"){
        if(obj->ledgerObject.LedgerName.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Name : "+obj->ledgerObject.LedgerName+"</td></tr>";
        }
        if(obj->ledgerObject.TRN.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">TRN : "+obj->ledgerObject.LedgerName+"</td></tr>";
        }
        if(obj->ledgerObject.mobileNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Mobile : "+obj->ledgerObject.mobileNumber+"</td></tr>";
        }
        if(obj->ledgerObject.phoneNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Phone : "+obj->ledgerObject.phoneNumber+"</td></tr>";
        }
        if(obj->ledgerObject.ContactPersonNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Contact : "+obj->ledgerObject.ContactPersonNumber+"</td></tr>";
        }
        if(obj->ledgerObject.Address.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Address : "+obj->ledgerObject.Address+"</td></tr>";
        }
        if(obj->ledgerObject.PoBox.length()>0){
            html += "<tr><td colspan =\"4\" align=\"right\"><font size=6>Code : "+obj->ledgerObject.PoBox+"</td></tr>";
        }
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";
    }
    html += "<tr>";
    html += "<th align=\"left\" width =\"5%\">#<\th>";
    html += "<th align=\"left\" width =\"40%\">Item Name";
    if(showArabicTitle){
        html += "<br>المعلومات";
    }
    html += "<\th>";

    int formatVal = ConfigurationSettingsDatabaseHelper::getValue(printFormat, "1").toInt();
    bool amtWithTaxFlag = ConfigurationSettingsDatabaseHelper::getValue(printBillLineAmountWithTax,false).toBool();    
    bool priceWithTaxFlag = ConfigurationSettingsDatabaseHelper::getValue(printBillLinePriceWithTax,false).toBool();


    if(formatVal == 1)
    {

        html += "<th align=\"right\" width =\"15%\">Price";
        if(showArabicTitle){
            html += "<br>سعر";
        }
        html += "<\th>";
        html += "<th align=\"center\"  width =\"20%\">Qty";
        if(showArabicTitle){
            html += "<br>كمية";
        }
        html += "<\th>";
    }
    else if(formatVal == 3)
    {
        html += "<th align=\"center\"  width =\"20%\">Qty";
        if(showArabicTitle){
            html += "<br>كمية";
        }
        html += "<\th>";
        html += "<th align=\"right\" width =\"15%\">Price";
        if(showArabicTitle){
            html += "<br>سعر";
        }
        html += "<\th>";

    }
    else {
        html += "<th align=\"left\" width =\"5%\"><\th>";
        html += "<th align=\"center\"  width =\"20%\">Qty";
        if(showArabicTitle){
            html += "<br>كمية";
        }
        html += "<\th>";
    }
    html += "<th align=\"right\" width =\"20%\">Amount";
    if(showArabicTitle){
        html += "<br>المجموع";
    }
    html += "<\th>";
    html += "</tr>";


    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";



    if(formatVal == 1){
        for(int i=0;i<obj->InventoryItems.size();i++){

            if(obj->InventoryItems[i].BaseItem.hideInPrint)
                continue;
            float amtToPrint = obj->InventoryItems[i].BaseItem.quantity*
                               obj->InventoryItems[i].BaseItem.price;
            if(amtWithTaxFlag){
                amtToPrint =  obj->InventoryItems[i].BaseItem.grandTotal;
            }

            float rateToPrint = obj->InventoryItems[i].BaseItem.price;
            if(priceWithTaxFlag){
                rateToPrint = obj->InventoryItems[i].BaseItem.price* (1+ obj->InventoryItems[i].BaseItem.taxRate/100);
            }
            html += "<tr>";
            html += "<td>";
            html += QString::number(i+1);
            html += "</td>";
            html += "<td>";
            html += obj->InventoryItems[i].BaseItem.ItemName;

            if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;

            if(obj->voucherType.contains("SalesOrder",Qt::CaseInsensitive)){
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinSO,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }
            else{
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinBill,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }

            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                //            html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
            }

            html += "<td align=\"right\">";
            html += QString::number(rateToPrint,'f',2);
            html += "</td>";

            html += "</td>";
            html += "<td align=\"center\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            html += "</td>";



            html += "<td align=\"right\">";
            html += QString::number((amtToPrint)
                                    /*-obj->InventoryItems[i].BaseItem.discountinAmount+obj->InventoryItems[i].BaseItem.taxAmount*/
                                    ,'f',2);

            html += "</td>";
            html += "</tr>";
        }
    }
    else if(formatVal == 3){
        for(int i=0;i<obj->InventoryItems.size();i++){
            if(obj->InventoryItems[i].BaseItem.hideInPrint)
                continue;

            float amtToPrint = obj->InventoryItems[i].BaseItem.quantity*
                               obj->InventoryItems[i].BaseItem.price;
            if(amtWithTaxFlag){
                amtToPrint =  obj->InventoryItems[i].BaseItem.grandTotal;
            }

            float rateToPrint = obj->InventoryItems[i].BaseItem.price;
            if(priceWithTaxFlag){
                rateToPrint = obj->InventoryItems[i].BaseItem.price *(1+ obj->InventoryItems[i].BaseItem.taxRate/100);
            }
            html += "<tr>";
            html += "<td>";
            html += QString::number(i+1);
            html += "</td>";
            html += "<td>";
            html += obj->InventoryItems[i].BaseItem.ItemName;

            if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;

            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                //            html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
            }

            if(obj->voucherType.contains("SalesOrder",Qt::CaseInsensitive)){
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinSO,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }
            else{
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinBill,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }

            html += "<td align=\"center\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            html += "</td>";

            html += "</td>";

            html += "<td align=\"right\">";
            html += QString::number(rateToPrint,'f',2);
            html += "</td>";



            html += "<td align=\"right\">";
            html += QString::number((amtToPrint)
                                    /*-obj->InventoryItems[i].BaseItem.discountinAmount+obj->InventoryItems[i].BaseItem.taxAmount*/
                                    ,'f',2);

            html += "</td>";
            html += "</tr>";
        }
    }

    else if(formatVal == 2){
        for(int i=0;i<obj->InventoryItems.size();i++){
            if(obj->InventoryItems[i].BaseItem.hideInPrint)
                continue;
            float amtToPrint = obj->InventoryItems[i].BaseItem.quantity*
                               obj->InventoryItems[i].BaseItem.price;
            if(amtWithTaxFlag){
                amtToPrint =  obj->InventoryItems[i].BaseItem.grandTotal;
            }

            float rateToPrint = obj->InventoryItems[i].BaseItem.price;
            if(priceWithTaxFlag){
                rateToPrint = obj->InventoryItems[i].BaseItem.price* (1+ obj->InventoryItems[i].BaseItem.taxRate/100);
            }
            html += "<tr>";
            html += "<td>";
            html += QString::number(i+1);
            html += "</td>";
            html += "<td>";
            html += obj->InventoryItems[i].BaseItem.ItemName;

            if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;

            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                //            html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
            }
            if(obj->voucherType.contains("SalesOrder",Qt::CaseInsensitive)){
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinSO,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }
            else{
                bool f1 = ConfigurationSettingsDatabaseHelper::getValue(printItemNarrationinBill,false).toBool();
                if(f1==true){
                    html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
            }

            html +=  "<br><I>@" + QString::number(rateToPrint,'f',2);
            html += "</td>";

            html += "<td>";
            html += "</td>";

            html += "<td align=\"right\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            html += "</td>";

            html += "<td align=\"right\">";
            html += QString::number((amtToPrint)
                                    /*-obj->InventoryItems[i].BaseItem.discountinAmount+obj->InventoryItems[i].BaseItem.taxAmount*/
                                    ,'f',2);

            html += "</td>";
            html += "</tr>";
        }
    }


    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    int colSpan = colCount-1;
    if(obj->subTotal != obj->grandTotal && obj->subTotal > 0 && obj->grossTotal!= obj->subTotal ){
        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Sub Total";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->subTotal,'f',2);
        html += "</td></tr>";
    }
    if(obj->discountinAmount>0){
        html += "<tr>";
        html += "<td colspan =\"4\">";
        html += "<font size =\"4\">";
        html += "Discount<br> الخصم";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\"><b>";
        html += QString::number(obj->discountinAmount,'f',2);
        html += "</td></tr>";

    }
    //     qDebug()<<"Start html 157";

    float taxTotal =0;

    if(obj->taxTotalAmount>0){

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        //        html += "<font size =\"4\">";
        html += "Taxable Total <br> المجموع دون الضرائب";
        html += "</td><td align=\"right\">";
        //        html += "<font size =\"4\">";
        html += QString::number(obj->grossTotal,'f',2);
        html += "</td></tr>";

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        //        html += "<font size =\"4\">";
        html += "Vat @ 5%<br> الضريبة ";
        //        html += "Total VAT";
        html += "</td><td align=\"right\">";
        //        html += "<font size =\"4\">";
        html += QString::number(obj->taxTotalAmount,'f',2);
        html += "</td></tr>";
    }

    //    qDebug()<<"##################################"<<"led Count : "<<obj->ledgersList.size();

    for(int i=0;i<obj->ledgersList.size();i++){
        //        qDebug()<<"Led Name : "<<obj->ledgersList[i].LedgerID;
        if(/*obj->ledgersList[i].LedgerID.contains("0x2x14")||*/
            obj->ledgersList[i].LedgerID.contains("0x12x11",Qt::CaseInsensitive)||
            obj->ledgersList[i].LedgerID.contains("0x10x",Qt::CaseInsensitive)||
            !obj->ledgersList[i].isInvoiceItem
            ){
            html += "<tr>";
            html += "<td></td>";
            html += "<td colspan =\""+QString::number(colSpan-1)+"\">";
            html += "<font size =\"3\"> ";
            html +=  obj->ledgersList[i].LedgerName;
            html += "</td><td align=\"right\">";
            html += "<font size =\"3\">";
            if(obj->ledgersList[i].drAmount>0)
                html += "(-)"+QString::number(obj->ledgersList[i].drAmount,'f',2);
            else{
                html += QString::number(obj->ledgersList[i].crAmount,'f',2);
            }
            html += "</td></tr>";
        }
    }

    html += "<tr>";
    html += "<td colspan =\""+QString::number(colSpan)+"\">";
    html += "<font size =\"4\"><b>";
    html += "Grand Total ("+LoginValues::company.Currency+") ";
    html += "<br>المجموع";
    html += "</td><td align=\"right\">";
    html += "<font size =\"4\"><b>";
    html += QString::number(obj->grandTotal,'f',2);
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    html += "</table>";
    html += "<table width =\"100%\">";
    html += "<tr><td width=\"20%\">Cashier :</td>";
    html += "<td>"+cashierName+"</td>";

    QString title = "Salesman";
    if(CounterSettingsDataModel::TypeOfTrade== "Restaurant" ){
        title = "Waiter :";
        if(obj->ModeOfService == 1){
            title = "Delivery";
        }
    }
    html += "<td width=\"20%\">"+title+"</td>";
    html += "<td>"+ userHelper->getEmployeeByID(obj->SalesmanID).Name+"</td>";
    html += "</tr>";
    html += "<tr> <td  width=\"20%\">Service :</td>";
    html += "<td font size =\"5\">"+ServiceMode::getStatusString(obj->ModeOfService)+"</td>";
    QString refSize = ConfigurationSettingsDatabaseHelper::getValue(RefPrintSize, "4").toString();
    if(CounterSettingsDataModel::TypeOfTrade=="Restaurant"){
        QString refStringVal = ConfigurationSettingsDatabaseHelper::getValue(refString,"Ref").toString();
        html += "<td  width=\"20%\">"+refStringVal+": </td>";
        html += "<td><font size =\""+refSize+"\">"+obj->reference+"</td>";
    }
    html += "</tr>";
    html += "<tr>";
    html += "<td width=\"25%\" >Payment Mode</td>";
    html += "<td width=\"75%\" colspan=\"3\">"+obj->ledgerObject.LedgerName+"</td>";
    html += "</tr>";

    if(obj->PaidAmount>0){
        html += "<tr>";
        html += "<td>Paid:</td>";
        html += "<td>" + QString::number(obj->PaidAmount,'f',2) + "</td>";

        html += "<td>Balance </td>";
        html += "<td>"+QString::number(obj->grandTotal - obj->PaidAmount,'f',2)+"</td>";
        html += "</tr>";
    }

    if(obj->Contact.PhoneNumber.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "Customer Details:<br>";
        html += "<font size =\"" + billAddrSize + "\">";
        html += "Name   : "+ obj->Contact.ContactName;
        html += "<br>Phone  : "+ obj->Contact.PhoneNumber;
        html += "<br>Address: "+ obj->Contact.address.replace("\n","<br>");
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
    }
    else if(obj->Contact.ContactName.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "<font size =\"5\">";
        html += "Name : "+ obj->Contact.ContactName;
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
    }




    if(obj->narration.length()>0){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><hr></td></tr>";
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\">Narration :<br><font size =\"5\"> "+obj->narration.replace("\n","<br>")+"</td></tr>";
    }
    html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><hr></td></tr>";
    if(CounterSettingsDataModel::tokenSystem){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><font size=5><center>Token : "+obj->kotNumber+"</center></font></td></tr>";
    }
    if(obj->status == QuotationStatus::SalesOrderDeliveryCreated){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><font size=5><center>Delivery : "+obj->DeliveryDate.toString("dd-MM-yyyy hh:mm")+"</center></font></td></tr>";
    }
    //    html += "<td colspan =\""+QString::number(colSpan)+"\">";
    html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><center>"+LoginValues::getCompany().billTagLine+"</center></td></tr>";
    //    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";


    html += "</table>";
    html += "</body>";
    //     qDebug()<<"send html "<<html;
    return html;
}


QString CounterPrinterHelper::getHtm4Test(GeneralVoucherDataObject *obj, QString printerName)
{
    QString html;
    //     qDebug()<<"Start html ";
    QString CompanyName = LoginValues::getCompany().CompanyName;
    QByteArray companyLogo = LoginValues::getCompany().logo;
    QString imageType;
    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
    QString POSNumber=CounterSettingsDataModel::POSNumber;
    QString fssai = LoginValues::getCompany().fssaiLicenseNo;

    QString vType ="";
    if(obj->voucherType.contains("SalesOrder",Qt::CaseInsensitive)){
        vType = "Ord ";
    }
    else{
        vType = "Bill";
    }

    int colCount;
    if(ConfigurationSettingsDatabaseHelper::getValue(printFormat, "1").toInt() == 2)
        colCount = 5;
    else
        colCount = 5;


    QString css="";
    css = "<style type=\"text/css\">";
    css += ".page-header, .page-header-space {";
    css += "  height: 100px;";
    css += "}";

    css += ".page-footer, .page-footer-space {";
    css += "  height: 150px;";

    css += "}";

    css += ".page-footer {";
    css += "  position: fixed;";
    css += "  bottom: 0;";
    css += "  width: 100%;";
    css += "  border-top: 1px solid black;";
    css += "  background: blue;";
    css += "}";

    css += ".page-header {";
    css += "  position: fixed;";
    css += "  top: 0mm;";
    css += "  width: 100%;";
    css += "  border-bottom: 1px solid black;";
    css += "  background: yellow;";
    css += "}";

    css += ".page {";
    css += "  page-break-after: always;";
    css += "}";

    css += "@page {";
    css += "  margin: 20mm";
    css += "}";

    //    css += "@media print {";
    //    css += "   thead {display: table-header-group;position: fixed;} ";
    //    css += "   tfoot {display: table-footer-group;position: fixed;}";

    //    css += "   button {display: none;}";

    //    css += "  body {margin: 500;}";
    //    css += "}";
    css += "</style>";

    QString htmlS ="";
    htmlS +="  <html>";
    htmlS +=" <head>";
    htmlS += css;
    htmlS +=" </head>";

    htmlS +=" <body>";

    htmlS +="  <div class=\"page-header\">";
    htmlS +="   I'm The Header";
    htmlS +="   <br/>";

    htmlS +="  </div>";

    htmlS +="<div>";
    htmlS += "<table>";
    for(int i=0;i<88;i++){
        htmlS += "<tr><td><br>"+QString::number(i)+"</td><tr>";
    }

    htmlS += "</table>";
    htmlS += "</div>";

    htmlS +="  <div class=\"page-footer\">";
    htmlS +="    I'm The Footer";
    htmlS +="  </div>";

    htmlS +="  <table>";

    htmlS +="  <thead>";
    htmlS +="   <tr>";
    htmlS +="     <td>";
    htmlS +="       <!--place holder for the fixed-position header-->";
    htmlS +="       <div class=\"page-header-space\"></div>";
    htmlS +="     </td>";
    htmlS +="   </tr>";
    htmlS +=" </thead>";

    htmlS +=" <tbody>";
    htmlS +="   <tr>";
    //    htmlS +="     <tr>";
    htmlS +="       <!--*** CONTENT GOES HERE ***-->";
    htmlS +="       <div class=\"page\"><tr><td>PAGE 1</tr></div>";
    htmlS +="       <div class=\"page\"><tr><td>PAGE 2</tr></div>";
    htmlS +="       <div class=\"page\" style=\"line-height: 3;\">";
    htmlS +="         <tr><td>PAGE 3 - Long Content</tr>";

    for(int i=0;i<88;i++){
        htmlS += "<tr><td><br>"+QString::number(i)+"</td><tr>";
    }

    htmlS +="       </div>";
    htmlS +="    </td>";
    htmlS +="   </tr>";
    htmlS +=" </tbody>";

    htmlS +=" <tfoot>";
    htmlS +="  <tr>";
    htmlS +="    <td>";
    htmlS +="     <!--place holder for the fixed-position footer-->";
    htmlS +="     <div class=\"page-footer-space\">";
    htmlS +="<center><h2>Footer!!!</center>";
    htmlS +="   </div>";
    htmlS +="   </td>";
    htmlS +="  </tr>";
    htmlS +="  </tfoot>";

    htmlS +=" </table>";

    htmlS +=" </body>";

    htmlS +=" </html> ";


    html += "<html><body>";

    html += "<table width =\"100%\">";
    html += "<thead>";
    html += "<tr>";
    if(companyLogo.size()>0){
        QPixmap pixmap(companyLogo);
        QBuffer buffer(&companyLogo);
        pixmap.save(&buffer, "PNG");
        QString url ="";

        url = QString("<img src=\"data:image/png;base64,") + companyLogo.toBase64() +">";

        //                    + "\"  width=\""+QString::number(item.imageWidth)
        //                    +"\" height =\""+QString::number(item.imageHeight)+"\">";

        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center>"+url+"<br></center></td</tr><tr><td></td></tr>";
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"4\"> </font></center>";
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"4\">"+CompanyName+"</font></center></td></tr>";
    }
    else{
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"6\">"+CompanyName+"</font></center></td></tr>";
    }

    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"3\">"+branch+"</font></center>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"3\">"+addressLine+"</font></center>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"3\">Phone : "+phoneNumber+"</font></center>";

    if(trnNo.length()>0)
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"4\">GST : "+trnNo+"</font></center>";

    if(fssai.length()>0)
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><center><font size =\"4\"><i>fssai : "+fssai+"</font></center>";

    //    html += "<tr><td><center><font size =\"4\"><u>TAX INVOICE</u></font></center>";

    html += "<tr></tr>";
    //    html += "</table>";

    //     qDebug()<<"Start html 80";

    //    html += "<table width =\"100%\">";

    html += "<tr><td width=\"25%\">"+vType+" : </td>";
    html += "<td width=\"25%\"><b>"+(multiVoucher==true?obj->VoucherPrefix+" - ":"")+obj->voucherNumber+"</td>";

    html += "<td width=\"25%\">POS # :</td>";
    html += "<td width=\"25%\">"+POSNumber+"</td></tr>";

    html += "<tr><td> Date :</td>";
    html += "<td>"+obj->VoucherDate.toString("dd-MM-yyyy")+"</td>";

    html += "<td>Time :</td>";
    if(obj->timestamp.toString("hh:mm:ss").length()>0)
        html += "<td>"+obj->timestamp.toString("hh:mm:ss")+"</td></tr>";
    else
        html += "<td>"+QDateTime::currentDateTime().toString("hh:mm:ss")+"</td></tr>";

    //     qDebug()<<"Start html 99";



    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";
    if(obj->ledgerObject.LedgerGroupId == "0x5x19" && obj->ledgerObject.LedgerID!="0x5x19A135"){
        if(obj->ledgerObject.LedgerName.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Name : "+obj->ledgerObject.LedgerName+"</td></tr>";
        }
        if(obj->ledgerObject.TRN.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">GST : "+obj->ledgerObject.LedgerName+"</td></tr>";
        }
        if(obj->ledgerObject.mobileNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Mobile : "+obj->ledgerObject.mobileNumber+"</td></tr>";
        }
        if(obj->ledgerObject.phoneNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Phone : "+obj->ledgerObject.phoneNumber+"</td></tr>";
        }
        if(obj->ledgerObject.ContactPersonNumber.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Contact : "+obj->ledgerObject.ContactPersonNumber+"</td></tr>";
        }
        if(obj->ledgerObject.Address.length()>0){
            html += "<tr><td colspan =\"4\"><font size =\"5\">Address : "+obj->ledgerObject.Address+"</td></tr>";
        }
        if(obj->ledgerObject.PoBox.length()>0){
            html += "<tr><td colspan =\"4\" align=\"right\"><font size=6>Code : "+obj->ledgerObject.PoBox+"</td></tr>";
        }
        html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";
    }

    html += "<tr>";
    html += "<td colspan=\"5\">";
    html += "<tr>";
    html += "<th align=\"left\" width =\"5%\">#<\th>";
    html += "<th align=\"left\" width =\"50%\">Item Name<\th>";
    if(ConfigurationSettingsDatabaseHelper::getValue(printFormat, "1").toInt() == 1)
    {

        html += "<th align=\"left\" width =\"15%\">Price<\th>";
    }
    else {
        html += "<th align=\"left\" width =\"1%\"><\th>";
    }
    html += "<th align=\"right\"  width =\"20%\">Qty<\th>";
    html += "<th align=\"right\" width =\"20%\">Amount<\th>";
    html += "</tr>";


    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";


    int formatVal = ConfigurationSettingsDatabaseHelper::getValue(printFormat, "1").toInt();
    html += "</thead>";

    html += "<tbody>";
    if(formatVal == 1){
        for(int i=0;i<obj->InventoryItems.size();i++){
            html += "<tr>";
            html += "<td>";
            html += QString::number(i+1);
            html += "</td>";
            html += "<td>";
            html += obj->InventoryItems[i].BaseItem.ItemName;

            if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;

            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                //            html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
            }

            html += "<td align=\"right\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.price,'f',2);
            html += "</td>";

            html += "</td>";
            html += "<td align=\"right\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            html += "</td>";



            html += "<td align=\"right\">";
            html += QString::number((obj->InventoryItems[i].BaseItem.quantity*
                                     obj->InventoryItems[i].BaseItem.price)
                                    /*-obj->InventoryItems[i].BaseItem.discountinAmount+obj->InventoryItems[i].BaseItem.taxAmount*/
                                    ,'f',2);

            html += "</td>";
            html += "</tr>";
        }
    }
    else if(formatVal == 2){
        for(int i=0;i<obj->InventoryItems.size();i++){
            html += "<tr>";
            html += "<td>";
            html += QString::number(i+1);
            html += "</td>";
            html += "<td>";
            html += obj->InventoryItems[i].BaseItem.ItemName;

            if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;

            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                //            html += "<br><font size =\"3\"><i>" + obj->InventoryItems[i].BaseItem.narration;
            }
            html +=  "<br><I>@" + QString::number(obj->InventoryItems[i].BaseItem.price,'f',2);
            html += "</td>";

            html += "<td>";
            html += "</td>";

            html += "<td align=\"right\">";
            html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
            html += "</td>";

            html += "<td align=\"right\">";
            html += QString::number((obj->InventoryItems[i].BaseItem.quantity*
                                     obj->InventoryItems[i].BaseItem.price)
                                    /*-obj->InventoryItems[i].BaseItem.discountinAmount+obj->InventoryItems[i].BaseItem.taxAmount*/
                                    ,'f',2);

            html += "</td>";
            html += "</tr>";
        }
    }

    html += "</tbody";

    html += "<tfoot>";



    //    html += "<table width =\"100%\">";
    html += "<tr></tr><tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    int colSpan = colCount-1;
    if(obj->subTotal != obj->grandTotal && obj->subTotal > 0){
        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Sub Total";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->subTotal,'f',2);
        html += "</td></tr>";
    }
    if(obj->discountinAmount>0){
        html += "<tr>";
        html += "<td colspan =\"4\">";
        html += "<font size =\"4\">";
        html += " <b>Discount  ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\"><b>";
        html += QString::number(obj->discountinAmount,'f',2);
        html += "</td></tr>";

    }
    //     qDebug()<<"Start html 157";

    float taxTotal =0;

    if(obj->taxTotalAmount>0){

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Taxable Total  ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->grossTotal,'f',2);
        html += "</td></tr>";

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Total GST ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->taxTotalAmount,'f',2);
        html += "</td></tr>";
    }

    //    qDebug()<<"##################################"<<"led Count : "<<obj->ledgersList.size();

    for(int i=0;i<obj->ledgersList.size();i++){
        //        qDebug()<<"Led Name : "<<obj->ledgersList[i].LedgerID;
        if(obj->ledgersList[i].LedgerID.contains("0x2x14")||
            obj->ledgersList[i].LedgerID.contains("cess",Qt::CaseInsensitive) ||
            obj->ledgersList[i].LedgerID.contains("0x12x11",Qt::CaseInsensitive)||
            obj->ledgersList[i].LedgerID.contains("0x10x",Qt::CaseInsensitive)||
            !obj->ledgersList[i].isInvoiceItem
            ){
            html += "<tr>";
            html += "<td></td>";
            html += "<td colspan =\""+QString::number(colSpan-1)+"\">";
            html += "<font size =\"3\"><i>~";
            html +=  obj->ledgersList[i].LedgerName;
            html += "</td><td align=\"right\">";
            html += "<font size =\"3\"><i>";
            if(obj->ledgersList[i].drAmount>0)
                html += "(-)"+QString::number(obj->ledgersList[i].drAmount,'f',2);
            else{
                html += QString::number(obj->ledgersList[i].crAmount,'f',2);
            }
            html += "</td></tr>";
        }
    }

    html += "<tr>";
    html += "<td colspan =\""+QString::number(colSpan)+"\">";
    html += "<font size =\"4\"><b>";
    html += "Grand Total ("+LoginValues::company.Currency+") ";
    html += "</td><td align=\"right\">";
    html += "<font size =\"4\"><b>";
    html += QString::number(obj->grandTotal,'f',2);
    html += "</td></tr>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    html += "</table>";
    html += "<table width =\"100%\">";
    html += "<tr><td width=\"25%\">Cashier :</td>";
    html += "<td>"+cashierName+"</td>";

    QString title = "Salesman";
    if(CounterSettingsDataModel::TypeOfTrade== "Restaurant" ){
        title = "WAITER";
        if(obj->ModeOfService == 1){
            title = "DELIVERY";
        }
    }
    html += "<td width=\"25%\">"+title+"</td>";
    html += "<td>"+ userHelper->getEmployeeByID(obj->SalesmanID).Name+"</td>";
    html += "</tr>";
    html += "<tr> <td  width=\"25%\">Service: </td>";
    html += "<td font size =\"5\">"+ServiceMode::getStatusString(obj->ModeOfService)+"</td>";
    QString refSize = ConfigurationSettingsDatabaseHelper::getValue(RefPrintSize, "4").toString();
    if(CounterSettingsDataModel::TypeOfTrade=="Restaurant"){
        QString refStringVal = ConfigurationSettingsDatabaseHelper::getValue(refString,"Ref").toString();
        html += "<td  width=\"25%\">"+refStringVal+": </td>";
        html += "<td><font size =\""+refSize+"\">"+obj->reference+"</td>";
    }
    html += "</tr>";
    html += "<tr>";
    html += "<td width=\"40%\" colspan=\"2\">Payment Mode</td>";
    html += "<td width=\"60%\" colspan=\"2\">"+obj->ledgerObject.LedgerName+"</td>";
    html += "</tr>";

    if(obj->PaidAmount>0){
        html += "<tr>";
        html += "<td>Paid:</td>";
        html += "<td>"+QString::number(obj->PaidAmount,'f',2)+"</td>";

        html += "<td>Balance: </td>";
        html += "<td>"+QString::number(obj->grandTotal - obj->PaidAmount,'f',2)+"</td>";
        html += "</tr>";
    }

    if(obj->Contact.PhoneNumber.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "Customer Details:<br>";
        html += "<font size =\"" + billAddrSize + "\">";
        html += "Name   : "+ obj->Contact.ContactName;
        html += "<br>Phone  : "+ obj->Contact.PhoneNumber;
        html += "<br>Address: "+ obj->Contact.address.replace("\n","<br>");
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
    }

    else if(obj->Contact.ContactName.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "<font size =\"5\">";
        html += "Name   : "+ obj->Contact.ContactName;
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
    }

    //    if(obj->ledgerObject.LedgerGroupId == "0x5x19"){
    //        if(obj->ledgerObject.LedgerName.length()>0){
    //            html += "<tr><td colspan =\"4\"><font size =\"5\">Name : "+obj->ledgerObject.LedgerName+"</td></tr>";
    //        }
    //        if(obj->ledgerObject.mobileNumber.length()>0){
    //            html += "<tr><td colspan =\"4\"><font size =\"5\">Mobile : "+obj->ledgerObject.mobileNumber+"</td></tr>";
    //        }
    //        if(obj->ledgerObject.phoneNumber.length()>0){
    //            html += "<tr><td colspan =\"4\"><font size =\"5\">Phone : "+obj->ledgerObject.phoneNumber+"</td></tr>";
    //        }
    //        if(obj->ledgerObject.ContactPersonNumber.length()>0){
    //            html += "<tr><td colspan =\"4\"><font size =\"5\">Contact : "+obj->ledgerObject.ContactPersonNumber+"</td></tr>";
    //        }
    //        if(obj->ledgerObject.Address.length()>0){
    //            html += "<tr><td colspan =\"4\"><font size =\"5\">Address : "+obj->ledgerObject.Address+"</td></tr>";
    //        }
    //        if(obj->ledgerObject.PoBox.length()>0){
    //            html += "<tr><td colspan =\"4\" align=\"right\"><font size=6>Code : "+obj->ledgerObject.PoBox+"</td></tr>";
    //        }
    //    }

    if(obj->narration.length()>0){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><hr></td></tr>";
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\">Narration :<br><font size =\"5\"> "+obj->narration.replace("\n","<br>")+"</td></tr>";
    }
    html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><hr></td></tr>";
    if(CounterSettingsDataModel::tokenSystem){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><font size=5><center>Token : "+obj->kotNumber+"</center></font></td></tr>";
    }
    if(obj->status == QuotationStatus::SalesOrderDeliveryCreated){
        html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><font size=5><center>Delivery : "+obj->DeliveryDate.toString("dd-MM-yyyy hh:mm")+"</center></font></td></tr>";
    }
    //    html += "<td colspan =\""+QString::number(colSpan)+"\">";
    html += "<tr><td colspan =\""+QString::number(colSpan)+"\"><center>"+LoginValues::getCompany().billTagLine+"</center></td></tr>";
    html += "<tr><td colspan =\""+QString::number(colCount)+"\"><hr></td></tr>";

    html += "</tfoot>";
    html += "</table>";
    html += "</body>";
    qDebug()<<"send html "<<htmlS;
    return htmlS;
}


void CounterPrinterHelper::printCancelledKOT(GeneralVoucherDataObject *obj)
{

    bool runningOrder =false;

    int flag =1;
    //    getBillasTextDocument(doc,obj,printerName);
    QStringList printers = QPrinterInfo::availablePrinterNames();
    QStringList printersCurr;
    for(int i=0;i<obj->InventoryItems.size();i++){
        qDebug()<<"List id of"<<i<<obj->InventoryItems[i].BaseItem.listId;
        obj->InventoryItems[i].BaseItem.listId=0;
        //        if(obj->InventoryItems[i].BaseItem.listId == 0 || obj->InventoryItems[i].BaseItem.ItemReqUuid == "X"){
        printersCurr.append(obj->InventoryItems[i].BaseItem.KOTPrinter.split("|"));
        //        }
        //        else {
        //            runningOrder = true;
        //        }
    }

    printers = printersCurr.toSet().toList();

    int n = 0;
    for(QString printerName:printers){
        QTextDocument doc ;
        n=0;
        QString html = getKOTHTML(obj,&n,runningOrder,printerName,"Cancelled KOT");
        doc.setHtml(html);
        QPrinter printer(QPrinter::ScreenResolution) ;
        printer.setPrinterName(printerName);
        QFont font;
        font.setFamily("Avant Garde");
        font.setPointSize(8);
        doc.setDefaultFont(font);
        QSizeF paperSize;
        paperSize.setWidth(printer.width());
        paperSize.setHeight(printer.height());
        doc.setPageSize(paperSize);
        qDebug()<<"n="<<n;
        if(n>0)
            doc.print(&printer);

    }


}

QString CounterPrinterHelper::getSalesOrderHtml(GeneralVoucherDataObject *obj, QString printerName)
{

    QString html;
    //     qDebug()<<"Start html ";
    QString CompanyName = LoginValues::getCompany().CompanyName;
    QByteArray companyLogo = LoginValues::getCompany().logo; QString imageType;
    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
    QString POSNumber=CounterSettingsDataModel::POSNumber;
    QString fssai =  LoginValues::getCompany().fssaiLicenseNo;
    QString vType = "Ord";

    html += "<html><body>";
    html += "<table width =\"100%\"><tr>";

    //        html += "<tr><td><center><img src=:/icons/eaters.png></center>";
    //    html += "<tr><td><center><img src=:/icons/teabreak1.png></center>";



    if(companyLogo.size()>0){
        QPixmap pixmap(companyLogo);
        QBuffer buffer(&companyLogo);
        pixmap.save(&buffer, "PNG");
        QString url ="";

        url = QString("<img src=\"data:image/png;base64,") + companyLogo.toBase64() +">";

        //                    + "\"  width=\""+QString::number(item.imageWidth)
        //                    +"\" height =\""+QString::number(item.imageHeight)+"\">";

        html += "<tr><td><center>"+url+"<br></center></td</tr><tr><td>br></td></tr>";
    }
    else{
        html += "<tr><td><center><font size =\"4\">"+CompanyName+"</font></center>";
    }


    html += "<tr><td><center><font size =\"4\">"+branch     +"</font></center>";
    html += "<tr><td><center><font size =\"4\">"+addressLine+"</font></center>";
    html += "<tr><td><center><font size =\"4\">Phone : "+phoneNumber+"</font></center>";
    if(trnNo.length()>0)
        html += "<tr><td><center><font size =\"4\">GSTRN : "+trnNo+"</font></center>";
    if(fssai.length()>0)
        html += "<tr><td><center><font size =\"4\"><i>fssai : "+fssai+"</font></center>";
    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";

    html += "<table width =\"100%\">";

    html += "<tr><td width=\"25%\">"+vType+" No :</td>";
    qDebug()<<"Is Multi Voucher : "<<multiVoucher;
    html += "<td width=\"25%\">"+(multiVoucher==true?obj->VoucherPrefix+ " - ":"")+obj->voucherNumber+"</td>";

    html += "<td width=\"25%\">POS #</td>";
    html += "<td>"+POSNumber+"</td></tr>";

    html += "<tr><td> Date : </td>";
    html += "<td>"+obj->VoucherDate.toString("dd-MM-yyyy")+"</td>";

    html += "<td>Time :</td>";
    if(obj->timestamp.toString("hh:mm:ss").length()>0)
        html += "<td>"+obj->timestamp.toString("hh:mm:ss")+"</td></tr>";
    else
        html += "<td>"+QDateTime::currentDateTime().toString("hh:mm:ss")+"</td></tr>";

    html += "</table>";
    html += "<table width=\"100%\">";
    //     qDebug()<<"Start html 99";
    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    html += "<tr>";
    html += "<th align=\"left\" width =\"5%\">#<\th>";
    html += "<th align=\"left\" width =\"65%\">Item Name<\th>";
    html += "<th align=\"right\" width =\"10%\">Qty<\th>";
    //    html += "<th align=\"left\" width =\"10%\">Price<\th>";
    html += "<th align=\"right\" width =\"10%\">Amount<\th>";
    html += "</tr>";

    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    for(int i=0;i<obj->InventoryItems.size();i++){
        html += "<tr>";

        html += "<td>";
        html += QString::number(i+1);
        html += "</td>";

        html += "<td>";
        html += obj->InventoryItems[i].BaseItem.ItemName;
        if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
            html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;
        //        if(obj->InventoryItems[i].BaseItem.narration.length()>0){
        //            obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
        //            html += "<br>" + obj->InventoryItems[i].BaseItem.narration;
        //        }

        html += "</td>";
        html += "<td align=\"right\">";
        html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
        html += "</td>";

        html += "<td align=\"right\">";
        html += QString::number((obj->InventoryItems[i].BaseItem.quantity*
                                 obj->InventoryItems[i].BaseItem.price)
                                //                                -
                                //                                obj->InventoryItems[i].BaseItem.discountinAmount+
                                //                                obj->InventoryItems[i].BaseItem.vatAmount
                                ,'f',2);
        html += "</td>";

        html += "</tr>";
    }

    html += "<tr></tr><tr></tr><tr><td colspan =\"4\"><hr></td></tr>";

    int colSpan = 3;

    if(obj->subTotal != obj->grandTotal && obj->subTotal > 0){
        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Sub Total";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->subTotal,'f',2);
        html += "</td></tr>";
    }
    if(obj->discountinAmount>0){
        html += "<tr>";
        html += "<td colspan =\"4\">";
        html += "<font size =\"4\">";
        html += " <b>Discount  ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\"><b>";
        html += QString::number(obj->discountinAmount,'f',2);
        html += "</td></tr>";

    }
    //     qDebug()<<"Start html 157";

    float taxTotal =0;

    if(obj->taxTotalAmount>0){

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Taxable Total  ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->grossTotal,'f',2);
        html += "</td></tr>";

        html += "<tr>";
        html += "<td colspan =\""+QString::number(colSpan)+"\">";
        html += "<font size =\"4\">";
        html += "Total GST ";
        html += "</td><td align=\"right\">";
        html += "<font size =\"4\">";
        html += QString::number(obj->taxTotalAmount,'f',2);
        html += "</td></tr>";
    }

    //    qDebug()<<"Led size at C PRINTER : "<<obj->ledgersList.size();
    for(int i=0;i<obj->ledgersList.size();i++){
        //        qDebug()<<"Item  Check"<<obj->ledgersList[i].LedgerName<<obj->ledgersList[i].isInvoiceItem;
        if(obj->ledgersList[i].LedgerID.contains("0x2x14")||
            obj->ledgersList[i].LedgerID.contains("cess",Qt::CaseInsensitive) ||
            obj->ledgersList[i].LedgerID.contains("0x12x11",Qt::CaseInsensitive)||
            !obj->ledgersList[i].isInvoiceItem){
            //            qDebug()<<"Item  inside"<<obj->ledgersList[i].LedgerName;
            html += "<tr>";
            html += "<td></td>";
            html += "<td colspan =\"2\">";
            html += "<font size =\"3\"><i>";
            html += obj->ledgersList[i].LedgerName;
            html += "</td><td align=\"right\">";
            html += "<font size =\"3\"><i>";
            if(obj->ledgersList[i].drAmount>0)
                html += "(-)"+QString::number(obj->ledgersList[i].drAmount,'f',2);
            else{
                html += QString::number(obj->ledgersList[i].crAmount,'f',2);
            }

            html += "</td></tr>";
        }
    }

    html += "<tr>";
    html += "<td colspan =\"3\">";
    html += "<font size =\"4\"><b>";
    html += "Grand Total  ";
    html += "</td><td align=\"right\">";
    html += "<font size =\"4\"><b>";
    html += QString::number(obj->grandTotal,'f',2);
    html += "</td></tr>";
    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    html += "</table>";

    html += "<table width =\"100%\">";

    html += "<tr><td width=\"25%\">Cashier:</td>";
    html += "<td>"+cashierName+"</td>";

    html += "<td width=\"25%\">Salesman:</td>";
    html += "<td>"+ userHelper->getEmployeeByID(obj->SalesmanID).Name+"</td>";
    html += "</tr>";

    QString title = "Salesman";
    if(CounterSettingsDataModel::TypeOfTrade== "Restaurant" ){
        title = "WAITER";
        if(obj->ModeOfService == 1){
            title = "DELIVERY";
        }
    }

    html += "<tr><td width=\"25%\">"+title+"</td>";
    html += "<td>"+ userHelper->getEmployeeNameFromID(obj->SalesmanID)+"</td>";
    html += "</tr>";

    //    qDebug()<<"Mode of Service = "<<obj->ModeOfService;
    html += "<tr> <td  width=\"25%\">Service: </td>";
    html += "<td>"+ServiceMode::getStatusString(obj->ModeOfService)+"</td>";



    html += "</tr>";

    if(CounterSettingsDataModel::TypeOfTrade=="Restaurant"){
        html += "<tr>";
        html += "<td colspan=\"4\" width=\"25%\"> <font size =\"5\"><center> Order No : "+obj->reference+"</td>";

        html += "</tr>";
    }

    if(obj->Contact.PhoneNumber.length() > 0|| obj->Contact.ContactName.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "Customer Details<br>";
        html += "<font size =\"" + billAddrSize + "\">";
        if(obj->Contact.ContactName.length()>0)
            html += "<font size =\"5\">Name   : "+ obj->Contact.ContactName;
        if(obj->Contact.PhoneNumber.length()>0)
            html += "<br><font size =\"5\">Phone  : "+ obj->Contact.PhoneNumber;
        if(obj->Contact.address.length()>0)
            html += "<br><font size =\"5\">Address: "+ obj->Contact.address.replace("\n","<br>");

        html += "<tr><td colspan =\"4\"><hr></td></tr>";
    }


    if(obj->narration.length()>0){
        html += "<tr><td colspan =\"4\"><hr></td></tr>";
        html += "<tr><td colspan =\"4\">Narration : "+obj->narration.replace("\n","<br>")+"</td></tr>";
    }

    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    html += "<tr><td colspan =\"4\"><center>"+LoginValues::getCompany().billTagLine+"</center></td></tr>";
    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    html += "</table>";

    //    qDebug()<<"send html "<<html;
    return html;
}

int CounterPrinterHelper::printSalesVoucher(GeneralVoucherDataObject *obj,QString printerName)
{
    int flag =1;
    QTextDocument doc ;

    QString html = getHtml(obj);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setPrinterName(printerName);

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    //    printer.setPageSize(QPrinter::A4);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    doc.print(&printer);


    printMeasurement(obj);

    return flag;

}

int CounterPrinterHelper::printSalesOrder(GeneralVoucherDataObject *obj, QString printerName)
{
    int flag =1;
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = getHtml(obj);

    QTextCursor Cursor(&doc);
    //    Cursor.insertHtml(html);
    //    Cursor.insertText("HELLO");
    QPrinter printer(QPrinter::ScreenResolution);
    //    qDebug()<<"Printer Setting name";

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    printer.setPrinterName(printerName);

    QPainter painter( &printer );


    double x = mmToPixels(printer,20);
    double y = printer.height() -  mmToPixels(printer,80);

    int id = QFontDatabase::addApplicationFont(":/images/code128.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont barCodeFont(family,36,QFont::Normal);

    barCodeFont.setLetterSpacing(QFont::AbsoluteSpacing,0.0);
    int vno = obj->voucherNumber.toInt();
    QString number = QString("%1").arg(vno, 6, 10, QChar('0'));

    number = "888"+number;
    qDebug()<<"Print Barcode : "<<number;

    QString barcodeString = encodeBarcode(number);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());

    //    qDebug()<<"Print Height"<<printer.height();

    //    qDebug()<<"Page Rect() = "<<printer.pageRect().size();
    //    qDebug()<<"Paper Rect() = "<<printer.paperRect();
    //    qDebug()<<"3 HEight "<<printer.pageRect().size().height();

    doc.setPageSize(paperSize);
    if(printBarcodeInBillFlag){
        //        html += "<br><br><br><br><br>";

    }
    Cursor.insertHtml(html);

    QSizeF htmlSize = doc.size();

    // Add a line spacing to the height to get the y coordinate of the barcode
    double y1 = htmlSize.height() + 10;

    painter.setFont(barCodeFont);

    //    painter.drawText(x,y,str);

    if(printBarcodeInBillFlag){
        painter.drawText(x-50,y1,printer.width()-x/2,x,Qt::AlignHCenter,barcodeString);
    }

    //    doc.print(&printer);

    doc.drawContents(&painter);

    //    qDebug()<<"deleting doc";
    printMeasurement(obj);

    //    qDebug()<<"Print  complete";
    return flag;

}

void CounterPrinterHelper::printKotViaNetwork(GeneralVoucherDataObject *obj, QString ipAddress,QString groups)
{

    bool flag = true;

    for(int i=0;i<obj->InventoryItems.count();i++)
    {

        QString groupname = "|"+obj->InventoryItems[i].BaseItem.GroupName+"|";

        if(obj->InventoryItems[i].BaseItem.listId == 0 &&
            (((groups.contains(groupname,Qt::CaseInsensitive)))||groups.length()==0)){

            flag = false;
        }
    }
    if(flag){
        return;
    }

    QMessageBox box;
    QTcpSocket *m_pSocket   = new QTcpSocket();
    if (m_pSocket->state() == QAbstractSocket::UnconnectedState || m_pSocket->waitForDisconnected(1000))
        m_pSocket->connectToHost(ipAddress, 9100);

    QDate d = QDate::currentDate();
    QString dText = d.toString("dd-MMM-yyyy");
    QTime t = QTime::currentTime();
    QString tText = t.toString("hh:mm:ss");
    QString itemname,print_qty;
    QString narration;

    bool m_ConnectStatus = true;

    const int Timeout = 2 * 1000;

    if (!m_ConnectStatus)
    {
        m_pSocket->connectToHost(ipAddress, 9100);
    }

    if (!m_pSocket->waitForConnected(Timeout))
    {
        //sent error

        box.setText("KITCHEN KOT PRINTER NOT CONNECTED!! PLEASE INFORM KITCHEN");
        box.exec();
        m_ConnectStatus = false;
        return;
    }

    m_ConnectStatus = true;


    m_pSocket->write("\n");

    m_pSocket->write("\n");
    QString restoName = LoginValues::getCompany().CompanyName;
    m_pSocket->write("\n");
    m_pSocket->write("                   ");
    char* p = new char[restoName.length() + 1];
    strcpy(p, restoName.toLatin1().constData());
    m_pSocket->write(p);


    m_pSocket->write("\n                      KOT");
    m_pSocket->write("\n");
    m_pSocket->write("              *******************");
    m_pSocket->write("\n");
    m_pSocket->write("-----------------------------------------------");
    m_pSocket->write("\n");

    p = new char[dText.length() + 1];
    strcpy(p, dText.toLatin1().constData());
    m_pSocket->write("Date : ");
    m_pSocket->write(p);



    p = new char[tText.length()+1];  strcpy(p, tText.toLatin1().constData());
    m_pSocket->write("          Time : ");
    m_pSocket->write(p);
    m_pSocket->write("\n");

    p = new char[dText.length() + 1];
    strcpy(p, ServiceMode::getStatusString(obj->ModeOfService).toLatin1().constData());

    if(obj->kotNumber.length()>0){
        p = new char[obj->kotNumber.length()+1];  strcpy(p, obj->kotNumber.toLatin1().constData());
        m_pSocket->write("KOT : ");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    if(ServiceMode::getStatusString(obj->ModeOfService).length()>0){
        m_pSocket->write("Service Type : ");
        p = new char[ServiceMode::getStatusString(obj->ModeOfService).length()+1];
        strcpy(p, ServiceMode::getStatusString(obj->ModeOfService).toLatin1().constData());
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    if(userHelper->getEmployeeNameFromID(obj->SalesmanID).length()>0){
        m_pSocket->write("Waiter Name : ");
        p = new char[userHelper->getEmployeeNameFromID(obj->SalesmanID).length()+1];
        strcpy(p, userHelper->getEmployeeNameFromID(obj->SalesmanID).toLatin1().constData());
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    m_pSocket->write("-----------------------------------------------\n");
    m_pSocket->write("   ITEMNAME                         QUANTITY\n");
    m_pSocket->write("-----------------------------------------------\n");
    qDebug()<<"**********IPaddress"<<ipAddress<<groups;
    float amountTotal=0;
    for(int i=0;i<obj->InventoryItems.count();i++)
    {

        QString groupname = "|"+obj->InventoryItems[i].BaseItem.GroupName+"|";

        if(obj->InventoryItems[i].BaseItem.listId== 0 && (groups.contains(groupname,Qt::CaseInsensitive)
                                                            ||groups.length()==0)){
            itemname=obj->InventoryItems[i].BaseItem.ItemName;
            itemname=itemname.leftJustified(34,' ',true);
            print_qty=QString::number(obj->InventoryItems[i].BaseItem.quantity);

            p = new char[itemname.length()+1];  strcpy(p, itemname.toLatin1().constData());
            m_pSocket->write(p);
            m_pSocket->write("   ");

            p = new char[print_qty.length()+1];  strcpy(p, print_qty.toLatin1().constData());
            m_pSocket->write(p);
            m_pSocket->write("\n");

            narration = obj->InventoryItems[i].BaseItem.narration;
            if(narration.length()>0){
                p = new char[narration.length()+1];  strcpy(p, narration.toLatin1().constData());
                m_pSocket->write(p);
                m_pSocket->write("\n");

            }
            amountTotal+=obj->InventoryItems[i].BaseItem.subTotal;
        }
    }

    QString amountStr = QString::number(obj->grandTotal,'f',2);
    m_pSocket->write("\n\n\n-----------------------------------------------\n");

    if(amountStr .length()>0){
        p = new char[amountStr .length()+1];  strcpy(p, amountStr .toLatin1().constData());
        m_pSocket->write("Total : ");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    if(obj->Contact.ContactName.length()>0){
        p = new char[obj->Contact.ContactName.length()+1];  strcpy(p, obj->Contact.ContactName.toLatin1().constData());
        m_pSocket->write("Customer Name : ");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    if(obj->Contact.PhoneNumber.length()>0){
        p = new char[obj->Contact.PhoneNumber.length()+1];  strcpy(p, obj->Contact.PhoneNumber.toLatin1().constData());
        m_pSocket->write("Phone         : ");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }
    if(obj->Contact.address.length()>0){
        p = new char[obj->Contact.address.length()+1];  strcpy(p, obj->Contact.address.toLatin1().constData());
        m_pSocket->write("Address       : \n");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }

    if(obj->reference.length()>0){
        p = new char[obj->reference.length()+1];  strcpy(p, obj->reference.toLatin1().constData());
        m_pSocket->write("                        ");
        m_pSocket->write(p);
        m_pSocket->write("\n");
    }

    m_pSocket->write("\n");
    m_pSocket->write("\n");

    //out <<"\n";
    //  m_pSocket->write(block);
    m_pSocket->write("\n");
    m_pSocket->write("\n\n\n\n\n\r");
    m_pSocket->write("\x1bi");
    //out <<char(27)<<char(112)<<char(48)<<char(25)<<char(250);
    // m_pSocket->

    m_pSocket->disconnectFromHost();
    if (m_pSocket->state() == QAbstractSocket::UnconnectedState || m_pSocket->waitForDisconnected(1000))
    {}

    m_pSocket->close();

}

void CounterPrinterHelper::printShiftReport(ShiftDataModel *Shift, bool isPDF, bool printDenom, QString printerName)
{
    qDebug()<<Q_FUNC_INFO<<isPDF<<printDenom;

    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = "";

    RestaurantShiftDatabaseHelper dbhelper;


    html += "<html><body>";
    html += "<table width =\"100%\">";
    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">SHIFT REPORT</font></td><tr>";
    html += "<tr><td>";

    html +="<table width=\"100%\">";


    html += getShiftRow("Cashier Name",Shift->cashierName);
    html += getShiftRow("Shift From",Shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Shift To",Shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Bill From",Shift->BillsFrom);
    html += getShiftRow("Bill To",Shift->BillsTo);
    html += getShiftRow("No. of Bills",QString::number(Shift->billsCount));
    html += getShiftRow("Opening Cash",QString::number(Shift->OpeningCash,'f',2));

    html += getShiftRow("Sales Total",QString::number(Shift->SalesTotal,'f',2));
    if(Shift->CashSales > 0)
        html += getShiftRow("Sales By Cash",QString::number(Shift->CashSales,'f',2));
    if(Shift->CardSales > 0)
        html += getShiftRow("Sales By Card",QString::number(Shift->CardSales,'f',2));
    if(Shift->CreditSales > 0)
        html += getShiftRow("Credit Sales",QString::number(Shift->CreditSales,'f',2));

    if(Shift->taxableTotal > 0)
        html += getShiftRow("Taxable Sales",QString::number(Shift->taxableTotal,'f',2));
    if(Shift->vatTotal > 0)
        html += getShiftRow("Tax Total : ",QString::number(Shift->vatTotal,'f',2));

    if(Shift->advanceRecieved>0){
        html += getShiftRow("Advance Recieved",QString::number(Shift->advanceRecieved,'f',2));
    }

    //    if(Shift->ZomatoSales>0){
    //        html += getShiftRow("Zomato Sales",QString::number(Shift->ZomatoSales,'f',2));
    //        html += getShiftRow("Zomato Cash",QString::number(Shift->ZomatoCash,'f',2));
    //        html += getShiftRow("Zomato Online",QString::number(Shift->ZomatoCredit,'f',2));
    //    }

    GeneralVoucherDataObject creditSales ;
    creditSales =  dbhelper.getBankAmountDetails(Shift);
    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Bank Transfers<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
            html += getShiftRow(name,amt);
        }
    }

    creditSales = dbhelper.getCreditSalesDetails(Shift);
    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Credit Sales<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            if(creditSales.ledgersList[i].drAmount > 0){
                QString name = creditSales.ledgersList[i].LedgerName;
                QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
                html += getShiftRow(name,amt);
            }
        }
    }

    qDebug()<<"1083";

    if(Shift->PurchaseTotal > 0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += getShiftRow("Purchase Total",QString::number(Shift->PurchaseTotal,'f',2));
        if(Shift->CashPurchase > 0){
            html += getShiftRow("Purchase By Cash",QString::number(Shift->CashPurchase,'f',2));
        }
    }
    if(Shift->PaymentsTotal > 0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";

        html += getShiftRow("Payments Total",QString::number(Shift->PaymentsTotal,'f',2));
        if(Shift->CashPayments > 0)
            html += getShiftRow("Payments in Cash",QString::number(Shift->CashPayments,'f',2));
    }
    if(Shift->ReceiptTotal > 0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += getShiftRow("Receipt Total",QString::number(Shift->ReceiptTotal,'f',2));
        if(Shift->CashReceipts > 0)
            html += getShiftRow("Receipt in Cash",QString::number(Shift->CashReceipts,'f',2));
    }

    html += "<tr><td colspan =\"3\"><hr></td></tr>";

    html += getShiftRow("Cash in Counter",QString::number(Shift->CashInCounter,'f',2));
    html += getShiftRow("Cash WithDrawn",QString::number(Shift->CashWithDrawn,'f',2));
    html += getShiftRow("Till Difference",QString::number(Shift->TillDifference,'f',2));
    html += getShiftRow("Total Cash ",QString::number(Shift->totalCash,'f',2));

    qDebug()<<"1083";

    html += "<tr><td colspan =\"3\"><hr></td></tr>";

    html += getShiftRow("Closing Cash",QString::number(Shift->CashBalance,'f',2));
    html += "<tr><td colspan =\"3\"><hr></td></tr><br><br>";
    html += "<tr><td colspan =\"3\">Narration<br>"+Shift->Narration+"</td></tr><br><br>";
    html += "<tr><td colspan =\"3\"><hr></td></tr><br><br>";

    qDebug()<<"1083";

    if(printDenom){
        html += "<tr>";
        html += "<th align=\"left\" width =\"60%\">Denomination<\th>";
        html += "<th align=\"right\" width =\"20%\">Count<\th>";
        html += "<th align=\"right\" width =\"20%\">Amount<\th>";
        if(Shift->denominations->total > 0){
            qDebug()<<"check denom";
            qDebug()<<Shift->denominations->amt5;
            html += getShiftRow("2000",QString::number(Shift->denominations->count2000), QString::number(Shift->denominations->amt2000, 'f', 2));
            html += getShiftRow("500",QString::number(Shift->denominations->count500), QString::number(Shift->denominations->amt500, 'f', 2));
            html += getShiftRow("200",QString::number(Shift->denominations->count200), QString::number(Shift->denominations->amt200, 'f', 2));
            html += getShiftRow("100",QString::number(Shift->denominations->count100), QString::number(Shift->denominations->amt100, 'f', 2));
            html += getShiftRow("50",QString::number(Shift->denominations->count50), QString::number(Shift->denominations->amt50, 'f', 2));
            html += getShiftRow("20",QString::number(Shift->denominations->count20), QString::number(Shift->denominations->amt20, 'f', 2));
            html += getShiftRow("10",QString::number(Shift->denominations->count10), QString::number(Shift->denominations->amt10, 'f', 2));
            html += getShiftRow("5",QString::number(Shift->denominations->count5), QString::number(Shift->denominations->amt5, 'f', 2));
            html += getShiftRow("Coins",QString::number(Shift->denominations->countCoins), QString::number(Shift->denominations->amtCoins, 'f', 2));
            html += getShiftRow("Total"," ", QString::number(Shift->denominations->total, 'f', 2));
        }
    }


    qDebug()<<"1127";

    html += "</tr>";

    doc.setHtml(html);

    qDebug()<<"printer 1124";

    QPrinter printer(QPrinter::ScreenResolution) ;

    printer.setPrinterName("printerName");
    if(isPDF){
        qDebug()<<"set as pDF"<<Q_FUNC_INFO<<__LINE__;
        QString fileName = QFileDialog::getSaveFileName(this,"File Name","Shift_" + QDate::currentDate().toString("dd_MMM_yyyy"),tr("*.pdf"));
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
    }
    qDebug()<<__LINE__;
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);


    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);

}

void CounterPrinterHelper::printDailyClosing(ShiftDataModel *Shift, bool isPDF, bool printDenom, QString printerName)
{
    qDebug()<<Q_FUNC_INFO;

    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = "";

    RestaurantShiftDatabaseHelper dbhelper;


    html += "<html><body>";
    html += "<table width =\"100%\">";
    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">DAILY CLOSING REPORT</font></td><tr>";
    html += "<tr><td>";

    html +="<table width=\"100%\">";


    html += getShiftRow("Cashier Name",Shift->cashierName);
    html += getShiftRow("Shift From",Shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Shift To",Shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Bill From",Shift->BillsFrom);
    html += getShiftRow("Bill To",Shift->BillsTo);
    html += getShiftRow("No. of Bills",QString::number(Shift->billsCount));
    html += getShiftRow("Opening Cash",QString::number(Shift->OpeningCash,'f',2));

    html += getShiftRow("Sales Total",QString::number(Shift->SalesTotal,'f',2));
    html += getShiftRow("Sales By Cash",QString::number(Shift->CashSales,'f',2));
    html += getShiftRow("Sales By Card",QString::number(Shift->CardSales,'f',2));
    html += getShiftRow("Credit Sales",QString::number(Shift->CreditSales,'f',2));

    html += getShiftRow("Taxable Sales",QString::number(Shift->taxableTotal,'f',2));
    html += getShiftRow("Tax Total : ",QString::number(Shift->vatTotal,'f',2));

    if(Shift->advanceRecieved>0){
        html += getShiftRow("Advance Recieved",QString::number(Shift->advanceRecieved,'f',2));
    }

    //    if(Shift->ZomatoSales>0){
    //        html += getShiftRow("Zomato Sales",QString::number(Shift->ZomatoSales,'f',2));
    //        html += getShiftRow("Zomato Cash",QString::number(Shift->ZomatoCash,'f',2));
    //        html += getShiftRow("Zomato Online",QString::number(Shift->ZomatoCredit,'f',2));
    //    }

    GeneralVoucherDataObject creditSales ;
    creditSales =  dbhelper.getBankAmountDetails(Shift);
    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Bank Transfers<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
            html += getShiftRow(name,amt);
        }
    }

    creditSales = dbhelper.getCreditSalesDetails(Shift);
    if(creditSales.ledgersList.size()>0){
        html += "<tr><td colspan =\"2\"><hr></td></tr>";
        html += "<tr><td colspan =\"2\"><font size=\"4\"><u>Credit Sales<u></td></tr>";
        for(int i=0;i<creditSales.ledgersList.size();i++){
            QString name = creditSales.ledgersList[i].LedgerName;
            QString amt  = QString::number(creditSales.ledgersList[i].drAmount,'f',2);
            html += getShiftRow(name,amt);
        }
    }

    qDebug()<<"1083";

    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Purchase Total",QString::number(Shift->PurchaseTotal,'f',2));
    html += getShiftRow("Purchase By Cash",QString::number(Shift->CashPurchase,'f',2));
    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Payments Total",QString::number(Shift->PaymentsTotal,'f',2));
    html += getShiftRow("Payments in Cash",QString::number(Shift->CashPayments,'f',2));
    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Receipt Total",QString::number(Shift->ReceiptTotal,'f',2));
    html += getShiftRow("Receipt in Cash",QString::number(Shift->CashReceipts,'f',2));
    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Cash in Counter",QString::number(Shift->CashInCounter,'f',2));
    html += getShiftRow("Cash WithDrawn",QString::number(Shift->CashWithDrawn,'f',2));
    html += getShiftRow("Till Difference",QString::number(Shift->TillDifference,'f',2));
    html += getShiftRow("Total Cash ",QString::number(Shift->totalCash,'f',2));

    qDebug()<<"1083";

    html += "<tr><td colspan =\"3\"><hr></td></tr>";

    html += getShiftRow("Closing Cash",QString::number(Shift->CashBalance,'f',2));
    html += "<tr><td colspan =\"3\"><hr></td></tr><br><br>";
    html += "<tr><td colspan =\"3\">Narration<br>"+Shift->Narration+"</td></tr><br><br>";
    html += "<tr><td colspan =\"3\"><hr></td></tr><br><br>";
    html += "<tr>";
    html += "<th align=\"left\" width =\"60%\">Denomination<\th>";
    html += "<th align=\"right\" width =\"20%\">Count<\th>";
    html += "<th align=\"right\" width =\"20%\">Amount<\th>";

    qDebug()<<"1083";

    if(printDenom){
        if(Shift->denominations->total > 0){
            qDebug()<<"check denom";
            qDebug()<<Shift->denominations->amt5;
            html += getShiftRow("2000",QString::number(Shift->denominations->count2000), QString::number(Shift->denominations->amt2000, 'f', 2));
            html += getShiftRow("500",QString::number(Shift->denominations->count500), QString::number(Shift->denominations->amt500, 'f', 2));
            html += getShiftRow("200",QString::number(Shift->denominations->count200), QString::number(Shift->denominations->amt200, 'f', 2));
            html += getShiftRow("100",QString::number(Shift->denominations->count100), QString::number(Shift->denominations->amt100, 'f', 2));
            html += getShiftRow("50",QString::number(Shift->denominations->count50), QString::number(Shift->denominations->amt50, 'f', 2));
            html += getShiftRow("20",QString::number(Shift->denominations->count20), QString::number(Shift->denominations->amt20, 'f', 2));
            html += getShiftRow("10",QString::number(Shift->denominations->count10), QString::number(Shift->denominations->amt10, 'f', 2));
            html += getShiftRow("5",QString::number(Shift->denominations->count5), QString::number(Shift->denominations->amt5, 'f', 2));
            html += getShiftRow("Coins",QString::number(Shift->denominations->countCoins), QString::number(Shift->denominations->amtCoins, 'f', 2));
            html += getShiftRow("Total"," ", QString::number(Shift->denominations->total, 'f', 2));
        }
    }


    qDebug()<<"1127";

    html += "</tr>";

    doc.setHtml(html);

    qDebug()<<"printer 1124";

    QPrinter printer(QPrinter::ScreenResolution) ;

    printer.setPrinterName("printerName");
    if(isPDF){
        qDebug()<<"set as pDF"<<Q_FUNC_INFO<<__LINE__;
        QString fileName = QFileDialog::getSaveFileName(this,"File Name","Shift_" + QDate::currentDate().toString("dd_MMM_yyyy"),tr("*.pdf"));
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
    }
    qDebug()<<__LINE__;
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);


    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);

}


void CounterPrinterHelper::printKOT(GeneralVoucherDataObject *obj,bool printAll)
{
    bool runningOrder =false;
    //    getBillasTextDocument(doc,obj,printerName);
    QStringList printers;
    QStringList printersAvl = QPrinterInfo::availablePrinterNames();
    QStringList printersCurr;
    for(int i=0;i<obj->InventoryItems.size();i++){
        //        qDebug()<<"List id of"<<i<<obj->InventoryItems[i].BaseItem.listId;
        if(obj->InventoryItems[i].BaseItem.listId == 0 || obj->InventoryItems[i].BaseItem.ItemReqUuid == "X"){
            printersCurr.append(obj->InventoryItems[i].BaseItem.KOTPrinter.split("|"));
        }
        else {
            runningOrder = true;
        }
    }

    printers = printersCurr.toSet().toList();
    int n = 0;
    for(QString printerName:printers){
        if(printersAvl.contains(printerName)){
            QTextDocument doc ;
            n=0;
            QString html = getKOTHTML(obj,&n,runningOrder,printerName);
            doc.setHtml(html);
            QPrinter printer(QPrinter::ScreenResolution) ;
            printer.setPrinterName(printerName);
            QFont font;
            font.setFamily("Avant Garde");
            font.setPointSize(8);
            doc.setDefaultFont(font);
            QSizeF paperSize;
            paperSize.setWidth(printer.width());
            paperSize.setHeight(printer.height());
            doc.setPageSize(paperSize);
            qDebug()<<"n="<<n;
            if(n>0)
                doc.print(&printer);
        }
    }
}

void CounterPrinterHelper::printKOTToPrinter(GeneralVoucherDataObject *obj,QString printerName)
{
    bool runningOrder =false;
    //    getBillasTextDocument(doc,obj,printerName);
    //    QStringList printers = QPrinterInfo::availablePrinterNames();
    QStringList printersCurr;
    for(int i=0;i<obj->InventoryItems.size();i++){
        obj->InventoryItems[i].BaseItem.KOTPrinter = "|"+printerName+"|";
        //        qDebug()<<"List id of"<<i<<obj->InventoryItems[i].BaseItem.listId;
        //        if(obj->InventoryItems[i].BaseItem.listId == 0 || obj->InventoryItems[i].BaseItem.ItemReqUuid == "X"){
        //            printersCurr.append(obj->InventoryItems[i].BaseItem.KOTPrinter.split("|"));
        //        }
        //        else {
        //            runningOrder = true;
        //        }
    }
    //    printers = printersCurr.toSet().toList();

    //    printersCurr.append(printerName);

    int n = 0;
    QTextDocument doc ;
    n=0;
    QString html = getKOTHTML(obj,&n,runningOrder,printerName);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPrinterName(printerName);
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    qDebug()<<"n="<<n;
    if(n>0)
        doc.print(&printer);

}

void CounterPrinterHelper::printReceiptVoucher(GeneralVoucherDataObject *voucher)
{
    QTextDocument doc ;
    QString html = getRVHTML(voucher);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution) ;
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(9);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);
}

void CounterPrinterHelper::printPaymentVoucher(GeneralVoucherDataObject *voucher)
{
    QTextDocument doc ;
    QString html = getPayVHTML(voucher);
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution) ;
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(9);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);
}

QString CounterPrinterHelper::getRVHTML(GeneralVoucherDataObject *obj)
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
    QString POSNumber=CounterSettingsDataModel::POSNumber;

    QString vType ="Receipt Voucher";

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
        html += "<tr><td><center><font size =\"4\">TIN : "+trnNo+"</font></center>";


    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";

    html += "<table width =\"100%\">";

    html += "<tr><td width=\"25%\">"+vType+" No :</td>";
    html += "<td width=\"25%\"><b>"+(multiVoucher == true ?obj->VoucherPrefix+ " - ":"")+obj->voucherNumber+"</td>";

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
        float amt = obj->ledgersList[i].crAmount;
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

QString CounterPrinterHelper::  getPayVHTML(GeneralVoucherDataObject *obj)
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
    QString POSNumber=CounterSettingsDataModel::POSNumber;

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
        html += "<tr><td><center><font size =\"4\">TIN : "+trnNo+"</font></center>";


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

QString CounterPrinterHelper::getKOTHTML(GeneralVoucherDataObject *obj, int *count,
                                         bool running, QString printerName, QString heading,
                                         bool printAll)
{
    QString html ="";
    //    qDebug()<<"588 for "<<printerName;
    //     qDebug()<<"Start html ";

    QString CompanyName = LoginValues::getCompany().CompanyName;
    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
    QString POSNumber="";
    //    qDebug()<<"599";
    QString vType = "Ord";

    QString line ="<tr><td colspan =\"4\"><hr></td></tr>";

    html += "<html><body>";
    html += "<table width =\"100%\"><tr>";
    int f = ConfigurationSettingsDatabaseHelper::getValue(kotCompanyName,1).toInt();
    if(f == 1)
        html += "<tr><td><center><font size =\"6\">"+CompanyName+"</font></center>";
    //    qDebug()<<"Mod service : "<<obj->ModeOfService;
    html += "<tr><td><center><font size =\"6\">"+ServiceMode::getStatusString(obj->ModeOfService)+"</font></center>";

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        html += "<tr><td><center><font size =\"4\">GATE PASS</font></center>";
    }
    else{

        if(heading.length()>0)
            html += "<tr><td><center><font size =\"6\">"+heading+"</font></center>";
        if(running)
            html += "<tr><td><center><font size =\"4\">Running Order</font></center>";
    }
    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";

    html += "<table width =\"100%\">";
    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){

    }
    else{
        html += "<tr><td width=\"20%\">"+vType+" No :</td>";
        html += "<td width=\"30%\"><b>"+(multiVoucher?obj->VoucherPrefix+" - ":"")+obj->voucherNumber+"</td>";
        html += "<td width=\"25%\">KOT # :</td>";
        html += "<td>"+obj->kotNumber+"</td></tr>";
    }

    html += "<tr><td>V Date :</td>";
    html += "<td>"+obj->VoucherDate.toString("dd-MM-yyyy")+"</td>";
    html += "<td >Time :</td>";

    QString timePrintSizeVal = ConfigurationSettingsDatabaseHelper::getValue(timePrintSize,"4").toString();

    if(obj->timestamp.toString("hh:mm:ss").length()>0)
        html += "<td> <font size =\""+timePrintSizeVal+"\">"+obj->timestamp.toString("hh:mm ap")+"</td></tr>";
    else
        html += "<td> <font size =\""+timePrintSizeVal+"\">"+QDateTime::currentDateTime().toString("hh:mm")+"</td></tr>";

    html += "</table>";
    html += "<table width=\"100%\">";
    //     qDebug()<<"Start html 99";
    html += line;
    html += "<tr>";
    int kotPrintformat = ConfigurationSettingsDatabaseHelper::getValue(kotFormat,1).toInt();
    //    kotPrintformat =2;
    if(kotPrintformat==1)
    {
        html += "<th align=\"left\" width =\"5%\">#<\th>";
        html += "<th align=\"left\" width =\"85%\">Item Name<\th>";
        html += "<th align=\"left\" width =\"10%\">Qty<\th>";
    }
    else if(kotPrintformat == 2)
    {
        html += "<th align=\"left\" width =\"20%\">Qty<\th>";
        html += "<th align=\"left\" width =\"80%\">Item Name<\th>";
    }
    else if(kotPrintformat == 3){
        html += "<th align=\"left\" width =\"80%\">Item Name<\th>";
        html += "<th align=\"left\" width =\"20%\">Qty<\th>";
    }

    html += "</tr>";
    //    qDebug()<<"639";
    *count  =0;
    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    QString kotItemsFontSizeVal
        = ConfigurationSettingsDatabaseHelper::getValue(kotItemsFontSize,"4").toString();

    for(int i=0;i<obj->InventoryItems.size();i++){
        QList<QString> printersList = obj->InventoryItems[i].BaseItem.KOTPrinter.split("|");
        //        qDebug()<<"Kot Printer of "<<obj->InventoryItems[i].BaseItem.ItemName<<obj->InventoryItems[i].BaseItem.KOTPrinter
        //               <<obj->InventoryItems[i].BaseItem.listId<< obj->InventoryItems[i].BaseItem.ItemReqUuid ;
        if( printAll || ((obj->InventoryItems[i].BaseItem.listId == 0 || obj->InventoryItems[i].BaseItem.ItemReqUuid == "X")
                         && printersList.contains(printerName))){
            *count = 1;
            html += "<tr>";
            if(kotPrintformat==1)
            {
                html += "<td>";
                html += QString::number(i+1);
                html += "</td>";
                //            qDebug()<<"646";
                html += "<td colspan=\"2\"> <font size=\""+kotItemsFontSizeVal+"\">";
                html += obj->InventoryItems[i].BaseItem.ItemName;
                if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                    html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;
                if(obj->InventoryItems[i].BaseItem.narration.length()>0)
                {
                    obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
                    html += "<br><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
                html += "</td>";
                html += "<td align=\"left\"><font size=\""+kotItemsFontSizeVal+"\"><b>";
                html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',
                                        obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
                html += "</td>";
            }
            else if(kotPrintformat==2){
                html += "<td align=\"left\"><font size=\""+kotItemsFontSizeVal+"\">";
                html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',
                                        obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
                html += "</td>";
                html += "<td colspan=\"2\"> <font size=\""+kotItemsFontSizeVal+"\">";
                html += obj->InventoryItems[i].BaseItem.ItemName;
                if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                    html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;
                if(obj->InventoryItems[i].BaseItem.narration.length() > 0){
                    obj->InventoryItems[i].BaseItem.narration.replace("\n","<br><i>");
                    html += "<br><font size=\"4\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
                html += "</td>";
            }
            else if(kotPrintformat==3){
                html += "<td colspan=\"2\"> <font size=\""+kotItemsFontSizeVal+"\">";
                html += obj->InventoryItems[i].BaseItem.ItemName;
                if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
                    html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;
                if(obj->InventoryItems[i].BaseItem.narration.length()>0){
                    obj->InventoryItems[i].BaseItem.narration.replace("\n","<br><i>");
                    html += "<br><font size=\"4\"><i>" + obj->InventoryItems[i].BaseItem.narration;
                }
                html += "</td>";
                html += "<td align=\"left\"><font size=\""+kotItemsFontSizeVal+"\">";
                html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',
                                        obj->InventoryItems[i].BaseItem.uomObject.decimalPoints);
                html += "</td>";
            }
            html += "</tr>";
        }
    }
    //        qDebug()<<"668"<<html;
    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    html += "</table>";

    html += "<table width =\"100%\">";
    //    html += "<tr><td colspan='2' width=\"25%\">Printed Date Time :</td>";
    //    html += "<td colspan='2' width=\"75%\"><font size =\""+timePrintSizeVal+"\">"+QDateTime::currentDateTime().toString("dd-MM-yy hh:mm:ss")+"</td>";
    //    html += "</tr>";

    html += "<tr><td width=\"15%\">Cashier:</td>";
    html += "<td>"+cashierName+"</td>";

    html += "<td width=\"18%\">Waiter :</td>";
    html += "<td>"+ userHelper->getEmployeeNameFromID(obj->SalesmanID)+"</td>";
    html += "</tr>";
    QString refStringVal = ConfigurationSettingsDatabaseHelper::getValue(refString,"Reference").toString();
    QString fSize = ConfigurationSettingsDatabaseHelper::getValue(refStringSize,"6").toString();
    html += "<tr><td width=\"15%\">"+refStringVal+" :</td>";
    html += "<td width=\"85%\"><font size="+fSize+">"+obj->reference;

    QString addrSize = ConfigurationSettingsDatabaseHelper::getValue(kotAddrPrintSize,"5").toString();
    if(printAddressInKotFlag)
    {
        if(obj->Contact.ContactName.length()>0)
            html += "<tr><td colspan=\"4\"><font size="+addrSize+"> Name : "+obj->Contact.ContactName +"</td>";
        if(add1Rout2 == 2)
            if(obj->Contact.route.length()>0)
                html += "<tr><td colspan=\"4\"><font size="+addrSize+"> Route : "+obj->Contact.route +"</td>";
        if(add1Rout2 == 1)
            if(obj->Contact.address.length()>0)
                html += "<tr><td colspan=\"4\"><font size="+addrSize+">   "+obj->Contact.address.split("<br")[0] +"</td>";

        html += "</td>";
    }

    if(obj->ledgerObject.LedgerGroupId.contains("0x5x19",Qt::CaseInsensitive)
        || obj->ledgerObject.LedgerGroupId.contains("0x2x15",Qt::CaseInsensitive))
    {
        html += "<tr><td width=\"15%\">To :</td>";
        html += "<td><font size=5 align='right'>"+obj->ledgerObject.LedgerName+"</td>";
        if(obj->ledgerObject.PoBox.length()>0){
            html += "<tr><td width=\"15%\">To :</td>";
            html += "<td><font size=5 align='right'>"+obj->ledgerObject.PoBox+"</td>";
        }
    }
    //    if(obj->ModeOfService<3){
    //        html +="<tr><td colspan=\"2\">Pick Up : ";
    //        html += "<font size=5><b>"+obj->DeliveryDate.toString("hh:mm ap");
    //    }
    if(obj->narration.length()>0)
    {
        html += "<tr><td colspan =\"4\"><font size="+fSize+">" + kotNarration +" : "+obj->narration.replace("\n","<br>")+"</font></td></tr>";
    }


    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    html += "</table>";
    qDebug()<<"______";
    qDebug()<<html;
    qDebug()<<"______";
    return html;
}

QString CounterPrinterHelper::getShiftRow(QString head, QString CR, QString DR)
{
    QString html ="";
    html += "<tr>";

    html += "<td width =\"50%\">";
    html += head;
    html += "</td>";

    html += "<td width =\"25%\" align=\"right\">";
    html += CR;
    html += "</td>";

    html += "<td width =\"25%\" align=\"right\">";
    html += DR;
    html += "</td>";

    html += "</tr>";


    return html;
}

QString CounterPrinterHelper::getLedgerRow(QDate date, QString billNo, float dr,float cr)
{
    QString html ="";
    html += "<tr>";

    html += "<td width =\"20%\">";
    html += date.toString("dd-MMM");
    html += "</td>";

    html += "<td width =\"20%\">";
    html += billNo;
    html += "</td>";

    html += "<td width =\"30%\" align=\"right\">";
    if(dr>0)
        html += QString::number(dr,'f',2);
    html += "</td>";

    html += "<td width =\"30%\" align=\"right\">";
    if(cr>0)
        html += QString::number(cr,'f',2);
    html += "</td>";

    html += "</tr>";


    return html;
}

QString CounterPrinterHelper::getLedgerRowHeader(QString date, QString billNo,QString dr,QString cr)
{
    QString html ="";
    html += "<tr>";

    html += "<td width =\"20%\">";
    html += date;
    html += "</td>";

    html += "<td width =\"20%\">";
    html += billNo;
    html += "</td>";

    html += "<td width =\"30%\" align=\"right\">";
    html += dr;
    html += "</td>";

    html += "<td width =\"30%\" align=\"right\">";
    html += cr;
    html += "</td>";

    html += "</tr>";


    return html;
}

void CounterPrinterHelper::printDaybookReport(ShiftDataModel *Shift,
                                              GeneralVoucherDataObject payments,
                                              GeneralVoucherDataObject receipts,
                                              GeneralVoucherDataObject sales,
                                              GeneralVoucherDataObject purchases)
{
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = "";


    html += "<html><body>";
    html += "<table width =\"100%\">";
    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">Day Book</font></td><tr>";
    html += "<tr><td>";

    html +="<table width=\"100%\">";


    html += getShiftRow("Date From",Shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Date To",Shift->ShiftEndDateTime.toString("dd-MM-yyyy hh:mm"));
    html += getShiftRow("Bill From",Shift->BillsFrom);
    html += getShiftRow("Bill To",Shift->BillsTo);
    html += getShiftRow("No. of Bills",QString::number(Shift->billsCount));
    html += getShiftRow("Opening Cash",QString::number(Shift->OpeningCash,'f',2));

    html += getShiftRow("Sales Total",QString::number(Shift->SalesTotal,'f',2));
    html += getShiftRow("Sales By Cash",QString::number(Shift->CashSales,'f',2));
    html += getShiftRow("Sales By Card",QString::number(Shift->CardSales,'f',2));
    html += getShiftRow("Credit Sales",QString::number(Shift->CreditSales,'f',2));

    html += getShiftRow("Taxable Sales",QString::number(Shift->taxableTotal,'f',2));
    html += getShiftRow("Vat @ 5 %",QString::number(Shift->vatTotal,'f',2));


    if(Shift->ZomatoSales>0){
        html += getShiftRow("Zomato Sales",QString::number(Shift->ZomatoSales,'f',2));
        html += getShiftRow("Zomato Cash",QString::number(Shift->ZomatoCash,'f',2));
        html += getShiftRow("Zomato Online",QString::number(Shift->ZomatoCredit,'f',2));
    }
    if(sales.ledgersList.size()>0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += "<tr><td colspan =\"3\"><font size=\"5\"><u>Credit Sales<u></td></tr>";
        for(int i=0;i<sales.ledgersList.size();i++){
            QString name = sales.ledgersList[i].LedgerName;
            QString amt  = QString::number(sales.ledgersList[i].drAmount,'f',2);
            html += getShiftRow(name,amt);
        }

    }
    html += "<tr><td colspan =\"3\"><hr></td></tr>";


    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Purchase Total",QString::number(Shift->PurchaseTotal,'f',2));
    html += getShiftRow("Purchase By Cash",QString::number(Shift->CashPurchase,'f',2));
    if(purchases.ledgersList.size()>0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += "<tr><td colspan =\"3\"><font size=\"5\"><u>Purchases<u></td></tr>";
        for(int i=0;i<purchases.ledgersList.size();i++){
            QString name = purchases.ledgersList[i].LedgerName;
            QString amt  = QString::number(purchases.ledgersList[i].crAmount,'f',2);
            html += getShiftRow(name,amt);
        }
    }
    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Payments Total",QString::number(Shift->PaymentsTotal,'f',2));
    html += getShiftRow("Payments in Cash",QString::number(Shift->CashPayments,'f',2));
    if(payments.ledgersList.size()>0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += "<tr><td colspan =\"3\"><font size=\"5\"><u>Payments<u></td></tr>";
        for(int i=0;i<payments.ledgersList.size();i++){
            QString name = payments.ledgersList[i].LedgerName;
            QString amt  = QString::number(payments.ledgersList[i].crAmount,'f',2);
            html += getShiftRow(name,amt);
        }

    }

    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += getShiftRow("Receipt Total",QString::number(Shift->ReceiptTotal,'f',2));
    html += getShiftRow("Receipt in Cash",QString::number(Shift->CashReceipts,'f',2));
    if(receipts.ledgersList.size()>0){
        html += "<tr><td colspan =\"3\"><hr></td></tr>";
        html += "<tr><td colspan =\"3\"><font size=\"5\"><u>Receipts<u></td></tr>";
        for(int i=0;i<receipts.ledgersList.size();i++){
            QString name = receipts.ledgersList[i].LedgerName;
            QString amt  = QString::number(receipts.ledgersList[i].drAmount,'f',2);
            html += getShiftRow(name,amt);
        }
    }

    html += "<tr><td colspan =\"3\"><hr></td></tr>";
    html += "<tr><td colspan =\"3\"><hr></td></tr>";

    html += getShiftRow("<font size=\"5\">Closing Cash","<font size=\"5\">"+QString::number(Shift->CashBalance,'f',2));

    html += "<tr><td colspan =\"3\"><hr></td></tr><br><br>";


    doc.setHtml(html);



    //    QPrinter printer(QPrinter::ScreenResolution) ;
    //    QPrintDialog pDial(&printer,this);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  ;
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height());
    doc.setPageSize(paperSize);
    doc.print(printer);

}

void CounterPrinterHelper::printPO(GeneralVoucherDataObject *obj, QString printerName)
{
    qDebug()<<"start print";
    int flag =1;
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = getPOHTML(obj);
    qDebug()<<"got as HTML";
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution);
    qDebug()<<"Printer Setting name";

    printer.setPrinterName(printerName);
    qDebug()<<"Printer Name set";

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    doc.print(&printer);

    qDebug()<<"deleting doc";

    qDebug()<<"Print  complete";
}

void CounterPrinterHelper::printPLU(QString html,QString printerName)
{
    qDebug()<<"start print";
    int flag =1;
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);


    qDebug()<<"got as HTML";
    doc.setHtml(html);
    QPrinter printer(QPrinter::ScreenResolution);
    qDebug()<<"Printer Setting name";

    printer.setPrinterName(printerName);
    qDebug()<<"Printer Name set";

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);

    doc.print(&printer);

    qDebug()<<"deleting doc";

    qDebug()<<"Print  complete";
}

QString CounterPrinterHelper::getPOHTML(GeneralVoucherDataObject *obj)
{
    QString html ="";

    //     qDebug()<<"Start html ";
    QString CompanyName = LoginValues::getCompany().CompanyName;

    QString branch = LoginValues::getCompany().branch;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString trnNo = LoginValues::getCompany().trn;
    QString phoneNumber = LoginValues::getCompany().PhoneNumber;
    QString cashierName= LoginValues::userName;
    QString POSNumber="";
    qDebug()<<"599";
    QString vType = "Purchase Order";

    html += "<html><body>";
    html += "<table width =\"100%\"><tr>";

    html += "<tr><td><center><font size =\"6\">"+CompanyName+"</font></center>";
    html += "<tr><td><center><font size =\"4\">"+branch+"</font></center>";



    html += "<tr></tr>";
    html += "</table>";

    //     qDebug()<<"Start html 80";

    html += "<table width =\"100%\">";

    html += "<tr><td colspan='4' align='center'><font size =\"4\"><u><b>"+vType+"</td></tr>";

    html += "<tr><td width=\"15%\"Voucher> No :</td>";
    html += "<td>"+obj->voucherNumber+"</td>";

    //    html += "<td width=\"15%\">POS # :</td>";
    //    html += "<td>"+POSNumber+"</td></tr>";
    html += "</tr>";

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
    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    html += "<tr>";
    //    html += "<th align=\"left\" width =\"5%\">#<\th>";
    html += "<th align=\"left\" width =\"85%\">Item Name<\th>";
    html += "<th align=\"left\" width =\"10%\">Qty<\th>";

    html += "</tr>";
    qDebug()<<"639";

    html += "<tr><td colspan =\"4\"><hr></td></tr>";

    for(int i=0;i<obj->InventoryItems.size();i++){

        html += "<tr>";
        qDebug()<<"646";

        html += "<td> <font size=\"4\">";
        html += obj->InventoryItems[i].BaseItem.ItemName;
        if(obj->InventoryItems[i].BaseItem.itemNameArabic.length()>0)
            html += "<br>" + obj->InventoryItems[i].BaseItem.itemNameArabic;
        //            if(obj->InventoryItems[i].BaseItem.narration.length()>0){
        //                obj->InventoryItems[i].BaseItem.narration.replace("\n","<br>");
        //                html += "<br>" + obj->InventoryItems[i].BaseItem.narration;
        //            }

        html += "</td>";
        html += "<td align=\"right\"><font size=\"4\">";
        html += QString::number(obj->InventoryItems[i].BaseItem.quantity,'f',2);
        html += "</td>";
        html += "</tr>";

    }

    qDebug()<<"668";
    html += "<tr></tr><tr></tr><tr><td colspan =\"4\"><hr></td></tr>";


    html += "</table>";

    html += "<table width =\"100%\">";

    html += "<tr><td width=\"100%\">Narration:</td></tr>";
    html += "<tr><td width=\"100%\">"+obj->narration.replace("\n","<br>")+"</td></tr>";

    //    html += "<td width=\"18%\">Waiter :</td>";
    //    html += "<td>"+ userHelper->getEmployeeNameFromID(obj->SalesmanID)+"</td>";
    //    html += "</tr>";


    html += "<tr><td colspan ='4'><hr></td></tr>";
    html += "</table>";

    qDebug()<<"688 html return";
    return html;
}




//void CounterPrinterHelper::printLedgerSimple(QString ledName,QTableView *table, QDate dateFrom, QDate dateTo,
//                                             float openingBal,float closingBal)
//{
//    QTextDocument doc ;
//    //    getBillasTextDocument(doc,obj,printerName);

//    QString html = "";
//    html += "<html><body>";
//    html += "<table width =\"100%\">";
//    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
//    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
//    html += "<tr><td align=\"center\"><font size=\"5\">Ledger :"+ledName+"</font></td><tr>";
//    html += "<tr><td align=\"center\"><font size=\"5\">Date :"+dateFrom.toString("dd-MMM-yyyy")+" to "
//            +dateTo.toString("dd-MMM-yyyy")+"</font></td><tr>";

//    html += "<tr><td>";

//    html +="<table width=\"100%\">";

//    html += "<tr><td colspan =\"4\"><hr></td></tr>";
//    html += getLedgerRowHeader("<b>Date","<b>Bill No","<b>Dr","<b>Cr");
//    html += "<tr><td colspan =\"4\"><hr></td></tr>";
//    if(openingBal>0)
//        html += getLedgerRowHeader("Opening","",QString::number(openingBal,'f',2),"");
//    else{

//        html += getLedgerRowHeader("Opening","","",QString::number(openingBal,'f',2));
//    }
//    //    html += getShiftRow("Shift From",Shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));



//    for(int i=0;i<table->model()->rowCount();i++){
//        QDate date = table->model()->index(i,1).data().toDate();
//        QString vNO = table->model()->index(i,2).data().toString();
//        QString vType = table->model()->index(i,3).data().toString();
//        float dr = table->model()->index(i,4).data().toFloat();
//        float cr = table->model()->index(i,5).data().toFloat();

//        html += getLedgerRow(date,vType+"-"+vNO.mid(0,4),dr,cr);
//    }
//    html += "<tr><td colspan =\"4\"><hr></td></tr>";
//    if(closingBal>0)
//        html += getLedgerRowHeader("Closing ","",QString::number(closingBal,'f',2),"");
//    else{

//        html += getLedgerRowHeader("Closing","","",QString::number(closingBal,'f',2));
//    }
//    html += "<tr><td colspan =\"4\"><hr></td></tr>";
//    doc.setHtml(html);


//    //    QPrinter printer(QPrinter::ScreenResolution) ;
//    printer->setPrinterName("printerName");
//    QFont font;
//    font.setFamily("Avant Garde");
//    font.setPointSize(8);


//    doc.setDefaultFont(font);
//    QSizeF paperSize;
//    paperSize.setWidth(printer->width());
//    paperSize.setHeight(printer->height());
//    doc.setPageSize(paperSize);
//    doc.print(printer);
//    qDebug()<<"8888";

//}

void CounterPrinterHelper::printLedgerSimple(QString ledName,QTableView *table, QDate dateFrom, QDate dateTo,
                                             float openingBal,float closingBal)
{
    bool printBillwise = false; // ConfigurationSettingsDatabaseHelper::getValue(showBillwiseInLedgerReport,false).toBool();
    BillwiseRecordsDatabaseHelper *billHelper = new BillwiseRecordsDatabaseHelper;
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = "";
    html += "<html><body>";
    html += "<table width =\"100%\">";
    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">Ledger :"+ledName+"</font></td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">Date :"+dateFrom.toString("dd-MMM-yyyy")+" to "
            +dateTo.toString("dd-MMM-yyyy")+"</font></td><tr>";

    html += "<tr><td>";

    html +="<table width=\"100%\">";

    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    html += getLedgerRowHeader("<b>Date","<b>Bill No","<b>Dr","<b>Cr");
    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    if(openingBal>0)
        html += getLedgerRowHeader("Opening","",QString::number(openingBal,'f',2),"");
    else{

        html += getLedgerRowHeader("Opening","","",QString::number(-1*openingBal,'f',2));
    }
    //    html += getShiftRow("Shift From",Shift->ShiftStartDateTime.toString("dd-MM-yyyy hh:mm"));



    for(int i=0;i<table->model()->rowCount();i++){
        //        QDate date = table->model()->index(i,1).data().toDate();
        //        QString vNO = table->model()->index(i,2).data().toString();
        //        QString vType = table->model()->index(i,3).data().toString();
        //        float dr = table->model()->index(i,4).data().toFloat();
        //        float cr = table->model()->index(i,5).data().toFloat();

        QDate date = table->model()->index(i,5).data().toDate();
        QString vNO = table->model()->index(i,1).data().toString();
        QString vType = table->model()->index(i,3).data().toString();
        QString vPrefix = table->model()->index(i,2).data().toString();
        float dr = table->model()->index(i,8).data().toFloat();
        float cr = table->model()->index(i,9).data().toFloat();

        html += getLedgerRow(date,vType+"-"+vNO.mid(0,4),dr,cr);
        if(printBillwise)
        {
            QSqlQueryModel *model = billHelper->getBillwiseMappingByVoucher(vNO, vType, vPrefix);
            if(model->rowCount() > 0)
                //                html += "<tr><td width=\"50%\">";
                html += getLedgerMapRow(model);
            //                html += "</td></tr>";
        }
    }
    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    if(closingBal<0)
        html += getLedgerRowHeader("Closing ","",QString::number(-1*closingBal,'f',2)+" Dr","");
    else{
        html += getLedgerRowHeader("Closing","","",QString::number(closingBal,'f',2)+" Cr");
    }

    html += "<tr><td colspan =\"4\"><hr></td></tr>";
    doc.setHtml(html);

    //    QPrinter printer(QPrinter::ScreenResolution) ;
    printer->setPrinterName("printerName");
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height());
    doc.setPageSize(paperSize);
    doc.print(printer);
    qDebug()<<"8888";

}


QString CounterPrinterHelper::getLedgerMapRow(QSqlQueryModel *model)
{
    QString html ;

    for(int i=0; i< model->rowCount(); i++){
        QDate date = model->record(i).value(0).toDate();
        QString vNo = model->record(i).value(1).toString() ;
        QString vType = model->record(i).value(2).toString() ;
        QString amount = model->record(i).value(4).toString();
        html += "<tr>";
        html += "<td ></td>";

        html += "<td width =\"20%\">";
        html += date.toString("dd-MMM");
        html += "</td>";

        html += "<td width =\"20%\">";
        html += vType + "-" + vNo;
        html += "</td>";

        html += "<td width =\"20%\" align=\"right\">";
        html += amount;
        html += "</td>";



        html += "</tr>";
    }

    return html;
}



void CounterPrinterHelper::printSettlementReport(QTableView *table, QString salesMan, float Total)
{

    QString text1 = LoginValues::getCompany().CompanyName;
    QString text2 = LoginValues::getCompany().branch;
    QString text3 = LoginValues::getCompany().addressLine;
    QString text4 = LoginValues::getCompany().PhoneNumber;

    QDate d = QDate::currentDate();
    QString dText = d.toString("dd-MMM-yyyy");
    QTime t = QTime::currentTime();
    QString tText = t.toString("hh:mm:ss");

    QPrinter printer(QPrinter::ScreenResolution) ;
    //printer.setOrientation(QPrinter::Portrait);
    printer.setPrinterName("POS-80");

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QTextDocument doc;
    //"<center>"+text2+"&trade;</center>"
    QString html = "<html><body>"
                   "<center>"+
                   text1+"</center>"
                           "<center>"+
                   text2+"</center>"
                           "<center>"+
                   text3+"</center>"
                           "<center>"+
                   text4+"</center>"
                           "------------------------------------------------------------------<br>"
                           "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Date :"+
                   dText+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                   "Time :"+tText+"<br>"
                                      "------------------------------------------------------------------"
                                      "<center>Delivery Orders Report</center><br>"
                                      "------------------------------------------------------------------"

                                      "<table><tr>"
                                      "<th align=left width=\"20%\">Voucher #</th>"
                                      "<th align=left width=\"60%\">Address</th>"
                                      "<th align=right width=\"20%\">Amount</th>"
                                      "</tr>";
    for(int i=0;i<table->model()-> rowCount();i++)
    {
        QString voucherNo = table->model()->data(table->model()->index(i,2),Qt::DisplayRole).toString();
        QString Address = table->model()->data(table->model()->index(i,11),Qt::DisplayRole).toString();
        QString Amount = QString::number(table->model()->data(table->model()->index(i,10),Qt::DisplayRole).toFloat(),'f',2);

        html += "<tr>";
        html += "<td align =\"left\">";
        html += voucherNo;
        html += "</td>";

        html += "<td align =\"left\">";
        html += Address;
        html += "</td>";

        html += "<td align =\"right\">";
        html += Amount;
        html += "</td>";

        html += "</tr>";


    }
    html +="<tr><td></td><td><td align =\"right\">"+QString::number(Total,'f',2) +"</td>";
    html+=
        "</table>"

        "------------------------------------------------------------------";


    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);


    doc.setHtml(html);
    doc.print(&printer);

}

void CounterPrinterHelper::printSettlementReportWithCheck(QTableView *table, QString salesMan,
                                                          float Total,QList<int> sel)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    QString text1 = LoginValues::getCompany().CompanyName;
    QString text2 = LoginValues::getCompany().branch;
    QString text3 = LoginValues::getCompany().addressLine;
    QString text4 = LoginValues::getCompany().PhoneNumber;

    QDate d = QDate::currentDate();
    QString dText = d.toString("dd-MMM-yyyy");
    QTime t = QTime::currentTime();
    QString tText = t.toString("hh:mm:ss");

    QPrinter printer(QPrinter::ScreenResolution) ;
    //printer.setOrientation(QPrinter::Portrait);
    printer.setPrinterName("abc");

    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    QString itemname;
    QString print_qty;
    QString print_amount;
    QTextDocument doc;
    //"<center>"+text2+"&trade;</center>"
    QString html = "<html><body>"
                   "<center>"+
                   text1+"</center>"
                           "<center>"+
                   text2+"</center>"
                           "<center>"+
                   text3+"</center>"
                           "<center>"+
                   text4+"</center>"
                           "------------------------------------------------------------------<br>"
                           "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Date :"+
                   dText+"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+
                   "Time :"+tText+"<br>"
                                      "------------------------------------------------------------------"
                                      "<center>Sales Bills Settled</center><br>"
                                      "------------------------------------------------------------------"

                                      "<table><tr>"
                                      "<th align=left width=\"20%\">Voucher #</th>"
                                      "<th align=left width=\"60%\">Address</th>"
                                      "<th align=right width=\"20%\">Amount</th>"
                                      "</tr>";
    for(int i=0;i<table->model()-> rowCount();i++)
    {
        QString voucherNo = table->model()->data(table->model()->index(i,2),Qt::DisplayRole).toString();
        QString Address = table->model()->data(table->model()->index(i,11),Qt::DisplayRole).toString();
        QString Amount = QString::number(table->model()->data(table->model()->index(i,10),Qt::DisplayRole).toFloat(),'f',2);
        if(!sel.contains(i))
            continue;

        html += "<tr>";
        html += "<td align =\"left\">";
        html += voucherNo;
        html += "</td>";

        html += "<td align =\"left\">";
        html += Address;
        html += "</td>";

        html += "<td align =\"right\">";
        html += Amount;
        html += "</td>";

        html += "</tr>";


    }
    qDebug()<<Q_FUNC_INFO<<__LINE__;




    qDebug()<<Q_FUNC_INFO<<__LINE__;

    html +="<tr><td></td><td></td><td align =\"right\">"+QString::number(Total,'f',2) +"</td>";
    html+=
        "</table>"

        "------------------------------------------------------------------";


    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);


    doc.setHtml(html);
    doc.print(&printer);

    qDebug()<<Q_FUNC_INFO<<__LINE__;

}



QString getReportHTML(QTableView *table,QList<int> widthlist,QString ReportName,QDate FromDate,QDate toDate){

    QString html = "<html><body>";

    html +="<table>";

    html += "<tr>";

    for(int i=0;i<widthlist.size();i++){
        html += "<th width=\""+QString::number(widthlist[i])+"%\">";
        html += table->model()->headerData(i,Qt::Horizontal).toString();
        html += "</th>";
    }
    html +="</tr>";

}



void CounterPrinterHelper::printReportGeneral(QTableView *table, QList<int> widthlist, QString ReportName,
                                              QDate FromDate, QDate toDate, QList<int> Allignment,
                                              QStringList extraTop,QStringList extraBottom,QList<int> avlCols)
{
    //    QPrinter printer(QPrinter::ScreenResolution) ;
    //    QPrintDialog pDial(&printer,this);
    //    if(pDial.exec()==QDialog::Rejected)
    //        return  ;



    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    QTextDocument doc;
    QTextCursor cursor(&doc);

    addGenralReportTable(&cursor,table,widthlist,ReportName,FromDate,toDate,Allignment,extraTop,extraBottom,avlCols);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height());
    doc.setPageSize(paperSize);



    //    doc.setHtml(html);
    doc.print(printer);

}

void CounterPrinterHelper::printReportGeneralwithModel(QSqlQueryModel *table, QList<int> widthlist, QString ReportName, QDate FromDate, QDate toDate, QList<int> Allignment, QStringList extraTop, QStringList extraBottom, QList<int> avlCols)
{
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);

    QTextDocument doc;
    QTextCursor cursor(&doc);

    addGenralReportTableModel(&cursor,table,widthlist,ReportName,FromDate,toDate,Allignment,extraTop,extraBottom,avlCols);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer->width());
    paperSize.setHeight(printer->height());
    doc.setPageSize(paperSize);

    //    doc.setHtml(html);
    doc.print(printer);
}

void CounterPrinterHelper::addGenralReportTable(QTextCursor *cursor, QTableView *table,
                                                QList<int> widthlist, QString ReportName,
                                                QDate FromDate, QDate toDate, QList<int> Allignment,
                                                QStringList extraTop, QStringList extraBottom, QList<int> avlCols)
{


    const int columns = avlCols.size();// table->model()->columnCount();

    const int rows = table->model()->rowCount();

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setHeaderRowCount( 5 );
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);


    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);


    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);

    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);


    QString companyName = LoginValues::getCompany().CompanyName;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString branch = LoginValues::getCompany().branch;

    addressLine = branch+", "+addressLine;

    QString dateLine = QString("Date : " +FromDate.toString("dd-MM-yyyy") + " to "+ toDate.toString("dd-MM-yyyy"));
    QString ReportLine = ReportName;

    QVector<QTextLength> constraints;

    for(int i=0;i<widthlist.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthlist[i]);
        qDebug()<<"Widdth"<<i<<widthlist.at(i);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormat.setColumnWidthConstraints(constraints);
    //    tableFormat.setHeight(QTextLength(QTextLength::FixedLength, 15));

    QTextTable* textTable = cursor->insertTable( rows + 7,
                                                columns,
                                                tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#DADADA" ) );
    tableHeaderFormat.setFontPointSize(11);

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


    for( int column = 0; column < columns; column++ ) {

        QString data = table->model()->headerData(avlCols[column],Qt::Horizontal).toString();
        QTextTableCell cell = textTable->cellAt( 4, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat( tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( data);
    }


    QTextBlockFormat format = centerAlignment;



    //    for(int i =0;i<widthlist.size();i++){
    //        int column = widthlist[i];
    //    //}

    int lineHeight = ConfigurationSettingsDatabaseHelper::getValue("reporLineHeight", 15).toInt();

    for( int column = 0; column < columns; column++ ) {

        qDebug()<<"Col : "<<column;
        if(Allignment[column]==0)
            format = leftAlignment;
        else if(Allignment[column]==1)
            format = rightAlignment;
        format.setLineHeight(lineHeight, QTextBlockFormat::FixedHeight);
        int row;
        for(row =0;row<rows;row++){
            qDebug()<<"Column : "<<column<<row;
            QModelIndex index = table->model()->index(row, avlCols[column], QModelIndex());

            QString data = table->model()->data(index).toString();

            //            qDebug()<<"Type :"<< table->model()->data(index).type();
            qDebug()<<data<<row<<column;
            QTextTableCell cell = textTable->cellAt( row + 5, column );
            Q_ASSERT( cell.isValid() );
            QTextCursor cellCursor = cell.firstCursorPosition();

            if(table->model()->data(index).type() == QVariant::Double){
                qDebug()<<"Converting to float"<<data;
                data = QString::number(data.toFloat(),'f',2);
            }
            cellCursor.setBlockFormat(format);
            const QString cellText = QString( data );
            cellCursor.insertText( cellText );

            //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);

        }

        // printTotal
        qDebug()<<"print bottom"<<row<<column<<extraBottom<<avlCols[column];
        qDebug()<<"Ex size :"<<extraBottom.length();

        if(extraBottom.size() >= column-1 && extraBottom.size() > 0){
            qDebug()<<"avl col of col"<<avlCols[column]<<extraBottom.size();
            qDebug()<<"avl col of col"<<extraBottom[avlCols[column]];
            QString data = extraBottom[avlCols[column]];
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            if(column ==0 && data == ""){
                qDebug()<<Q_FUNC_INFO<<__LINE__;
                data = "Total";
            }
            qDebug()<<Q_FUNC_INFO<<__LINE__;

            qDebug()<<"print"<<data;
            QTextTableCell cell = textTable->cellAt( row + 6, column );
            cell.setFormat(tableHeaderFormat);
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            Q_ASSERT( cell.isValid() );
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            QTextCursor cellCursor = cell.firstCursorPosition();
            qDebug()<<Q_FUNC_INFO<<__LINE__;

            //        if(data.toFloat() > 0){
            //            qDebug()<<"Converting to float"<<data;
            //            data = QString::number(data.toFloat(),'f',2);
            //        }
            cellCursor.setBlockFormat(format);
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            const QString cellText = QString( data );
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            cellCursor.insertText( cellText );
            qDebug()<<Q_FUNC_INFO<<__LINE__;
        }


    }
    cursor->movePosition( QTextCursor::End );
}

void CounterPrinterHelper::addGenralReportTableModel(QTextCursor *cursor, QSqlQueryModel *table, QList<int> widthlist, QString ReportName, QDate FromDate, QDate toDate, QList<int> Allignment, QStringList extraTop, QStringList extraBottom, QList<int> avlCols)
{
    const int columns = avlCols.size();// table->model()->columnCount();

    const int rows = table->rowCount();

    qDebug()<<"Rows Count = "<<rows;

    QTextTableFormat tableFormat;
    tableFormat.setBorder(0);
    tableFormat.setHeaderRowCount( 5 );
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);

    QTextBlockFormat centerAlignment;
    centerAlignment.setAlignment(Qt::AlignCenter);

    QTextBlockFormat rightAlignment;
    rightAlignment.setAlignment(Qt::AlignRight);

    QTextBlockFormat leftAlignment;
    leftAlignment.setAlignment(Qt::AlignLeft);


    QString companyName = LoginValues::getCompany().CompanyName;
    QString addressLine = LoginValues::getCompany().addressLine;
    QString branch = LoginValues::getCompany().branch;

    addressLine = branch+", "+addressLine;

    QString dateLine = QString("Date : " +FromDate.toString("dd-MM-yyyy") + " to "+ toDate.toString("dd-MM-yyyy"));
    QString ReportLine = ReportName;

    QVector<QTextLength> constraints;

    for(int i=0;i<widthlist.size();i++){
        constraints << QTextLength(QTextLength::PercentageLength, widthlist[i]);
        qDebug()<<"Widdth"<<i<<widthlist.at(i);

    }
    //    constraints << QTextLength(QTextLength::PercentageLength, 10)
    //                << QTextLength(QTextLength::PercentageLength, 50)
    //                << QTextLength(QTextLength::PercentageLength, 20)
    //                << QTextLength(QTextLength::PercentageLength, 20);


    tableFormat.setColumnWidthConstraints(constraints);

    QTextTable* textTable = cursor->insertTable( rows + 5,
                                                columns,
                                                tableFormat );
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground( QColor( "#DADADA" ) );
    tableHeaderFormat.setFontPointSize(11);

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


    for( int column = 0; column < columns; column++ ) {

        QString data = table->headerData(avlCols[column],Qt::Horizontal).toString();
        QTextTableCell cell = textTable->cellAt( 4, column );
        Q_ASSERT( cell.isValid() );
        cell.setFormat( tableHeaderFormat );

        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText( data);
    }


    QTextBlockFormat format = centerAlignment;

    qDebug()<<"Columns = "<<columns;
    qDebug()<<"Rows = "<<rows;

    for( int column = 0; column < columns; column++ ) {
        qDebug()<<"Col : "<<column;
        if(Allignment[column]==0)
            format = leftAlignment;
        else if(Allignment[column]==1)
            format = rightAlignment;

        for(int row =0;row<rows;row++){
            qDebug()<<"Column : "<<column<<row;
            qDebug()<<"2199";
            QModelIndex index = table->index(row, avlCols[column], QModelIndex());
            QString data = table->data(index).toString();
            //            qDebug()<<"Type :"<< table->model()->data(index).type();
            qDebug()<<data<<row<<column;
            QTextTableCell cell = textTable->cellAt( row + 5, column );
            Q_ASSERT( cell.isValid() );
            QTextCursor cellCursor = cell.firstCursorPosition();

            if(table->data(index).type() == QVariant::Double){
                qDebug()<<"Converting to float"<<data;
                data = QString::number(data.toFloat(),'f',2);
            }
            cellCursor.setBlockFormat(format);qDebug()<<"2211 END";
            const QString cellText = QString( data );
            cellCursor.insertText( cellText );
            qDebug()<<"2214 END";
            //        insertAlignedText(textTable,row+1,column+1,Qt::AlignRight,cellText);

        }
    }
    qDebug()<<"Done";
    cursor->movePosition( QTextCursor::End );
}


void CounterPrinterHelper::printExpenses(GeneralVoucherDataObject obj, bool isPDF)
{
    QTextDocument doc ;
    //    getBillasTextDocument(doc,obj,printerName);

    QString html = "";


    html += "<html><body>";
    html += "<table width =\"100%\">";
    html += "<tr><td align=\"center\"> <font size=\"6\">"+LoginValues::getCompany().CompanyName   + "</font></td><tr>";
    html += "<tr><td align=\"center\">"+LoginValues::getCompany().branch+"</td><tr>";
    html += "<tr><td align=\"center\"><font size=\"5\">Cash Payments</font></td><tr>";
    html += "<tr><td>";

    html +="<table width=\"100%\">";
    float total =0;

    html += "<tr>";
    html += "<th align=\"left\" width =\"60%\">Ledger<\th>";
    html += "<th align=\"left\" width =\"20%\"><\th>";
    html += "<th align=\"right\" width =\"20%\">Amount<\th>";

    html += "</tr>";

    for(int i=0;i<obj.ledgersList.size();i++){
        QString name = obj.ledgersList[i].LedgerName;
        QString amt  = QString::number(obj.ledgersList[i].crAmount,'f',2);
        total += obj.ledgersList[i].crAmount;
        html += getShiftRow(name,"",amt);
    }

    html += "<tr></tr><tr></tr><tr><td colspan =\"3\"><hr></td></tr>";

    html += getShiftRow("<b>Total</b>","","<b>"+QString::number(total,'f',2));

    html += "<tr></tr><tr></tr><tr><td colspan =\"3\"><hr></td></tr>";

    doc.setHtml(html);

    QPrinter printer(QPrinter::ScreenResolution) ;
    printer.setPrinterName("printerName");
    if(isPDF){
        printer.setOutputFormat(QPrinter::PdfFormat);
        QString fileName = QFileDialog::getSaveFileName(this,"File Name","Expenses_" + QDate::currentDate().toString("dd_MMM_yyyy"),tr("*.pdf"));
        printer.setOutputFileName(fileName);
    }
    QFont font;
    font.setFamily("Avant Garde");
    font.setPointSize(8);
    doc.setDefaultFont(font);
    QSizeF paperSize;
    paperSize.setWidth(printer.width());
    paperSize.setHeight(printer.height());
    doc.setPageSize(paperSize);
    doc.print(&printer);

}

void CounterPrinterHelper::openTillUSB(){

    QProcess process;
    process.start("c:\\openC.bat");
    process.waitForFinished();
    return;
}

void CounterPrinterHelper::openTillByCode(){


    bool f = ConfigurationSettingsDatabaseHelper::getValue(autoOpenTill,false).toBool();

    if(!f)
        return;

    QPrinter printer(QPrinter::ScreenResolution);
    QString printer_name = printer.printerName();

    QByteArray print_content_ba;
    print_content_ba.append(QChar(0x1B));
    print_content_ba.append(QChar(0x40));
    print_content_ba.append(QChar(0x1B));
    print_content_ba.append(QChar(0x70));
    print_content_ba.append(QChar(0x00));
    print_content_ba.append(QChar(0x19));
    print_content_ba.append(QChar(0xfa));

    HANDLE p_hPrinter;
    DOC_INFO_1 DocInfo;
    DWORD   dwJob = 0L;
    DWORD   dwBytesWritten = 0L;
    BOOL    bStatus = FALSE;

    //code to convert QString to wchar_t
    wchar_t szPrinterName[255];
    int length = printer_name.toWCharArray(szPrinterName);
    szPrinterName[length]=0;

    if (OpenPrinter(szPrinterName,&p_hPrinter,NULL)){
        qDebug() << "Printer opening success " << QString::fromWCharArray(szPrinterName);
        DocInfo.pDocName = L"Receipt";
        DocInfo.pOutputFile = NULL;
        DocInfo.pDatatype = L"RAW";
        dwJob = StartDocPrinter( p_hPrinter, 1, (LPBYTE)&DocInfo );
        if (dwJob > 0) {
            qDebug() << "Job is set.";
            bStatus = StartPagePrinter(p_hPrinter);
            if (bStatus) {
                qDebug() << "Writing text to printer" << print_content_ba ;
                bStatus = WritePrinter(p_hPrinter,print_content_ba.data(),print_content_ba.length(),&dwBytesWritten);
                if(bStatus > 0){
                    qDebug() << "printer write success" << bStatus;
                }
                EndPagePrinter(p_hPrinter);
            } else {
                qDebug() << "could not start printer";
            }
            EndDocPrinter(p_hPrinter);
            qDebug() << "closing doc";
        } else {
            qDebug() << "Couldn't create job";
        }
        ClosePrinter(p_hPrinter);
        qDebug() << "closing printer";
    }
    else{
        qDebug() << "Printer opening Failed";
    }

}
