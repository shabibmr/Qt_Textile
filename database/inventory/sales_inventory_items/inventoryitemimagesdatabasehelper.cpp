#include "inventoryitemimagesdatabasehelper.h"
#include <QFile>

InventoryItemImagesDatabaseHelper::InventoryItemImagesDatabaseHelper()
{
    db = new DatabaseHelper();
}

int InventoryItemImagesDatabaseHelper::startTransaction()
{
    db->startTransaction();
}

int InventoryItemImagesDatabaseHelper::insertItemImage(ItemImageDatamodel itemImage)
{
    // startTransaction();

    QMap<QString,QVariant> map;
    map.insert(itemIDCol,itemImage.itemID);
    map.insert(fileCol,itemImage.img);
    map.insert(width,itemImage.imageWidth);
    map.insert(height,itemImage.imageHeight);
    db->insertRecordsPlus(map,tablename);
    // Writing the image into table


    //return commit();
}

int InventoryItemImagesDatabaseHelper::updateItemImage(ItemImageDatamodel itemImage)
{

    QMap<QString,QVariant> map;
    QMap<QString,QVariant> map1;

    map1.insert(itemIDCol,itemImage.itemID);

    map.insert(fileCol,itemImage.img);
    map.insert(width,itemImage.imageWidth);
    map.insert(height,itemImage.imageHeight);
    QString qstr = " SELECT "+itemIDCol + " FROM " + tablename + " WHERE "+itemIDCol+ "='"+itemImage.itemID+"'";
    QSqlQuery q(QSqlDatabase::database(DatabaseValues::connectionString));
    q.exec(qstr);
    int n=q.size();

    qDebug()<<"Found N = "<<n;
    if(n==0){
        map.insert(itemIDCol,itemImage.itemID);
        db->insertRecordsPlus(map,tablename);
    }
    else{
        db->updateRecordsPlus(map,map1,tablename);
    }

}

int InventoryItemImagesDatabaseHelper::deleteitemImage(ItemImageDatamodel itemImage)
{
    startTransaction();
    QMap<QString,QVariant> map1;

    map1.insert(itemIDCol,itemImage.itemID);

    db->deleteRecordsPlus(map1,tablename);
    return commit();
}

int InventoryItemImagesDatabaseHelper::commit()
{
    if(flag){
        db->commitTransaction();
        return 1;
    }
    else{
        db->rollBackTransaction();
        return 0;
    }
}

ItemImageDatamodel  InventoryItemImagesDatabaseHelper::getItemImageAsQimage(ItemImageDatamodel itemImage)
{
    QByteArray ba;

    QSqlQuery query1(QSqlDatabase::database(DatabaseValues::connectionString));
    QString qStr = " SELECT " + itemIDCol + ", "+ fileCol
            + ", " + width
            + ", " + height
            + " FROM "+tablename + " where "+itemIDCol + "=:ID";
    query1.prepare(qStr);
    query1.bindValue(":ID", itemImage.itemID);
    query1.exec();
    if( query1.next())
    {
        ba = query1.value(1).toByteArray();
        itemImage.imageHeight = query1.value(3).toInt();
        itemImage.imageWidth = query1.value(2).toInt();

    } else {
        qDebug()<< " cannot query image / no image ";
    }
    itemImage.img = ba;
    //    QPixmap pic;
    //    pic.loadFromData( ba);

    //    // Show the image into a QLabel object
    //    ui->imglbl_2->setPixmap(pic);
    //
    return itemImage;

}
