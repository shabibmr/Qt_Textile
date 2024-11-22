#ifndef CUSTOMITEMVIEW_H
#define CUSTOMITEMVIEW_H

#include <QWidget>
#include <QAbstractItemView>
#include <datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h>
#include <QDebug>
#include <QLabel>
class CustomItemView : public QAbstractItemView
{
public:
    CustomItemView(QWidget *parent = nullptr,inventoryItemDataModel *item = new inventoryItemDataModel());
    QLabel *NameLabel ;
    QLabel *ModelNumberLabel;
};

#endif // CUSTOMITEMVIEW_H
