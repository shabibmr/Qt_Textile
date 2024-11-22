#include "printdotmatrix.h"
#include <QSqlRecord>

PrintDotMatrix::PrintDotMatrix()
{

}

int i;

bool PrintDotMatrix::printLineDotMatrix(QPrinter &printer,QPainter *painter, QString &Str, int *x, int *y,QRect &ptr,
                                        Qt::Alignment alignment, int charLength, int FontSize){

    QFont font1 = QFont("Lucida Sans Typewriter");


    //Verdana Courier  Sans Serif
    qDebug()<<"104 = "<<mmToPixels(printer,104);

    int width = 104;
    int lineHeight = 10;

    font1.setStyleStrategy(QFont::PreferDevice);
    font1.setWeight(QFont::Normal);
    font1.setStyleHint(QFont::TypeWriter);
    font1.setPointSize(FontSize);
    font1.setLetterSpacing(QFont::AbsoluteSpacing,5);

    painter->setFont(font1);
    painter->drawText(*x, *y,  mmToPixels(printer,104),mmToPixels(printer,lineHeight),alignment, Str, &ptr);

    *y=ptr.bottom()+3;
}


bool PrintDotMatrix::printSalesVoucherForItemwise(GeneralVoucherDataObject *voucher)
{

    i=0;
    QPrinter printer;

    LedgerMasterDatabaseHelper l;


    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);

    QString companyName = LoginValues::getCompany().CompanyName;
    QString ledName    = "      Cust Name : "+voucher->ledgerObject.LedgerName;
    QString dateString = "      Date      : "+voucher->VoucherDate.toString("dd-MM-yyyy");
    QString vouchNoStr = "      Bill No   : "+voucher->voucherNumber;
    QString voucherType= "Sales Voucher";


    printLineDotMatrix(printer,&painter2,companyName,&x,&y,*ptr,Qt::AlignCenter,charCount,8);

    printLineDotMatrix(printer,&painter2,voucherType,&x,&y,*ptr,Qt::AlignCenter,charCount,7);
    printLineDotMatrix(printer,&painter2,dateString,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,vouchNoStr,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,ledName,&x,&y,*ptr,Qt::AlignLeft,charCount,7);

    QStringList headline;

    headline <<"Rate"<<"Ledger"<<"Qty"<<"Tray"<<"Amt";
    QString hhead = getColumnStringSalesVoucher(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    for(int i=0;i<voucher->InventoryItems.size();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2);
        Line<<l.getLedgerNameByID(voucher->InventoryItems[i].BaseItem.defaultSalesLedgerID);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.requestQty,'f',0);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.subTotal,'f',2);
        str = getColumnStringSalesVoucher(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);
    }

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    QString totalString = "Total : "+QString::number(voucher->grandTotal,'f',2);
    totalString = totalString.rightJustified(35);

    printLineDotMatrix(printer,&painter2,totalString,&x,&y,*ptr,Qt::AlignLeft,charCount,8);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    painter2.end();

}



bool PrintDotMatrix::printSalesVoucher(GeneralVoucherDataObject *voucher)
{

    i=0;
    QPrinter printer;

    LedgerMasterDatabaseHelper l;


    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);

    QString companyName = LoginValues::getCompany().CompanyName;
    QString ledName    = "      Cust Name : "+voucher->ledgerObject.LedgerName;
    QString dateString = "      Date      : "+voucher->VoucherDate.toString("dd-MM-yyyy");
    QString vouchNoStr = "      Bill No   : "+voucher->voucherNumber;
    QString voucherType= "Sales Voucher";


    printLineDotMatrix(printer,&painter2,companyName,&x,&y,*ptr,Qt::AlignCenter,charCount,8);

    printLineDotMatrix(printer,&painter2,voucherType,&x,&y,*ptr,Qt::AlignCenter,charCount,7);
    printLineDotMatrix(printer,&painter2,dateString,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,vouchNoStr,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,ledName,&x,&y,*ptr,Qt::AlignLeft,charCount,7);

    QStringList headline;

    headline <<"Rate"<<"Item"<<"Qty"<<"Tray"<<"Amt";
    QString hhead = getColumnStringSalesVoucher(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    for(int i=0;i<voucher->InventoryItems.size();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2);
        Line<<voucher->InventoryItems[i].BaseItem.ItemName;
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.requestQty,'f',0);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.subTotal,'f',2);
        str = getColumnStringSalesVoucher(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);
    }

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    QString totalString = "Total : "+QString::number(voucher->grandTotal,'f',2);
    totalString = totalString.rightJustified(35);

    printLineDotMatrix(printer,&painter2,totalString,&x,&y,*ptr,Qt::AlignLeft,charCount,8);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    painter2.end();

}



