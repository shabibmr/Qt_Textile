#ifndef COUNTERPRINTERHELPER_H
#define COUNTERPRINTERHELPER_H

#include <QTextDocument>
#include <QTextCursor>
#include <QPrinter>
#include <QPrinterInfo>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "login/loginvalues.h"
#include "datamodels/Counter/shiftdatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"
#include <QPrintDialog>
#include <QTableView>
#include <QTextTableCell>
#include <QProcess>
#include <datamodels/Counter/servicemode.h>
#include <QBuffer>
#include <QHeaderView>
#include "windows.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include <QTableWidget>
#include <QCheckBox>
#include <QSet>
#include <QTcpSocket>
#include <QPrintDialog>
#include <QMessageBox>
#include <database/Settings/ConfigurationSettingsDatabaseHelper.h>
#include <datamodels/projects/Quotation/quotationstatus.h>


#define CODE128_B_START 104
#define CODE128_STOP 106

class CounterPrinterHelper : public QWidget
{
    Q_OBJECT
public:

    bool multiVoucher=false;
    bool printAddressInKotFlag = true;
    int add1Rout2 = 1;
    bool noDefaultPrint = false;
    QString kotAddrSize;
    QString billAddrSize;
    QString kotNarration;

    bool printBarcodeInBillFlag=false;

    explicit CounterPrinterHelper(QWidget *parent = nullptr);

    UserProfileDatabaseHelper *userHelper;
    ConfigurationSettingsDatabaseHelper *configHelper;

    QPrinter *printer;

    QString taxNameStr;

    bool printVPref;

    QString getHTMLMeasurement(GeneralVoucherDataObject *voucher);
    QString getHTMLMeasurementContact(ContactMeasurement *measurement,AddressBookDataModel contact);

    void getBillasTextDocument(QTextDocument *doc,GeneralVoucherDataObject obj,QString printerName="");

    QString getHtml(GeneralVoucherDataObject *obj,QString printerName="");

    QString getSalesOrderHtml(GeneralVoucherDataObject *obj,QString printerName="");

    int printSalesVoucher(GeneralVoucherDataObject *obj,QString printerName="abc");

    int printSalesOrder(GeneralVoucherDataObject *obj,QString printerName="def");

    void printKotViaNetwork(GeneralVoucherDataObject *obj,QString ipAddress,QString group);

    void printShiftReport(ShiftDataModel *Shift, bool isPDF = false, bool printDenom = true,   QString printerName="");

    void printKOT(GeneralVoucherDataObject *obj,bool printAll=false);

    QString  getKOTHTML(GeneralVoucherDataObject *obj,int *count,bool running,QString printerName,QString heading="",bool printAll=false);

    QString getShiftRow(QString,QString Cr="",QString Dr ="");

    void printLedgerSimple(QString ledName,QTableView *table,QDate dateFrom,QDate dateTo,
                           float openingBal,float closingBal);

    void printSettlementReport(QTableView *table,QString salesMan,float Total);

    void printSettlementReportWithCheck(QTableView *table,QString salesMan,float Total,QList<int> sel);

    void printReportGeneral(QTableView *table,QList<int> widthlist,
                            QString ReportName,QDate FromDate,QDate toDate,
                            QList<int> Allignment,
                            QStringList extraTop,QStringList extraBottom,QList<int> avlCols);

    void printReportGeneralwithModel(QSqlQueryModel *table,QList<int> widthlist,
                            QString ReportName,QDate FromDate,QDate toDate,
                            QList<int> Allignment,
                            QStringList extraTop,QStringList extraBottom,QList<int> avlCols);

    void addGenralReportTable(QTextCursor *cursor,QTableView *table, QList<int> widthlist, QString ReportName,
                              QDate FromDate, QDate toDate, QList<int> Allignment,QStringList extraTop,
                              QStringList extraBottom,QList<int> avlCols);

    void addGenralReportTableModel(QTextCursor *cursor,QSqlQueryModel *table, QList<int> widthlist, QString ReportName,
                              QDate FromDate, QDate toDate, QList<int> Allignment,QStringList extraTop,
                              QStringList extraBottom,QList<int> avlCols);
    void openTillUSB();

     void openTillByCode();

    void printExpenses(GeneralVoucherDataObject obj, bool isPDF = false);

    QString getLedgerRow(QDate date, QString billNo="", float dr=0,float cr=0);
    QString getLedgerRowHeader(QString date="", QString billNo="", QString dr="", QString cr="");

    void printDaybookReport(ShiftDataModel *shift,
                            GeneralVoucherDataObject payments,
                            GeneralVoucherDataObject receipts,
                            GeneralVoucherDataObject sales,
                            GeneralVoucherDataObject purchases
                            );

    void printPO(GeneralVoucherDataObject *obj,QString printerName="abc");

    void printPLU(QString html,QString printerName="abc");

    QString  getPOHTML(GeneralVoucherDataObject *obj);
    void printCancelledKOT(GeneralVoucherDataObject *obj);

    void setPrinter(QPrinter *value);
    void printKOTToPrinter(GeneralVoucherDataObject *obj, QString printerName);

    void printReceiptVoucher(GeneralVoucherDataObject *voucher);
    void printPaymentVoucher(GeneralVoucherDataObject *voucher);

    QString getRVHTML(GeneralVoucherDataObject *voucher);
    QString getPayVHTML(GeneralVoucherDataObject *obj);
    void printDailyClosing(ShiftDataModel *Shift, bool isPDF, bool printDenom, QString printerName);
    QString getHtm4Test(GeneralVoucherDataObject *obj, QString printerName="");

    static double mmToPixels(QPrinter& printer, int mm)
    {
        qDebug()<< "Printer Resolution ="<<printer.resolution();
        qDebug()<< "MM = "<<mm;
        qDebug()<<"Ratio "<<mm * 0.039370147 * printer.resolution();

        return mm * 0.039370147 * printer.resolution();
    }

    QString toCamelCase(const QString& s)
    {
        QStringList parts = s.split(' ', QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
        {parts[i].replace(0, 1, parts[i][0].toUpper());}

        return parts.join(" ");
    }

    QString toCapsFirstCase(const QString& str)
    {
        QString tmp = str;
        // if you want to ensure all other letters are lowercase:
        tmp = tmp.toLower();
        tmp[0] = str[0].toUpper();
        return tmp;
    }


    QString formatNumber(float numberf)
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
    QString encodeBarcode(QString code)
    {
        QString encoded;

        encoded.prepend(QChar(codeToChar(CODE128_B_START))); //Start set with B Code 104
        encoded.append(code);
        encoded.append(QChar(calculateCheckCharacter(code)));
        encoded.append(QChar(codeToChar(CODE128_STOP))); //End set with Stop Code 106

        return encoded;
    }

    int calculateCheckCharacter(QString code)
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

    int charToCode(int ch)
    {
        return ch - 32;
    }
    int codeToChar(int code)
    {
        return code + 105;
    }
    QString getLedgerMapRow(QSqlQueryModel *model);
    int printMeasurement(GeneralVoucherDataObject *voucher);

    int printMeasurementContact(ContactMeasurement *measurement,AddressBookDataModel contact);
};

#endif // COUNTERPRINTERHELPER_H
