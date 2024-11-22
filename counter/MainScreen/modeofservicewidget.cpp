#include "modeofservicewidget.h"
#include "ui_modeofservicewidget.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "datamodels/Counter/countersettingsdatamodel.h"

ModeOfServiceWidget::ModeOfServiceWidget(GeneralVoucherDataObject* v,
                                         PricelistDatabaseHelper *pHelp,
                                         LedgerMasterDatabaseHelper *ledHelper,
                                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeOfServiceWidget)
{
    ui->setupUi(this);
    voucher = v;
    priceHelper = pHelp;
    ledgerHelper = ledHelper;
    font.setPointSize(11);

    addressDialog = new AddressDialog(ledgerHelper, this);
    askAddressFlag = ConfigurationSettingsDatabaseHelper::getValue(askAddress,false).toBool();
    neverAskAddressFlag = ConfigurationSettingsDatabaseHelper::getValue(NeverAskAddress,false).toBool();
    ledgerWidget = new GMLedgerLineMaterial(ledgerHelper,ledgerHelper->getLedgerQueryString(),
                                            &voucher->ledgerObject);

    //    QWidget *wid = new QWidget(this);
    //    wid->setMinimumWidth(200);
    //this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    //    wid->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);

    //    mlay = new FlowLayout();

    //    mlay->setSizeConstraint(
    //    this->setMinimumHeight(40);

    //    ui->horizontalLayout_2->addWidget(wid);

    //    wid->setLayout(mlay);
    //    drawer = new QtMaterialDrawer(this);
    //    drawer->setClickOutsideToClose(true);
    //    drawer->setOverlayMode(true);
    //    QVBoxLayout *drawerLayout = new QVBoxLayout;
    //    drawer->setDrawerLayout(drawerLayout);
    //    drawerLayout->addWidget(ledgerWidget);
    settings = new SettingsHelper();
    setView();

    if(CounterSettingsDataModel::TypeOfTrade == "ERP"){
        ledgerWidget->show();
    }
    else{
        ledgerWidget->hide();
    }
}

ModeOfServiceWidget::~ModeOfServiceWidget()
{
    delete ui;
}

void ModeOfServiceWidget::setVoucher(GeneralVoucherDataObject *v)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    voucher = v;
    ledgerWidget->setItem(&voucher->ledgerObject);
    addressDialog->setVoucher(voucher);
    resetIcons();


}

void ModeOfServiceWidget::setView()
{
    //    QToolButton *button = new QToolButton(this);
    //    ui->horizontalLayout->addWidget(button);
    //    button->setIcon(QtMaterialTheme::icon("navigation", "more_vert"));
    //    QObject::connect(button,&QToolButton::clicked,drawer,&QtMaterialDrawer::openDrawer);

    ui->horizontalLayout->addWidget(ledgerWidget);
    QSqlQueryModel *model = priceHelper->getPriceListsAsQueryModel();
    for(int i=0;i<model->rowCount();i++){
        if(model->record(i).value(4).toBool() == true ||
                model->record(i).value(0).toInt() == voucher->ModeOfService){
            ModeOfServiceButton* button = new ModeOfServiceButton(this);
            button->setMinimumWidth(80);
            button->setMaximumWidth(100);
            button->setMinimumHeight(25);
            button->setText(model->record(i).value(1).toString());
            if(voucher->ModeOfService == model->record(i).value(0).toInt())
                button->setIcon(QIcon(":/icons/check.ico"));
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            button->setAutoRaise(true);
            button->setProperty("id",model->record(i).value(0).toString());
            button->setFont(font);
            buttonsList.append(button);
            ui->horizontalLayout->addWidget(button);
            QObject::connect(button,&ModeOfServiceButton::clicked,this,
                             &ModeOfServiceWidget::ServiceSelected);
            QObject::connect(button,&ModeOfServiceButton::rightClicked,this,
                             [=](){
                settings->setValue("POSPriceDefault",model->record(i).value(0).toInt());
                setDefault();
            });

        }
    }
}

