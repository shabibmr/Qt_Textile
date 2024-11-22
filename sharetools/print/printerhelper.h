#ifndef PRINTERHELPER_H
#define PRINTERHELPER_H

#include <QWidget>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "QTableWidget"
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <qsqlquery.h>
#include <QMessageBox>
#include <stdio.h>
#include <string.h>
#include <qmath.h>
#include <QPrintDialog>
#include "database/hr/userprofiledatabasehelper.h"
#include "database/inventory/sales_inventory_items/inventoryitemimagesdatabasehelper.h"
#include <QBuffer>
#include <QTextCursor>
#include "datamodels/projects/Documents/documentsdatamodel.h"
#include <database/inventory/salesorder/salesorderdatabasehelper.h>
#include <QTextTable>
#include <login/loginvalues.h>


class PrinterHelper : public QWidget
{
    Q_OBJECT
public:

    UserProfileDatabaseHelper *userHelper;
    InventoryItemImagesDatabaseHelper *imageHelper;
    SalesOrderDatabaseHelper salesOrderDatabaseHelper;
    int textMargins = 12;
    QString QuoteRef="";

    int textMarginsExample = 12; // in millimeters
    int borderMarginsExample = 10; // in millimeters


    bool printAsPdfG = false;
    int completedDescription;

    QPrinter *printer;

//    void testPrint(GeneralVoucherDataObject *voucher);


    explicit PrinterHelper(QWidget *parent = nullptr);

    int PrintInventoryVoucher(GeneralVoucherDataObject* data);

    int PrintDeliveryNoteVoucher(GeneralVoucherDataObject data);

    int PrintAccountVoucher(GeneralVoucherDataObject data);

    int PrintLedgerList(QList<LedgerMasterDataModel> data);

    int PrintInventoryList(QList<inventoryItemDataModel> data);

    int PrintGeneralReport(QTableWidget *table,QList<int> colSizes,QString ReportName,
                           QDate dateFrom, QDate dateTo);

    int printQuotation(QString validity,QString subject,QString payment,
                       QString warranty,QString extra,QString delivery,QString exclusion,
                       bool printExtra,bool printNow,GeneralVoucherDataObject voucher,bool printAsPdf=false
                       );

    int printWorkOrder(GeneralVoucherDataObject voucher);

    int printDummy();

    QString getTermsinHtml(QString validity,QString subject,QString payment,
                           QString warranty,QString extra,QString delivery,QString exclusion);

    QString getDescription(GeneralVoucherDataObject voucher,int i);
    QString numberToText(uint);

    void printDocument(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget,GeneralVoucherDataObject obj,
                       QString,QString,QString,QString,QString,QString);
    void paintPage(QPrinter& printer, int pageNumber, int pageCount,
                          QPainter* painter, QTextDocument* doc,
                          const QRectF& textRect, qreal footerHeight,GeneralVoucherDataObject obj,
                   QString a,QString b,QString c,QString,QString,QString);

    void paintPageDeliveryNote(QPrinter& printer, int pageNumber, int pageCount,
                   QPainter* painter, QTextDocument* doc,
                   const QRectF& textRect, qreal footerHeight,GeneralVoucherDataObject obj);

    void printWorkOrderDrafts(CompoundItemDataObject item,DocumentsDataModel image);

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

    QString formatNumber(float numberf);

    int printIssueRequest(GeneralVoucherDataObject *voucher);

    void printIssueVoucher(QPrinter& printer, int pageNumber, int pageCount,
                           QPainter* painter, QTextDocument* doc,
                           const QRectF& textRect, qreal footerHeight,
                           GeneralVoucherDataObject *v);

    void formatIssueRequest(QPrinter& printer, QTextDocument* doc, QWidget* parentWidget,
                            GeneralVoucherDataObject *obj);

    void getIssueTable(QTextCursor* cursor,GeneralVoucherDataObject *obj);


    void testBarcode();

    void getInventoryVoucherTableSales(QTextCursor *cursor, GeneralVoucherDataObject *obj,QList<qreal> widthList);

    void getInventoryVoucherTablePurchase(QTextCursor *cursor, GeneralVoucherDataObject *obj,QList<qreal> widthList);

