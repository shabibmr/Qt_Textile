#include "sharetools.h"
#include "ui_sharetools.h"
#include <QLabel>
#include <QScreen>

ShareTools::ShareTools(QString type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareTools)
{
    ui->setupUi(this);

    docType = type;
    this->setWindowTitle(windowTitle);
    this->setStyleSheet("QWidget#editor{background-color:" + bgcolor +"}");
    this->setFixedWidth(400);

//    this->move(parent->pos().x()/*-parent->geometry().width()*/+this->geometry().width(), parent->geometry().y());
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

//    this->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);
    setWidgets();

}

ShareTools::~ShareTools()
{
    delete ui;
}

void ShareTools::setDocType(const QString &value)
{
    docType = value;
}

void ShareTools::exportToExcel()
{
    emit excelClicked();
}

void ShareTools::exportToPdf()
{
    QPrinter *printer = new QPrinter(QPrinter::ScreenResolution) ;
    QString dir = QDir::home().path();
    QString fileName = dir+"/" + docType + ".pdf";
    printer->setOutputFormat(QPrinter::PdfFormat);
    fileName = QFileDialog::getSaveFileName(this,"File Name",fileName,"*.pdf");
    printer->setOutputFileName(fileName);
    this->setProperty("PrintType",4);
    emit pdfClicked(printer);
}

void ShareTools::printVoucher()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        //        printVoucher();
        printHelper = new PrinterHelper();
        //        printHelper->PrintInventorySalesVoucher(voucher);
    }
}

void ShareTools::printReport(QPrinter *printer, QTableView *table,
                             QList<int> widthlist, QString ReportName,
                             QDate FromDate, QDate toDate,
                             QList<int> Alignment, QStringList extraTop,
                             QStringList extraBottom, QList<int> avlCols)
{
    printer->setPrinterName("printerName");
    CounterPrinterHelper *printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    printerHelper-> printReportGeneral(table
                                       ,widthlist,ReportName,FromDate,toDate,Alignment
                                       ,extraTop,extraBottom,avlCols);
}

void ShareTools::printReportFromModel(QPrinter *printer, QSqlQueryModel *model, QList<int> widthlist,
                             QString ReportName, QDate FromDate, QDate toDate,
                             QList<int> Alignment, QStringList extraTop,
                             QStringList extraBottom, QList<int> avlCols)
{
    printer->setPrinterName("printerName");
    CounterPrinterHelper *printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    printerHelper-> printReportGeneralwithModel(model
                                       ,widthlist,ReportName,FromDate,toDate,Alignment
                                       ,extraTop,extraBottom,avlCols);
}

void ShareTools::showPrintOptions()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Print Voucher","Would You Like to Print?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        PrintOptions *printWidget = new PrintOptions(this);
        printWidget->show();
        connect(printWidget, &PrintOptions::printerSet, this, [=](QPrinter *printer){
            emit printClicked(printer);

        });

        connect(printWidget, &PrintOptions::printerTypeSelected, this, [=](int type){
            qDebug()<<"Type set to "<<type;
              this->setProperty("PrintType",type);
            printType = type;
        });



    }
}

void ShareTools::sendEmail()
{

}

void ShareTools::closeWindow()
{
    this->close();
}

void ShareTools::setWidgets()
{
    printButton = new QtMaterialFlatButton("Print");
    printButton->setIcon(QtMaterialTheme::icon("action", "print"));
    printButton->setIconPlacement(Material::LeftIcon);
    printButton->setFont(QFont("Calibri", 9, QFont::Normal));
    //    printButton->setBackgroundMode(Qt::OpaqueMode);
    printButton->setForegroundColor(QColor(iconColor));
    printButton->setMaximumWidth(60);

    excelButton = new QtMaterialFlatButton("Excel");
    excelButton->setIcon(QtMaterialTheme::icon("content", "excel"));
    excelButton->setIconPlacement(Material::LeftIcon);
    excelButton->setFont(QFont("Calibri", 9, QFont::Normal));
    excelButton->setForegroundColor(QColor(iconColor));
    excelButton->setMaximumWidth(60);

    emailButton = new QtMaterialFlatButton("Email");
    emailButton->setIcon(QtMaterialTheme::icon("communication", "email"));
    emailButton->setIconPlacement(Material::LeftIcon);
    emailButton->setFont(QFont("Calibri", 9, QFont::Normal));
    emailButton->setForegroundColor(QColor(iconColor));
    emailButton->setMaximumWidth(60);
    emailButton->hide();

    pdfButton = new QtMaterialFlatButton("PDF  ");
    pdfButton->setIcon(QtMaterialTheme::icon("image", "picture_as_pdf"));
    pdfButton->setIconPlacement(Material::LeftIcon);
    pdfButton->setFont(QFont("Calibri", 9, QFont::Normal));
    pdfButton->setForegroundColor(QColor(iconColor));
    pdfButton->setMaximumWidth(60);


    connect(excelButton,&QtMaterialFlatButton::clicked, this, &ShareTools::exportToExcel);
    connect(pdfButton,&QtMaterialFlatButton::clicked, this, &ShareTools::exportToPdf);
    connect(printButton,&QtMaterialFlatButton::clicked, this, &ShareTools::showPrintOptions);
    //    ui->vLayout->addSpacing(15);

    QLabel *label = new QLabel("Share with... ");
    label->setFont(QFont("Calibri", 9, QFont::Normal));
    label->setStyleSheet("QLabel{color:" + iconColor + "};");
    ui->vLayout->addWidget(label);
    ui->vLayout->addWidget(printButton);
    ui->vLayout->addWidget(excelButton);
    ui->vLayout->addWidget(emailButton);
    ui->vLayout->addWidget(pdfButton);

    if(docType == "Voucher")
        excelButton->hide();

}