QString PrintDotMatrix::getColumnStringSalesVoucher(QStringList Line)
{
    QString line;
    QString val;
    line += Line[0].rightJustified  (6, ' ',true)+"  ";
    line += Line[1].leftJustified   (10,' ',true);
    line += Line[2].rightJustified  (6, ' ',true);
    line += Line[3].rightJustified  (6, ' ',true);
    line += Line[4].rightJustified  (7, ' ',true);

    return  line;
}

QString PrintDotMatrix::getColumnStringLedger(QStringList Line)
{
    QString line;
    QString val;

    line += Line[0] +"  ";
    line += Line[1].leftJustified   (6,' ',true);
    line += Line[2].rightJustified  (7, ' ',true);
    line += Line[3].rightJustified  (7, ' ',true);

    return  line;
}

QString PrintDotMatrix::getColumnStringItemwiseReport(QStringList Line)
{
    QString line;
    QString val;
    line += Line[0].rightJustified (6, ' ',true)+"  ";
    line += Line[1].leftJustified  (10,' ',true);
    line += Line[2].rightJustified (7, ' ',true);
    line += Line[3].rightJustified (4, ' ',true);
    line += Line[4].rightJustified (9, ' ',true);
    return  line;
}

void PrintDotMatrix::printReportHeading(QPrinter &printer,QPainter *painter,int *x, int *y,QRect &ptr,
                                        int charLength,
                                        QString reportName, QString dateString,QString lastLine)
{
    QString companyName = LoginValues::getCompany().CompanyName;
    printLineDotMatrix(printer,painter,companyName,x,y,ptr,Qt::AlignCenter,charLength,8);
    printLineDotMatrix(printer,painter,reportName,x,y,ptr,Qt::AlignCenter,charLength,7);
    if(dateString.length()>0)
        printLineDotMatrix(printer,painter,dateString,x,y,ptr,Qt::AlignLeft,charLength,7);
    if(lastLine.length()>0)
        printLineDotMatrix(printer,painter,lastLine,x,y,ptr,Qt::AlignLeft,charLength,7);

}



bool PrintDotMatrix::printGeneralReport(QSqlQueryModel *model, QDate fromDate, QDate toDate,
                                        QString reportname, QList<int> totalCols)
{
    i=0;
    QPrinter printer;

    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);

    QString companyName = LoginValues::getCompany().CompanyName;


    QString dateString = " From : "+fromDate.toString("dd-MM-yyyy")+ "  To : "+toDate.toString("dd-MM-yyyy");


    printReportHeading(printer,&painter2,&x,&y,*ptr,charCount,reportname,dateString);
    //    printLineDotMatrix(printer,&painter2,companyName,&x,&y,*ptr,Qt::AlignCenter,charCount,8);
    //    printLineDotMatrix(printer,&painter2,reportname,&x,&y,*ptr,Qt::AlignCenter,charCount,7);
    //    printLineDotMatrix(printer,&painter2,dateString,&x,&y,*ptr,Qt::AlignLeft,charCount,7);




    QStringList headline;
    headline <<"Date"<<"Vno"<<"Cr"<<"Dr.";
    QString hhead = getColumnStringLedger(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    QList<float> totals;
    for(int i=0;i<totalCols.size();i++){
        totals.append(0);
    }

    for(int i=0;i<model->rowCount();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();
        Line<<model->record(i).value(0).toDate().toString("dd-MM");
        Line<<model->record(i).value(0).toString();
        Line<<QString::number(model->record(i).value(0).toFloat(),'f',2);
        Line<<QString::number(model->record(i).value(0).toFloat(),'f',2);

        str = getColumnStringLedger(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);

        for (int j=0;j<totalCols.size();j++) {
            totals[j] += model->record(i).value(totalCols[j]).toFloat();
        }

    }

    QStringList tLine;

    for (int i=0;i<model->rowCount();i++) {

    }

    QString totalLine = getColumnStringLedger(tLine);

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,totalLine ,&x,&y,*ptr,Qt::AlignLeft,charCount);


    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    painter2.end();
}

