#ifndef BRANDSDATABASEHELPER_H
#define BRANDSDATABASEHELPER_H


#include "datamodels/inventory/brands/brandsdatamodel.h"
#include "database/databasehelper/databasehelper.h"
class BrandsDatabaseHelper
{
public:
    BrandsDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    static QString brandTableName ;
    QString brandID ="_id";
    QString brandName = "Brand_Name";
    QString brandDesc = "Brand_Description";



    int insertBrand(BrandsDataModel *obj);
    int updateBrand(BrandsDataModel *obj,QString id);
    int deleteBrand(int id);

    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();
    int commit();

    QMap<int,QString> getAllBrandsAsMap();
    QString getBrandNameByID(QString id);

    BrandsDataModel *getBrandById(int id);

    QList<BrandsDataModel *> getAllBrands();
    QSqlQueryModel *getBrandsQueryModel();
    bool checkItemExists(int BrandID);
};

#endif // BRANDSDATABASEHELPER_H
