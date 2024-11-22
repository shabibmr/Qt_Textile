#include "restaurantsaveddrafts.h"
#include "ui_restaurantsaveddrafts.h"

RestaurantSavedDrafts::RestaurantSavedDrafts(salesVoucherDeletedDatabaseHelper  *deletedSalesHelper2,
                                             SalesOrderDatabaseHelper *sOHelper,
                                             AccountGroupMasterDatabaseHelper *acCHelper,
                                             LedgerMasterDatabaseHelper *leDHelper,
                                             salesVoucherDatabaseHelper *saleSHelper,
                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantSavedDrafts)
{
    ui->setupUi(this);

    soHelper = sOHelper;
    accHelper = acCHelper;
    ledHelper = leDHelper;
    salesHelper = saleSHelper;

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(1);
    this->deletedSalesHelper = deletedSalesHelper2;


}

RestaurantSavedDrafts::~RestaurantSavedDrafts()
{
    delete ui;
}

void RestaurantSavedDrafts::setPermissions(UiAccessControlDataModel *value)
{
    permissions = value;
}

void RestaurantSavedDrafts::setTabWidget()
{
    qDebug()<<"A";
    SalesOrdersReportWidget = new RestaurantSalesOrdersReport (deletedSalesHelper,
                                                                                            soHelper,accHelper,ledHelper,
                                                                                            this);
    if(!permissions->allowUpdate)
        SalesOrdersReportWidget->disableEdit();
    if(!permissions->allowDelete)
        SalesOrdersReportWidget->disableDelete();

    SalesOrdersReportWidget->setWindowFlags(Qt::Window);
    ui->ordersLayout->addWidget(SalesOrdersReportWidget);

    RestaurantDeliveryReport *DeliveryReportWidget = new RestaurantDeliveryReport(this);
    DeliveryReportWidget->setWindowFlags(Qt::Window);
    ui->deliveryLayout->addWidget(DeliveryReportWidget);

    RestaurantPendingDelivery *PendingDeliveryWidget = new RestaurantPendingDelivery(this);
    PendingDeliveryWidget->setWindowFlags(Qt::Window);
    ui->pendingLayout->addWidget(PendingDeliveryWidget);

//    DineInReport *DineInReportWidget = new DineInReport(this);
//    DineInReportWidget->setWindowFlags(Qt::Window);
//    ui->dineInLayout->addWidget(DineInReportWidget);

//    TakeAwayReport *TakeAwayReportWidget = new TakeAwayReport(this);
//    TakeAwayReportWidget->setWindowFlags(Qt::Window);
//    ui->takeAwayLayout->addWidget(TakeAwayReportWidget);

    QObject::connect(SalesOrdersReportWidget,SIGNAL(closeWindow()),this,SLOT(closeWidget()));
    QObject::connect(SalesOrdersReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(VoucherSelected(GeneralVoucherDataObject*)));
    QObject::connect(SalesOrdersReportWidget,SIGNAL(directCheckOut()),
                     this,SLOT(checkoutDirect()));

    connect(SalesOrdersReportWidget,&RestaurantSalesOrdersReport::VoucherModeSelected,this,[=](GeneralVoucherDataObject *obj){
        emit VoucherModeSelected(obj);
    });

    QObject::connect(DeliveryReportWidget,SIGNAL(closeWindow()),
                     this,SLOT(closeWidget()));

    QObject::connect(DeliveryReportWidget,SIGNAL(directCheckOut()),
                     this,SLOT(checkoutDirect()));

    QObject::connect(PendingDeliveryWidget,SIGNAL(closeWindow()),
                     this,SLOT(closeWidget()));

//    QObject::connect(DineInReportWidget,SIGNAL(closeWindow()),this,SLOT(closeWidget()));
//    QObject::connect(TakeAwayReportWidget,SIGNAL(closeWindow()),this,SLOT(closeWidget()));

    QObject::connect(DeliveryReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(VoucherSelected(GeneralVoucherDataObject*)));

    QObject::connect(PendingDeliveryWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
                     this,SLOT(VoucherSelected(GeneralVoucherDataObject*)));
//    QObject::connect(DineInReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
//                     this,SLOT(VoucherSelected(GeneralVoucherDataObject*)));
//    QObject::connect(TakeAwayReportWidget,SIGNAL(selectedVoucher(GeneralVoucherDataObject*)),
//                     this,SLOT(VoucherSelected(GeneralVoucherDataObject*)));


}


void RestaurantSavedDrafts::closeWidget()
{
    this->close();
}

void RestaurantSavedDrafts::VoucherSelected(GeneralVoucherDataObject *obj)
{
    emit selectedVoucher(obj);
    this->close();
}

void RestaurantSavedDrafts::checkoutDirect()
{
    emit directCheckOutSignal();
}

void RestaurantSavedDrafts::setTab(int tab, int plist)
{
    ui->tabWidget->setCurrentIndex(tab);
    if(plist!=-1){
        SalesOrdersReportWidget->setPlistID(plist);
    }
}


void RestaurantSavedDrafts::on_pushButton_clicked()
{
    closeWidget();
}

