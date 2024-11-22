#include "tableselect.h"
#include "ui_tableselect.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"
#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "QFormLayout"
#include <QFontMetrics>

TableSelect::TableSelect(GeneralVoucherDataObject *voucher,
                         SalesOrderDatabaseHelper *helper,
                         LedgerMasterDatabaseHelper *ledHelper,
                         AccountGroupMasterDatabaseHelper *accHelper,
                         PricelistDatabaseHelper *priceHelper,
                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableSelect)
{
    ui->setupUi(this);
    colCount = ConfigurationSettingsDatabaseHelper::getValue(tablesViewColCount,8).toInt();
    setManualTime = ConfigurationSettingsDatabaseHelper::getValue(TimeSetManuallyPOS,true).toBool();

    showAllDraftsFlag = ConfigurationSettingsDatabaseHelper::getValue(showAllDrafts, true).toBool();
    focusOnCreateNewFlag = ConfigurationSettingsDatabaseHelper::getValue(focusOnCreateNew, true).toBool();

    buttonWidth = ConfigurationSettingsDatabaseHelper::getValue(tableGridButtonWidth,130).toInt();
    buttonHeight = ConfigurationSettingsDatabaseHelper::getValue(tableGridButtonHeight,70).toInt();
    fsize = ConfigurationSettingsDatabaseHelper::getValue(tableGridFontSize,14).toInt();
    // qDebug()<<fsize<<buttonWidth<<buttonHeight;
    this->voucher = voucher;
    this->ledHelper = ledHelper;
    this->accHelper = accHelper;
    this->priceHelper = priceHelper;
    //    this->AddressDialogWidget = AddressDialog;

    dbHelper = new TablesMasterHelper();
    ui->deliveryButton->hide();

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ui->label->setText("Select Reference");
    }
    setTable();
    ui->splitToolButton->setCheckable(true);
    ui->splitToolButton->setChecked(CounterSettingsDataModel::splitTables);
    if(focusOnCreateNewFlag)
        ui->createNewLineEdit->setFocus();
    ui->createNewLineEdit->setMaxLength(30);
    //soHelper = new SalesOrderDatabaseHelper();
    soHelper = helper;
    this->setObjectName("POSMain");
    this->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");

    ui->widget->hide();
    if(voucher->status == QuotationStatus::Started){
        voucher->DeliveryDate = QDateTime::currentDateTime();
    }
    delivery = new GMPlainTextEdit(&voucher->narration);
    delivery->setFont(QFont("Arial",12));
    QString kotNarration = ConfigurationSettingsDatabaseHelper::getValue(kotNarrationTitle,"Narration").toString();
    delivery->setPlaceholderText(kotNarration);


    ui->horizontalLayout_2->insertWidget(3,delivery);

}

TableSelect::~TableSelect()
{
    delete ui;
}

void TableSelect::setSwitchFlag(bool value)
{
    switchFlag = value;
    ui->deliveryButton->hide();
}

void TableSelect::setNewLineText(QString text)
{
    ui->createNewLineEdit->setText(text);
    tempRef = text;
}

void TableSelect::setLineFocus()
{
    if(focusOnCreateNewFlag)
        ui->createNewLineEdit->setFocus();

}

void TableSelect::SelectedTable(QString reference)
{
    //    qDebug()<<"reference = "<<reference;
    if(!CounterSettingsDataModel::splitTables){
        voucher->reference = reference;
        tableSelected();

    }
    else{
        voucher->reference = reference;
        ui->widget->show();
    }


}


void TableSelect::tableSelected()
{
    this->close();
    if(switchFlag)
        emit switchTable();
    else
        emit selectTable();
}

