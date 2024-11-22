#include "excelreporthelper.h"
#include <QMessageBox>
#include <QDebug>


ExcelReportHelper::ExcelReportHelper()
{


}

int ExcelReportHelper::createExcel(QString FileName,QTableWidget *mytable,QString dateFrom,QString Dateto){


    //         QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Excel file"),"F:\\excel\\"+FileName+".xls",QObject::tr("Excel Files (*.xls)"));
    //         if (fileName.isEmpty())
    //         {
    //             return 0;
    //         }
    //         //qApp->applicationDirPath ()
    //         ExportExcelObject obj(fileName, "mydata", mytable);

    //         int colCount = mytable->columnCount();

    //         for(int i =0;i<colCount;i++){
    //            QString headData = mytable->horizontalHeaderItem(i)->text();
    //            obj.addField(i+1,headData,"char()");
    //         }


    //         // change the column order and
    //         // choose which column to export
    //         //obj.addField(0, "BILLDATE", "char(20)");
    //         obj.addField(1, "GROUPNAME", "char(20)");
    //         obj.addField(2, "PARENTGROUP", "char(20)");
    //         //obj.addField(3, "TOTAL", "char(20)");
    //         //obj.addField(4, "column5", "char(20)");
    //         //obj.addField(5, "column6", "char(20)");
    //         //obj.addField(6, "column7", "char(20)");

    //         int retVal = obj.export2Excel();
    //         if( retVal > 0)
    //         {
    //             QMessageBox::information(this, QObject::tr("Done"),QString(QObject::tr("%1 records exported!")).arg(retVal));
    //         }

}


void ExcelReportHelper::GenericReport(QString Filename,QString reportName,
                                      QStringList colSizes,QTableWidget *table,QString dateFrom,QString dateTo){

    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);


    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();
    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i].toInt());
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);

    for(int i =0;i<colSize;i++){
        QString Head = table->horizontalHeaderItem(i)->text();
        //  xlsx.mergeCells("A5:D5");
        xlsx.write(5,i+1,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<table->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;

    qDebug()<<"110";
    for(int i=0;i<table->rowCount();i++){
        for(int j=0;j<table->columnCount();j++){
            QString data = table->item(i,j)->text();
            qDebug()<<i<<j<<data;
            bool ok;
            int value = data.toInt(&ok);
            if (!ok){
                // not an integer
                xlsx.write(i+defRowCount+1,j+defColCount+1,data);
            }
            else{
                float vf = data.toFloat();
                if(vf == value){
                    xlsx.write(i+defRowCount+1,j+defColCount+1,value,rAlign);
                }
                else{
                    xlsx.write(i+defRowCount+1,j+defColCount+1,vf,rAlign);
                }

            }

        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";

}

void ExcelReportHelper::InventoryVoucherSimple(QString FileName, GeneralVoucherDataObject *voucher)
{
    QString reportName = voucher->voucherType;
    //Because This is simple we need 3 cols

    QList<int> colSizes;

    colSizes <<20<<60<<20<<10;

    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);

    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);

    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();

    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i]);
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);
    if(LoginValues::company.trn.length()>0)
        xlsx.write("B2","GST : " + LoginValues::getCompany().trn);
    xlsx.mergeCells("A3:B3");
    xlsx.write("A3",reportName,centerAlignBranchName);

    xlsx.write("A4","Date : ");
    xlsx.write("B4",voucher->VoucherDate.toString("dd-MM-yyyy"));

    xlsx.write("C4","Voucher No : ");
    xlsx.write("D4", voucher->voucherNumber);

    xlsx.mergeCells("B5:C5");
    xlsx.write("A5","To : ");
    xlsx.write("B5",voucher->ledgerObject.LedgerName);
    xlsx.setRowHeight(5,13);

    int defRow = 6;

    xlsx.write(defRow,1,"SL NO",justBold);
    xlsx.write(defRow,2,"Item Name",justBold);
    xlsx.write(defRow,3,"Quantity",justBold);

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);

    defRow++;
    for(int i=0;i<voucher->InventoryItems.size();i++){
        xlsx.write(defRow+i,1,QString::number(i+1),rAlign);
        xlsx.write(defRow+i,2,voucher->InventoryItems[i].BaseItem.ItemName);
        QString qty = QString::number(voucher->InventoryItems[i].BaseItem.quantity,'f',
                                      voucher->InventoryItems[i].BaseItem.uomObject.decimalPoints);

        QString uom = voucher->InventoryItems[i].BaseItem.uomObject.UomSymbol;

        xlsx.write(defRow+i,3,qty,rAlign);
        xlsx.write(defRow+i,4,uom);

    }

    xlsx.saveAs(FileName);
    qDebug()<<"Write Complete";

}