void ModeOfServiceWidget::recalculatePrice()
{

    qDebug()<<"RESET FIRED!!!! {}{}{} ";
    int freeCount =0;
    int freeUsed =0;
    int diff =0;

    for(int i=0;i<voucher->InventoryItems.size();i++){

        double currPrice = voucher->InventoryItems[i].BaseItem.price;
        // qDebug()<<"Current : "<<currPrice;
        voucher->InventoryItems[i].BaseItem.price =
                priceHelper->getPriceByItemID(voucher->InventoryItems[i].BaseItem.ItemID,
                                              QString::number(voucher->ModeOfService));
        float p = priceHelper->getPriceByItemIDAndUOM(voucher->InventoryItems[i].BaseItem.ItemID,
                                                      voucher->priceListId,
                                                      voucher->InventoryItems[i].BaseItem.uomObject._id);

        if(p == 0 && currPrice>0){}
        else if(p > 0 ){
            // qDebug()<<"escalade Price : "<<currPrice<<p;
            voucher->InventoryItems[i].BaseItem.price = p;
        }
        else if(p==-1){
            voucher->InventoryItems[i].BaseItem.price = currPrice;
        }

        if(freeUsed>freeCount)
        {
            // qDebug()<<"Update Price as FREE";
            voucher->InventoryItems[i].BaseItem.price=0;
            freeUsed += voucher->InventoryItems[i].BaseItem.quantity;
        }

        if(voucher->InventoryItems[0].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
            freeCount+= voucher->InventoryItems[0].BaseItem.quantity*2;
        }

        //        if(i>0){


        //            if(i==1){
        //                if(voucher->InventoryItems[0].BaseItem.Category.contains("4",Qt::CaseInsensitive)){
        //                    voucher->InventoryItems[i].BaseItem.price = 0;
        //                }
        //            }
        //            else{
        //                if(voucher->InventoryItems[i-1].BaseItem.Category.contains("4",Qt::CaseInsensitive)
        //                        ||
        //                        voucher->InventoryItems[i-2].BaseItem.Category.contains("4",Qt::CaseInsensitive)
        //                        ){
        //                    voucher->InventoryItems[i].BaseItem.price = 0;
        //                }
        //            }
        //        }
        qDebug()<<"item price : "<<voucher->InventoryItems[i].BaseItem.price;
    }
    emit itemPricesChanged();
}

void ModeOfServiceWidget::ServiceSelected()
{
    QString id = sender()->property("id").toString();
    voucher->ModeOfService = id.toInt();
    voucher->priceListId = id;
    if(voucher->InventoryItems.size() > 0){
        for(int i=0; i<voucher->InventoryItems.size(); i++){
            voucher->InventoryItems[i].BaseItem.PriceLevel = voucher->priceListId;
        }
    }
    qDebug()<<"Service "<<voucher->ModeOfService;
    resetIcons();
    recalculatePrice();
    //    if(voucher->ModeOfService == 1 )
    qDebug()<<"When Service Selected";
    openAddressDialog();

}

void ModeOfServiceWidget::resetIcons()
{
    for(int i=0;i<buttonsList.size();i++){
        if(buttonsList[i]->property("id").toInt() == voucher->ModeOfService){
            buttonsList[i]->setIcon(QIcon(":/icons/check.ico"));
            buttonsList[i]->setStyleSheet("border-style: outset;border-bottom-color: rgb(255, 127, 80);"
                                          "border-top-color: rgb(215, 60, 122);border-left-color:  rgb(255, 10, 0);"
                                          "background-color: rgb(125, 255, 155);border-right-color: rgb(255, 40, 0);"
                                          "border-radius: 10px;border-width: 1px;");
            buttonsList[i]->setEffectEnable(true);
        }
        else{
            buttonsList[i]->setIcon(QIcon());
            buttonsList[i]->setStyleSheet("border-style: outset;border-bottom-color: rgb(255, 127, 80);"
                                          "border-top-color: rgb(255, 160, 122);border-left-color:  rgb(255, 210, 0);"
                                          "background-color: rgb(255, 255, 255);border-right-color: rgb(255, 140, 0);"
                                          "border-radius: 10px;border-width: 1px;");
            buttonsList[i]->setEffectEnable(false);
        }
    }
}

void ModeOfServiceWidget::setDefault()
{
    voucher->ModeOfService = settings->getValue("POSPriceDefault",1).toInt();
    qDebug()<<"deflt : "<<settings->getValue("POSPriceDefault",1).toInt();
    resetIcons();
    qDebug()<<"From deflt";
    openAddressDialog();

}

void ModeOfServiceWidget::setService(int i )
{
    voucher->ModeOfService = i;
    resetIcons();
}

void ModeOfServiceWidget::setAddress()
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(addressDialog != nullptr){
        addressDialog->setVoucher(voucher);
        //        addressDialog->setAddressText();
    }

}

void ModeOfServiceWidget::openAddressDialog(QString number)
{
    qDebug()<<"Ask For Address : "<<askAddressFlag;

    if(neverAskAddressFlag)
        return;
    // qDebug()<<"FIRST PASS";
    if(voucher->ModeOfService != 1 && !askAddressFlag)
        return;
    // qDebug()<<"PASSED ALL IF";
    //    addressDialog = new AddressDialog(ledgerHelper, this);

    //    addressDialog->setWindowFlag(Qt::alwa)
    //    addressDialog->setAttribute(Qt::WA_DeleteOnClose);
    addressDialog->setVoucher(voucher);
    //    addressDialog->setVoucher(voucher);
    //    addressDialog->setAddressText();

    //    QObject::connect(addressDialog,&AddressDialog::setAddress,
    //                     [=](QString address,QString phone,QString name, QString route){
    //        voucher->Contact.address = address;
    //        voucher->Contact.PhoneNumber = phone;
    //        voucher->Contact.ContactName = name;
    //        voucher->Contact.route = route;
    //        //delete  addressDialog;
    //    });

    //        QObject::connect(addressDialog,&AddressDialog::addressSelected,
    //                         [=](){
    //            delete  addressDialog;
    //        });

    if(number.length()>0){
        addressDialog->setNumberAndOpen(number);
    }
    else
        addressDialog->show();
}