void TableSelect::setTable()
{
    qDebug()<<fsize<<buttonWidth<<buttonHeight;
    if(ui->layout != NULL ){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= NULL){
            delete item->widget();
            delete item;
        }
    }

    QStringList Occupiedtables;

    QStringList TableNames;
    TableNames = LoginValues::getCompany().notes.split("|");

    QSqlQueryModel *tModel = dbHelper->getTableNamesForSearch();
    if(tModel->rowCount()>0){
        TableNames.clear();
        for(int i=0;i<tModel->rowCount();i++){
            TableNames.append(tModel->record(i).value(1).toString());
        }
    }
    float total = 0;
    QString vPref = LoginValues::voucherPrefix;
    if(showAllDraftsFlag)
        vPref="%";

    QMap<QString,float> map = soHelper->getOccupiedTables(vPref);

    QMapIterator<QString, float> iter(map);
    while (iter.hasNext()) {
        iter.next();
        QString tableName = iter.key();
        total +=  iter.value();
        if(!TableNames.contains(tableName,Qt::CaseInsensitive)){
            TableNames<<tableName;
        }
        Occupiedtables << tableName;
    }

    ui->totalLineEdit->setText(QString::number(total,'f',2));


    QSignalMapper* signalMapper = new QSignalMapper(this);

    QWidget *client= new QWidget(this);

    QGridLayout *gLayout = new QGridLayout(client);
    //QLayoutItem item = new QLayoutItem;
    QObject::connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(SelectedTable(QString)));
    int cnt =0;

    QFont tfont;
    QFont sfont;

    tfont.setPointSize(fsize);
    sfont.setPointSize(6);

    for(int i=0;i<TableNames.size();i++) {
        QString name = TableNames[i];
        // QString icon = CounterSettingsDataModel::TypeOfTrade == "ERP"?"":":/icons/icons/toggle/svg/production/ic_check_box_outline_blank_24px.svg";
        // QString icon = "";
        bool flag = false;
        QString color = "#AFF0ff00;color:#000000;font-weight:bold";
        if(Occupiedtables.contains(name,Qt::CaseInsensitive)){
            color = "#70f01f80;color:#000000;font-weight:bold";
            flag = true;
        }

        QToolButton* button = new QToolButton(this);

        button->setProperty("id",name);

        QVBoxLayout *blay = new QVBoxLayout();

        button->setToolButtonStyle(Qt::ToolButtonTextOnly);

        button->setAutoRaise(false);

        QLabel *tbname = new QLabel(button);

        // QLabel *shortCut = new QLabel(this);


        tbname->setText(name);

        float amt = map[name];

        // QWidget *bwidget = new QWidget;
        // bwidget->setMinimumSize(100,50);

        tfont.setBold(flag);
        tbname->setFont(tfont);
        tbname->setAlignment(Qt::AlignCenter);

        // QHBoxLayout* layout = new QHBoxLayout;
        // // layout->addWidget(tbname);
        // tbname->setLayout(layout);


        // QFontMetrics fm(tbname->font());
        // QRect rect = fm.boundingRect(name);

        // int wid = rect.width() + 16;

        // int minwidth = wid>120 ? wid:120;

        // shortCut->setFont(sfont);
        // shortCut->setAlignment(Qt::AlignRight);

        QString col = flag==true? "#fa2312":"#000000";
        tbname->setStyleSheet("color:"+col);
        tfont.setBold(flag);

        // blay->addWidget(bwidget);
        blay->addWidget(tbname,Qt::AlignCenter);

        if(flag){
            blay->setAlignment(tbname,Qt::AlignTop|Qt::AlignCenter);
            button->setObjectName("GridButton");
            button->setStyleSheet("QToolButton#GridButton: { background-color: #e11010; }");
            if(amt>0){
                QLabel *totalLbl = new QLabel(button);
                totalLbl->setText(QString::number(amt,'f',2));
                totalLbl->setFont(tfont);
                totalLbl->setAlignment(Qt::AlignBottom|Qt::AlignRight);

                blay->addWidget(totalLbl,Qt::AlignRight|Qt::AlignBottom);

                QString css = "";
            }

        }
        // blay->addWidget(shortCut);
        button->setLayout(blay);
        // button->setMinimumWidth(minwidth);
        // button->setMinimumHeight(70);

        button->setFixedSize(buttonWidth,buttonHeight);

        // button->setFixedSize(QSize(120,70));
        button->setStyleSheet("QToolButton{background-color:#e0f7fa; border:1px solid blue}");
        signalMapper->setMapping(button, name);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        gLayout->addWidget(button,cnt/colCount,cnt%colCount);
        cnt++;
    }

    client->setLayout(gLayout);
    client->setObjectName("POSMain");
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setObjectName("POSMain");
    scroll->verticalScrollBar()->
        setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                        "    border: 1px solid #999999;"
                                        "    background:white;"
                                        "    width:10px;    "
                                        "    margin: 0px 0px 0px 0px;"
                                        "}"
                                        "QScrollBar::handle:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    min-height: 0px;"
                                        ""
                                        "}"
                                        "QScrollBar::add-line:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    height: px;"
                                        "    subcontrol-position: bottom;"
                                        "    subcontrol-origin: margin;"
                                        "}"
                                        "QScrollBar::sub-line:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    height: 0px;"
                                        "    subcontrol-position: top;"
                                        "    subcontrol-origin: margin;"
                                        "}"
                                        ""));

    scroll->setWidget(client);
    scroll->setAlignment(Qt::AlignCenter);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(scroll);
    //vLayout->addWidget(new QLabel("ITEMS"));
    ui->layout->addLayout(vLayout);

    if(focusOnCreateNewFlag)
        ui->createNewLineEdit->setFocus();
}


void TableSelect::on_createNewButton_clicked()
{
    voucher->reference = ui->createNewLineEdit->text();
    if(voucher->reference.length()<1)
        return;
    if(switchFlag)
        emit switchTable();
    else
        tableSelected();
    ui->createNewLineEdit->clear();

    this->close();
}