void ExcelReportHelper::GenericReportFromTableView(QString Filename, QString reportName,
                                                   QStringList colSizes, QTableView *table,
                                                   QString dateFrom, QString dateTo, QList<int> avlCols,
                                                   bool vhead)
{


    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);

    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();

    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i].toInt());
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);
    int vHead = vhead==true?1:0;

    if(vhead)
        for(int i=0;i<table->model()->rowCount();i++){
            QString Head = table->model()->headerData(i,Qt::Vertical).toString();
            xlsx.write(5+1+i,1,Head,justBold);
        }
    for(int i =0;i<colSize;i++){
        QString Head = table->model()->headerData(avlCols[i],Qt::Horizontal).toString();
        //  xlsx.mergeCells("A5:D5");
        xlsx.write(5,i+1+vHead,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<table->model()->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;
    for(int i=0;i<table->model()->rowCount();i++){
        for(int j=0;j<avlCols.size();j++){

            QModelIndex index = table->model()->index(i, avlCols[j], QModelIndex());


            if(table->model()->data(index).type() == QVariant::Double){
                 xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toFloat(),rAlign);
            }
            else if(table->model()->data(index).type() == QVariant::Int){
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toInt(),rAlign);
            }
            else{
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toString());
            }
        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";
}

void ExcelReportHelper::GenericReportFromModel(QString Filename, QString reportName,
                                                   QList<int> colSizes, QSqlQueryModel *model,
                                                   QString dateFrom, QString dateTo, QList<int> avlCols,
                                                   bool vhead)
{


    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);

    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();

    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i]);
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);
    int vHead = vhead==true?1:0;

    if(vhead)
        for(int i=0;i<model->rowCount();i++){
            QString Head = model->headerData(i,Qt::Vertical).toString();
            xlsx.write(5+1+i,1,Head,justBold);
        }
    for(int i =0;i<colSize;i++){
        QString Head = model->headerData(avlCols[i],Qt::Horizontal).toString();
        //  xlsx.mergeCells("A5:D5");
        xlsx.write(5,i+1+vHead,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<model->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;
    for(int i=0;i<model->rowCount();i++){
        for(int j=0;j<avlCols.size();j++){

            QModelIndex index = model->index(i, avlCols[j], QModelIndex());


            if(model->data(index).type() == QVariant::Double){
                 xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,model->data(index).toFloat(),rAlign);
            }
            else if(model->data(index).type() == QVariant::Int){
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,model->data(index).toInt(),rAlign);
            }
            else{
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,model->data(index).toString());
            }
        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";
}


