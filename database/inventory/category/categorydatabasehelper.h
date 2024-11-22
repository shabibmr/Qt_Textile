#ifndef CATEGORYDATABASEHELPER_H
#define CATEGORYDATABASEHELPER_H


#include "datamodels/inventory/Category/Categorydatamodel.h"
#include "database/databasehelper/databasehelper.h"
class CategoryDatabaseHelper
{
public:
    CategoryDatabaseHelper(DatabaseHelper *db = new DatabaseHelper());
    static QString CategoryTableName ;
    QString Category_CategoryID ="Category_id";
    QString CategoryName = "Category_Name";
    QString CategoryDesc = "Description";



    int insertCategory(CategoryDataModel *obj);
    int updateCategory(CategoryDataModel *obj,QString id);
    int deleteCategory(int id);

    bool flag = true;
    DatabaseHelper *db;

    int startTransaction();
    int commit();

    QMap<int,QString> getAllCategoryAsMap();
    QString getCategoryNameByID(int id);

    CategoryDataModel *getCategoryById(int id);

    QList<CategoryDataModel *> getAllCategory();
    QSqlQueryModel *getCategoryQueryModel();
    bool checkItemExists(int CategoryID);
};

#endif // CATEGORYDATABASEHELPER_H