bool PrintDotMatrix::printItemwiseReportDetailed(QSqlQueryModel *model, inventoryItemDataModel item, QDate salesDate, QDate pDate, QString reportname)
{
    i=0;
    QPrinter printer;

    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);


    QString dateString = "Date : "+salesDate.toString("dd-MM-yyyy");

    QString lastline = "Item : "+item.ItemName+"("+pDate.toString("dd-MM")+")";

    printReportHeading(printer,&painter2,&x,&y,*ptr,charCount,reportname,dateString,lastline);
    QStringList headline;
    headline <<"Rate"<<"Ledger"<<"Qty "<<"Tray "<<"Total";
    QString hhead = getColumnStringItemwiseReport(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    //    QList<float> totals;
    //    for(int i=0;i<totalCols.size();i++){
    //        totals.append(0);
    //    }

    float qtyTotal = 0;
    float trayTotal = 0;
    float grandT = 0;
    for(int i=0;i<model->rowCount();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();

        Line<<QString::number(model->record(i).value(8).toFloat(),'f',2); //rate
        Line<<model->record(i).value(3).toString(); //Ledger
        Line<<QString::number(model->record(i).value(6).toFloat(),'f',2); //qty
        Line<<QString::number(model->record(i).value(7).toFloat(),'f',0)+" "; //tray
        Line<<QString::number(model->record(i).value(9).toFloat(),'f',2); //total

        str = getColumnStringItemwiseReport(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);

        qtyTotal += model->record(i).value(6).toFloat();
        trayTotal +=  model->record(i).value(7).toFloat();
        grandT += model->record(i).value(9).toFloat();

    }

    QStringList tLine;

    tLine<<" "<<" "<<QString::number(qtyTotal,'f',2)<<QString::number(trayTotal)+" "<<QString::number(grandT,'f',2);


    QString totalLine = getColumnStringItemwiseReport(tLine);

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,totalLine ,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);

    painter2.end();
}

bool PrintDotMatrix::printItemwiseReport(QSqlQueryModel *model, QDate salesDate, QString reportname)
{
    i=0;
    QPrinter printer;

    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);


    QString dateString = "Date : "+salesDate.toString("dd-MM-yyyy");

    QString lastline = "";

    printReportHeading(printer,&painter2,&x,&y,*ptr,charCount,reportname,dateString,lastline);
    QStringList headline;

    headline <<"Date"<<"Item"<<"Qty "<<"Tray "<<"Total";
    QString hhead = getColumnStringItemwiseReport(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    //    QList<float> totals;
    //    for(int i=0;i<totalCols.size();i++){
    //        totals.append(0);
    //    }

    float qtyTotal = 0;
    float trayTotal = 0;
    float grandT = 0;

    for(int i=0;i<model->rowCount();i++){
        QStringList Line;
        QString str;
        Line<<model->record(i).value(1).toDate().toString("dd-MM"); //rate
        Line<<model->record(i).value(2).toString(); //Item
        Line<<QString::number(model->record(i).value(5).toFloat(),'f',2); //qty
        Line<<QString::number(model->record(i).value(6).toFloat(),'f',0)+" "; //tray
        Line<<QString::number(model->record(i).value(7).toFloat(),'f',2); //total
        str = getColumnStringItemwiseReport(Line);

        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);
        qtyTotal += model->record(i).value(5).toFloat();
        trayTotal +=  model->record(i).value(6).toFloat();
        grandT += model->record(i).value(8).toFloat();
    }

    QStringList tLine;

    tLine<<" "<<" "<<QString::number(qtyTotal,'f',2)<<QString::number(trayTotal)+" "<<QString::number(grandT,'f',2);


    QString totalLine = getColumnStringItemwiseReport(tLine);

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,totalLine ,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    //    QSizeF paperSize;
    //    paperSize.setWidth(printer.width());
    //    paperSize.setHeight(printer.height());
    //    doc.setPageSize(paperSize);

    painter2.end();
}

bool PrintDotMatrix::printItemOrderList(GeneralVoucherDataObject *voucher)
{

    i=0;
    QPrinter printer;

    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);

    QString companyName = LoginValues::getCompany().CompanyName;

    QString ledName    = "      Item Name : "+voucher->ledgerObject.LedgerName;
    QString dateString = "      Date      : "+voucher->VoucherDate.toString("dd-MM-yyyy");
    QString vouchNoStr = "      Bill No   : "+voucher->voucherNumber;
    QString voucherType= "Item Order List";


    printLineDotMatrix(printer,&painter2,companyName,&x,&y,*ptr,Qt::AlignCenter,charCount,8);

    printLineDotMatrix(printer,&painter2,voucherType,&x,&y,*ptr,Qt::AlignCenter,charCount,7);
    printLineDotMatrix(printer,&painter2,dateString,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,vouchNoStr,&x,&y,*ptr,Qt::AlignLeft,charCount,7);
    printLineDotMatrix(printer,&painter2,ledName,&x,&y,*ptr,Qt::AlignLeft,charCount,7);

    QStringList headline;

    headline <<"Rate"<<"Item"<<"Qty"<<"Disc."<<"Amt";
    QString hhead = getColumnStringSalesVoucher(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    for(int i=0;i<voucher->InventoryItems.size();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.price,'f',2);
        Line<<voucher->InventoryItems[i].BaseItem.ItemAlias;
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',2);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.discQuantity,'f',2);
        Line<<QString::number(voucher->InventoryItems[i].BaseItem.subTotal,'f',2);
        str = getColumnStringSalesVoucher(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);
    }

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    QString totalString = "Total : "+QString::number(voucher->grandTotal,'f',2);
    totalString = totalString.rightJustified(35);

    printLineDotMatrix(printer,&painter2,totalString,&x,&y,*ptr,Qt::AlignLeft,charCount,8);

    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    painter2.end();
}