void ExcelReportHelper::GenericReportVList(QString Filename,QString reportName,
                                           QStringList colSizes,QTableWidget *table,QString dateFrom,QString dateTo){

    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);


    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();
    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i].toInt());
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);

    for(int i =1;i<colSize;i++){
        QString Head = table->horizontalHeaderItem(i)->text();
        //   xlsx.mergeCells("A5:D5");
        xlsx.write(5,i,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<table->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;
    for(int i=0;i<table->rowCount();i++){
        for(int j=1;j<table->columnCount();j++){
            QString data = table->item(i,j)->text();


            qDebug()<<i<<j<<data;
            bool ok;
            int value = data.toInt(&ok);
            if (!ok){
                // not an integer
                xlsx.write(i+defRowCount+1,j+defColCount,data);
            }
            else{
                float vf = data.toFloat();
                if(vf==value){
                    xlsx.write(i+defRowCount+1,j+defColCount+1,value,rAlign);
                }
                else{
                    xlsx.write(i+defRowCount+1,j+defColCount+1,vf,rAlign);
                }

            }

        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";

}




void ExcelReportHelper::testExcel()
{
    xlsx.addSheet("Aligns & Borders");
    xlsx.setColumnWidth(2, 20); //Column B
    xlsx.setColumnWidth(8, 12); //Column H
    xlsx.currentWorksheet()->setGridLinesVisible(false);

    //Alignment
    writeHorizontalAlignCell(xlsx, "B3", "AlignLeft", Format::AlignLeft);
    writeHorizontalAlignCell(xlsx, "B5", "AlignHCenter", Format::AlignHCenter);
    writeHorizontalAlignCell(xlsx, "B7", "AlignRight", Format::AlignRight);
    writeVerticalAlignCell(xlsx, "D3:D7", "AlignTop", Format::AlignTop);
    writeVerticalAlignCell(xlsx, "F3:F7", "AlignVCenter", Format::AlignVCenter);
    writeVerticalAlignCell(xlsx, "H3:H7", "AlignBottom", Format::AlignBottom);

    //Border
    writeBorderStyleCell(xlsx, "B13", "BorderMedium", Format::BorderMedium);
    writeBorderStyleCell(xlsx, "B15", "BorderDashed", Format::BorderDashed);
    writeBorderStyleCell(xlsx, "B17", "BorderDotted", Format::BorderDotted);
    writeBorderStyleCell(xlsx, "B19", "BorderThick", Format::BorderThick);
    writeBorderStyleCell(xlsx, "B21", "BorderDouble", Format::BorderDouble);
    writeBorderStyleCell(xlsx, "B23", "BorderDashDot", Format::BorderDashDot);

    //Fill
    writeSolidFillCell(xlsx, "D13", Qt::red);
    writeSolidFillCell(xlsx, "D15", Qt::blue);
    writeSolidFillCell(xlsx, "D17", Qt::yellow);
    writeSolidFillCell(xlsx, "D19", Qt::magenta);
    writeSolidFillCell(xlsx, "D21", Qt::green);
    writeSolidFillCell(xlsx, "D23", Qt::gray);

    writePatternFillCell(xlsx, "F13", Format::PatternMediumGray, Qt::red);
    writePatternFillCell(xlsx, "F15", Format::PatternDarkHorizontal, Qt::blue);
    writePatternFillCell(xlsx, "F17", Format::PatternDarkVertical, Qt::yellow);
    writePatternFillCell(xlsx, "F19", Format::PatternDarkDown, Qt::magenta);
    writePatternFillCell(xlsx, "F21", Format::PatternLightVertical, Qt::green);
    writePatternFillCell(xlsx, "F23", Format::PatternLightTrellis, Qt::gray);

    writeBorderAndFontColorCell(xlsx, "H13", "Qt::red", Qt::red);
    writeBorderAndFontColorCell(xlsx, "H15", "Qt::blue", Qt::blue);
    writeBorderAndFontColorCell(xlsx, "H17", "Qt::yellow", Qt::yellow);
    writeBorderAndFontColorCell(xlsx, "H19", "Qt::magenta", Qt::magenta);
    writeBorderAndFontColorCell(xlsx, "H21", "Qt::green", Qt::green);
    writeBorderAndFontColorCell(xlsx, "H23", "Qt::gray", Qt::gray);

    //---------------------------------------------------------------
    //Create the second sheet.
    xlsx.addSheet("Fonts");

    xlsx.write("B3", "Normal");
    Format font_bold;
    font_bold.setFontBold(true);
    xlsx.write("B4", "Bold", font_bold);
    Format font_italic;
    font_italic.setFontItalic(true);
    xlsx.write("B5", "Italic", font_italic);
    Format font_underline;
    font_underline.setFontUnderline(Format::FontUnderlineSingle);
    xlsx.write("B6", "Underline", font_underline);
    Format font_strikeout;
    font_strikeout.setFontStrikeOut(true);
    xlsx.write("B7", "StrikeOut", font_strikeout);

    writeFontNameCell(xlsx, "D3", "Arial");
    writeFontNameCell(xlsx, "D4", "Arial Black");
    writeFontNameCell(xlsx, "D5", "Comic Sans MS");
    writeFontNameCell(xlsx, "D6", "Courier New");
    writeFontNameCell(xlsx, "D7", "Impact");
    writeFontNameCell(xlsx, "D8", "Times New Roman");
    writeFontNameCell(xlsx, "D9", "Verdana");

    writeFontSizeCell(xlsx, "G3", 10);
    writeFontSizeCell(xlsx, "G4", 12);
    writeFontSizeCell(xlsx, "G5", 14);
    writeFontSizeCell(xlsx, "G6", 16);
    writeFontSizeCell(xlsx, "G7", 18);
    writeFontSizeCell(xlsx, "G8", 20);
    writeFontSizeCell(xlsx, "G9", 25);

    Format font_vertical;
    font_vertical.setRotation(255);
    font_vertical.setFontSize(16);
    xlsx.write("J3", "vertical", font_vertical);
    xlsx.mergeCells("J3:J9");

    //---------------------------------------------------------------
    //Create the third sheet.
    xlsx.addSheet("Formulas");
    xlsx.setColumnWidth(1, 2, 40);
    Format rAlign;
    rAlign.setHorizontalAlignment(Format::AlignRight);
    Format lAlign;
    lAlign.setHorizontalAlignment(Format::AlignLeft);
    xlsx.write("B3", 40, lAlign);
    xlsx.write("B4", 30, lAlign);
    xlsx.write("B5", 50, lAlign);
    xlsx.write("A7", "SUM(B3:B5)=", rAlign);
    xlsx.write("B7", "=SUM(B3:B5)", lAlign);
    xlsx.write("A8", "AVERAGE(B3:B5)=", rAlign);
    xlsx.write("B8", "=AVERAGE(B3:B5)", lAlign);
    xlsx.write("A9", "MAX(B3:B5)=", rAlign);
    xlsx.write("B9", "=MAX(B3:B5)", lAlign);
    xlsx.write("A10", "MIN(B3:B5)=", rAlign);
    xlsx.write("B10", "=MIN(B3:B5)", lAlign);
    xlsx.write("A11", "COUNT(B3:B5)=", rAlign);
    xlsx.write("B11", "=COUNT(B3:B5)", lAlign);

    xlsx.write("A13", "IF(B7>100,\"large\",\"small\")=", rAlign);
    xlsx.write("B13", "=IF(B7>100,\"large\",\"small\")", lAlign);

    xlsx.write("A15", "SQRT(25)=", rAlign);
    xlsx.write("B15", "=SQRT(25)", lAlign);
    xlsx.write("A16", "RAND()=", rAlign);
    xlsx.write("B16", "=RAND()", lAlign);
    xlsx.write("A17", "2*PI()=", rAlign);
    xlsx.write("B17", "=2*PI()", lAlign);

    xlsx.write("A19", "UPPER(\"qtxlsx\")=", rAlign);
    xlsx.write("B19", "=UPPER(\"qtxlsx\")", lAlign);
    xlsx.write("A20", "LEFT(\"ubuntu\",3)=", rAlign);
    xlsx.write("B20", "=LEFT(\"ubuntu\",3)", lAlign);
    xlsx.write("A21", "LEN(\"Hello Qt!\")=", rAlign);
    xlsx.write("B21", "=LEN(\"Hello Qt!\")", lAlign);

    Format dateFormat;
    dateFormat.setHorizontalAlignment(Format::AlignLeft);
    dateFormat.setNumberFormat("yyyy-mm-dd");
    xlsx.write("A23", "DATE(2013,8,13)=", rAlign);
    xlsx.write("B23", "=DATE(2013,8,13)", dateFormat);
    xlsx.write("A24", "DAY(B23)=", rAlign);
    xlsx.write("B24", "=DAY(B23)", lAlign);
    xlsx.write("A25", "MONTH(B23)=", rAlign);
    xlsx.write("B25", "=MONTH(B23)", lAlign);
    xlsx.write("A26", "YEAR(B23)=", rAlign);
    xlsx.write("B26", "=YEAR(B23)", lAlign);
    xlsx.write("A27", "DAYS360(B23,TODAY())=", rAlign);
    xlsx.write("B27", "=DAYS360(B23,TODAY())", lAlign);

    xlsx.write("A29", "B3+100*(2-COS(0)))=", rAlign);
    xlsx.write("B29", "=B3+100*(2-COS(0))", lAlign);
    xlsx.write("A30", "ISNUMBER(B29)=", rAlign);
    xlsx.write("B30", "=ISNUMBER(B29)", lAlign);
    xlsx.write("A31", "AND(1,0)=", rAlign);
    xlsx.write("B31", "=AND(1,0)", lAlign);

    xlsx.write("A33", "HYPERLINK(\"http://qt-project.org\")=", rAlign);
    xlsx.write("B33", "=HYPERLINK(\"http://qt-project.org\")", lAlign);

    //---------------------------------------------------------------
    //Create the fourth sheet.
    xlsx.addSheet("NumFormats");
    xlsx.setColumnWidth(2, 40);
    writeInternalNumFormatsCell(xlsx, 4, 2.5681, 2);
    writeInternalNumFormatsCell(xlsx, 5, 2500000, 3);
    writeInternalNumFormatsCell(xlsx, 6, -500, 5);
    writeInternalNumFormatsCell(xlsx, 7, -0.25, 9);
    writeInternalNumFormatsCell(xlsx, 8, 890, 11);
    writeInternalNumFormatsCell(xlsx, 9, 0.75, 12);
    writeInternalNumFormatsCell(xlsx, 10, 41499, 14);
    writeInternalNumFormatsCell(xlsx, 11, 41499, 17);

    writeCustomNumFormatsCell(xlsx, 13, 20.5627, "#.###");
    writeCustomNumFormatsCell(xlsx, 14, 4.8, "#.00");
    writeCustomNumFormatsCell(xlsx, 15, 1.23, "0.00 \"RMB\"");
    writeCustomNumFormatsCell(xlsx, 16, 60, "[Red][<=100];[Green][>100]");

    //---------------------------------------------------------------
    //Create the fifth sheet.
    xlsx.addSheet("Merging");
    Format centerAlign;
    centerAlign.setHorizontalAlignment(Format::AlignHCenter);
    centerAlign.setVerticalAlignment(Format::AlignVCenter);
    xlsx.write("B4", "Hello Qt!");
    xlsx.mergeCells("B4:F6", centerAlign);
    xlsx.write("B8", 1);
    xlsx.mergeCells("B8:C21", centerAlign);
    xlsx.write("E8", 2);
    xlsx.mergeCells("E8:F21", centerAlign);

    //---------------------------------------------------------------
    //Create the fifth sheet.
    xlsx.addSheet("Grouping");
    qsrand(QDateTime::currentMSecsSinceEpoch());
    for (int row=2; row<31; ++row) {
        for (int col=1; col<=10; ++col)
            xlsx.write(row, col, qrand() % 100);
    }
    xlsx.groupRows(4, 7);
    xlsx.groupRows(11, 26, false);
    xlsx.groupRows(15, 17, false);
    xlsx.groupRows(20, 22, false);
    xlsx.setColumnWidth(1, 10, 10.0);
    xlsx.groupColumns(1, 2);
    xlsx.groupColumns(5, 8, false);

    xlsx.saveAs("Book1.xlsx");

}

void ExcelReportHelper::GenerateReportFromTableView(QString Filename, QString reportName,
                                                   QList<int> colSizes, QTableView *table,
                                                   QString dateFrom, QString dateTo, QList<int> avlCols,
                                                   bool vhead)
{


    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);


    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();

    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i]);
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3:C3:D3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);
    int vHead = vhead==true?1:0;

    if(vhead)
        for(int i=0;i<table->model()->rowCount();i++){
            QString Head = table->model()->headerData(i,Qt::Vertical).toString();
            xlsx.write(5+1+i,1,Head,justBold);
        }
    for(int i =0;i<colSize;i++){
        QString Head = table->model()->headerData(avlCols[i],Qt::Horizontal).toString();
        //  xlsx.mergeCells("A5:D5");
        xlsx.write(5,i+1+vHead,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<table->model()->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;
    for(int i=0;i<table->model()->rowCount();i++){
        for(int j=0;j<avlCols.size();j++){

            QModelIndex index = table->model()->index(i, avlCols[j], QModelIndex());

            QString data = table->model()->data(index).toString();


            qDebug()<<i<<avlCols[j]<<data;
            \
            bool ok;
            int value = data.toInt(&ok);
            if (!ok){
                // not an integer
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,data);
            }
            else{
                float vf = data.toFloat();
                if(vf == value){
                    if(vf < 1 && vf >= 0)
                        qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<vf;
                    xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,value,rAlign);
                }
                else{
                    xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,vf,rAlign);
                }


            }

        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";


}

