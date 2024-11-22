#ifndef GMLISTWIDGET_H
#define GMLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"
#include "searchlistitemwidget.h"

class GMListWidget : public QListWidget
{
public:
    GMListWidget(QListWidget *parent = nullptr);
    ~GMListWidget();

public slots:
    void setData(QList<inventoryItemDataModel> list);
    void setFilter(QString text);
protected:
    virtual void focusOutEvent(QFocusEvent* e);
};

#endif // GMLISTWIDGET_H