bool PrintDotMatrix::printLedger(QSqlQueryModel *model, QDate fromDate, QDate toDate,
                                 QString ledgerName, float openingBalance, float closingBalance)
{
    i=0;
    QPrinter printer;

    int charCount = 38;
    QString blank = "";

    QString horizontalLine;
    QString hyphenLine;
    QString equalLine;

    for(int i=0;i<charCount;i++){
        horizontalLine +="_";
        hyphenLine += "-";
        equalLine += "=";
    }

    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return -1;

    QPainter painter2(&printer);

    painter2.begin(&printer);

    int x=10;
    int y=10;
    QRect *ptr = new QRect();
    ptr->setBottom(y);

    QString companyName = LoginValues::getCompany().CompanyName;

    QString ledName    = "      Cust Name : "+ledgerName;
    QString dateString = " From : "+fromDate.toString("dd-MM-yyyy")+ "  To : "+toDate.toString("dd-MM-yyyy");

    QString voucherType= "Ledger Report";


    printLineDotMatrix(printer,&painter2,companyName,&x,&y,*ptr,Qt::AlignCenter,charCount,8);

    printLineDotMatrix(printer,&painter2,voucherType,&x,&y,*ptr,Qt::AlignCenter,charCount,7);
    printLineDotMatrix(printer,&painter2,dateString,&x,&y,*ptr,Qt::AlignLeft,charCount,7);

    printLineDotMatrix(printer,&painter2,ledName,&x,&y,*ptr,Qt::AlignLeft,charCount,7);

    QStringList headline;

    headline <<"Date"<<"Vno"<<"Cr"<<"Dr.";
    QString hhead = getColumnStringLedger(headline);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,horizontalLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hhead,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    float  crTotal = 0;
    float  drTotal = 0;


    for(int i=0;i<model->rowCount();i++){
        QStringList Line;
        QString str;
        //          y=ptr->bottom();
        Line<<model->record(i).value(0).toDate().toString("dd-MM");
        Line<<model->record(i).value(0).toString();
        Line<<QString::number(model->record(i).value(0).toFloat(),'f',2);
        Line<<QString::number(model->record(i).value(0).toFloat(),'f',2);

        str = getColumnStringLedger(Line);
        printLineDotMatrix(printer,&painter2,str,&x,&y,*ptr,Qt::AlignLeft,charCount);

        crTotal += model->record(i).value(0).toFloat();
        drTotal += model->record(i).value(0).toFloat();
    }

    QStringList tLine;
    QString cTotal = QString::number(crTotal ,'f',2);
    QString dTotal = QString::number(drTotal ,'f',2);
    tLine<<" "<<"Total"<<cTotal<<dTotal;

    QString totalLine = getColumnStringLedger(tLine);

    printLineDotMatrix(printer,&painter2,blank,&x,&y,*ptr,Qt::AlignLeft,charCount);

    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);
    printLineDotMatrix(printer,&painter2,totalLine ,&x,&y,*ptr,Qt::AlignLeft,charCount);

    QString openingString = "Op Bal : "+QString::number(openingBalance,'f',2).rightJustified  (9, ' ',true);
    QString closingString = "Cl Bal : "+QString::number(closingBalance,'f',2).rightJustified  (9, ' ',true);

    openingString = openingString.rightJustified(35);
    closingString = closingString.rightJustified(35);


    printLineDotMatrix(printer,&painter2,openingString,&x,&y,*ptr,Qt::AlignLeft,charCount,8);

    printLineDotMatrix(printer,&painter2,closingString,&x,&y,*ptr,Qt::AlignLeft,charCount,8);

    printLineDotMatrix(printer,&painter2,hyphenLine,&x,&y,*ptr,Qt::AlignLeft,charCount);

    painter2.end();
}
