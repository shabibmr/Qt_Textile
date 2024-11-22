#ifndef PRINTDOTMATRIX_H
#define PRINTDOTMATRIX_H

#include <QSet>
#include <QTcpSocket>
#include <QPrintDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrinterInfo>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "login/loginvalues.h"
#include "datamodels/Counter/shiftdatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"
#include <QPrintDialog>
#include <QTableView>
#include <QTextTableCell>
#include <QProcess>
#include <datamodels/Counter/servicemode.h>
#include <QBuffer>
#include <QHeaderView>
#include <QPainter>
#include "datamodels/Counter/countersettingsdatamodel.h"


class PrintDotMatrix
{
public:
    PrintDotMatrix();

    static double mmToPixels(QPrinter& printer, int mm)
    {
        return mm * 0.039370147 * printer.resolution();
    }

    static double PixelsToMM(QPrinter& printer, float pixel)
    {
        return pixel/(0.039370147*printer.resolution());
    }

    QString toCamelCase(const QString& s)
    {
        QStringList parts = s.split(' ', QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
        {
            parts[i].replace(0, 1, parts[i][0].toUpper());
        }

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

    QString formatNumber(float numberf){
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

    QString getTableRowTextDotMatrix(){

    }

    bool printLineDotMatrix(QPrinter &printer, QPainter *painter, QString &Str, int *x, int *y, QRect &ptr,
                           Qt::Alignment alignment=Qt::AlignLeft, int charLength=40, int FontSize=7);
    bool printSalesVoucherForItemwise(GeneralVoucherDataObject *voucher);


    QString getColumnStringSalesVoucher(QStringList Line);

    bool printItemOrderList(GeneralVoucherDataObject *voucher);

    bool printLedger(QSqlQueryModel *model, QDate fromDate,QDate toDate,QString ledgerName,float openingBalance,float closingBalance);

    QString getColumnStringLedger(QStringList Line);

    void printReportHeading(QPrinter &printer,QPainter *painter,int *x, int *y,QRect &ptr,
                            int charLength, QString reportName, QString dateString,QString lastLine="");


    bool printGeneralReport(QSqlQueryModel *model,QDate fromDate,QDate toDate,QString reportname,QList<int> totalCols);

    bool printItemwiseReportDetailed(QSqlQueryModel *model,inventoryItemDataModel item, QDate salesDate,QDate pDate,QString reportname);

    bool printItemwiseReport(QSqlQueryModel *model, QDate salesDate,QString reportname);

    bool printSalesVoucher(GeneralVoucherDataObject *voucher);
public slots:
    QString getColumnStringItemwiseReport(QStringList Line);
};

#endif // PRINTDOTMATRIX_H
