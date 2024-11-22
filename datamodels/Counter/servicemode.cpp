#include "servicemode.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
int ServiceMode::Delivery = 1;
int ServiceMode::TakeAway = 2;
int ServiceMode::DineIn = 3;
int ServiceMode::Store = 4;
#include "database/inventory/pricelist/pricelistdatabasehelper.h"

ServiceMode::ServiceMode()
{

}

QString ServiceMode::getStatusString(int id)
{
    return PricelistDatabaseHelper::getPriceListNameByID(QString::number(id));
//    QString a;

//    switch (id) {
//    case 1:
//        a="Delivery";
//        break;
//    case 2:
//        a="Take Away";
//        break;
//    case 3:
//        a="Dine In";
//        if(CounterSettingsDataModel::tokenSystem)
//            a="Self Service";
//        break;
//    case 4:
//        a="Store";
//        break;
//    default :
//        a ="";
//    }

//    return a;
}
