#include "salesmaindash.h"
#include "ui_salesmaindash.h"

SalesMainDash::SalesMainDash(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesMainDash)
{
    ui->setupUi(this);

    this->setObjectName("dashboard");
    this->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);

    dbHelper = new salesVoucherDatabaseHelper;

    setFilterLayout();
    setWidgets();
}

SalesMainDash::~SalesMainDash()
{
    delete ui;
}

void SalesMainDash::setWidgets()
{
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);

    mainLayout->addLayout(filterLayout);
    mainLayout->addStretch(1);

    ui->verticalLayout->addWidget(mainWidget);

    mainWidget->setObjectName("dashboard");
    mainWidget->setStyleSheet("QWidget#dashboard{background-color:" + bgcolor + "}");


    GMCardWidget *salesRegisterCard = new GMCardWidget("Last Sales");
    salesRegister = new QTableView;
    salesRegister->setStyleSheet(tableViewSS);
    salesRegisterCard->setDataWidget(salesRegister);





}

void SalesMainDash::setFilterLayout()
{
    filterLayout = new QHBoxLayout;

    QString dateEditSS = "GMDateEdit{background-color:" + bgcolor + ";border: none; color: " + textColor + "} "
                         "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
                         "QDateEdit::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }";

    GMDateEdit *fromDateEdit = new GMDateEdit(&fromDate);
    fromDateEdit->setFixedWidth(80);
    fromDateEdit->setFrame(false);
    fromDateEdit->setStyleSheet(dateEditSS);
    fromDateEdit->setFont(QFont("Calibri", 9, QFont::Normal));

    GMDateEdit *toDateEdit = new GMDateEdit(&toDate);
    toDateEdit->setFixedWidth(80);
    toDateEdit->setFrame(false);
    toDateEdit->setStyleSheet(dateEditSS);
    toDateEdit->setFont(QFont("Calibri", 9, QFont::Normal));

    QLabel *fromDateLabel = new QLabel("From");
    fromDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *fromDateLay = new QVBoxLayout;
    fromDateLay->addWidget(fromDateLabel);
    fromDateLay->addWidget(fromDateEdit);

    QWidget *fromDateWidget = new QWidget;
    fromDateWidget->setLayout(fromDateLay);

    QLabel *toDateLabel = new QLabel("To");
    toDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *toDateLay = new QVBoxLayout;
    toDateLay->addWidget(toDateLabel);
    toDateLay->addWidget(toDateEdit);

    QWidget *toDateWidget = new QWidget;
    toDateWidget->setLayout(toDateLay);

    QtMaterialIconButton *okButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "refresh"));
    okButton->setParent(this);
    okButton->setColor(QColor(iconColor));
    okButton->setText("Refresh");
    QObject::connect(okButton,&QtMaterialIconButton::clicked,[=]{
        setValues();

    });

    QtMaterialIconButton *newVoucherButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "add_circle"));
    newVoucherButton->setParent(this);
    newVoucherButton->setColor(QColor(iconColor));
    newVoucherButton->setText("Create new");
    QObject::connect(okButton,&QtMaterialIconButton::clicked,[=]{

//        SalesVoucherEditor *editor = new SalesVoucherEditor(new salesVoucherDatabaseHelper(),this);
//        editor->setObjectName("Editor");
//        editor->setWindowFlags(Qt::Window);
//        editor->showMaximized();
//        QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
    });

    filterLayout->addWidget(fromDateWidget);
    filterLayout->addWidget(toDateWidget);
    filterLayout->addWidget(okButton);
    filterLayout->addStretch(1);
    filterLayout->addWidget(newVoucherButton);

}

void SalesMainDash::setValues()
{
//    salesRegister->setModel(dbHelper->getVoucherListByDateTimeModel(fromDateTime, toDateTime))
}



//1. Sales Average <- M/W/Y
//2. Salesmanwise
//3. Godownwise Sales
//4. Datewise Graph
//5. Open Sales Register
//6. Create New Sales Voucher
//7. Avg. No. Bills  <- M/W/Y
//8. Brandwise Sales
//9. Groupwise Graph
//10.