    void getInventoryVoucherTableDeliveryNote(QTextCursor *cursor, GeneralVoucherDataObject *obj,QList<qreal> widthList);

    void getInventoryVoucherTableWorkOrder(QTextCursor *cursor, GeneralVoucherDataObject *obj,QList<qreal> widthList);

    void getInventoryVoucherTableIssueReq(QTextCursor *cursor, GeneralVoucherDataObject *obj,QList<qreal> widthList);

    void getInventoryVoucherTableNew(QTextTable *table, GeneralVoucherDataObject *obj,QList<float> widthList);


    void getHSNCodeTable(QTextCursor *cursor, GeneralVoucherDataObject *obj);

    int PrintInventorySalesVoucher(GeneralVoucherDataObject *voucher);

    int PrintInventoryPurchaseVoucher(GeneralVoucherDataObject *voucher);

    int PrintWorkOrderVoucher(GeneralVoucherDataObject *voucher);

    int PrintIssueRequestVoucher(GeneralVoucherDataObject *voucher);

    int PrintInventoryDeliveryNote(GeneralVoucherDataObject *voucher);

    int PrintDispatchSlip(GeneralVoucherDataObject *voucher);

    void formatInventoryVoucherPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget=nullptr);

    void formatIssueVoucherPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget=nullptr);

    void formatWorkOrderPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget=nullptr);


    void printVoucherHeaderFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter,
                                  QTextDocument *doc, const QRectF &textRect, qreal footerHeight,
                                  GeneralVoucherDataObject *v,QList<qreal> widthList);
    void printWorkOrderHeaderFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter,
                                  QTextDocument *doc, const QRectF &textRect, qreal footerHeight,
                                  GeneralVoucherDataObject *v,QList<qreal> widthList);

    void printIssueReqHeaderFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter,
                                  QTextDocument *doc, const QRectF &textRect, qreal footerHeight,
                                  GeneralVoucherDataObject *v,QList<qreal> widthList);



    void printDocumentExample(QPrinter &printer, QTextDocument *doc, QWidget *parentWidget);
    void paintPageExample(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter, QTextDocument *doc, const QRectF &textRect, qreal footerHeight);
    void addTable(QTextCursor &cursor);
    void printBarcode(int CopyCount, inventoryItemDataModel item,QDate Mfd,QDate Exp);

    void printBarcode3Lines(int CopyCount, inventoryItemDataModel item,QDate Mfd,QDate Exp,bool printMFD=false);

    void printBarcodeOld(int CopyCount, inventoryItemDataModel item,QDate Mfd,QDate Exp);


    void printBarcodeAgro(int CopyCount, inventoryItemDataModel item, QDate Mfd, QDate Exp);
    void setPrinter(QPrinter *printer);



    void getDispatchTextTable(QTextCursor *cursor, GeneralVoucherDataObject *obj, QList<qreal> widthList);
    void formatDispatchPages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget=Q_NULLPTR);
    void printDispatchFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter, QTextDocument *doc, QRectF textRect, qreal footerHeight, GeneralVoucherDataObject *voucher, QList<qreal> widthList);
    QString getDispatchHTML(GeneralVoucherDataObject *obj,QList<qreal> widthList);
    void formatDelNotePages(QPrinter &printer, QTextDocument *doc, GeneralVoucherDataObject *obj, QList<qreal> widthList, QWidget *parentWidget=nullptr);
    void printDelHeaderFooter(QPrinter &printer, int pageNumber, int pageCount, QPainter *painter, QTextDocument *doc, const QRectF &textRect, qreal footerHeight, GeneralVoucherDataObject *voucher, QList<qreal> widthList);
    QString getPayVHTML(GeneralVoucherDataObject *obj);
signals:

public slots:
    QString encodeBarcode(QString code);
    int calculateCheckCharacter(QString code);
    int codeToChar(int code);
    int charToCode(int ch);

//    void setDSInfo(DataSetInfo &dsInfo);
//    void setField(RptFieldObject &fieldObject);
};

#endif // PRINTERHELPER_H
