#ifndef EXCELREPORTHELPER_H
#define EXCELREPORTHELPER_H
#include "QTableWidget"
#include <QSqlQueryModel>
#include <QWidget>
#include <QtCore>

#include "login/loginvalues.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellrange.h"
#include "xlsxworksheet.h"
#include <QFileDialog>

QTXLSX_USE_NAMESPACE
class ExcelReportHelper
{
public:
    ExcelReportHelper();
    Document xlsx;

    int createExcel(QString,QTableWidget*,QString,QString);
    void testExcel();
    void GenericReport(QString Filename,QString reportName,
                       QStringList colSizes,QTableWidget *table,QString dateFrom,QString dateTo);

    void InventoryVoucherSimple(QString FileName,GeneralVoucherDataObject *voucher);

    void GenericReportFromTableView(QString Filename,QString reportName,
                       QStringList colSizes,QTableView *table,QString dateFrom,QString dateTo,
                                    QList<int> avlCols,bool vhead=false);



    void GenericReportVList(QString Filename,QString reportName,
                                          QStringList colSizes,QTableWidget *table,QString dateFrom,QString dateTo);


    void writeHorizontalAlignCell(Document &xlsx, const QString &cell, const QString &text, Format::HorizontalAlignment align)
    {
       Format format;
       format.setHorizontalAlignment(align);
       format.setBorderStyle(Format::BorderThin);
       xlsx.write(cell, text, format);
    }

    void writeVerticalAlignCell(Document &xlsx, const QString &range, const QString &text, Format::VerticalAlignment align)
    {
       Format format;
       format.setVerticalAlignment(align);
       format.setBorderStyle(Format::BorderThin);
       CellRange r(range);
       xlsx.write(r.firstRow(), r.firstColumn(), text);
       xlsx.mergeCells(r, format);
    }

    void writeBorderStyleCell(Document &xlsx, const QString &cell, const QString &text, Format::BorderStyle bs)
    {
       Format format;
       format.setBorderStyle(bs);
       xlsx.write(cell, text, format);
    }

    void writeSolidFillCell(Document &xlsx, const QString &cell, const QColor &color)
    {
       Format format;
       format.setPatternBackgroundColor(color);
       xlsx.write(cell, QVariant(), format);
    }

    void writePatternFillCell(Document &xlsx, const QString &cell, Format::FillPattern pattern, const QColor &color)
    {
       Format format;
       format.setPatternForegroundColor(color);
       format.setFillPattern(pattern);
       xlsx.write(cell, QVariant(), format);
    }

    void writeBorderAndFontColorCell(Document &xlsx, const QString &cell, const QString &text, const QColor &color)
    {
       Format format;
       format.setBorderStyle(Format::BorderThin);
       format.setBorderColor(color);
       format.setFontColor(color);
       xlsx.write(cell, text, format);
    }

    void writeFontNameCell(Document &xlsx, const QString &cell, const QString &text)
    {
        Format format;
        format.setFontName(text);
        format.setFontSize(16);
        xlsx.write(cell, text, format);
    }

    void writeFontSizeCell(Document &xlsx, const QString &cell, int size)
    {
        Format format;
        format.setFontSize(size);
        xlsx.write(cell, "Qt Xlsx", format);
    }

    void writeInternalNumFormatsCell(Document &xlsx, int row, double value, int numFmt)
    {
        Format format;
        format.setNumberFormatIndex(numFmt);
        xlsx.write(row, 1, value);
        xlsx.write(row, 2, QString("Builtin NumFmt %1").arg(numFmt));
        xlsx.write(row, 3, value, format);
    }

    void writeCustomNumFormatsCell(Document &xlsx, int row, double value, const QString &numFmt)
    {
        Format format;
        format.setNumberFormat(numFmt);
        xlsx.write(row, 1, value);
        xlsx.write(row, 2, numFmt);
        xlsx.write(row, 3, value, format);
    }

    void GenerateReportFromTableView(QString Filename, QString reportName, QList<int> colSizes, QTableView *table, QString dateFrom, QString dateTo, QList<int> avlCols, bool vhead = false);
    void GenericReportFromModel(QString Filename, QString reportName, QList<int> colSizes, QSqlQueryModel *model, QString dateFrom, QString dateTo, QList<int> avlCols, bool vhead = false);
    void HSNReportFromTableView(QString Filename, QString reportName, QStringList colSizes, QTableView *table, QString dateFrom, QString dateTo, QList<int> avlCols, bool vhead);
};

#endif // EXCELREPORTHELPER_H