void TableSelect::on_deliveryButton_clicked()
{
    voucher->ModeOfService = ServiceMode::Delivery;
    tableSelected();
}

void TableSelect::on_backButton_clicked()
{
    this->close();
}

void TableSelect::on_createNewLineEdit_returnPressed()
{
    on_createNewButton_clicked();
}

void TableSelect::on_pushButton_1_clicked()
{
    voucher->reference += "- 1";
    tableSelected();
}

void TableSelect::on_pushButton_Existing_clicked()
{
    tableSelected();
}

void TableSelect::on_pushButton_2_clicked()
{
    voucher->reference += "- 2";
    tableSelected();
}

void TableSelect::on_pushButton_3_clicked()
{
    voucher->reference += "- 3";
    tableSelected();
}

void TableSelect::on_pushButton_4_clicked()
{
    voucher->reference += "- 4";
    tableSelected();

}



void TableSelect::on_splitToolButton_toggled(bool checked)
{


    //    if(checked==true){
    //        ui->widget->show();
    //    }
    //    else{
    //        ui->widget->hide();
    //    }
    CounterSettingsDataModel::setSplitTables(checked);
    CounterSettingsDatabaseHelper db;

}

void TableSelect::on_preOrderButton_clicked()
{
    voucher->DeliveryDate.setDate(voucher->VoucherDate);

    if(voucher->reference.length()==0)
        voucher->reference = tempRef;

    QDialog *dialog = new QDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowFlags(Qt::FramelessWindowHint|Qt::Popup|Qt::WindowStaysOnTopHint);
    QVBoxLayout *lay = new QVBoxLayout(dialog);

    QFormLayout *formLay = new QFormLayout(dialog);

    dialog->setLayout(lay);
    QFont fontHeading;fontHeading.setPointSize(18);
    QFont font;font.setPointSize(12);

    dialog->setFont(font);
    QLabel *heading = new QLabel("Advanced Booking");
    QLabel *ledgerLabel = new QLabel("Select Ledger");
    QLabel *vReflbl = new QLabel("Reference");
    QLabel *deliveryDate = new QLabel("Select Delivery Date");

    QToolButton *lunchButton = new QToolButton(dialog);
    QToolButton *dinnerButton = new QToolButton(dialog);
    QToolButton *OkButton = new QToolButton(dialog);

    OkButton->setText("OK");
    connect(OkButton,&QToolButton::clicked,this,[=](bool){
        voucher->status = QuotationStatus::SalesOrderDeliveryCreated;
        if(voucher->Contact.PhoneNumber.length()>0){
            tableSelected();
        }
        else{
            showContactDetailsBlank();
        }
    });

    QPalette pal2 = palette();
    pal2.setColor(QPalette::Background,QColor("#f1f0f7fa"));
    setAutoFillBackground(true);
    lunchButton->setPalette(pal2);
    dinnerButton->setPalette(pal2);
    GMDateTimeEdit *delDateWidget = new GMDateTimeEdit(&voucher->DeliveryDate,"dd-MM-yyyy hh:mm");
    lunchButton->setText("LUNCH");

    voucher->CustomerExpectingDate = voucher->DeliveryDate.addSecs(60*30);

    GMDateTimeEdit *toDateWidget = new GMDateTimeEdit(&voucher->CustomerExpectingDate,"hh:mm");

    toDateWidget->setMinimumDateTime(voucher->DeliveryDate);
    connect(delDateWidget,&GMDateTimeEdit::editFinished,this,[=](){
        toDateWidget->setMinimumDateTime(voucher->DeliveryDate);
        voucher->CustomerExpectingDate = voucher->DeliveryDate.addSecs(30*60);
        toDateWidget->setVariable(&voucher->CustomerExpectingDate);
    });


    connect(lunchButton,&QToolButton::clicked,this,[=](bool){
        voucher->DeliveryDate.setTime(QTime::fromString("13:00:00","hh:mm:ss"));
        lunchButton->setIcon(QIcon(":/icons/check.ico"));
        dinnerButton->setIcon(QIcon(""));
        delDateWidget->setVariable(&voucher->DeliveryDate);
        OkButton->show();
    });


    dinnerButton->setText("Dinner");
    connect(dinnerButton,&QToolButton::clicked,this,[=](bool){
        voucher->DeliveryDate.setTime(QTime::fromString("19:00:00","hh:mm:ss"));
        lunchButton->setIcon(QIcon(""));
        dinnerButton->setIcon(QIcon(":/icons/check.ico"));
        delDateWidget->setVariable(&voucher->DeliveryDate);
        OkButton->show();
    });

    lunchButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    dinnerButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    lunchButton->setMinimumSize(150,40);
    dinnerButton->setMinimumSize(150,40);

    lunchButton->hide();
    dinnerButton->hide();

    delDateWidget->setParent(dialog);
    QStringList salesGroups;

    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive("0x5x21")){
        salesGroups.append(acc.groupID);
    }
    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive("0x5x23")){
        salesGroups.append(acc.groupID);
    }
    for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive("0x5x19")){
        salesGroups.append(acc.groupID);
    }
    GMLedgerLineMaterial *ledgerWidget = new GMLedgerLineMaterial(ledHelper,
                                                                  ledHelper->getLedgerQueryString(salesGroups),
                                                                  &voucher->ledgerObject);
    ledgerWidget->setText(voucher->ledgerObject.LedgerName);
    ledgerWidget->setParent(dialog);
    heading->setFont(fontHeading);

    GMMaterialLineEdit *refEdit = new GMMaterialLineEdit(&voucher->reference);

    connect(ledgerWidget,&GMLedgerLineMaterial::itemSelectedwithItem,this,&TableSelect::showContactDetails);

    formLay->addRow(ledgerLabel,ledgerWidget);
    formLay->addRow(vReflbl,refEdit);
    formLay->addRow(deliveryDate,delDateWidget);
    formLay->addRow(new QLabel("Till",dialog),toDateWidget);


    QHBoxLayout *ldsel = new QHBoxLayout(dialog);
    ldsel->addWidget(lunchButton);
    ldsel->addWidget(dinnerButton);

    lay->addWidget(heading);
    lay->addLayout(formLay);
    lay->addLayout(ldsel);
    lay->addWidget(OkButton);
    lay->setAlignment(OkButton,Qt::AlignRight);

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor("#f1aec6cf"));
    setAutoFillBackground(true);
    dialog->setPalette(pal);
    dialog->show();
    if(setManualTime){
        dinnerButton->hide();
        lunchButton->hide();
    }
    else
        OkButton->hide();

}

