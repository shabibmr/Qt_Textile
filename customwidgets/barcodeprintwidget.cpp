#include "barcodeprintwidget.h"
#include "ui_barcodeprintwidget.h"
#include <QScreen>

BarcodePrintWidget::BarcodePrintWidget(inventoryItemDataModel *item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarcodePrintWidget)
{
    ui->setupUi(this);

    this->invItem = item;
    this->setWindowTitle("Barcode Print Options");
    this->setObjectName("barCodePrint");
    this->setStyleSheet("QWidget#barCodePrint{background-color:" + bgcolor + "}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    this->setWindowFlags(Qt::Popup|Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);

    this->setFixedSize(250,230);

    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    setWidgets();
}

BarcodePrintWidget::~BarcodePrintWidget()
{
    delete ui;
}

void BarcodePrintWidget::setWidgets()
{
    QHBoxLayout *lay1 = new QHBoxLayout;
    QLabel *label1 = new QLabel("No. Of Copies");
    barcodeCopiesLineEdit = new QLineEdit;
    barcodeCopiesLineEdit->setText(QString::number(invItem->quantity, 'f', 2));
    lay1->addWidget(label1);
    lay1->addWidget(barcodeCopiesLineEdit);

    QHBoxLayout *lay2 = new QHBoxLayout;
    QLabel *label2 = new QLabel("Mfg Date");
    mfdDate = new QDateEdit;
    mfdDate->setDate(QDate::currentDate());
    lay2->addWidget(label2);
    lay2->addWidget(mfdDate);

    QHBoxLayout *lay3 = new QHBoxLayout;
    QLabel *label3 = new QLabel("Exp Date");
    expDate = new QDateEdit;
    expDate->setDate(QDate::currentDate().addDays(invItem->shelfLife));
    lay3->addWidget(label3);
    lay3->addWidget(expDate);

    QHBoxLayout *lay4 = new QHBoxLayout;
    QLabel *label4 = new QLabel("Small");
    barcodeSize = new QCheckBox;
    lay4->addWidget(label4);
    lay4->addWidget(barcodeSize);

    QLabel *label5 = new QLabel("With MFD");
    withMFD = new QCheckBox;
    lay4->addWidget(label5);
    lay4->addWidget(withMFD);

    snackbar = new QtMaterialSnackbar(this);

    ui->verticalLayout->addWidget(new QLabel(" Item : " + invItem->ItemName));
    ui->verticalLayout->addWidget(new QLabel(" Item code : " + invItem->ItemCode + " / " + invItem->uomObject.barCode));
    ui->verticalLayout->addSpacing(30);
    ui->verticalLayout->addLayout(lay1);
    ui->verticalLayout->addLayout(lay2);
    ui->verticalLayout->addLayout(lay3);
    ui->verticalLayout->addLayout(lay4);
    ui->verticalLayout->addStretch();


    okButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "print"));
    okButton->setMini(true);
    okButton->setXOffset(10);
    okButton->setYOffset(10);
    okButton->setParent(this);
    okButton->setToolTip("Print Barcode");

    connect(okButton, &QPushButton::clicked, this, &BarcodePrintWidget::printBarcode);

    cancelButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("navigation", "cancel"));
    cancelButton->setMini(true);
    cancelButton->setXOffset(70);
    cancelButton->setYOffset(10);
    cancelButton->setParent(this);
    cancelButton->setToolTip("Reject");
    connect(cancelButton, &QPushButton::clicked, this,&BarcodePrintWidget::closeWindow);
}

void BarcodePrintWidget::printBarcode()
{
    int n = barcodeCopiesLineEdit->text().toInt();
    QDate Mfd= mfdDate->date();
    QDate Exp= expDate->date();

    if(invItem->ItemCode.length()<1){
        snackbar->addMessage("No Barcode Found");
        return;
    }

    PrinterHelper phelper;
    if(barcodeSize->isChecked())
        phelper.printBarcode3Lines(n/2,*invItem,Mfd,Exp,withMFD->isChecked());
    else
    {
        if(withMFD->isChecked()){
            phelper.printBarcode(n/2,*invItem,Mfd,Exp);
        }
        else
            phelper.printBarcodeAgro(n/2,*invItem,Mfd,Exp);
    }

    closeWindow();
}

void BarcodePrintWidget::closeWindow()
{
    emit closing();
    this->close();
}
