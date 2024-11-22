#include "printoptions.h"
#include "ui_printoptions.h"

#include <QLabel>
#include <QScreen>
#include <QStyle>

PrintOptions::PrintOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrintOptions)
{
    ui->setupUi(this);
    this->setWindowTitle(windowTitle);
    this->setStyleSheet("QWidget#editor{background-color:" + bgcolor +"}");
    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    this->setFixedSize(400,150);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Popup);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));
    setWidgets();
}

PrintOptions::~PrintOptions()
{
    delete ui;
}

void PrintOptions::setWidgets()
{
    printer = new QPrinter(QPrinter::ScreenResolution) ;
    printer->setFullPage(true);

    okButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("navigation", "check"));
    okButton->setParent(this);
    okButton->setMini(true);
    okButton->setCorner(Qt::BottomRightCorner);
    okButton->setOffset(0,0);
    connect(okButton, &QPushButton::clicked, this, [=](){
        qDebug()<<"Ok Clikced";
        emit printerSet(printer);
        this->close();
    });

    selectPrinter = new QtMaterialCheckBox();
    selectPrinter->setText("Choose Printer");
    selectPrinter->setFont(QFont("Calibri", 9, QFont::Normal));
    connect(selectPrinter, &QtMaterialCheckBox::toggled, this, &PrintOptions::openPrinterDialog);

    //    printSize = new QtMaterialAutoComplete;
    //    QStringList printSizes;
    //    printSizes<<"A4"<<"Thermal";
    //    printSize->setDataSource(printSizes);
    //    printSize->setText("A4");
    //    connect(printSize, &QtMaterialAutoComplete::itemSelected, this, [=](QString size){
    //        if(size == "A4")
    //            printer->setPageSize(QPrinter::A4);
    //    });

    a4Size = new QtMaterialRadioButton;
    a4Size->setText("A4");
    a4Size->setFont(QFont("Calibri", 9, QFont::Normal));
    a4Size->setCheckable(true);
    qDebug()<<"A4 sized";
    connect(a4Size, &QAbstractButton::toggled, this, [=](bool fal){
        qDebug()<<"A4 clicked";
        printer->setFullPage(true);
        printer->setPageSize(QPrinter::A4);
        emit printerTypeSelected(PrintTypes::Default);
    });

    a5Size = new QtMaterialRadioButton;
    a5Size->setText("A5");
    a5Size->setFont(QFont("Calibri", 9, QFont::Normal));
    a5Size->setCheckable(true);
    connect(a5Size, &QAbstractButton::toggled, this, [=](bool fal){
        qDebug()<<"A5 clicked";
        printer->setFullPage(true);
        printer->setPageSize(QPrinter::A5);
        emit printerTypeSelected(PrintTypes::Default);
    });

    thermal = new QtMaterialRadioButton;
    thermal->setText("Thermal");
    thermal->setFont(QFont("Calibri", 9, QFont::Normal));
    connect(thermal,&QAbstractButton::toggled, this, [=](bool fal){
        //        printer->setPageSize(QPrinter::A4);
    emit printerTypeSelected(PrintTypes::Thermal);
        qDebug()<<"Value : "<<1<<PrintTypes::Thermal;

    });

    dotMatrix = new QtMaterialRadioButton;
    dotMatrix->setText("Dot Matrix");
    dotMatrix->setFont(QFont("Calibri", 9, QFont::Normal));
    connect(dotMatrix, &QAbstractButton::toggled, this, [=](bool fal){
        //        printer->setPageSize(QPrinter::A4);
        qDebug()<<"Dot matrix selected";
        emit printerTypeSelected(PrintTypes::DotMatrix);
    });

    QHBoxLayout *printSizeLay = new QHBoxLayout;
    printSizeLay->addWidget(a4Size);
    printSizeLay->addWidget(a5Size);
    printSizeLay->addWidget(thermal);
    printSizeLay->addWidget(dotMatrix);

    numCopies = new QSpinBox;
    numCopies->setValue(1);
    numCopies->setMaximumWidth(50);
    connect(numCopies, SIGNAL(valueChanged(int)), this, SLOT(setNumCopies(int)));

    QHBoxLayout *numCopyLay = new QHBoxLayout;
    numCopyLay->addWidget(new QLabel("Num Copies"));
    numCopyLay->addWidget(numCopies);
    numCopyLay->addStretch(1);

    ui->verticalLayout->addWidget(selectPrinter);
    ui->verticalLayout->addLayout(printSizeLay);
    ui->verticalLayout->addLayout(numCopyLay);

}

void PrintOptions::openPrinterDialog(bool checked)
{
    if(checked){
        QPrintDialog pDial(printer,this);
        int val = pDial.exec();
        if(val ==QDialog::Rejected)
            selectPrinter->setChecked(false);
        else if(val==QDialog::Accepted){
            emit printerSet(printer);
            this->close();
        }
    }


}

void PrintOptions::setNumCopies(int value)
{
    printer->setNumCopies(value) ;

}