void TableSelect::showContactDetails(LedgerMasterDataModel led)
{

    AddressDialogWidget = new AddressDialog(ledHelper,this);
    AddressDialogWidget->setAttribute(Qt::WA_DeleteOnClose);
    if(led.LedgerID==CounterSettingsDataModel::DefaultCash){

    }
    else{
        voucher->Contact.address= led.Address;
        voucher->Contact.ContactName = led.LedgerName;
        voucher->Contact.PhoneNumber = led.phoneNumber;
    }
    AddressDialogWidget->setWindowFlags(Qt::Window);
    AddressDialogWidget->setVoucher(voucher);
    //    AddressDialogWidget->setAddressText();
    AddressDialogWidget->setFocus();
    //    AddressDialogWidget->show();
    AddressDialogWidget->setFocustoPhone();
    //    QObject::connect(AddressDialogWidget,&AddressDialog::setAddress,
    //                     this, [=](QString Address, QString Phone, QString Name, QString route){
    //        voucher->Contact.address= Address;
    //        voucher->Contact.PhoneNumber= Phone;
    //        voucher->Contact.ContactName= Name;
    //        voucher->ledgerObject.ContactPersonNumber = Phone;
    //        voucher->ledgerObject.emailAddress = Address;
    //        voucher->ledgerObject.ContactPersonName = Name;
    //        voucher->Contact.route = route;
    //        AddressDialogWidget->hide();
    //    });


}

void TableSelect::showContactDetailsBlank()
{
    AddressDialogWidget = new AddressDialog(ledHelper,this);
    AddressDialogWidget->setAttribute(Qt::WA_DeleteOnClose);
    AddressDialogWidget->setWindowFlags(Qt::Window);
    AddressDialogWidget->setVoucher(voucher);
    //    AddressDialogWidget->setAddressText();
    AddressDialogWidget->setFocus();
    AddressDialogWidget->show();
    AddressDialogWidget->setFocustoPhone();

    //    QObject::connect(AddressDialogWidget,&AddressDialog::setAddress,
    //                     this, [=](QString Address, QString Phone, QString Name){
    //        voucher->Contact.address= Address;
    //        voucher->Contact.PhoneNumber= Phone;
    //        voucher->Contact.ContactName= Name;
    //        voucher->ledgerObject.ContactPersonNumber = Phone;
    //        voucher->ledgerObject.emailAddress = Address;
    //        voucher->ledgerObject.ContactPersonName = Name;
    //        AddressDialogWidget->hide();
    //        tableSelected();
    //    });
    QObject::connect(AddressDialogWidget,&AddressDialog::addressSelected,
                     this, [=](){
                         qDebug()<<Q_FUNC_INFO<<__LINE__;
                         tableSelected();
                     });

}

void TableSelect::on_createNewLineEdit_cursorPositionChanged(int arg1, int arg2)
{
    //    ui->createNewLineEdit->selectAll();
}