void ExcelReportHelper::HSNReportFromTableView(QString Filename, QString reportName,
                                                   QStringList colSizes, QTableView *table,
                                                   QString dateFrom, QString dateTo, QList<int> avlCols,
                                                   bool vhead)
{


    QFile file(":/json/HSN.json");  // :/json/json/dbchanges.json doesnt work
    file.open(QIODevice::ReadOnly);

    QString data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject hsnList = doc.object();

    file.close();

    xlsx.addSheet(reportName);
    Format centerAlignCompanyName;
    Format centerAlignBranchName;
    Format justBold;

    justBold.setFontBold(true);


    centerAlignCompanyName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignCompanyName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignCompanyName.setFontSize(20);

    centerAlignBranchName.setHorizontalAlignment(Format::AlignHCenter);
    centerAlignBranchName.setVerticalAlignment(Format::AlignVCenter);
    centerAlignBranchName.setFontSize(14);

    int colSize = colSizes.length();

    for(int i=0;i<colSize;i++){
        xlsx.setColumnWidth(i+1,colSizes[i].toInt());
    }

    char c = 65+(colSize>4?colSize:4);

    if(colSize==2){
        xlsx.setColumnWidth(1,50);
        xlsx.setColumnWidth(2,50);

    }

    xlsx.mergeCells("A1:"+QString(c)+"1",centerAlignCompanyName);
    xlsx.mergeCells("A2:"+QString(c)+"2",centerAlignBranchName);

    xlsx.write("A1",LoginValues::getCompany().CompanyName);
    xlsx.write("A2",LoginValues::getCompany().branch);

    xlsx.mergeCells("A3:B3");
    xlsx.write("A3","Report : "+reportName);


    xlsx.write("A4","Date : ");
    xlsx.write("B4",dateFrom+" to " + dateTo);
    int vHead = vhead==true?1:0;

    if(vhead)
        for(int i=0;i<table->model()->rowCount();i++){
            QString Head = table->model()->headerData(i,Qt::Vertical).toString();
            xlsx.write(5+1+i,1,Head,justBold);
        }
    for(int i =0;i<colSize;i++){
        QString Head = table->model()->headerData(avlCols[i],Qt::Horizontal).toString();
        //  xlsx.mergeCells("A5:D5");
        xlsx.write(5,i+1+vHead,Head,justBold);
    }

    Format rAlign;
    rAlign.setNumberFormat("#.00");
    rAlign.setHorizontalAlignment(Format::AlignRight);
    qDebug()<<"Col count = "<<table->model()->columnCount();

    int defColCount = 0;
    int defRowCount = 0+5;
    for(int i=0;i<table->model()->rowCount();i++){
        for(int j=0;j<avlCols.size();j++){

            QModelIndex index = table->model()->index(i, avlCols[j], QModelIndex());

            if(reportName == "HSN" && (j==1))
                qDebug()<<Q_FUNC_INFO<<__LINE__<<table->model()->data(index).toString();


            if(table->model()->data(index).type() == QVariant::Double){
                 xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toFloat(),rAlign);
            }
            else if(table->model()->data(index).type() == QVariant::Int){
                xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toInt(),rAlign);
            }
            else{
                if(j ==1){
                    QModelIndex ind1 = index.model()->index(index.row(),0); // index of HSN

                    QString HSN =  index.model()->data(ind1).toString() ;
                    QString desc = hsnList[HSN].toString();

                    xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,desc);
                }
                else
                    xlsx.write(i+defRowCount+1,j+defColCount+1+vHead,table->model()->data(index).toString());
            }
        }
    }
    xlsx.saveAs(Filename);
    qDebug()<<"Write Complete";
}

