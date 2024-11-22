#ifndef SHARETOOLS_H
#define SHARETOOLS_H

#include <QWidget>
#include <QTableView>

#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialfab.h"

#include "datamodels/transaction/generalvoucherdataobject.h"
#include "sharetools/print/printerhelper.h"
#include "sharetools/email/emailthread.h"
#include "sharetools/excel/excelreporthelper.h"
#include "sharetools/print/printoptions.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

namespace Ui {
class ShareTools;
}

class ShareTools : public QWidget
{
    Q_OBJECT

public:
    explicit ShareTools(QString type = "", QWidget *parent = nullptr);
    ~ShareTools();

    QString windowTitle="Share with";
    QString bgcolor = "#e0f7fa";
    QString iconColor = "#00bcd4";


    QtMaterialFlatButton *printButton;
    QtMaterialFlatButton *excelButton;
    QtMaterialFlatButton *pdfButton;
    QtMaterialFlatButton *emailButton;

    QtMaterialFloatingActionButton *closeButton;

    PrinterHelper *printHelper;

//    GeneralVoucherDataObject *voucher;
    QString docType;
public :
    int printType = 0;
    void setDocType(const QString &value);

public slots:
    void exportToExcel();
    void exportToPdf();
    void printVoucher();
    void printReport(QPrinter *printer, QTableView *table, QList<int> widthlist,
                     QString ReportName, QDate FromDate, QDate toDate,
                     QList<int> Alignment, QStringList extraTop,
                     QStringList extraBottom, QList<int> avlCols);
    void showPrintOptions();
    void sendEmail();
    void closeWindow();


    void printReportFromModel(QPrinter *printer, QSqlQueryModel *model,
                              QList<int> widthlist, QString ReportName,
                              QDate FromDate, QDate toDate, QList<int> Alignment,
                              QStringList extraTop, QStringList extraBottom, QList<int> avlCols);
private slots:
    void setWidgets();


private:
    Ui::ShareTools *ui;

signals:
    void printClicked(QPrinter *);
    void pdfClicked(QPrinter *);
    void excelClicked();

};



#endif // SHARETOOLS_H
